#include "PluginEditor.h"
#include "PluginProcessor.h"

PianoTrackReaderAudioProcessorEditor::PianoTrackReaderAudioProcessorEditor (PianoTrackReaderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // 设置窗口大小
    setSize (600, 500);
    
    // 设置标题标签
    titleLabel.setText ("Piano Track Reader - MIDI File Analyzer", juce::dontSendNotification);
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    titleLabel.setFont (juce::Font (18.0f, juce::Font::bold));
    #pragma clang diagnostic pop
    titleLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (titleLabel);
    
    // 设置文件路径标签
    filePathLabel.setText ("No file loaded", juce::dontSendNotification);
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    filePathLabel.setFont (juce::Font (12.0f, juce::Font::plain));
    #pragma clang diagnostic pop
    filePathLabel.setJustificationType (juce::Justification::centredLeft);
    filePathLabel.setColour (juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible (filePathLabel);
    
    // 设置音符显示区域
    noteDisplayArea.setMultiLine (true);
    noteDisplayArea.setReturnKeyStartsNewLine (true);
    noteDisplayArea.setReadOnly (true);
    noteDisplayArea.setScrollbarsShown (true);
    noteDisplayArea.setCaretVisible (false);
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    noteDisplayArea.setFont (juce::Font (juce::Font::getDefaultMonospacedFontName(), 12.0f, juce::Font::plain));
    #pragma clang diagnostic pop
    noteDisplayArea.setText ("Click 'Load MIDI File' to analyze a MIDI file\nSupported formats: .mid, .midi");
    addAndMakeVisible (noteDisplayArea);
    
    // 设置加载文件按钮
    loadFileButton.setButtonText ("Load MIDI File");
    loadFileButton.onClick = [this]() { loadMidiFile(); };
    addAndMakeVisible (loadFileButton);
    
    // 设置清除按钮
    clearButton.setButtonText ("Clear Analysis");
    clearButton.onClick = [this]()
    {
        noteHistory.clear();
        activeNotes.clear();
        fileLoaded = false;
        currentMidiFile = juce::File();
        filePathLabel.setText ("No file loaded", juce::dontSendNotification);
        noteDisplayArea.setText ("Click 'Load MIDI File' to analyze a MIDI file\nSupported formats: .mid, .midi");
    };
    addAndMakeVisible (clearButton);
    
    // 启动定时器，每 50ms 更新一次显示
    startTimer (50);
    
    // 获取当前采样率
    lastSampleRate = audioProcessor.getSampleRate();
    if (lastSampleRate <= 0) lastSampleRate = 44100.0;
}

PianoTrackReaderAudioProcessorEditor::~PianoTrackReaderAudioProcessorEditor()
{
    stopTimer();
}

void PianoTrackReaderAudioProcessorEditor::paint (juce::Graphics& g)
{
    // 设置渐变背景
    g.fillAll (juce::Colour (0xff2d3748));
    
    auto bounds = getLocalBounds();
    juce::ColourGradient gradient (juce::Colour (0xff4a5568), 0, 0, 
                                  juce::Colour (0xff2d3748), 0, bounds.getHeight(), false);
    g.setGradientFill (gradient);
    g.fillRect (bounds);
    
    // 绘制边框
    g.setColour (juce::Colour (0xff718096));
    g.drawRect (bounds, 2);
}

void PianoTrackReaderAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (10);
    
    // 标题区域
    titleLabel.setBounds (bounds.removeFromTop (30));
    bounds.removeFromTop (5);
    
    // 文件路径显示区域
    filePathLabel.setBounds (bounds.removeFromTop (20));
    bounds.removeFromTop (10);
    
    // 按钮区域
    auto buttonArea = bounds.removeFromBottom (40);
    clearButton.setBounds (buttonArea.removeFromRight (120));
    buttonArea.removeFromRight (10);
    loadFileButton.setBounds (buttonArea.removeFromRight (120));
    bounds.removeFromBottom (10);
    
    // 音符显示区域
    noteDisplayArea.setBounds (bounds);
}

void PianoTrackReaderAudioProcessorEditor::timerCallback()
{
    // 在文件模式下，不需要实时更新
    // 只在文件分析完成后更新显示
    if (!fileLoaded)
    {
        // 可以在这里添加任何需要定期检查的逻辑
        return;
    }
}

