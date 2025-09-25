#!/bin/bash

echo "🎹 Piano Track Reader - Test Script"
echo "=================================="

# 检查应用是否存在
APP_PATH="/Users/lizhi/Desktop/Internship/VST_demo/PianoTrackReader/build/PianoTrackReader_artefacts/Debug/Standalone/Piano Track Reader.app"

if [ -d "$APP_PATH" ]; then
    echo "✅ 找到独立应用"
    echo "📂 路径: $APP_PATH"
    
    # 清理扩展属性并重新签名
    echo "🔧 清理和重新签名应用..."
    xattr -cr "$APP_PATH"
    codesign --force --deep --sign - "$APP_PATH"
    
    echo "🚀 启动 Piano Track Reader..."
    open "$APP_PATH"
    
    echo ""
    echo "💡 测试说明："
    echo "1. 应用启动后，连接 MIDI 键盘或使用虚拟键盘"
    echo "2. 演奏音符查看实时显示"
    echo "3. 使用 'Clear History' 按钮清除历史记录"
    echo ""
    echo "🎯 Logic Pro 测试："
    echo "1. 插件已安装到:"
    echo "   - VST3: ~/Library/Audio/Plug-Ins/VST3/"
    echo "   - AU: ~/Library/Audio/Plug-Ins/Components/"
    echo "2. 在 Logic Pro 中重新扫描插件后即可使用"
    
else
    echo "❌ 未找到独立应用"
    echo "请先编译项目: cmake --build build --config Debug"
fi