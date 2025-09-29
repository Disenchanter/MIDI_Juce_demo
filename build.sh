#!/bin/bash

# Piano Track Reader VST Plugin Build Script
# Piano Track Reader Build Script

echo "🎹 Piano Track Reader - Build Script"
echo "=================================="

# Check if we are in the correct directory
if [ ! -f "PianoTrackReader.jucer" ]; then
    echo "❌ Error: Please run this script from the project root directory"
    exit 1
fi

# Create build directory
echo "📁 Creating build directory..."
mkdir -p build
cd build

# Configure project
echo "⚙️  Configuring CMake project (generating Xcode project)..."
cmake .. -G "Xcode" -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "❌ CMake configuration failed"
    exit 1
fi

# Build project
echo "🔨 Building project..."
cmake --build . --config Release --parallel

if [ $? -ne 0 ]; then
    echo "❌ Build failed"
    exit 1
fi

echo "✅ Build successful!"

# Check if plugin files were generated
if [ -d "PianoTrackReader_artefacts/Release/VST3/PianoTrackReader.vst3" ]; then
    echo "🎉 VST3 plugin generated: build/PianoTrackReader_artefacts/Release/VST3/PianoTrackReader.vst3"
    
    # Ask if user wants to install to system directory
    read -p "Install plugin to system directory? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "📦 Installing plugin to system directory..."
        sudo cp -r "PianoTrackReader_artefacts/Release/VST3/PianoTrackReader.vst3" "/Library/Audio/Plug-Ins/VST3/"
        echo "✅ Plugin installed to /Library/Audio/Plug-Ins/VST3/"
    fi
fi

if [ -d "PianoTrackReader_artefacts/Release/AU/PianoTrackReader.component" ]; then
    echo "🎉 AU plugin generated: build/PianoTrackReader_artefacts/Release/AU/PianoTrackReader.component"
    
    # Ask if user wants to install AU plugin
    read -p "Install AU plugin to system directory? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "📦 Installing AU plugin to system directory..."
        sudo cp -r "PianoTrackReader_artefacts/Release/AU/PianoTrackReader.component" "/Library/Audio/Plug-Ins/Components/"
        echo "✅ AU plugin installed to /Library/Audio/Plug-Ins/Components/"
    fi
fi

if [ -f "PianoTrackReader_artefacts/Release/Standalone/PianoTrackReader.app" ]; then
    echo "🎉 Standalone application generated: build/PianoTrackReader_artefacts/Release/Standalone/PianoTrackReader.app"
fi

echo ""
echo "🎵 Build complete! You can now use the Piano Track Reader plugin in your DAW."
echo ""
echo "Usage Instructions:"
echo "1. Create a Software Instrument track in Logic Pro"
echo "2. Find and load 'Piano Track Reader' in the plugin list"
echo "3. Play piano notes and observe the note information in the plugin interface"
echo ""
echo "To rebuild, run: ./build.sh"