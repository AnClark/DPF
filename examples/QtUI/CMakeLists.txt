# CMake file for DISTRHO Plugins #
# ------------------------------ #

# Qt-specific CMake options
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt6 now requires compiler to support C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and load Qt libraries, and setup Qt environment
find_package(Qt6 REQUIRED COMPONENTS Widgets)

dpf_add_plugin(d_QtUI
  TARGETS jack lv2 vst2 vst3 clap
  FILES_DSP
      QtExamplePlugin.cpp
  FILES_UI
      QtExamplePluginUI.cpp
      PluginWindow.cpp
      PluginWindow.ui)

target_include_directories(
  d_QtUI PUBLIC ".")

# Must explicitly include Qt include directories for DSP and UI part,
# since they will not be applied by dpf_add_plugin().
# Read more: https://doc.qt.io/qt-6/cmake-variable-reference.html#module-variables
target_include_directories(d_QtUI-dsp PRIVATE ${Qt6Widgets_INCLUDE_DIRS})
target_include_directories(d_QtUI-ui PRIVATE ${Qt6Widgets_INCLUDE_DIRS})

# Remember to link against Qt libraries
target_link_libraries(d_QtUI PRIVATE Qt6::Widgets)
