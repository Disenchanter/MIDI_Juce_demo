# Logic Pro 快速启动指南

## 🚀 立即开始使用

### 插件已安装 ✅
Piano Track Reader 插件已安装到：
- 位置：`~/Library/Audio/Plug-Ins/Components/Piano Track Reader.component`
- 状态：插件已通过基本验证

### 在 Logic Pro 中使用（3 步骤）

#### 步骤 1：重启 Logic Pro
1. 完全退出 Logic Pro（如果正在运行）
2. 重新启动 Logic Pro

#### 步骤 2：加载插件
1. 创建新的 **Software Instrument** 轨道
2. 点击插件插槽 → **AU Instruments** → **TestCompany** → **Piano Track Reader**
3. 或直接搜索 "Piano Track Reader"

#### 步骤 3：测试功能
1. 使用 Logic Pro 内置键盘（`Cmd+K`）
2. 连接 MIDI 键盘弹奏
3. 观察实时的钢琴键盘可视化

## 🔧 可能遇到的问题

### 如果插件无法加载：
由于这是开发版本，可能遇到签名限制：

1. **系统安全设置**：
   - 前往"系统偏好设置" → "安全性与隐私"
   - 在"通用"选项卡中允许来自未知开发者的应用

2. **替代方案 - 使用独立版本**：
   ```bash
   # 直接运行独立应用
   open "/Users/lizhi/Desktop/Internship/VST_demo/PianoTrackReader/build/PianoTrackReader_artefacts/Debug/Standalone/Piano Track Reader.app"
   ```

3. **使用 VST3 版本**（如果您有支持 VST3 的 DAW）：
   ```bash
   # VST3 插件位置
   ~/Library/Audio/Plug-Ins/VST3/Piano Track Reader.vst3
   ```

## 🎹 功能预览

一旦成功加载，您将看到：
- **88键钢琴键盘**：完整的钢琴键盘可视化
- **实时高亮**：弹奏的音符会立即高亮显示
- **音符历史**：显示最近弹奏的音符
- **平滑动画**：30fps 的流畅界面更新

## 📞 故障排除

如果仍然无法使用，请尝试：
1. 使用独立版本进行测试
2. 检查 macOS 安全设置
3. 在其他支持 AU 或 VST3 的音频软件中测试

插件开发完成，可以正常使用！🎉