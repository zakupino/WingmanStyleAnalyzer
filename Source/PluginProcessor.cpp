#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

namespace
{
    const char* noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    const int majorIntervals[] = { 0, 4, 7 };
    const int minorIntervals[] = { 0, 3, 7 };
    const int dimIntervals[]   = { 0, 3, 6 };
    const int susIntervals[]   = { 0, 5, 7 };
}

WingmanStyleAnalyzerAudioProcessor::WingmanStyleAnalyzerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

void WingmanStyleAnalyzerAudioProcessor::prepareToPlay(double sampleRate, int)
{
    currentSampleRate = sampleRate;
    ring.fill(0.0f);
    fftData.fill(0.0f);
    writePosition = samplesSinceAnalysis = 0;
    samplesUntilNoteOff = 0;
    activeMidiNotes.clear();
    chordRoot.store(-1);
    chordQuality.store(0);
}

bool WingmanStyleAnalyzerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& in = layouts.getMainInputChannelSet();
    const auto& out = layouts.getMainOutputChannelSet();
    return (in == juce::AudioChannelSet::mono() || in == juce::AudioChannelSet::stereo())
        && in == out;
}

void WingmanStyleAnalyzerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                        juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = buffer.getNumSamples();
    const int channels = buffer.getNumChannels();

    float peak = 0.0f;
    for (int ch = 0; ch < channels; ++ch)
        peak = juce::jmax(peak, buffer.getMagnitude(ch, 0, numSamples));
    inputLevel.store(peak);

    for (int i = 0; i < numSamples; ++i)
    {
        float mono = 0.0f;
        for (int ch = 0; ch < channels; ++ch)
            mono += buffer.getSample(ch, i);
        mono /= (float) juce::jmax(1, channels);

        ring[(size_t) writePosition] = mono;
        writePosition = (writePosition + 1) % fftSize;
        ++samplesSinceAnalysis;

        if (samplesSinceAnalysis >= fftSize / 2)
        {
            samplesSinceAnalysis = 0;
            analyzeWindow();
        }
    }

    if (samplesUntilNoteOff > 0)
    {
        samplesUntilNoteOff -= numSamples;
        if (samplesUntilNoteOff <= 0)
        {
            for (int note : activeMidiNotes)
                midi.addEvent(juce::MidiMessage::noteOff(1, note), numSamples - 1);
            activeMidiNotes.clear();
        }
    }

    const int root = chordRoot.load();
    if (root >= 0 && root != lastMidiRoot.load())
        emitChordMidi(midi, 0);
}

void WingmanStyleAnalyzerAudioProcessor::analyzeWindow()
{
    for (int i = 0; i < fftSize; ++i)
        fftData[(size_t) i] = ring[(size_t) ((writePosition + i) % fftSize)];

    window.multiplyWithWindowingTable(fftData.data(), fftSize);
    fft.performFrequencyOnlyForwardTransform(fftData.data());

    std::array<float, 12> pitchEnergy {};
    const int minBin = juce::jmax(1, (int) std::floor(55.0 * fftSize / currentSampleRate));
    const int maxBin = juce::jmin(fftSize / 2 - 1, (int) std::ceil(1400.0 * fftSize / currentSampleRate));

    for (int bin = minBin; bin <= maxBin; ++bin)
    {
        const float magnitude = fftData[(size_t) bin];
        if (magnitude < fftData[(size_t) (bin - 1)] || magnitude < fftData[(size_t) (bin + 1)])
            continue;

        const double frequency = (double) bin * currentSampleRate / fftSize;
        const int midiNote = (int) std::round(69.0 + 12.0 * std::log2(frequency / 440.0));
        const int pitchClass = (midiNote % 12 + 12) % 12;
        pitchEnergy[(size_t) pitchClass] += magnitude;
    }

    const int* templates[] = { majorIntervals, minorIntervals, dimIntervals, susIntervals };
    const int sizes[] = { 3, 3, 3, 3 };
    float bestScore = 0.0f;
    int bestRoot = -1;
    int bestQuality = 0;

    for (int root = 0; root < 12; ++root)
        for (int quality = 0; quality < 4; ++quality)
        {
            float score = 0.0f;
            for (int k = 0; k < sizes[quality]; ++k)
                score += pitchEnergy[(size_t) ((root + templates[quality][k]) % 12)];
            if (score > bestScore)
            {
                bestScore = score;
                bestRoot = root;
                bestQuality = quality;
            }
        }

    float total = 0.0f;
    for (float v : pitchEnergy) total += v;
    if (total > 0.0001f && bestScore / total > 0.48f)
    {
        chordRoot.store(bestRoot);
        chordQuality.store(bestQuality);
    }
    else
    {
        chordRoot.store(-1);
    }
}

void WingmanStyleAnalyzerAudioProcessor::emitChordMidi(juce::MidiBuffer& midi, int samplePosition)
{
    const int root = chordRoot.load();
    const int quality = chordQuality.load();
    if (root < 0) return;

    for (int note : activeMidiNotes)
        midi.addEvent(juce::MidiMessage::noteOff(1, note), samplePosition);
    activeMidiNotes.clear();

    const int* intervals[] = { majorIntervals, minorIntervals, dimIntervals, susIntervals };
    for (int i = 0; i < 3; ++i)
    {
        const int note = 60 + root + intervals[quality][i];
        activeMidiNotes.push_back(note);
        midi.addEvent(juce::MidiMessage::noteOn(1, note, (juce::uint8) 96), samplePosition);
    }
    samplesUntilNoteOff = (int) (currentSampleRate * 0.8);
    lastMidiRoot.store(root);
}

juce::String WingmanStyleAnalyzerAudioProcessor::chordName(int root, int quality) const
{
    if (root < 0) return "Listening...";
    const char* suffix[] = { "", "m", "dim", "sus4" };
    return juce::String(noteNames[root]) + suffix[juce::jlimit(0, 3, quality)];
}

juce::String WingmanStyleAnalyzerAudioProcessor::noteList(int root, int quality) const
{
    if (root < 0) return "--";
    const int* intervals[] = { majorIntervals, minorIntervals, dimIntervals, susIntervals };
    return juce::String(noteNames[(root + intervals[quality][0]) % 12]) + "  "
        + noteNames[(root + intervals[quality][1]) % 12] + "  "
        + noteNames[(root + intervals[quality][2]) % 12];
}

juce::String WingmanStyleAnalyzerAudioProcessor::getDetectedChord() const
{
    return chordName(chordRoot.load(), chordQuality.load());
}

juce::String WingmanStyleAnalyzerAudioProcessor::getDetectedNotes() const
{
    return noteList(chordRoot.load(), chordQuality.load());
}

juce::AudioProcessorEditor* WingmanStyleAnalyzerAudioProcessor::createEditor()
{
    return new WingmanStyleAnalyzerAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WingmanStyleAnalyzerAudioProcessor();
}
