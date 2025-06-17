#include <napi.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <stdexcept>

#include "compressor.h"
#include "processor_router.h"
#include "random_lfo.h"
#include "sound_engine.h"
#include "synth_base.h"
#include "synth_filter.h"
#include "synth_lfo.h"
#include "synth_oscillator.h"
#include "value.h"
#include "voice_handler.h"
#include "wave_frame.h"
#include "synth_parameters.h"

using namespace vital;

// Forward declarations
class ControlValueWrapper;
class SynthWrapper;

// Helper function to get formatted display text for a control
static std::string GetControlText(HeadlessSynth &synth, const std::string &name) {
    auto &controls = synth.getControls();
    auto it = controls.find(name);
    if (it == controls.end())
        throw std::runtime_error("No control: " + name);
    
    mono_float raw = it->second->value();
    const auto &details = Parameters::getDetails(name);
    
    // Discrete/indexed parameters
    if (details.string_lookup) {
        int count = static_cast<int>(details.max - details.min + 1);
        int idx = static_cast<int>(std::lround(raw - details.min));
        if (idx < 0) idx = 0;
        else if (idx >= count) idx = count - 1;
        return details.string_lookup[idx];
    }
    
    // Continuous parameters: apply scaling
    float skewed;
    switch (details.value_scale) {
    case ValueDetails::kQuadratic: skewed = raw * raw; break;
    case ValueDetails::kCubic: skewed = raw * raw * raw; break;
    case ValueDetails::kQuartic: skewed = raw * raw; skewed = skewed * skewed; break;
    case ValueDetails::kExponential:
        if (details.display_invert)
            skewed = 1.0f / std::pow(2.0f, raw);
        else
            skewed = std::pow(2.0f, raw);
        break;
    case ValueDetails::kSquareRoot: skewed = std::sqrt(raw); break;
    default: skewed = raw; break;
    }
    float display_val = details.display_multiply * skewed + details.post_offset;
    return std::to_string(display_val) + details.display_units;
}

// Cache for modulation sources and destinations
class ModulationListCache {
private:
    static std::vector<std::string> sources_cache;
    static std::vector<std::string> destinations_cache;
    static bool initialized;
    
    static void initialize() {
        if (!initialized) {
            SoundEngine engine;
            
            // Cache destinations
            vital::input_map &dest_map = engine.getMonoModulationDestinations();
            for (auto &dest : dest_map) {
                destinations_cache.push_back(dest.first);
            }
            
            // Cache sources
            vital::output_map &src_map = engine.getModulationSources();
            for (auto &src : src_map) {
                sources_cache.push_back(src.first);
            }
            
            initialized = true;
        }
    }
    
public:
    static const std::vector<std::string>& getSources() {
        initialize();
        return sources_cache;
    }
    
    static const std::vector<std::string>& getDestinations() {
        initialize();
        return destinations_cache;
    }
};

std::vector<std::string> ModulationListCache::sources_cache;
std::vector<std::string> ModulationListCache::destinations_cache;
bool ModulationListCache::initialized = false;

// Wrapper for ControlValue that provides normalized parameter access
class ControlValueWrapper : public Napi::ObjectWrap<ControlValueWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "ControlValue", {
            InstanceMethod("value", &ControlValueWrapper::GetValue),
            InstanceMethod("set", &ControlValueWrapper::SetValue),
            InstanceMethod("setNormalized", &ControlValueWrapper::SetNormalized),
            InstanceMethod("getNormalized", &ControlValueWrapper::GetNormalized),
            InstanceMethod("getText", &ControlValueWrapper::GetText),
            // Python compatibility names
            InstanceMethod("set_normalized", &ControlValueWrapper::SetNormalized),
            InstanceMethod("get_normalized", &ControlValueWrapper::GetNormalized),
            InstanceMethod("get_text", &ControlValueWrapper::GetText)
        });
        
        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        
        return exports;
    }
    
    ControlValueWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<ControlValueWrapper>(info) {
        // Constructor is called internally, not from JS
    }
    
    void Initialize(vital::Value* value, const std::string& name, HeadlessSynth* synth) {
        value_ = value;
        name_ = name;
        synth_ = synth;
    }
    
public:
    static Napi::FunctionReference constructor;
    
