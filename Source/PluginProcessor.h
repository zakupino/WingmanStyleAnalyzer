#pragma once

#include <JuceHeader.h>

class WingmanStyleAnalyzerAudioProcessor  : public juce::AudioProcessor
{
public:
    WingmanStyleAnalyzerAudioProcessor();
    ~WingmanStyleAnalyzerAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Pitch & Chord Analysis Engine
    float getDetectedPitch() const { return detectedPitchHz; }
    juce::String getDetectedNoteName() const { return currentNoteName; }

private:
    static constexpr auto fftOrder = 10;
    static constexpr auto fftSize  = 1 << fftOrder;
    
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;

    float fifo [fftSize];
    float fftData [2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;

    float detectedPitchHz = 0.0f;
    juce::String currentNoteName = "C";

    void pushNextSampleIntoFifo (float sample);
    void analyzeAudioBlock();
    juce::String convertHzToNote (float frequency);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WingmanStyleAnalyzerAudioProcessor)
};
