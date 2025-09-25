#include "PluginProcessor.h"
#include "PluginEditor.h"

PianoTrackReaderAudioProcessor::PianoTrackReaderAudioProcessor()
    : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

PianoTrackReaderAudioProcessor::~PianoTrackReaderAudioProcessor()
{
}

const juce::String PianoTrackReaderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PianoTrackReaderAudioProcessor::acceptsMidi() const
{
   return true;
}

bool PianoTrackReaderAudioProcessor::producesMidi() const
{
    return false;
}

bool PianoTrackReaderAudioProcessor::isMidiEffect() const
{
    return false;
}

double PianoTrackReaderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PianoTrackReaderAudioProcessor::getNumPrograms()
{
    return 1;
}

int PianoTrackReaderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PianoTrackReaderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PianoTrackReaderAudioProcessor::getProgramName (int index)
{
    return {};
}

void PianoTrackReaderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void PianoTrackReaderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    totalSampleCount = 0;
}

void PianoTrackReaderAudioProcessor::releaseResources()
{
}

bool PianoTrackReaderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

juce::String PianoTrackReaderAudioProcessor::getNoteNameFromMidiNumber(int midiNoteNumber)
{
    const char* noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    int noteIndex = midiNoteNumber % 12;
    int octave = (midiNoteNumber / 12) - 1;
    return juce::String(noteNames[noteIndex]) + juce::String(octave);
}

void PianoTrackReaderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // 处理 MIDI 消息
    for (const auto metadata : midiMessages)
    {
        const auto midiMessage = metadata.getMessage();
        int samplePosition = metadata.samplePosition;
        
        // 计算绝对时间戳
        double absoluteTimeStamp = totalSampleCount + samplePosition;
        
        if (midiMessage.isNoteOn())
        {
            int noteNumber = midiMessage.getNoteNumber();
            juce::String noteName = getNoteNameFromMidiNumber(noteNumber);
            
            // 线程安全地添加事件
            juce::ScopedLock lock (eventLock);
            recentNoteEvents.push_back (MidiNoteEvent (noteName, noteNumber, absoluteTimeStamp, true));
            activeNotes[noteNumber] = true;
            
            // 输出到控制台
            juce::Logger::writeToLog("NOTE ON: " + noteName + " at time " + juce::String(absoluteTimeStamp));
        }
        else if (midiMessage.isNoteOff())
        {
            int noteNumber = midiMessage.getNoteNumber();
            juce::String noteName = getNoteNameFromMidiNumber(noteNumber);
            
            // 线程安全地添加事件
            juce::ScopedLock lock (eventLock);
            recentNoteEvents.push_back (MidiNoteEvent (noteName, noteNumber, absoluteTimeStamp, false));
            activeNotes[noteNumber] = false;
            
            // 输出到控制台
            juce::Logger::writeToLog("NOTE OFF: " + noteName + " at time " + juce::String(absoluteTimeStamp));
        }
    }
    
    // 更新总样本计数
    totalSampleCount += buffer.getNumSamples();
}

bool PianoTrackReaderAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PianoTrackReaderAudioProcessor::createEditor()
{
    return new PianoTrackReaderAudioProcessorEditor (*this);
}

void PianoTrackReaderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void PianoTrackReaderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

// 与编辑器通信的方法实现
std::vector<MidiNoteEvent> PianoTrackReaderAudioProcessor::getRecentNoteEvents()
{
    juce::ScopedLock lock (eventLock);
    return recentNoteEvents;
}

std::map<int, bool> PianoTrackReaderAudioProcessor::getCurrentActiveNotes()
{
    juce::ScopedLock lock (eventLock);
    return activeNotes;
}

void PianoTrackReaderAudioProcessor::clearProcessedEvents()
{
    juce::ScopedLock lock (eventLock);
    recentNoteEvents.clear();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PianoTrackReaderAudioProcessor();
}
