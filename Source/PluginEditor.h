#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class Wingman3CloneAudioProcessorEditor : public juce::AudioProcessorEditor,
                                           public juce::Button::Listener
{
public:
    Wingman3CloneAudioProcessorEditor (Wingman3CloneAudioProcessor&);
    ~Wingman3CloneAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button* button) override;

private:
    Wingman3CloneAudioProcessor& audioProcessor;

    juce::TextButton futureBassButton  { "FUTURE BASS" };
    juce::TextButton trapButton        { "TRAP" };
    juce::TextButton edmButton         { "EDM POP" };
    juce::TextButton melodicButton     { "MELODIC" };

    juce::Slider energyKnob;
    juce::Label energyLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wingman3CloneAudioProcessorEditor)
};
