# Piano Track Reader

A VST3 audio plugin developed using the JUCE framework, specifically designed to read and analyze MIDI note data from piano tracks.

## Features

### Core Functionality
- 🎹 **Real-time MIDI Input Processing** - Captures piano notes from DAW or MIDI devices
- 🎵 **Note Detection & Recognition** - Automatically identifies note names (e.g., C4, D#5, etc.)
- 📊 **Real-time Display** - Shows currently playing notes and historical records
- 🔍 **Detailed Information** - Displays note numbers, velocity values, and timestamps
- 📈 **Statistics** - Shows total detected notes and current playing note count

### User Interface
- 🖥️ **Modern GUI** - Beautiful gradient background with clear information display
- 🎨 **Color Coding** - Different information types use different colors for distinction
- 📜 **Scrolling Display** - Automatically scrolls to show historical note records
- 🧹 **Clear Function** - One-click clear of historical records

## Technical Specifications

### Plugin Information
- **Format**: VST3, AU, Standalone
- **Type**: MIDI Effect / Analysis Tool
- **MIDI Input**: ✅ Supported
- **MIDI Output**: ✅ Supported (pass-through mode)
- **Audio Processing**: Pass-through mode (does not alter audio signal)

### System Requirements
- **macOS**: 10.11 or higher (supports Intel and Apple Silicon)
- **Windows**: Windows 10 or higher
- **DAW**: Any digital audio workstation that supports VST3 or AU formats

## Build Instructions

### Building with CMake (Recommended)

```bash
# Clone or download the project
cd VST_demo

# Create build directory
mkdir build
cd build

# Configure project (generate Xcode project)
cmake .. -G "Xcode"

# Build project
cmake --build . --config Release

# Install plugin (optional)
cmake --install .
```

### Building with Projucer

1. Download and install the [JUCE Framework](https://juce.com/get-juce)
2. Open Projucer
3. Open the `PianoTrackReader.jucer` file
4. Adjust settings as needed
5. Generate project files (Xcode/Visual Studio)
6. Compile the project in the generated IDE

## Usage

### Using in Logic Pro

1. Copy the compiled VST3 file to the plugin directory:
   - macOS: `/Library/Audio/Plug-Ins/VST3/`
   
2. Launch Logic Pro and create a new project

3. Create a piano track (Software Instrument)

4. Load "Piano Track Reader" in the track's plugin slot

5. Play MIDI notes or record piano performance

6. Observe the note information displayed in the plugin interface

### Using in Other DAWs

- **Ableton Live**: Load as MIDI effect on MIDI track
- **Pro Tools**: Load in plugin position on MIDI track
- **Cubase/Nuendo**: Load as MIDI plugin on MIDI track
- **Studio One**: Load in the effects chain of piano track

## Interface Guide

### Main Interface Areas

1. **Title Bar** - Displays plugin name
2. **Statistics** - Shows total detected notes and current playing notes
3. **Current Playing Notes** - Real-time display of notes being played
4. **History Log** - Shows the most recent 50 note events detected
5. **Clear Button** - Clears all historical records

### Information Format

```
[Timestamp] ON/OFF NoteName (Note:NoteNumber, Vel:VelocityValue)
Example: [1.234s] ON C4 (Note:60, Vel:100)
```

## Developer Information

### Project Structure

```
VST_demo/
├── Source/
│   ├── PluginProcessor.h/.cpp    # Main processor class
│   ├── PluginEditor.h/.cpp       # GUI editor class
│   └── MidiNoteDetector.h/.cpp   # MIDI note detector
├── PianoTrackReader.jucer        # Projucer project file
├── CMakeLists.txt                # CMake build configuration
└── README.md                     # This file
```

### Core Class Descriptions

- **PianoTrackReaderAudioProcessor**: Inherits from AudioProcessor, handles audio and MIDI data streams
- **MidiNoteDetector**: Specialized class for detecting and managing MIDI note events
- **PianoTrackReaderAudioProcessorEditor**: GUI interface class that provides user interaction

## Suggested Feature Extensions

- [ ] Note duration calculation
- [ ] Chord detection functionality
- [ ] Export MIDI data functionality
- [ ] Beat detection and quantization
- [ ] Note filter (filter by note range)
- [ ] Note statistical analysis (most frequently used notes, etc.)

## License

This project is developed based on the JUCE framework. Please comply with JUCE license requirements.

## Contact Information

For questions or suggestions, please contact through the following:
- Project Repository: [GitHub Repository URL]
- Email: [Developer Email]

---

**Note**: This is a demonstration project, primarily for learning the JUCE framework and VST plugin development. It is recommended to conduct thorough testing before using in production environments.