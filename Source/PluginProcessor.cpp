juce::AudioProcessorEditor* WingmanStyleAnalyzerAudioProcessor::createEditor()
{
    return new WingmanStyleAnalyzerAudioProcessorEditor (*this);
}
