/*
 * Copyright 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef FLOWGRAPH_SINK_FLOAT_H
#define FLOWGRAPH_SINK_FLOAT_H

#include <unistd.h>
#include <sys/types.h>

#include "FlowGraphNode.h"

namespace flowgraph {

/**
 * AudioSink that lets you read data as 32-bit floats.
 */
class SinkFloat : public FlowGraphSink {
public:
    explicit SinkFloat(int32_t channelCount);

    int32_t read(int64_t framePosition, void *data, int32_t numFrames) override;

    const char *getName() override {
        return "SinkFloat";
    }
};

} /* namespace flowgraph */

#endif //FLOWGRAPH_SINK_FLOAT_H
