#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>
#include <map>

// MIDI 事件结构体
struct MidiNoteEvent
{
    juce::String noteName;
    int noteNumber;
    double timeStamp;
    bool isNoteOn;
    
    MidiNoteEvent() : noteNumber(0), timeStamp(0.0), isNoteOn(false) {}
    
    MidiNoteEvent(const juce::String& name, int note, double time, bool on)
        : noteName(name), noteNumber(note), timeStamp(time), isNoteOn(on) {}
};

class PianoTrackReaderAudioProcessor : public juce::AudioProcessor
{
public:
    PianoTrackReaderAudioProcessor();
    ~PianoTrackReaderAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // 与编辑器通信的方法
    std::vector<MidiNoteEvent> getRecentNoteEvents();
    std::map<int, bool> getCurrentActiveNotes();
    void clearProcessedEvents();
    
    // 获取当前采样率
    double getCurrentSampleRate() const { return currentSampleRate; }
    
    // 将 MIDI 音符号转换为音符名称（公共方法）
    juce::String getNoteNameFromMidiNumber(int midiNoteNumber);

private:
    
    // 存储最近的 MIDI 事件
    std::vector<MidiNoteEvent> recentNoteEvents;
    std::map<int, bool> activeNotes;  // noteNumber -> isActive
    
    // 当前采样率
    double currentSampleRate = 44100.0;
    
    // 累积样本位置用于精确时间计算
    juce::int64 totalSampleCount = 0;
    
    // 线程安全锁
    juce::CriticalSection eventLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoTrackReaderAudioProcessor)
};
