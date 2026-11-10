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

Wingman3CloneAudioProcessorEditor::~Wingman3CloneAudioProcessorEditor()
{
    futureBassButton.removeListener(this);
    trapButton.removeListener(this);
    edmButton.removeListener(this);
    melodicButton.removeListener(this);
}

void Wingman3CloneAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(0xff121216));

    g.setColour (juce::Colours::white);
    g.setFont (22.0f);
    g.drawText ("WINGMAN 3 // STYLE ANALYZER (CLONE)", 25, 20, 450, 30, juce::Justification::left);

    g.setColour (juce::Colours::cyan.withAlpha(0.6f));
    g.drawHorizontalLine(60, 25.0f, (float)getWidth() - 25.0f);

    g.setColour (juce::Colour(0xff1a1d24));
    g.fillRoundedRectangle(420, 90, 300, 210, 8.0f);

    g.setColour (juce::Colours::cyan.withAlpha(0.3f));
    g.drawRoundedRectangle(420, 90, 300, 210, 8.0f, 1.5f);

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("STYLE ANALYSIS WAVE", 440, 105, 200, 20, juce::Justification::left);

    juce::Path wavePath;
    wavePath.startNewSubPath(440, 195);
    wavePath.cubicTo(480, 140, 520, 250, 560, 180);
    wavePath.cubicTo(600, 120, 640, 220, 700, 160);
    g.setColour(juce::Colours::cyan);
    g.strokePath(wavePath, juce::PathStrokeType(2.0f));

    g.setColour (juce::Colour(0xff1a1d24));
    g.fillRoundedRectangle(420, 320, 300, 130, 8.0f);
    g.setColour(juce::Colours::white);
    g.drawText("PRESET PROFILES", 440, 332, 200, 20, juce::Justification::left);
}

void Wingman3CloneAudioProcessorEditor::resized()
{
    energyKnob.setBounds(60, 140, 140, 160);
    energyLabel.setBounds(50, 310, 160, 20);

    futureBassButton.setBounds(440, 365, 135, 35);
    trapButton.setBounds(585, 365, 120, 35);
    edmButton.setBounds(440, 410, 135, 35);
    melodicButton.setBounds(585, 410, 120, 35);
}

void Wingman3CloneAudioProcessorEditor::buttonClicked (juce::Button* button)
{
    if (button == &futureBassButton) {
        // Future Bass logic
    }
    else if (button == &trapButton) {
        // Trap logic
    }
}