private:
    vital::Value* value_;
    std::string name_;
    HeadlessSynth* synth_;
    
    Napi::Value GetValue(const Napi::CallbackInfo& info) {
        return Napi::Number::New(info.Env(), value_->value());
    }
    
    void SetValue(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 1 || !info[0].IsNumber()) {
            Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
            return;
        }
        float value = info[0].As<Napi::Number>().FloatValue();
        value_->set(poly_float(value));
    }
    
    void SetNormalized(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 1 || !info[0].IsNumber()) {
            Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
            return;
        }
        
        double normalized = info[0].As<Napi::Number>().DoubleValue();
        normalized = std::max(0.0, std::min(1.0, normalized));
        
        const auto &details = Parameters::getDetails(name_);
        float value;
        
        if (details.value_scale == ValueDetails::kIndexed) {
            int num_options = static_cast<int>(details.max - details.min + 1);
            int index = static_cast<int>(std::round(normalized * (num_options - 1)));
            value = details.min + index;
        } else {
            float value_normalized = static_cast<float>(normalized);
            
            switch (details.value_scale) {
            case ValueDetails::kQuadratic:
                value = details.min + std::sqrt(value_normalized) * (details.max - details.min);
                break;
            case ValueDetails::kCubic:
                value = details.min + std::pow(value_normalized, 1.0f/3.0f) * (details.max - details.min);
                break;
            case ValueDetails::kQuartic:
                value = details.min + value_normalized * (details.max - details.min);
                break;
            case ValueDetails::kExponential:
                if (details.display_invert)
                    value = details.min + (1.0f / std::pow(2.0f, value_normalized)) * (details.max - details.min);
                else
                    value = details.min + std::pow(2.0f, value_normalized) * (details.max - details.min);
                break;
            case ValueDetails::kSquareRoot:
                value = details.min + (value_normalized * value_normalized) * (details.max - details.min);
                break;
            default:
                value = details.min + value_normalized * (details.max - details.min);
                break;
            }
        }
        
        value_->set(value);
    }
    
    Napi::Value GetNormalized(const Napi::CallbackInfo& info) {
        const auto &details = Parameters::getDetails(name_);
        float raw = value_->value();
        
        double normalized;
        if (details.value_scale == ValueDetails::kIndexed) {
            int num_options = static_cast<int>(details.max - details.min + 1);
            int index = static_cast<int>(std::round(raw - details.min));
            normalized = static_cast<double>(index) / (num_options - 1);
        } else {
            float normalized_internal = (raw - details.min) / (details.max - details.min);
            
            switch (details.value_scale) {
            case ValueDetails::kQuadratic:
                normalized = normalized_internal * normalized_internal;
                break;
            case ValueDetails::kCubic:
                normalized = normalized_internal * normalized_internal * normalized_internal;
                break;
            case ValueDetails::kQuartic:
                normalized = normalized_internal;
                break;
            case ValueDetails::kExponential:
                if (details.display_invert)
                    normalized = -std::log2(normalized_internal + 1e-10f);
                else
                    normalized = std::log2(normalized_internal + 1e-10f);
                break;
            case ValueDetails::kSquareRoot:
                normalized = std::sqrt(normalized_internal);
                break;
            default:
                normalized = normalized_internal;
                break;
            }
        }
        
        return Napi::Number::New(info.Env(), std::max(0.0, std::min(1.0, normalized)));
    }
    
    Napi::Value GetText(const Napi::CallbackInfo& info) {
        return Napi::String::New(info.Env(), GetControlText(*synth_, name_));
    }
};

Napi::FunctionReference ControlValueWrapper::constructor;

