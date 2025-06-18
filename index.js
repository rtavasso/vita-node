// Main entry point that provides the same API as Python

// 1. Load the native addon using node-gyp-build (works with both regular builds and prebuildify)
const vita = require('node-gyp-build')(__dirname);

// 2. Load batch renderer
const VitaBatchRenderer = require('./batch-renderer');

// 3. Build the object you want to export
const exportsObject = {
  // Use the correct variable name: 'vita'
  Synth: vita.Synth,
  constants: vita.constants,
  
  // Python-style function names (snake_case)
  get_modulation_sources: vita.get_modulation_sources,
  get_modulation_destinations: vita.get_modulation_destinations,
  
  // JavaScript-style function names (camelCase)
  getModulationSources: vita.getModulationSources,
  getModulationDestinations: vita.getModulationDestinations,
  
  // Batch rendering
  BatchRenderer: VitaBatchRenderer
};

// 4. For compatibility with Python's import style, add a self-reference
exportsObject.vita = exportsObject;

// 5. Export the final object
module.exports = exportsObject;