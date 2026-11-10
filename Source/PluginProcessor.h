#pragma once
#include <JuceHeader.h>
#include <array>
#include <atomic>
#include <vector>

class WingmanStyleAnalyzerAudioProcessor : public juce::AudioProcessor
{
public:
    WingmanStyleAnalyzerAudioProcessor();
    ~WingmanStyleAnalyzerAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Wingman Style Analyzer"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 1.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

    juce::String getDetectedChord() const;
    juce::String getDetectedNotes() const;
    float getInputLevel() const { return inputLevel.load(); }

private:
    static constexpr int fftOrder = 12;
    static constexpr int fftSize = 1 << fftOrder;
    juce::dsp::FFT fft { fftOrder };
    juce::dsp::WindowingFunction<float> window {
        fftSize, juce::dsp::WindowingFunction<float>::hann
    };
    std::array<float, fftSize> fftData {};
    std::array<float, fftSize> ring {};
    int writePosition = 0;
    int samplesSinceAnalysis = 0;
    double currentSampleRate = 44100.0;
    std::atomic<float> inputLevel { 0.0f };
    std::atomic<int> chordRoot { -1 };
    std::atomic<int> chordQuality { 0 };
    std::atomic<int> lastMidiRoot { -1 };
    int samplesUntilNoteOff = 0;
    std::vector<int> activeMidiNotes;

    void analyzeWindow();
    void emitChordMidi(juce::MidiBuffer&, int samplePosition);
    juce::String chordName(int root, int quality) const;
    juce::String noteList(int root, int quality) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WingmanStyleAnalyzerAudioProcessor)
};