// Main Synth wrapper class
class SynthWrapper : public Napi::ObjectWrap<SynthWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "Synth", {
            InstanceMethod("connectModulation", &SynthWrapper::ConnectModulation),
            InstanceMethod("disconnectModulation", &SynthWrapper::DisconnectModulation),
            InstanceMethod("setBpm", &SynthWrapper::SetBpm),
            InstanceMethod("render", &SynthWrapper::Render),
            InstanceMethod("renderFile", &SynthWrapper::RenderFile),
            InstanceMethod("loadJson", &SynthWrapper::LoadJson),
            InstanceMethod("toJson", &SynthWrapper::ToJson),
            InstanceMethod("loadPreset", &SynthWrapper::LoadPreset),
            InstanceMethod("loadInitPreset", &SynthWrapper::LoadInitPreset),
            InstanceMethod("clearModulations", &SynthWrapper::ClearModulations),
            InstanceMethod("getControls", &SynthWrapper::GetControls),
            InstanceMethod("getControlDetails", &SynthWrapper::GetControlDetails),
            InstanceMethod("getControlText", &SynthWrapper::GetControlText),
            // Python compatibility names
            InstanceMethod("connect_modulation", &SynthWrapper::ConnectModulation),
            InstanceMethod("disconnect_modulation", &SynthWrapper::DisconnectModulation),
            InstanceMethod("set_bpm", &SynthWrapper::SetBpm),
            InstanceMethod("render_file", &SynthWrapper::RenderFile),
            InstanceMethod("load_json", &SynthWrapper::LoadJson),
            InstanceMethod("to_json", &SynthWrapper::ToJson),
            InstanceMethod("load_preset", &SynthWrapper::LoadPreset),
            InstanceMethod("load_init_preset", &SynthWrapper::LoadInitPreset),
            InstanceMethod("clear_modulations", &SynthWrapper::ClearModulations),
            InstanceMethod("get_controls", &SynthWrapper::GetControls),
            InstanceMethod("get_control_details", &SynthWrapper::GetControlDetails),
            InstanceMethod("get_control_text", &SynthWrapper::GetControlText),
            InstanceAccessor("__state__", &SynthWrapper::GetState, &SynthWrapper::SetState)
        });
        
        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();
        
        exports.Set("Synth", func);
        return exports;
    }
    
    SynthWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SynthWrapper>(info) {
        synth_ = new HeadlessSynth();
        synth_->loadInitPreset();
    }
    
    ~SynthWrapper() {
        delete synth_;
    }
    
public:
    static Napi::FunctionReference constructor;
    
