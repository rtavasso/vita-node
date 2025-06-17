# Vita for Node.js (`vita-node`)

[![npm version](https://badge.fury.io/js/vita-node.svg)](https://badge.fury.io/js/vita-node)
[![CI](https://github.com/rtavasso/vita-node/actions/workflows/ci.yml/badge.svg)](https://github.com/rtavasso/vita-node/actions)

Node.js bindings for [Vital](https://vital.audio/). It was directly inspired by and leveraged the work of David Braun's **[Vita](https://github.com/DBraun/Vita)**, a Python module for interacting with Vital. **This is not an official product related to Vital or the original Vita project.**

## Installation

```bash
npm install vita-node
```

## Example

The API is designed to be very similar to the original Python version.

```javascript
const fs = require('fs');
const wav = require('node-wav'); // Run: npm install node-wav
const vita = require('vita-node');

const SAMPLE_RATE = 44100;

const bpm = 120.0;
const noteDur = 1.0;
const renderDur = 3.0;
const pitch = 36;   // integer
const velocity = 0.7; // [0.0 to 1.0]

const synth = new vita.Synth();
// The initial preset is loaded by default.

synth.setBpm(bpm);

// Let's make a custom modulation using
// the available modulation sources and destinations.
// These lists are constant.
console.log("Potential sources:", vita.getModulationSources());
console.log("Potential destinations:", vita.getModulationDestinations());

// "lfo_1" is a potential source,
// and "filter_1_cutoff" is a potential destination.
console.assert(synth.connectModulation("lfo_1", "filter_1_cutoff"));

const controls = synth.getControls();
controls.get("modulation_1_amount").set(1.0);
controls.get("filter_1_on").set(1.0);
const val = controls.get("filter_1_on").value();
controls.get("lfo_1_tempo").set(vita.constants.SyncedFrequency.k1_16);

// Use normalized parameter control (0-1 range, VST-style)
controls.get("filter_1_cutoff").setNormalized(0.5); // Set knob to 50%
console.log(controls.get("filter_1_cutoff").getNormalized()); // Get normalized value

// Get parameter details and display text
const info = synth.getControlDetails("delay_style");
console.log(`Options: ${info.options}`); // ["Mono", "Stereo", "Ping Pong", "Mid Ping Pong"]
console.log(`Current: ${synth.getControlText('delay_style')}`); // e.g., "Stereo"

// Render audio to an array of Float32Arrays: [leftChannel, rightChannel]
const audio = synth.render(pitch, velocity, noteDur, renderDur);

// Encode and write the WAV file
const encodedWav = wav.encode(audio, { sampleRate: SAMPLE_RATE, float: true, bitDepth: 32 });
fs.writeFileSync("generated_preset.wav", encodedWav);

// Dump current state to JSON text
const presetPath = "generated_preset.vital";
const jsonText = synth.toJson();
fs.writeFileSync(presetPath, jsonText);

// Load JSON text from a string
const loadedJson = fs.readFileSync(presetPath, 'utf8');
console.assert(synth.loadJson(loadedJson));

// Or load directly from file
console.assert(synth.loadPreset(presetPath));

// Load the initial preset, which also clears modulations
synth.loadInitPreset();
// Or just clear modulations.
synth.clearModulations();
```

## Documentation

The API is not yet formally documented. Please browse [bindings.cpp](https://github.com/rtavasso/vita-node/blob/main/src/headless/bindings.cpp) in this repository to see the full list of available functions and classes exposed to Node.js.

## Issues

If you find any issues with the **Node.js bindings**, please report them at: https://github.com/rtavasso/vita-node/issues.

---

### A Note on Licensing

The following sections are from the original Vital source code repository and apply to the C++ source code included in this project. Because `vita-node` is a derivative work licensed under GPLv3, all users must comply with these terms.

#### Code Licensing
If you are making a proprietary or closed source app and would like to use Vital's source code, contact licensing@vital.audio for non GPLv3 licensing options.

#### What can you do with the source
The source code is licensed under the GPLv3. If you download the source or create builds you must comply with that license.

#### Things you can't do with this source
- Do not create an app and distribute it on the iOS app store. The app store is not comptabile with GPLv3 and you'll only get an exception for this if you're paying for a GPLv3 exception for Vital's source (see Code Licensing above).
- Do not use the name "Vital", "Vital Audio", "Tytel" or "Matt Tytel" for marketing or to name any distribution of binaries built with this source. This source code does not give you rights to infringe on trademarks.
- Do not connect to any web service at https://vital.audio, https://account.vital.audio or https://store.vital.audio from your own builds. This is against the terms of using those sites.
- Do not distribute the presets that come with the free version of Vital. They're under a separate license that does not allow redistribution.