#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "PluginProcessor.h"

// 音符信息结构体
struct NoteInfo
{
    juce::String noteName;
    double startTime;
    double endTime;
    bool isActive;  // 用来跟踪音符是否仍在播放
    
    NoteInfo() : startTime(0.0), endTime(0.0), isActive(false) {}
    
    NoteInfo(const juce::String& name, double start)
        : noteName(name), startTime(start), endTime(0.0), isActive(true) {}
};

class PianoTrackReaderAudioProcessorEditor : public juce::AudioProcessorEditor,
                                           private juce::Timer
{
public:
    PianoTrackReaderAudioProcessorEditor (PianoTrackReaderAudioProcessor&);
    ~PianoTrackReaderAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Timer callback 用于定期更新显示
    void timerCallback() override;
    
    // 更新音符显示列表
    void updateNoteDisplay();
    
    // 文件处理方法
    void loadMidiFile();
    void analyzeMidiFile(const juce::File& file);
    void displayFileAnalysis();
    void createSampleMidiFile(const juce::File& directory);
    
    // 格式化时间显示
    juce::String formatTime(double timeInSamples, double sampleRate);
    juce::String formatTimeFromTicks(int ticks, int ticksPerQuarter, double tempo);

    PianoTrackReaderAudioProcessor& audioProcessor;
    
    // UI 组件
    juce::TextEditor noteDisplayArea;
    juce::Label titleLabel;
    juce::TextButton clearButton;
    juce::TextButton loadFileButton;
    juce::Label filePathLabel;
    
    // 存储音符信息
    std::vector<NoteInfo> noteHistory;
    std::map<int, NoteInfo> activeNotes;  // MIDI note number -> NoteInfo
    
    // 文件相关
    juce::File currentMidiFile;
    juce::File lastBrowsedDirectory;
    bool fileLoaded = false;
    juce::String analysisResults;
    std::unique_ptr<juce::FileChooser> fileChooser;
    
    // 用于时间计算
    double lastSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoTrackReaderAudioProcessorEditor)
};