private:
    HeadlessSynth* synth_;
    
    void ConnectModulation(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
            Napi::TypeError::New(env, "Two strings expected").ThrowAsJavaScriptException();
            return;
        }
        std::string source = info[0].As<Napi::String>().Utf8Value();
        std::string dest = info[1].As<Napi::String>().Utf8Value();
        synth_->pyConnectModulation(source, dest);
    }
    
    void DisconnectModulation(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
            Napi::TypeError::New(env, "Two strings expected").ThrowAsJavaScriptException();
            return;
        }
        std::string source = info[0].As<Napi::String>().Utf8Value();
        std::string dest = info[1].As<Napi::String>().Utf8Value();
        synth_->disconnectModulation(source, dest);
    }
    
    void SetBpm(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 1 || !info[0].IsNumber()) {
            Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
            return;
        }
        double bpm = info[0].As<Napi::Number>().DoubleValue();
        synth_->pySetBPM(bpm);
    }
    
    Napi::Value Render(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 4 || !info[0].IsNumber() || !info[1].IsNumber() || 
            !info[2].IsNumber() || !info[3].IsNumber()) {
            Napi::TypeError::New(env, "Four numbers expected").ThrowAsJavaScriptException();
            return env.Null();
        }
        
        int midi_note = info[0].As<Napi::Number>().Int32Value();
        double midi_velocity = info[1].As<Napi::Number>().DoubleValue();
        double note_dur = info[2].As<Napi::Number>().DoubleValue();
        double render_dur = info[3].As<Napi::Number>().DoubleValue();
        
        // Get audio data from the synth - this returns a VitalAudioBuffer
        auto audio_data = synth_->renderAudioToNumpy(midi_note, midi_velocity, note_dur, render_dur);
        
        // Extract the raw data from the VitalAudioBuffer
        float* data_ptr = audio_data.data;
        size_t total_samples = audio_data.size;
        
        // Create a Buffer from the audio data
        Napi::Buffer<float> buffer = Napi::Buffer<float>::Copy(env, data_ptr, total_samples);
        
        // Clean up the allocated memory (VitalAudioBuffer doesn't own the data automatically like numpy did)
        delete[] data_ptr;
        
        return buffer;
    }
    
    Napi::Value RenderFile(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 5 || !info[0].IsString() || !info[1].IsNumber() || 
            !info[2].IsNumber() || !info[3].IsNumber() || !info[4].IsNumber()) {
            Napi::TypeError::New(env, "String and four numbers expected").ThrowAsJavaScriptException();
            return env.Null();
        }
        
        std::string output_path = info[0].As<Napi::String>().Utf8Value();
        int midi_note = info[1].As<Napi::Number>().Int32Value();
        double midi_velocity = info[2].As<Napi::Number>().DoubleValue();
        double note_dur = info[3].As<Napi::Number>().DoubleValue();
        double render_dur = info[4].As<Napi::Number>().DoubleValue();
        
        bool success = synth_->renderAudioToFile2(output_path, midi_note, midi_velocity, note_dur, render_dur);
        return Napi::Boolean::New(env, success);
    }
    
    void LoadJson(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 1 || !info[0].IsString()) {
            Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
            return;
        }
        std::string json = info[0].As<Napi::String>().Utf8Value();
        synth_->loadFromString(json);
    }
    
    Napi::Value ToJson(const Napi::CallbackInfo& info) {
        return Napi::String::New(info.Env(), synth_->pyToJson());
    }
    
    Napi::Value LoadPreset(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 1 || !info[0].IsString()) {
            Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
            return env.Null();
        }
        std::string filepath = info[0].As<Napi::String>().Utf8Value();
        bool success = synth_->pyLoadFromFile(filepath);
        return Napi::Boolean::New(env, success);
    }
    
    void LoadInitPreset(const Napi::CallbackInfo& info) {
        synth_->loadInitPreset();
    }
    
    void ClearModulations(const Napi::CallbackInfo& info) {
        synth_->clearModulations();
    }
    
    Napi::Value GetControls(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::Object result = Napi::Object::New(env);
        
        auto &controls = synth_->getControls();
        for (const auto &[name, value] : controls) {
            // Create a ControlValue wrapper for each control
            Napi::Object wrapper = ControlValueWrapper::constructor.New({});
            ControlValueWrapper* unwrapped = Napi::ObjectWrap<ControlValueWrapper>::Unwrap(wrapper);
            unwrapped->Initialize(value, name, synth_);
            result.Set(name, wrapper);
        }
        
        return result;
    }
    
    Napi::Value GetControlDetails(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 1 || !info[0].IsString()) {
            Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
            return env.Null();
        }
        
        std::string name = info[0].As<Napi::String>().Utf8Value();
        if (!vital::Parameters::isParameter(name)) {
            Napi::Error::New(env, "No metadata for control: " + name).ThrowAsJavaScriptException();
            return env.Null();
        }
        
        const auto &details = vital::Parameters::getDetails(name);
        Napi::Object obj = Napi::Object::New(env);
        
        obj.Set("name", details.name);
        obj.Set("min", details.min);
        obj.Set("max", details.max);
        obj.Set("defaultValue", details.default_value);
        obj.Set("versionAdded", details.version_added);
        obj.Set("postOffset", details.post_offset);
        obj.Set("displayMultiply", details.display_multiply);
        obj.Set("scale", static_cast<int>(details.value_scale));
        obj.Set("displayUnits", details.display_units);
        obj.Set("displayName", details.display_name);
        obj.Set("isDiscrete", details.value_scale == vital::ValueDetails::kIndexed);
        
        // Add options array for discrete parameters
        if (details.value_scale == vital::ValueDetails::kIndexed && details.string_lookup) {
            Napi::Array options = Napi::Array::New(env);
            int count = static_cast<int>(details.max - details.min + 1);
            for (int i = 0; i < count; ++i) {
                options.Set(i, std::string(details.string_lookup[i]));
            }
            obj.Set("options", options);
        }
        
        return obj;
    }
    
    Napi::Value GetControlText(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        if (info.Length() < 1 || !info[0].IsString()) {
            Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
            return env.Null();
        }
        std::string name = info[0].As<Napi::String>().Utf8Value();
        return Napi::String::New(env, ::GetControlText(*synth_, name));
    }
    
    // Serialization support (equivalent to Python's __getstate__ and __setstate__)
    Napi::Value GetState(const Napi::CallbackInfo& info) {
        return Napi::String::New(info.Env(), synth_->pyToJson());
    }
    
    void SetState(const Napi::CallbackInfo& info, const Napi::Value& value) {
        if (!value.IsString()) {
            Napi::TypeError::New(info.Env(), "String expected").ThrowAsJavaScriptException();
            return;
        }
        std::string json = value.As<Napi::String>().Utf8Value();
        synth_->loadFromString(json);
    }
};