void PianoTrackReaderAudioProcessorEditor::updateNoteDisplay()
{
    juce::String displayText;
    
    if (fileLoaded && !analysisResults.isEmpty())
    {
        displayText = analysisResults;
    }
    else if (fileLoaded)
    {
        displayText = "File loaded but no analysis results available.";
    }
    else
    {
        displayText = "No MIDI file loaded. Click 'Load MIDI File' to analyze a file.";
    }
    
    noteDisplayArea.setText(displayText, juce::dontSendNotification);
}

void PianoTrackReaderAudioProcessorEditor::loadMidiFile()
{
    juce::File initialDirectory;

    if (currentMidiFile.existsAsFile())
        initialDirectory = currentMidiFile.getParentDirectory();
    else if (lastBrowsedDirectory.isDirectory())
        initialDirectory = lastBrowsedDirectory;
    else
        initialDirectory = juce::File::getSpecialLocation (juce::File::userDocumentsDirectory);

    fileChooser = std::make_unique<juce::FileChooser> ("Select a MIDI file", initialDirectory, "*.mid;*.midi");

    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    fileChooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
    {
        auto resultFile = fc.getResult();

        if (resultFile.existsAsFile())
        {
            lastBrowsedDirectory = resultFile.getParentDirectory();
            currentMidiFile = resultFile;
            filePathLabel.setText ("Loading: " + resultFile.getFileName(), juce::dontSendNotification);
            analyzeMidiFile (resultFile);
        }
        else
        {
            if (! fileLoaded)
                noteDisplayArea.setText ("No MIDI file selected.", juce::dontSendNotification);

            fileLoaded = false;
        }

        fileChooser.reset();
    });
}

void PianoTrackReaderAudioProcessorEditor::analyzeMidiFile(const juce::File& file)
{
    juce::MidiFile midiFile;
    juce::FileInputStream fileStream (file);
    
    if (! fileStream.openedOk())
    {
        noteDisplayArea.setText ("Error: Could not open file\n" + file.getFullPathName());
        return;
    }
    
    if (! midiFile.readFrom (fileStream))
    {
        noteDisplayArea.setText ("Error: Could not parse MIDI file\n" + file.getFullPathName());
        return;
    }
    
    // 清除之前的数据
    noteHistory.clear();
    activeNotes.clear();
    
    // 获取MIDI文件信息
    int numTracks = midiFile.getNumTracks();
    int ticksPerQuarter = midiFile.getTimeFormat();
    
    juce::String analysisText;
    analysisText << "=== MIDI File Analysis ===\n\n";
    analysisText << "File: " << file.getFileName() << "\n";
    analysisText << "Tracks: " << numTracks << "\n";
    analysisText << "Ticks per Quarter Note: " << ticksPerQuarter << "\n\n";
    
    // 分析每个轨道
    int totalNotes = 0;
    std::map<int, int> noteCount; // note number -> count
    
    for (int trackIndex = 0; trackIndex < numTracks; ++trackIndex)
    {
        const juce::MidiMessageSequence* track = midiFile.getTrack (trackIndex);
        if (track->getNumEvents() == 0) continue;
        
        analysisText << "--- Track " << (trackIndex + 1) << " ---\n";
        
        int trackNotes = 0;
        std::map<int, double> activeNoteStarts; // note number -> start time
        
        for (int i = 0; i < track->getNumEvents(); ++i)
        {
            const juce::MidiMessageSequence::MidiEventHolder* eventHolder = track->getEventPointer (i);
            const juce::MidiMessage& message = eventHolder->message;
            double timeStamp = message.getTimeStamp();
            
            if (message.isNoteOn())
            {
                int noteNumber = message.getNoteNumber();
                juce::String noteName = audioProcessor.getNoteNameFromMidiNumber (noteNumber);
                
                activeNoteStarts[noteNumber] = timeStamp;
                noteCount[noteNumber]++;
                trackNotes++;
                totalNotes++;
                
                analysisText << "  " << formatTimeFromTicks (static_cast<int>(timeStamp), ticksPerQuarter, 120.0) 
                            << " - " << noteName << " ON\n";
            }
            else if (message.isNoteOff())
            {
                int noteNumber = message.getNoteNumber();
                juce::String noteName = audioProcessor.getNoteNameFromMidiNumber (noteNumber);
                
                if (activeNoteStarts.find (noteNumber) != activeNoteStarts.end())
                {
                    double startTime = activeNoteStarts[noteNumber];
                    double duration = timeStamp - startTime;
                    
                    analysisText << "  " << formatTimeFromTicks (static_cast<int>(timeStamp), ticksPerQuarter, 120.0) 
                                << " - " << noteName << " OFF (Duration: " 
                                << formatTimeFromTicks (static_cast<int>(duration), ticksPerQuarter, 120.0) << ")\n";
                    
                    activeNoteStarts.erase (noteNumber);
                }
            }
        }
        
        analysisText << "Track " << (trackIndex + 1) << " Notes: " << trackNotes << "\n\n";
    }
    
    // 显示统计信息
    analysisText << "=== Statistics ===\n";
    analysisText << "Total Notes: " << totalNotes << "\n";
    analysisText << "Unique Notes Used:\n";
    
    for (const auto& pair : noteCount)
    {
        juce::String noteName = audioProcessor.getNoteNameFromMidiNumber (pair.first);
        analysisText << "  " << noteName << ": " << pair.second << " times\n";
    }
    
    analysisResults = analysisText;
    noteDisplayArea.setText (analysisResults);
    fileLoaded = true;
    
    // 自动滚动到顶部
    noteDisplayArea.moveCaretToTop(false);
}

