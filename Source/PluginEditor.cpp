#include "PluginEditor.h"

WingmanStyleAnalyzerAudioProcessorEditor::WingmanStyleAnalyzerAudioProcessorEditor(
    WingmanStyleAnalyzerAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(420, 260);
    title.setText("WINGMAN-STYLE ANALYZER", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);
    title.setFont(juce::Font(20.0f, juce::Font::bold));
    addAndMakeVisible(title);

    chord.setText("Listening...", juce::dontSendNotification);
    chord.setJustificationType(juce::Justification::centred);
    chord.setFont(juce::Font(42.0f, juce::Font::bold));
    addAndMakeVisible(chord);

    notes.setText("Notes: --", juce::dontSendNotification);
    notes.setJustificationType(juce::Justification::centred);
    notes.setFont(juce::Font(19.0f));
    addAndMakeVisible(notes);

    level.setText("Input level: --", juce::dontSendNotification);
    level.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(level);

    hint.setText("Route audio through this plugin to analyze it. Detected triads are sent as MIDI.",
                 juce::dontSendNotification);
    hint.setJustificationType(juce::Justification::centred);
    hint.setFont(juce::Font(12.0f));
    addAndMakeVisible(hint);
    startTimerHz(10);
}

void WingmanStyleAnalyzerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff171a22));
    g.setColour(juce::Colour(0xff30394a));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(8.0f), 12.0f, 1.5f);
}

void WingmanStyleAnalyzerAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(18);
    title.setBounds(area.removeFromTop(38));
    area.removeFromTop(18);
    chord.setBounds(area.removeFromTop(62));
    notes.setBounds(area.removeFromTop(34));
    level.setBounds(area.removeFromTop(28));
    hint.setBounds(area.removeFromTop(48));
}

void WingmanStyleAnalyzerAudioProcessorEditor::timerCallback()
{
    chord.setText(processor.getDetectedChord(), juce::dontSendNotification);
    notes.setText("Notes: " + processor.getDetectedNotes(), juce::dontSendNotification);
    level.setText("Input peak: " + juce::String(processor.getInputLevel(), 3),
                  juce::dontSendNotification);
}