Napi::FunctionReference SynthWrapper::constructor;

// Create constants object with all enums
Napi::Object CreateConstantsObject(Napi::Env env) {
    Napi::Object constants = Napi::Object::New(env);
    
    // SourceDestination enum
    Napi::Object sourceDestination = Napi::Object::New(env);
    sourceDestination.Set("Filter1", static_cast<int>(constants::SourceDestination::kFilter1));
    sourceDestination.Set("Filter2", static_cast<int>(constants::SourceDestination::kFilter2));
    sourceDestination.Set("DualFilters", static_cast<int>(constants::SourceDestination::kDualFilters));
    sourceDestination.Set("Effects", static_cast<int>(constants::SourceDestination::kEffects));
    sourceDestination.Set("DirectOut", static_cast<int>(constants::SourceDestination::kDirectOut));
    constants.Set("SourceDestination", sourceDestination);
    
    // Effect enum
    Napi::Object effect = Napi::Object::New(env);
    effect.Set("Chorus", static_cast<int>(constants::Effect::kChorus));
    effect.Set("Compressor", static_cast<int>(constants::Effect::kCompressor));
    effect.Set("Delay", static_cast<int>(constants::Effect::kDelay));
    effect.Set("Distortion", static_cast<int>(constants::Effect::kDistortion));
    effect.Set("Eq", static_cast<int>(constants::Effect::kEq));
    effect.Set("FilterFx", static_cast<int>(constants::Effect::kFilterFx));
    effect.Set("Flanger", static_cast<int>(constants::Effect::kFlanger));
    effect.Set("Phaser", static_cast<int>(constants::Effect::kPhaser));
    effect.Set("Reverb", static_cast<int>(constants::Effect::kReverb));
    constants.Set("Effect", effect);
    
    // FilterModel enum
    Napi::Object filterModel = Napi::Object::New(env);
    filterModel.Set("Analog", static_cast<int>(constants::FilterModel::kAnalog));
    filterModel.Set("Dirty", static_cast<int>(constants::FilterModel::kDirty));
    filterModel.Set("Ladder", static_cast<int>(constants::FilterModel::kLadder));
    filterModel.Set("Digital", static_cast<int>(constants::FilterModel::kDigital));
    filterModel.Set("Diode", static_cast<int>(constants::FilterModel::kDiode));
    filterModel.Set("Formant", static_cast<int>(constants::FilterModel::kFormant));
    filterModel.Set("Comb", static_cast<int>(constants::FilterModel::kComb));
    filterModel.Set("Phase", static_cast<int>(constants::FilterModel::kPhase));
    constants.Set("FilterModel", filterModel);
    
    // RetriggerStyle enum  
    Napi::Object retriggerStyle = Napi::Object::New(env);
    retriggerStyle.Set("Free", static_cast<int>(constants::RetriggerStyle::kFree));
    retriggerStyle.Set("Retrigger", static_cast<int>(constants::RetriggerStyle::kRetrigger));
    retriggerStyle.Set("SyncToPlayHead", static_cast<int>(constants::RetriggerStyle::kSyncToPlayHead));
    constants.Set("RetriggerStyle", retriggerStyle);
    
    // ValueScale enum
    Napi::Object valueScale = Napi::Object::New(env);
    valueScale.Set("Indexed", static_cast<int>(vital::ValueDetails::kIndexed));
    valueScale.Set("Linear", static_cast<int>(vital::ValueDetails::kLinear));
    valueScale.Set("Quadratic", static_cast<int>(vital::ValueDetails::kQuadratic));
    valueScale.Set("Cubic", static_cast<int>(vital::ValueDetails::kCubic));
    valueScale.Set("Quartic", static_cast<int>(vital::ValueDetails::kQuartic));
    valueScale.Set("SquareRoot", static_cast<int>(vital::ValueDetails::kSquareRoot));
    valueScale.Set("Exponential", static_cast<int>(vital::ValueDetails::kExponential));
    constants.Set("ValueScale", valueScale);
    
    // SpectralMorph enum
    Napi::Object spectralMorph = Napi::Object::New(env);
    spectralMorph.Set("NoSpectralMorph", static_cast<int>(SynthOscillator::SpectralMorph::kNoSpectralMorph));
    spectralMorph.Set("Vocode", static_cast<int>(SynthOscillator::SpectralMorph::kVocode));
    spectralMorph.Set("FormScale", static_cast<int>(SynthOscillator::SpectralMorph::kFormScale));
    spectralMorph.Set("HarmonicScale", static_cast<int>(SynthOscillator::SpectralMorph::kHarmonicScale));
    spectralMorph.Set("InharmonicScale", static_cast<int>(SynthOscillator::SpectralMorph::kInharmonicScale));
    spectralMorph.Set("Smear", static_cast<int>(SynthOscillator::SpectralMorph::kSmear));
    spectralMorph.Set("RandomAmplitudes", static_cast<int>(SynthOscillator::SpectralMorph::kRandomAmplitudes));
    spectralMorph.Set("LowPass", static_cast<int>(SynthOscillator::SpectralMorph::kLowPass));
    spectralMorph.Set("HighPass", static_cast<int>(SynthOscillator::SpectralMorph::kHighPass));
    spectralMorph.Set("PhaseDisperse", static_cast<int>(SynthOscillator::SpectralMorph::kPhaseDisperse));
    spectralMorph.Set("ShepardTone", static_cast<int>(SynthOscillator::SpectralMorph::kShepardTone));
    spectralMorph.Set("Skew", static_cast<int>(SynthOscillator::SpectralMorph::kSkew));
    constants.Set("SpectralMorph", spectralMorph);
    
    // DistortionType enum
    Napi::Object distortionType = Napi::Object::New(env);
    distortionType.Set("None", static_cast<int>(SynthOscillator::DistortionType::kNone));
    distortionType.Set("Sync", static_cast<int>(SynthOscillator::DistortionType::kSync));
    distortionType.Set("Formant", static_cast<int>(SynthOscillator::DistortionType::kFormant));
    distortionType.Set("Quantize", static_cast<int>(SynthOscillator::DistortionType::kQuantize));
    distortionType.Set("Bend", static_cast<int>(SynthOscillator::DistortionType::kBend));
    distortionType.Set("Squeeze", static_cast<int>(SynthOscillator::DistortionType::kSqueeze));
    distortionType.Set("PulseWidth", static_cast<int>(SynthOscillator::DistortionType::kPulseWidth));
    distortionType.Set("FmOscillatorA", static_cast<int>(SynthOscillator::DistortionType::kFmOscillatorA));
    distortionType.Set("FmOscillatorB", static_cast<int>(SynthOscillator::DistortionType::kFmOscillatorB));
    distortionType.Set("FmSample", static_cast<int>(SynthOscillator::DistortionType::kFmSample));
    distortionType.Set("RmOscillatorA", static_cast<int>(SynthOscillator::DistortionType::kRmOscillatorA));
    distortionType.Set("RmOscillatorB", static_cast<int>(SynthOscillator::DistortionType::kRmOscillatorB));
    distortionType.Set("RmSample", static_cast<int>(SynthOscillator::DistortionType::kRmSample));
    constants.Set("DistortionType", distortionType);
    
    // UnisonStackType enum
    Napi::Object unisonStackType = Napi::Object::New(env);
    unisonStackType.Set("Normal", static_cast<int>(SynthOscillator::UnisonStackType::kNormal));
    unisonStackType.Set("CenterDropOctave", static_cast<int>(SynthOscillator::UnisonStackType::kCenterDropOctave));
    unisonStackType.Set("CenterDropOctave2", static_cast<int>(SynthOscillator::UnisonStackType::kCenterDropOctave2));
    unisonStackType.Set("Octave", static_cast<int>(SynthOscillator::UnisonStackType::kOctave));
    unisonStackType.Set("Octave2", static_cast<int>(SynthOscillator::UnisonStackType::kOctave2));
    unisonStackType.Set("PowerChord", static_cast<int>(SynthOscillator::UnisonStackType::kPowerChord));
    unisonStackType.Set("PowerChord2", static_cast<int>(SynthOscillator::UnisonStackType::kPowerChord2));
    unisonStackType.Set("MajorChord", static_cast<int>(SynthOscillator::UnisonStackType::kMajorChord));
    unisonStackType.Set("MinorChord", static_cast<int>(SynthOscillator::UnisonStackType::kMinorChord));
    unisonStackType.Set("HarmonicSeries", static_cast<int>(SynthOscillator::UnisonStackType::kHarmonicSeries));
    unisonStackType.Set("OddHarmonicSeries", static_cast<int>(SynthOscillator::UnisonStackType::kOddHarmonicSeries));
    constants.Set("UnisonStackType", unisonStackType);
    
    // RandomLFOStyle enum
    Napi::Object randomLFOStyle = Napi::Object::New(env);
    randomLFOStyle.Set("Perlin", static_cast<int>(RandomLfo::RandomType::kPerlin));
    randomLFOStyle.Set("SampleAndHold", static_cast<int>(RandomLfo::RandomType::kSampleAndHold));
    randomLFOStyle.Set("SinInterpolate", static_cast<int>(RandomLfo::RandomType::kSinInterpolate));
    randomLFOStyle.Set("LorenzAttractor", static_cast<int>(RandomLfo::RandomType::kLorenzAttractor));
    constants.Set("RandomLFOStyle", randomLFOStyle);
    
    // VoicePriority enum
    Napi::Object voicePriority = Napi::Object::New(env);
    voicePriority.Set("Newest", static_cast<int>(VoiceHandler::VoicePriority::kNewest));
    voicePriority.Set("Oldest", static_cast<int>(VoiceHandler::VoicePriority::kOldest));
    voicePriority.Set("Highest", static_cast<int>(VoiceHandler::VoicePriority::kHighest));
    voicePriority.Set("Lowest", static_cast<int>(VoiceHandler::VoicePriority::kLowest));
    voicePriority.Set("RoundRobin", static_cast<int>(VoiceHandler::VoicePriority::kRoundRobin));
    constants.Set("VoicePriority", voicePriority);
    
    // VoiceOverride enum
    Napi::Object voiceOverride = Napi::Object::New(env);
    voiceOverride.Set("Kill", static_cast<int>(VoiceHandler::VoiceOverride::kKill));
    voiceOverride.Set("Steal", static_cast<int>(VoiceHandler::VoiceOverride::kSteal));
    constants.Set("VoiceOverride", voiceOverride);
    
    // WaveShape enum
    Napi::Object waveShape = Napi::Object::New(env);
    waveShape.Set("Sin", static_cast<int>(PredefinedWaveFrames::kSin));
    waveShape.Set("SaturatedSin", static_cast<int>(PredefinedWaveFrames::kSaturatedSin));
    waveShape.Set("Triangle", static_cast<int>(PredefinedWaveFrames::kTriangle));
    waveShape.Set("Square", static_cast<int>(PredefinedWaveFrames::kSquare));
    waveShape.Set("Pulse", static_cast<int>(PredefinedWaveFrames::kPulse));
    waveShape.Set("Saw", static_cast<int>(PredefinedWaveFrames::kSaw));
    constants.Set("WaveShape", waveShape);
    
    // SynthLFOSyncType enum
    Napi::Object synthLFOSyncType = Napi::Object::New(env);
    synthLFOSyncType.Set("Trigger", static_cast<int>(SynthLfo::SyncType::kTrigger));
    synthLFOSyncType.Set("Sync", static_cast<int>(SynthLfo::SyncType::kSync));
    synthLFOSyncType.Set("Envelope", static_cast<int>(SynthLfo::SyncType::kEnvelope));
    synthLFOSyncType.Set("SustainEnvelope", static_cast<int>(SynthLfo::SyncType::kSustainEnvelope));
    synthLFOSyncType.Set("LoopPoint", static_cast<int>(SynthLfo::SyncType::kLoopPoint));
    synthLFOSyncType.Set("LoopHold", static_cast<int>(SynthLfo::SyncType::kLoopHold));
    constants.Set("SynthLFOSyncType", synthLFOSyncType);
    
    // CompressorBandOption enum
    Napi::Object compressorBandOption = Napi::Object::New(env);
    compressorBandOption.Set("Multiband", static_cast<int>(MultibandCompressor::BandOptions::kMultiband));
    compressorBandOption.Set("LowBand", static_cast<int>(MultibandCompressor::BandOptions::kLowBand));
    compressorBandOption.Set("HighBand", static_cast<int>(MultibandCompressor::BandOptions::kHighBand));
    compressorBandOption.Set("SingleBand", static_cast<int>(MultibandCompressor::BandOptions::kSingleBand));
    constants.Set("CompressorBandOption", compressorBandOption);
    
    // SynthFilterStyle enum  
    Napi::Object synthFilterStyle = Napi::Object::New(env);
    synthFilterStyle.Set("k12Db", static_cast<int>(SynthFilter::Style::k12Db));
    synthFilterStyle.Set("k24Db", static_cast<int>(SynthFilter::Style::k24Db));
    synthFilterStyle.Set("NotchPassSwap", static_cast<int>(SynthFilter::Style::kNotchPassSwap));
    synthFilterStyle.Set("DualNotchBand", static_cast<int>(SynthFilter::Style::kDualNotchBand));
    synthFilterStyle.Set("BandPeakNotch", static_cast<int>(SynthFilter::Style::kBandPeakNotch));
    synthFilterStyle.Set("Shelving", static_cast<int>(SynthFilter::Style::kShelving));
    constants.Set("SynthFilterStyle", synthFilterStyle);
    
    // SyncedFrequency enum (defined in bindings.cpp)
    Napi::Object syncedFrequency = Napi::Object::New(env);
    syncedFrequency.Set("k32_1", 0);
    syncedFrequency.Set("k16_1", 1);
    syncedFrequency.Set("k8_1", 2);
    syncedFrequency.Set("k4_1", 3);
    syncedFrequency.Set("k2_1", 4);
    syncedFrequency.Set("k1_1", 5);
    syncedFrequency.Set("k1_2", 6);
    syncedFrequency.Set("k1_4", 7);
    syncedFrequency.Set("k1_8", 8);
    syncedFrequency.Set("k1_16", 9);
    syncedFrequency.Set("k1_32", 10);
    syncedFrequency.Set("k1_64", 11);
    constants.Set("SyncedFrequency", syncedFrequency);
    
    // SynthLFOSyncOption enum (defined in bindings.cpp)
    Napi::Object synthLFOSyncOption = Napi::Object::New(env);
    synthLFOSyncOption.Set("Time", 0);
    synthLFOSyncOption.Set("Tempo", 1);
    synthLFOSyncOption.Set("DottedTempo", 2);
    synthLFOSyncOption.Set("TripletTempo", 3);
    synthLFOSyncOption.Set("Keytrack", 4);
    constants.Set("SynthLFOSyncOption", synthLFOSyncOption);
    
    return constants;
}

