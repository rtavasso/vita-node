/* Copyright 2013-2019 Matt Tytel
 *
 * vital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vital.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "synth_constants.h"
#include "synth_module.h"

namespace vital {

  class Phaser;

  class PhaserModule : public SynthModule {
    public:
      enum {
        kAudioOutput,
        kCutoffOutput,
        kNumOutputs
      };

      PhaserModule(const Output* beats_per_second);
      virtual ~PhaserModule();

      void init() override;
      void hardReset() override;
      void enable(bool enable) override;

      void correctToTime(double seconds) override;
      void setSampleRate(int sample_rate) override;
      void processWithInput(const poly_float* audio_in, int num_samples) override;
      Processor* clone() const override { return new PhaserModule(*this); }

    protected:
      const Output* beats_per_second_;
      Phaser* phaser_;

      JUCE_LEAK_DETECTOR(PhaserModule)
  };
} // namespace vital

