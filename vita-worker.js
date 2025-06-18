const { parentPort, workerData } = require('worker_threads');
const fs = require('fs');
const path = require('path');
const vita = require('./index.js');

// Create synth instance
const synth = new vita.Synth();

// Signal ready
parentPort.postMessage({ type: 'ready' });

// Handle render jobs
parentPort.on('message', async (message) => {
    if (message.type !== 'render') return;
    
    const { job } = message;
    const startTime = Date.now();
    
    try {
        // Load preset
        if (!synth.loadPreset(job.presetPath)) {
            throw new Error(`Failed to load preset: ${job.presetPath}`);
        }
        
        // Ensure output directory exists
        const outputDir = path.dirname(job.outputPath);
        await fs.promises.mkdir(outputDir, { recursive: true });
        
        // Render to file
        const { note, velocity, noteDuration, renderDuration } = job.params;
        const success = synth.renderFile(
            job.outputPath,
            note,
            velocity,
            noteDuration,
            renderDuration
        );
        
        if (!success) {
            throw new Error(`Failed to render audio`);
        }
        
        // Get file size
        const stats = await fs.promises.stat(job.outputPath);
        
        parentPort.postMessage({
            type: 'complete',
            duration: Date.now() - startTime,
            fileSize: stats.size
        });
        
    } catch (error) {
        parentPort.postMessage({
            type: 'error',
            error: error.message
        });
    }
});