// Module functions
Napi::Value GetModulationSources(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    const auto &sources = ModulationListCache::getSources();
    
    Napi::Array result = Napi::Array::New(env, sources.size());
    for (size_t i = 0; i < sources.size(); ++i) {
        result.Set(i, sources[i]);
    }
    return result;
}

Napi::Value GetModulationDestinations(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    const auto &destinations = ModulationListCache::getDestinations();
    
    Napi::Array result = Napi::Array::New(env, destinations.size());
    for (size_t i = 0; i < destinations.size(); ++i) {
        result.Set(i, destinations[i]);
    }
    return result;
}

// Initialize the module
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // Initialize wrapper classes
    ControlValueWrapper::Init(env, exports);
    SynthWrapper::Init(env, exports);
    
    // Add module functions with both naming conventions
    exports.Set("getModulationSources", Napi::Function::New(env, GetModulationSources));
    exports.Set("getModulationDestinations", Napi::Function::New(env, GetModulationDestinations));
    exports.Set("get_modulation_sources", Napi::Function::New(env, GetModulationSources));
    exports.Set("get_modulation_destinations", Napi::Function::New(env, GetModulationDestinations));
    
    // Add constants object
    exports.Set("constants", CreateConstantsObject(env));
    
    return exports;
}

NODE_API_MODULE(vita, Init)