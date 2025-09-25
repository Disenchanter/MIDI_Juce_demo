cmake_minimum_required(VERSION 3.15)

# 锁定生成器为Xcode（仅在macOS上）
if(APPLE)
    set(CMAKE_GENERATOR "Xcode" CACHE STRING "Generator" FORCE)
endif()

# 项目设置
project(PianoTrackReader VERSION 1.0.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 使用本地的JUCE
add_subdirectory(JUCE)

# 创建VST3插件目标
juce_add_plugin(PianoTrackReader
    # VERSION ...                               # 在project()中设置
    # ICON_BIG ...                              # 大图标文件路径
    # ICON_SMALL ...                            # 小图标文件路径
    COMPANY_NAME "JUCEDemoCompany"              # 公司名称
    IS_SYNTH FALSE                              # 不是合成器
    NEEDS_MIDI_INPUT TRUE                       # 需要MIDI输入
    NEEDS_MIDI_OUTPUT TRUE                      # 需要MIDI输出
    IS_MIDI_EFFECT FALSE                        # 不是MIDI效果器
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE           # 编辑器不需要键盘焦点
    COPY_PLUGIN_AFTER_BUILD TRUE                # 构建后复制插件到系统目录
    PLUGIN_MANUFACTURER_CODE Jdco               # 制造商代码
    PLUGIN_CODE Ptrk                            # 插件代码
    FORMATS AU VST3 Standalone                  # 支持的格式
    PRODUCT_NAME "Piano Track Reader")          # 产品名称

# 设置输出目录到根目录的build文件夹
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# 对于JUCE插件，设置artifact输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/Release)

# 设置源文件
target_sources(PianoTrackReader
    PRIVATE
        PianoTrackReader/Source/PluginEditor.cpp
        PianoTrackReader/Source/PluginProcessor.cpp)

# 设置编译定义
target_compile_definitions(PianoTrackReader
    PUBLIC
        # JUCE插件特性
        JUCE_WEB_BROWSER=0  # 如果不需要Web浏览器功能
        JUCE_USE_CURL=0     # 如果不需要HTTP功能
        JUCE_VST3_CAN_REPLACE_VST2=0)

# 链接JUCE模块
target_link_libraries(PianoTrackReader
    PRIVATE
        # AudioPlugin为插件提供的基础设施，它本身又会链接到其他模块
        juce::juce_audio_utils
        juce::juce_audio_plugin_client
        juce::juce_audio_formats      # 支持MIDI文件读取
        juce::juce_gui_basics         # 支持文件选择对话框
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# 平台特定设置
if(APPLE)
    # 设置macOS部署目标 - JUCE 8.0.10支持最新的macOS版本
    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum OS X deployment version")
    
    # 支持Apple Silicon和Intel
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for macOS")
    
    # 代码签名配置
    set(MACOS_CODESIGN_IDENTITY "LiZhi Demo Developer" CACHE STRING "Code signing identity")
    
    # 如果没有设置签名身份，使用默认的本地签名
    if(NOT MACOS_CODESIGN_IDENTITY OR MACOS_CODESIGN_IDENTITY STREQUAL "")
        set(MACOS_CODESIGN_IDENTITY "-")
    endif()
    
    # macOS特定设置
    set_target_properties(PianoTrackReader PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "vst3"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${MACOS_CODESIGN_IDENTITY}"
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ""
        XCODE_ATTRIBUTE_PROVISIONING_PROFILE_SPECIFIER ""
        # 允许本地签名
        XCODE_ATTRIBUTE_CODE_SIGN_ALLOW_ENTITLEMENTS_MODIFICATION "YES"
        XCODE_ATTRIBUTE_CODE_SIGN_INJECT_BASE_ENTITLEMENTS "YES")
        
    # 添加编译标志 - JUCE 8.0.10已经支持最新SDK
    target_compile_definitions(PianoTrackReader PRIVATE
        JUCE_SILENCE_XCODE_15_LINKER_WARNING=1)
        
    # 为每个插件格式设置代码签名
    set_target_properties(PianoTrackReader_AU PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${MACOS_CODESIGN_IDENTITY}")
    set_target_properties(PianoTrackReader_VST3 PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${MACOS_CODESIGN_IDENTITY}")
    set_target_properties(PianoTrackReader_Standalone PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${MACOS_CODESIGN_IDENTITY}")
endif()

if(WIN32)
    # Windows特定设置
    target_compile_definitions(PianoTrackReader PRIVATE
        _CRT_SECURE_NO_WARNINGS)
endif()

# 调试输出信息
message(STATUS "Building PianoTrackReader")
message(STATUS "  - Formats: AU VST3 Standalone")
message(STATUS "  - MIDI Input: ON")
message(STATUS "  - MIDI Output: ON")
message(STATUS "  - C++ Standard: ${CMAKE_CXX_STANDARD}")

# 可选：创建安装目标
install(TARGETS PianoTrackReader
    COMPONENT PianoTrackReader
    BUNDLE DESTINATION /Library/Audio/Plug-Ins/VST3  # macOS VST3路径
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)