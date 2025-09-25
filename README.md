# 钢琴轨道读取器 (Piano Track Reader)

一个使用JUCE框架开发的VST3音频插件，专门用于读取和分析钢琴轨道上的MIDI音符数据。

## 功能特性

### 核心功能
- 🎹 **实时MIDI输入处理** - 捕获来自DAW或MIDI设备的钢琴音符
- 🎵 **音符检测与识别** - 自动识别音符名称（如C4, D#5等）
- 📊 **实时显示** - 显示当前正在播放的音符和历史记录
- 🔍 **详细信息** - 显示音符编号、力度值和时间戳
- 📈 **统计信息** - 显示总检测音符数和当前播放音符数

### 用户界面
- 🖥️ **现代GUI界面** - 美观的渐变背景和清晰的信息显示
- 🎨 **颜色编码** - 不同类型的信息使用不同颜色区分
- 📜 **滚动显示** - 自动滚动显示历史音符记录
- 🧹 **清除功能** - 一键清除历史记录

## 技术规格

### 插件信息
- **格式**: VST3, AU, Standalone
- **类型**: MIDI效果器 / 分析工具
- **MIDI输入**: ✅ 支持
- **MIDI输出**: ✅ 支持（通过模式）
- **音频处理**: 直通模式（不改变音频信号）

### 系统要求
- **macOS**: 10.11 或更高版本（支持Intel和Apple Silicon）
- **Windows**: Windows 10 或更高版本
- **DAW**: 任何支持VST3或AU格式的数字音频工作站

## 构建说明

### 使用CMake构建（推荐）

```bash
# 克隆或下载项目
cd VST_demo

# 创建构建目录
mkdir build
cd build

# 配置项目（生成Xcode项目）
cmake .. -G "Xcode"

# 构建项目
cmake --build . --config Release

# 安装插件（可选）
cmake --install .
```

### 使用Projucer构建

1. 下载并安装[JUCE框架](https://juce.com/get-juce)
2. 打开Projucer
3. 打开 `PianoTrackReader.jucer` 文件
4. 根据需要调整设置
5. 生成项目文件（Xcode/Visual Studio）
6. 在生成的IDE中编译项目

## 使用方法

### 在Logic Pro中使用

1. 将编译好的VST3文件复制到插件目录：
   - macOS: `/Library/Audio/Plug-Ins/VST3/`
   
2. 启动Logic Pro并创建新项目

3. 创建一个钢琴轨道（Software Instrument）

4. 在轨道的插件插槽中加载"Piano Track Reader"

5. 播放MIDI音符或录制钢琴演奏

6. 观察插件界面中显示的音符信息

### 在其他DAW中使用

- **Ableton Live**: 在MIDI轨道上加载为MIDI效果
- **Pro Tools**: 在MIDI轨道的插件位置加载
- **Cubase/Nuendo**: 作为MIDI插件加载到MIDI轨道
- **Studio One**: 在钢琴轨道的效果链中加载

## 界面说明

### 主界面区域

1. **标题栏** - 显示插件名称
2. **统计信息** - 显示总检测音符数和当前播放音符数
3. **当前播放音符** - 实时显示正在播放的音符
4. **历史记录** - 显示最近检测到的50个音符事件
5. **清除按钮** - 清除所有历史记录

### 信息格式

```
[时间戳] ON/OFF 音符名称 (Note:音符编号, Vel:力度值)
例: [1.234s] ON C4 (Note:60, Vel:100)
```

## 开发者信息

### 项目结构

```
VST_demo/
├── Source/
│   ├── PluginProcessor.h/.cpp    # 主处理器类
│   ├── PluginEditor.h/.cpp       # GUI编辑器类
│   └── MidiNoteDetector.h/.cpp   # MIDI音符检测器
├── PianoTrackReader.jucer        # Projucer项目文件
├── CMakeLists.txt                # CMake构建配置
└── README.md                     # 本文件
```

### 核心类说明

- **PianoTrackReaderAudioProcessor**: 继承自AudioProcessor，处理音频和MIDI数据流
- **MidiNoteDetector**: 专门用于检测和管理MIDI音符事件的类
- **PianoTrackReaderAudioProcessorEditor**: GUI界面类，提供用户交互

## 扩展功能建议

- [ ] 音符持续时间计算
- [ ] 和弦检测功能
- [ ] 导出MIDI数据功能
- [ ] 节拍检测和量化
- [ ] 音符过滤器（按音符范围过滤）
- [ ] 音符统计分析（最常用音符等）

## 许可证

本项目基于JUCE框架开发，请遵循JUCE的许可证要求。

## 联系信息

如有问题或建议，请通过以下方式联系：
- 项目地址: [GitHub仓库地址]
- 邮箱: [开发者邮箱]

---

**注意**: 这是一个演示项目，主要用于学习JUCE框架和VST插件开发。在生产环境中使用前，建议进行充分的测试。