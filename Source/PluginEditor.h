#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class WingmanStyleAnalyzerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                                  private juce::Timer
{
public:
    explicit WingmanStyleAnalyzerAudioProcessorEditor(WingmanStyleAnalyzerAudioProcessor&);
    ~WingmanStyleAnalyzerAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    WingmanStyleAnalyzerAudioProcessor& processor;
    juce::Label title, chord, notes, hint, level;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WingmanStyleAnalyzerAudioProcessorEditor)
};
