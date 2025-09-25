#!/bin/bash

# Piano Track Reader VST Plugin Build Script
# 钢琴轨道读取器构建脚本

echo "🎹 Piano Track Reader - 构建脚本"
echo "=================================="

# 检查是否在正确的目录
if [ ! -f "PianoTrackReader.jucer" ]; then
    echo "❌ 错误: 请在项目根目录运行此脚本"
    exit 1
fi

# 创建构建目录
echo "📁 创建构建目录..."
mkdir -p build
cd build

# 配置项目
echo "⚙️  配置CMake项目 (生成Xcode项目)..."
cmake .. -G "Xcode" -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "❌ CMake配置失败"
    exit 1
fi

# 构建项目
echo "🔨 构建项目..."
cmake --build . --config Release --parallel

if [ $? -ne 0 ]; then
    echo "❌ 构建失败"
    exit 1
fi

echo "✅ 构建成功!"

# 检查插件文件是否生成
if [ -d "PianoTrackReader_artefacts/Release/VST3/PianoTrackReader.vst3" ]; then
    echo "🎉 VST3插件已生成: build/PianoTrackReader_artefacts/Release/VST3/PianoTrackReader.vst3"
    
    # 询问是否安装到系统目录
    read -p "是否将插件安装到系统目录? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "📦 安装插件到系统目录..."
        sudo cp -r "PianoTrackReader_artefacts/Release/VST3/PianoTrackReader.vst3" "/Library/Audio/Plug-Ins/VST3/"
        echo "✅ 插件已安装到 /Library/Audio/Plug-Ins/VST3/"
    fi
fi

if [ -d "PianoTrackReader_artefacts/Release/AU/PianoTrackReader.component" ]; then
    echo "🎉 AU插件已生成: build/PianoTrackReader_artefacts/Release/AU/PianoTrackReader.component"
    
    # 询问是否安装AU插件
    read -p "是否将AU插件安装到系统目录? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "📦 安装AU插件到系统目录..."
        sudo cp -r "PianoTrackReader_artefacts/Release/AU/PianoTrackReader.component" "/Library/Audio/Plug-Ins/Components/"
        echo "✅ AU插件已安装到 /Library/Audio/Plug-Ins/Components/"
    fi
fi

if [ -f "PianoTrackReader_artefacts/Release/Standalone/PianoTrackReader.app" ]; then
    echo "🎉 独立应用已生成: build/PianoTrackReader_artefacts/Release/Standalone/PianoTrackReader.app"
fi

echo ""
echo "🎵 构建完成! 现在可以在DAW中使用Piano Track Reader插件了。"
echo ""
echo "使用说明:"
echo "1. 在Logic Pro中创建一个Software Instrument轨道"
echo "2. 在插件列表中找到并加载 'Piano Track Reader'"
echo "3. 播放钢琴音符，观察插件界面中的音符信息"
echo ""
echo "如需重新构建，请运行: ./build.sh"