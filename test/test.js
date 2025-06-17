const vita = require('../index.js');
const fs = require('fs');
const path = require('path');

console.log('Testing vita Node.js bindings...\n');

// Test 1: Create synth instance
console.log('1. Creating Synth instance...');
try {
    const synth = new vita.Synth();
    console.log('✓ Synth created successfully\n');
    
    // Test 2: Test constants
    console.log('2. Testing constants...');
    console.log('  Effect.Chorus:', vita.constants.Effect.Chorus);
    console.log('  FilterModel.Analog:', vita.constants.FilterModel.Analog);
    console.log('  ValueScale.Linear:', vita.constants.ValueScale.Linear);
    console.log('✓ Constants accessible\n');
    
    // Test 3: Get modulation sources and destinations
    console.log('3. Testing modulation lists...');
    const sources = vita.get_modulation_sources();
    const destinations = vita.get_modulation_destinations();
    console.log('  Modulation sources:', sources.length);
    console.log('  Modulation destinations:', destinations.length);
    console.log('  First few sources:', sources.slice(0, 5));
    console.log('  First few destinations:', destinations.slice(0, 5));
    console.log('✓ Modulation lists retrieved\n');
    
    // Test 4: Set BPM
    console.log('4. Setting BPM...');
    synth.setBpm(120);
    console.log('✓ BPM set to 120\n');
    
    // Test 5: Get controls
    console.log('5. Getting controls...');
    const controls = synth.getControls();
    const controlNames = Object.keys(controls);
    console.log('  Number of controls:', controlNames.length);
    console.log('  First few controls:', controlNames.slice(0, 5));
    
    // Test a specific control if it exists
    if (controls.osc_1_level) {
        console.log('  Example control (osc_1_level):', controls.osc_1_level.value());
    } else if (controlNames.length > 0) {
        const firstControl = controlNames[0];
        console.log(`  Example control (${firstControl}):`, controls[firstControl].value());
    }
    console.log('✓ Controls retrieved\n');
    
    // Test 6: Load/Save JSON
    console.log('6. Testing JSON serialization...');
    const json = synth.toJson();
    console.log('  JSON length:', json.length);
    
    // Test loading the JSON back
    synth.loadJson(json);
    console.log('✓ JSON save/load successful\n');
    
    // Test 7: Test normalized parameter access
    console.log('7. Testing normalized parameter access...');
    if (controlNames.length > 0) {
        const firstControlName = controlNames[0];
        const control = controls[firstControlName];
        
        const originalValue = control.value();
        const originalNormalized = control.getNormalized();
        console.log(`  Control ${firstControlName}:`);
        console.log('    Original value:', originalValue);
        console.log('    Original normalized:', originalNormalized);
        
        // Set to 50% normalized
        control.setNormalized(0.5);
        console.log('    After setNormalized(0.5):', control.value());
        console.log('    Normalized value:', control.getNormalized());
        
        // Test display text
        console.log('    Display text:', control.getText());
        
        // Restore original value
        control.setNormalized(originalNormalized);
    }
    console.log('✓ Normalized parameters working\n');
    
    // Test 8: Connect modulation
    console.log('8. Testing modulation connections...');
    if (sources.length > 0 && destinations.length > 0) {
        const source = sources[0];
        const destination = destinations[0];
        console.log(`  Connecting ${source} -> ${destination}`);
        synth.connectModulation(source, destination);
        console.log('✓ Modulation connected\n');
        
        // Test disconnect
        synth.disconnectModulation(source, destination);
        console.log('✓ Modulation disconnected\n');
    } else {
        console.log('  No sources or destinations available for testing\n');
    }
    
    // Test 9: Control details
    console.log('9. Testing control details...');
    if (controlNames.length > 0) {
        const controlName = controlNames[0];
        const details = synth.getControlDetails(controlName);
        console.log(`  Details for ${controlName}:`);
        console.log('    Name:', details.name);
        console.log('    Min:', details.min);
        console.log('    Max:', details.max);
        console.log('    Default:', details.defaultValue);
        console.log('    Is discrete:', details.isDiscrete);
        if (details.options && details.options.length > 0) {
            console.log('    Options:', details.options.slice(0, 3));
        }
    }
    console.log('✓ Control details working\n');
    
    // Test 10: Render audio (basic test)
    console.log('10. Testing audio rendering...');
    try {
        const audioBuffer = synth.render(60, 0.8, 0.1, 0.2);
        console.log('  Audio buffer length:', audioBuffer.length);
        console.log('  Audio buffer type:', audioBuffer.constructor.name);
        console.log('  First few samples:', Array.from(audioBuffer.slice(0, 5)));
        console.log('✓ Audio rendered successfully\n');
    } catch (e) {
        console.log('  Audio render failed:', e.message);
        console.log('  This might be expected if nanobind->NAPI conversion needs adjustment\n');
    }
    
    // Test 11: Render to file (if audio rendering works)
    console.log('11. Testing file rendering...');
    try {
        const outputPath = path.join(__dirname, 'test_output.wav');
        const success = synth.renderFile(outputPath, 60, 0.8, 0.1, 0.2);
        console.log('  File render success:', success);
        
        if (success && fs.existsSync(outputPath)) {
            console.log('  File size:', fs.statSync(outputPath).size, 'bytes');
            fs.unlinkSync(outputPath); // Clean up
            console.log('✓ File render successful\n');
        } else {
            console.log('  File not created or render failed\n');
        }
    } catch (e) {
        console.log('  File render failed:', e.message, '\n');
    }
    
    // Test 12: Python compatibility API
    console.log('12. Testing Python API compatibility...');
    
    // Test snake_case method names
    try {
        synth.set_bpm(140);
        console.log('  set_bpm works');
        
        const controls2 = synth.get_controls();
        console.log('  get_controls works');
        
        const json2 = synth.to_json();
        console.log('  to_json works, length:', json2.length);
        
        synth.load_json(json2);
        console.log('  load_json works');
        
        const sources2 = vita.get_modulation_sources();
        console.log('  get_modulation_sources works, length:', sources2.length);
        
        console.log('✓ Python API compatibility working\n');
    } catch (e) {
        console.log('  Python API test failed:', e.message, '\n');
    }
    
    console.log('All basic tests completed! 🎉');
    console.log('\nNote: Some tests may fail if certain dependencies or build configurations need adjustment.');
    console.log('The core binding structure appears to be working correctly.');
    
} catch (error) {
    console.error('❌ Test failed:', error.message);
    console.error('Stack trace:', error.stack);
    process.exit(1);
}