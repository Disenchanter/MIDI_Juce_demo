# 在 Logic Pro 中使用 Piano Track Reader 插件指南

## 🎹 安装步骤

### 1. 插件安装 ✅
插件已成功安装到：
```
~/Library/Audio/Plug-Ins/Components/Piano Track Reader.component
```

### 2. 重启 Logic Pro
- 完全退出 Logic Pro（如果正在运行）
- 重新启动 Logic Pro 让其扫描新插件

## 🎵 在 Logic Pro 中使用插件

### 步骤 1：创建新项目
1. 打开 Logic Pro
2. 创建新项目或打开现有项目

### 步骤 2：添加软件乐器轨道
1. 点击 **Track** → **New Track** (或按 `Option+Cmd+N`)
2. 选择 **Software Instrument**
3. 点击 **Create**

### 步骤 3：加载 Piano Track Reader 插件
1. 在新创建的轨道上，点击 **Instrument** 插槽
2. 在弹出的插件菜单中查找：
   - **AU Instruments** → **TestCompany** → **Piano Track Reader**
   - 或搜索 "Piano Track Reader"

### 步骤 4：打开插件界面
1. 双击插件名称或点击 **Edit** 按钮
2. Piano Track Reader 的界面将打开，显示：
   - 88键钢琴键盘可视化界面
   - 活跃音符显示区域
   - 音符历史记录面板

## 🎹 测试插件功能

### 方法 1：使用屏幕键盘
1. 在 Logic Pro 中按 `Cmd+K` 打开屏幕键盘
2. 点击屏幕键盘上的按键
3. 观察 Piano Track Reader 界面中的实时高亮显示

### 方法 2：使用 MIDI 键盘
1. 连接 MIDI 键盘到您的 Mac
2. 在 Logic Pro 中确保 MIDI 输入已启用
3. 弹奏 MIDI 键盘
4. 观察插件界面中的实时音符可视化

### 方法 3：录制 MIDI 片段
1. 点击录音按钮开始录制
2. 弹奏一些音符
3. 停止录制
4. 播放录制的 MIDI 片段
5. 观察插件的实时显示

## 🔧 功能说明

### 主要特性
- **实时音符检测**：插件会检测所有输入的 MIDI 音符（A0-C8 范围）
- **可视化钢琴键盘**：88键完整钢琴键盘，活跃音符会高亮显示
- **音符历史**：显示最近弹奏的音符列表
- **实时更新**：30fps 的流畅界面更新

### 支持的音符范围
- **完整88键钢琴**：从 A0 (MIDI 21) 到 C8 (MIDI 108)
- **标准钢琴音域**：涵盖所有常用钢琴音符

## 📋 故障排除

### 如果插件没有出现在 Logic Pro 中：

1. **重新扫描插件**：
   ```bash
   # 重新启动 Logic Pro
   # 或在 Logic Pro 中：Preferences → Audio → General → Reset All Except Key Commands
   ```

2. **检查插件安装**：
   ```bash
   ls -la ~/Library/Audio/Plug-Ins/Components/ | grep Piano
   ```

3. **验证插件有效性**：
   ```bash
   auval -v aufx Ptr1 Tstc
   ```

4. **清除缓存**（如果需要）：
   ```bash
   rm -rf ~/Library/Caches/AudioUnitCache
   rm -rf ~/Library/Caches/com.apple.audiounits.cache
   ```

### 如果插件界面无法打开：
1. 确保您使用的是 macOS 10.15 或更高版本
2. 尝试在"系统偏好设置"→"安全性与隐私"中允许未知开发者的应用

## 🎶 使用技巧

### 最佳实践
1. **MIDI 输入设置**：确保在 Logic Pro 的 MIDI 设置中启用了您的 MIDI 设备
2. **缓冲区设置**：为了获得最佳实时性能，建议使用较小的音频缓冲区（64-128 samples）
3. **多轨使用**：可以在多个轨道上同时使用插件，每个实例都会独立显示该轨道的音符

### 创意用途
- **教学工具**：实时可视化学生的弹奏
- **作曲辅助**：在创作时观察和弦结构
- **表演可视化**：在现场表演中提供视觉反馈
- **MIDI 分析**：分析现有 MIDI 文件的音符分布

## 📞 支持信息

如果遇到任何问题，请检查：
- macOS 版本兼容性（需要 10.15+）
- Logic Pro 版本（建议使用最新版本）
- 系统安全设置

插件版本：1.0.0
开发基于：JUCE 8.0.10