juce::String PianoTrackReaderAudioProcessorEditor::formatTime (double timeInSamples, double sampleRate)
{
    if (sampleRate <= 0) return "0.000s";
    
    double timeInSeconds = timeInSamples / sampleRate;
    
    if (timeInSeconds < 60.0)
    {
        return juce::String (timeInSeconds, 3) + "s";
    }
    else
    {
        int minutes = (int) (timeInSeconds / 60.0);
        double seconds = timeInSeconds - (minutes * 60.0);
        return juce::String (minutes) + ":" + juce::String (seconds, 3) + "s";
    }
}

juce::String PianoTrackReaderAudioProcessorEditor::formatTimeFromTicks (int ticks, int ticksPerQuarter, double tempo)
{
    // 将 ticks 转换为秒
    // 假设默认 tempo = 120 BPM (每分钟120拍)
    double secondsPerBeat = 60.0 / tempo;
    double secondsPerTick = secondsPerBeat / ticksPerQuarter;
    double timeInSeconds = ticks * secondsPerTick;
    
    if (timeInSeconds < 60.0)
    {
        return juce::String (timeInSeconds, 3) + "s";
    }
    else
    {
        int minutes = (int) (timeInSeconds / 60.0);
        double seconds = timeInSeconds - (minutes * 60.0);
        return juce::String (minutes) + ":" + juce::String (seconds, 3) + "s";
    }
}

void PianoTrackReaderAudioProcessorEditor::createSampleMidiFile(const juce::File& directory)
{
    // 创建一个简单的示例MIDI文件
    juce::MidiFile midiFile;
    juce::MidiMessageSequence track;
    
    // 设置一些基本参数
    int ticksPerQuarter = 480;
    midiFile.setTicksPerQuarterNote(ticksPerQuarter);
    
    // 添加一些示例音符 (C大调音阶)
    juce::Array<int> notes = {60, 62, 64, 65, 67, 69, 71, 72}; // C4 to C5
    
    for (int i = 0; i < notes.size(); ++i)
    {
        int noteNumber = notes[i];
        int startTime = i * ticksPerQuarter / 2; // 每个音符间隔半拍
        int duration = ticksPerQuarter / 4; // 每个音符持续四分之一拍
        
        // 添加Note On消息
        juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, noteNumber, (juce::uint8)100);
        track.addEvent(noteOn, startTime);
        
        // 添加Note Off消息
        juce::MidiMessage noteOff = juce::MidiMessage::noteOff(1, noteNumber);
        track.addEvent(noteOff, startTime + duration);
    }
    
    // 添加轨道到MIDI文件
    midiFile.addTrack(track);
    
    // 保存文件
    juce::File sampleFile = directory.getChildFile("sample.mid");
    juce::FileOutputStream outputStream(sampleFile);
    
    if (outputStream.openedOk())
    {
        midiFile.writeTo(outputStream);
        outputStream.flush();
        
        // 分析刚创建的文件
        currentMidiFile = sampleFile;
        filePathLabel.setText ("Created and loading: " + sampleFile.getFileName(), juce::dontSendNotification);
        analyzeMidiFile (sampleFile);
    }
    else
    {
        noteDisplayArea.setText ("Failed to create sample MIDI file.\nPlease check write permissions.", juce::dontSendNotification);
    }
}
