const { BatchRenderer } = require('../index');
const fs = require('fs');
const path = require('path');
const os = require('os');

// Create a temporary directory for test outputs
const testDir = path.join(os.tmpdir(), 'vita-batch-test-' + Date.now());
fs.mkdirSync(testDir, { recursive: true });

console.log('Testing BatchRenderer...');
console.log('Output directory:', testDir);

async function runTests() {
    const renderer = new BatchRenderer({
        workers: 4,
        defaultParams: {
            note: 60,
            velocity: 0.7,
            noteDuration: 0.5,
            renderDuration: 1.0
        }
    });

    // Test presets - using the 303 preset we tested earlier
    const testPreset = 'C:/Users/Riley/Desktop/SummerProject/2023AndBeyond/DanceMe/presetSorting/discord_presets/303.vital';
    
    // Check if preset exists
    if (!fs.existsSync(testPreset)) {
        console.error('Test preset not found:', testPreset);
        console.log('Please update the test with a valid preset path');
        return;
    }

    // Test 1: Single preset to output directory
    console.log('\nTest 1: Single preset to output directory');
    try {
        const results = await renderer.renderBatch(
            [testPreset],
            testDir
        );
        console.log('✓ Rendered:', results[0]);
    } catch (error) {
        console.error('✗ Failed:', error.message);
    }

    // Test 2: Multiple copies with custom names
    console.log('\nTest 2: Multiple outputs with custom names');
    try {
        const presets = Array(5).fill(testPreset);
        const outputs = [
            path.join(testDir, 'bass_1.wav'),
            path.join(testDir, 'bass_2.wav'),
            path.join(testDir, 'bass_3.wav'),
            path.join(testDir, 'bass_4.wav'),
            path.join(testDir, 'bass_5.wav')
        ];
        
        const results = await renderer.renderBatch(presets, outputs);
        const successful = results.filter(r => r.success).length;
        console.log(`✓ Rendered ${successful}/${results.length} files`);
    } catch (error) {
        console.error('✗ Failed:', error.message);
    }

    // Test 3: Batch with progress tracking
    console.log('\nTest 3: Batch rendering with progress');
    try {
        const presets = Array(10).fill(testPreset);
        let lastProgress = 0;
        
        await renderer.renderBatchWithProgress(
            presets,
            testDir,
            { note: 48, velocity: 0.8 },
            (progress) => {
                if (progress.percentage - lastProgress >= 20 || progress.percentage === 100) {
                    console.log(`  Progress: ${progress.percentage.toFixed(0)}% (${progress.completed}/${progress.total})`);
                    lastProgress = progress.percentage;
                }
            }
        );
        console.log('✓ Batch rendering completed');
    } catch (error) {
        console.error('✗ Failed:', error.message);
    }

    // Test 4: Performance test
    console.log('\nTest 4: Performance comparison');
    const numPresets = 20;
    const presets = Array(numPresets).fill(testPreset);
    
    // Sequential rendering (single worker)
    const sequentialRenderer = new BatchRenderer({ workers: 1 });
    const startSequential = Date.now();
    await sequentialRenderer.renderBatch(presets, path.join(testDir, 'sequential'));
    const sequentialTime = Date.now() - startSequential;
    
    // Parallel rendering (multiple workers)
    const parallelRenderer = new BatchRenderer({ workers: os.cpus().length });
    const startParallel = Date.now();
    await parallelRenderer.renderBatch(presets, path.join(testDir, 'parallel'));
    const parallelTime = Date.now() - startParallel;
    
    console.log(`✓ Sequential (1 worker): ${sequentialTime}ms`);
    console.log(`✓ Parallel (${os.cpus().length} workers): ${parallelTime}ms`);
    console.log(`✓ Speedup: ${(sequentialTime / parallelTime).toFixed(2)}x`);

    // Cleanup
    console.log('\nTest complete. Output files in:', testDir);
}

// Run tests
runTests().catch(console.error);