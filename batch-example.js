const { BatchRenderer } = require('./index');

async function quickExample() {
    const renderer = new BatchRenderer();
    
    // Example 1: Simple batch render
    const presets = [
        'preset1.vital',
        'preset2.vital',
        'preset3.vital'
    ];
    
    const results = await renderer.renderBatch(presets, './output');
    console.log(`Rendered ${results.filter(r => r.success).length} files`);
    
    // Example 2: With progress tracking
    await renderer.renderBatchWithProgress(
        presets,
        './output',
        (progress) => {
            console.log(`${progress.percentage.toFixed(0)}% complete`);
        }
    );
}

if (require.main === module) {
    quickExample().catch(console.error);
}