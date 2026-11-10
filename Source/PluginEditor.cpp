#include "PluginProcessor.h"
#include "PluginEditor.h"

Wingman3CloneAudioProcessorEditor::Wingman3CloneAudioProcessorEditor (Wingman3CloneAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    auto setupButton = [this] (juce::TextButton& btn) {
        btn.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff1e2229));
        btn.setColour(juce::TextButton::textColourOnId, juce::Colours::cyan);
        btn.setColour(juce::TextButton::textColourOffId, juce::Colours::lightgrey);
        btn.addListener(this);
        addAndMakeVisible(btn);
    };

    setupButton(futureBassButton);
    setupButton(trapButton);
    setupButton(edmButton);
    setupButton(melodicButton);

    energyKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    energyKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(energyKnob);

    energyLabel.setText("ENERGY LEVEL", juce::dontSendNotification);
    energyLabel.setJustificationType(juce::Justification::centred);
    energyLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(energyLabel);

    setSize (750, 480);
}
