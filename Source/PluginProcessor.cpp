#include "PluginProcessor.h"
#include "PluginEditor.h"

WingmanStyleAnalyzerAudioProcessor::WingmanStyleAnalyzerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       forwardFFT (fftOrder),
       window (fftSize, juce::dsp::WindowingFunction<float>::hann)
#endif
{
    std::fill (std::begin (fifo), std::end (fifo), 0.0f);
    std::fill (std::begin (fftData), std::end (fftData), 0.0f);
}

WingmanStyleAnalyzerAudioProcessor::~WingmanStyleAnalyzerAudioProcessor()
{
}

const juce::String WingmanStyleAnalyzerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WingmanStyleAnalyzerAudioProcessor::acceptsMidi() const { return false; }
bool WingmanStyleAnalyzerAudioProcessor::producesMidi() const { return true; }
bool WingmanStyleAnalyzerAudioProcessor::isMidiEffect() const { return false; }
double WingmanStyleAnalyzerAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int WingmanStyleAnalyzerAudioProcessor::getNumPrograms() { return 1; }
int WingmanStyleAnalyzerAudioProcessor::getCurrentProgram() { return 0; }
void WingmanStyleAnalyzerAudioProcessor::setCurrentProgram (int index) {}
const juce::String WingmanStyleAnalyzerAudioProcessor::getProgramName (int index) { return {}; }
void WingmanStyleAnalyzerAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

void WingmanStyleAnalyzerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fifoIndex = 0;
    nextFFTBlockReady = false;
    std::fill (std::begin (fifo), std::end (fifo), 0.0f);
    std::fill (std::begin (fftData), std::end (fftData), 0.0f);
}

void WingmanStyleAnalyzerAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WingmanStyleAnalyzerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}
#endif

void WingmanStyleAnalyzerAudioProcessor::pushNextSampleIntoFifo (float sample)
{
    if (fifoIndex == fftSize)
    {
        if (! nextFFTBlockReady)
        {
            std::fill (std::begin (fftData), std::end (fftData), 0.0f);
            std::copy (std::begin (fifo), std::end (fifo), std::begin (fftData));
            nextFFTBlockReady = true;
        }
        fifoIndex = 0;
    }
    fifo [fifoIndex++] = sample;
}

juce::String WingmanStyleAnalyzerAudioProcessor::convertHzToNote (float frequency)
{
    if (frequency <= 20.0f) return "--";
    
    static const juce::String noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    int midiNote = std::round (69.0f + 12.0f * std::log2 (frequency / 440.0f));
    int noteIndex = (midiNote % 12 + 12) % 12;
    int octave = (midiNote / 12) - 1;

    return noteNames[noteIndex] + juce::String (octave);
}

void WingmanStyleAnalyzerAudioProcessor::analyzeAudioBlock()
{
    window.multiplyWithWindowingTable (fftData, fftSize);
    forwardFFT.performFrequencyOnlyForwardTransform (fftData);

    float maxMagnitude = 0.0f;
    int maxIndex = 0;

    for (int i = 1; i < fftSize / 2; ++i)
    {
        if (fftData[i] > maxMagnitude)
        {
            maxMagnitude = fftData[i];
            maxIndex = i;
        }
    }

    if (maxMagnitude > 0.5f)
    {
        float sampleRate = getSampleRate();
        detectedPitchHz = (maxIndex * sampleRate) / fftSize;
        currentNoteName = convertHzToNote (detectedPitchHz);
    }
}

void WingmanStyleAnalyzerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();

    if (totalNumInputChannels > 0)
    {
        auto* channelData = buffer.getReadPointer (0);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            pushNextSampleIntoFifo (channelData[i]);
        }
    }

    if (nextFFTBlockReady)
    {
        analyzeAudioBlock();
        nextFFTBlockReady = false;
    }
}

bool WingmanStyleAnalyzerAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* WingmanStyleAnalyzerAudioProcessor::createEditor()
{
    return new WingmanStyleAnalyzerAudioProcessorEditor (*this);
}

void WingmanStyleAnalyzerAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void WingmanStyleAnalyzerAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WingmanStyleAnalyzerAudioProcessor();
}
