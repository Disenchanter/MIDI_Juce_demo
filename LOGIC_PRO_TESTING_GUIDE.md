# Piano Track Reader - Logic Pro 测试指南

## 方法一：在 Logic Pro 中测试 VST3 插件

### 安装步骤
1. **插件已安装到**：`~/Library/Audio/Plug-Ins/VST3/Piano Track Reader.vst3`
2. **重启 Logic Pro**（如果已经运行）
3. **重新扫描插件**：Logic Pro → Preferences → Plug-In Manager → Reset & Rescan Selection

### 在 Logic Pro 中使用步骤

#### 测试方法 A：使用 MIDI 轨道
1. **打开 Logic Pro**
2. **创建新项目** → 选择 "Empty Project" → 创建 "Software Instrument" 轨道
3. **加载插件**：
   - 在轨道上点击第一个插件槽位
   - 选择 "MIDI FX" → "Piano Track Reader"
4. **录制/播放 MIDI**：
   - 使用虚拟键盘演奏
   - 或录制 MIDI 音符
   - 或导入 MIDI 文件
5. **查看结果**：插件界面会实时显示音符信息

#### 测试方法 B：使用现有 MIDI 数据
1. **打开包含 MIDI 数据的项目**
2. **在 MIDI 轨道上插入插件**：
   - 选择轨道 → Track → New Channel EQ → MIDI FX → Piano Track Reader
3. **播放轨道**：插件会实时读取并显示音符信息

## 方法二：使用独立应用测试

### 启动独立应用
```bash
open "/Users/lizhi/Desktop/Internship/VST_demo/PianoTrackReader/build/PianoTrackReader_artefacts/Debug/Standalone/Piano Track Reader.app"
```

### 测试步骤
1. **启动应用**
2. **连接 MIDI 设备**：
   - USB MIDI 键盘
   - 虚拟 MIDI 键盘应用
   - 或使用电脑键盘（如果应用支持）
3. **演奏音符**：应用会实时显示音符信息

## 方法三：使用 AU 插件测试（推荐用于 Logic Pro）

### 安装 AU 插件
```bash
# 复制 AU 插件到系统目录
sudo cp -R "/Users/lizhi/Desktop/Internship/VST_demo/PianoTrackReader/build/PianoTrackReader_artefacts/Debug/AU/Piano Track Reader.component" "/Library/Audio/Plug-Ins/Components/"

# 或复制到用户目录
cp -R "/Users/lizhi/Desktop/Internship/VST_demo/PianoTrackReader/build/PianoTrackReader_artefacts/Debug/AU/Piano Track Reader.component" "~/Library/Audio/Plug-Ins/Components/"
```

### 在 Logic Pro 中使用 AU 插件
1. **重启 Logic Pro**
2. **加载插件**：Audio FX → "Piano Track Reader"（AU 版本）
3. **测试功能**

## 测试内容验证

### 应该看到的显示内容
```
=== Logic Pro Track Note Information ===

♪ Currently Playing Notes:
   C4 - Start Time: 2.450s
   E4 - Start Time: 2.460s

♫ Note History:
   G3 | Start: 1.200s | End: 1.800s | Duration: 0.600s
   C4 | Start: 0.500s | End: 1.100s | Duration: 0.600s
   E4 | Start: 0.510s | End: 1.110s | Duration: 0.600s
```

### 功能测试清单
- [ ] **音符检测**：按下键盘音符时显示音符名称
- [ ] **时间记录**：显示准确的开始和结束时间
- [ ] **实时显示**：当前播放音符实时更新
- [ ] **历史记录**：已结束音符显示在历史列表中
- [ ] **清除功能**：点击 "Clear History" 按钮清空历史
- [ ] **多音符**：同时按多个键显示多个音符

## 故障排除

### 插件未出现在 Logic Pro 中
1. **检查插件位置**：
   ```bash
   ls -la "~/Library/Audio/Plug-Ins/VST3/"
   ls -la "~/Library/Audio/Plug-Ins/Components/"
   ```
2. **重新扫描插件**：Logic Pro → Preferences → Plug-In Manager → Reset & Rescan
3. **检查插件兼容性**：确保插件是 64 位版本

### 插件无法加载
1. **检查代码签名**：
   ```bash
   codesign --verify --deep --strict --verbose=2 "~/Library/Audio/Plug-Ins/VST3/Piano Track Reader.vst3"
   ```
2. **重新签名**：
   ```bash
   codesign --force --deep --sign - "~/Library/Audio/Plug-Ins/VST3/Piano Track Reader.vst3"
   ```

### 无 MIDI 输入
1. **检查 MIDI 设备连接**
2. **在 Logic Pro 中检查 MIDI 输入设置**
3. **确保 MIDI 轨道已选中并录音就绪**

## 高级测试

### 测试不同 MIDI 场景
1. **单音符**：测试单个音符的开始/结束检测
2. **和弦**：测试多个同时按下的音符
3. **快速演奏**：测试快速连续音符的处理
4. **长音符**：测试长时间按住的音符
5. **踏板**：如果有延音踏板，测试其影响

### 性能测试
1. **CPU 使用率**：在 Logic Pro 中检查插件的 CPU 占用
2. **内存使用**：长时间运行后检查内存占用
3. **延迟测试**：测试 MIDI 输入到显示的延迟

## 开发调试

### 查看控制台日志
```bash
# 查看插件的调试输出
log stream --predicate 'process == "Logic Pro X"' --level debug
```

### 重新编译和测试循环
```bash
# 重新编译
cd /Users/lizhi/Desktop/Internship/VST_demo/PianoTrackReader
cmake --build build --config Debug

# 重新安装插件
cp -R "build/PianoTrackReader_artefacts/Debug/VST3/Piano Track Reader.vst3" "~/Library/Audio/Plug-Ins/VST3/"

# 在 Logic Pro 中重新加载插件
```

---

**推荐测试顺序**：
1. 先使用独立应用测试基本功能
2. 然后在 Logic Pro 中使用 AU 插件测试
3. 最后使用 VST3 插件进行完整测试

这样可以逐步验证插件在不同环境下的工作情况。