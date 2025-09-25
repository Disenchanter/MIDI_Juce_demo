cmake_minimum_required(VERSION 3.15)

project(PianoTrackReader VERSION 1.0.0)

# C++17 标准
set(CMAKE_CXX_STANDARD 17)

# 强制使用 Xcode 生成器
if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
    message(FATAL_ERROR "This project requires Xcode generator. Please use: cmake -G \"Xcode\" ...")
endif()

# macOS 部署目标
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

# 添加本地 JUCE 子目录
add_subdirectory(../JUCE _deps/juce-build)

# 添加插件目标
juce_add_plugin(PianoTrackReader
    VERSION 1.0.0
    COMPANY_NAME "TestCompany"
    BUNDLE_ID "com.TestCompany.PianoTrackReader"
    PLUGIN_MANUFACTURER_CODE "Tstc"
    PLUGIN_CODE "Ptr1"
    PLUGIN_NAME "Piano Track Reader"
    PLUGIN_DESCRIPTION "Piano track reader with MIDI note output"
    FORMATS AU VST3 Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    PRODUCT_NAME "Piano Track Reader"
)

# 源文件
target_sources(PianoTrackReader
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

# JUCE 模块
target_link_libraries(PianoTrackReader
    PRIVATE
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_audio_utils
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_basics
        juce::juce_core
        juce::juce_events
        juce::juce_graphics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# 预处理器定义
target_compile_definitions(PianoTrackReader
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_SILENCE_XCODE_15_LINKER_WARNING=1
        JUCE_DISABLE_DEPRECATION_WARNINGS=1
)
