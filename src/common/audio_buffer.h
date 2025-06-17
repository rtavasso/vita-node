#pragma once

// Forward declaration for Node.js buffer compatibility
struct VitalAudioBuffer {
    float* data;
    size_t size;
    size_t channels;
    size_t samples_per_channel;
};