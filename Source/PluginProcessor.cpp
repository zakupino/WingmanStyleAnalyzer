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
                       )
#endif
{
}

WingmanStyleAnalyzerAudioProcessor::~WingmanStyleAnalyzerAudioProcessor()
{
}

const juce::String WingmanStyleAnalyzerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WingmanStyleAnalyzerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WingmanStyleAnalyzerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WingmanStyleAnalyzerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WingmanStyleAnalyzerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WingmanStyleAnalyzerAudioProcessor::getNumPrograms()
{
    return 1;
}

int WingmanStyleAnalyzerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WingmanStyleAnalyzerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WingmanStyleAnalyzerAudioProcessor::getProgramName (int index)
{
    return {};
}

void WingmanStyleAnalyzerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void WingmanStyleAnalyzerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void WingmanStyleAnalyzerAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WingmanStyleAnalyzerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsSynth
    unreferencedParameter (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WingmanStyleAnalyzerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
    }
}

bool WingmanStyleAnalyzerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* WingmanStyleAnalyzerAudioProcessor::createEditor()
{
    return new WingmanStyleAnalyzerAudioProcessorEditor (*this);
}

void WingmanStyleAnalyzerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void WingmanStyleAnalyzerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WingmanStyleAnalyzerAudioProcessor();
}
