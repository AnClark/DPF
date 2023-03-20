# CMake file for DISTRHO Plugins #
# ------------------------------ #

set(CMAKE_CXX_STANDARD 17)  # std::make_unique() is not included in C++11

# Load FLTK dependencies
set (OpenGL_GL_PREFERENCE GLVND)  # Suppress CMP0072
find_package(FLTK REQUIRED)

# Compile FLUID source code
find_program (FLTK_FLUID_EXECUTABLE fluid REQUIRED)
fltk_wrap_ui (d_fltk_ui-ui FltkTestUI.fl)

dpf_add_plugin(d_fltk_ui
  TARGETS lv2 vst2 vst3 clap
  UI_TYPE external
  FILES_DSP
    FltkExamplePlugin.cpp
  FILES_UI
    FltkExampleUI.cpp
    ${d_fltk_ui-ui_FLTK_UI_SRCS})

target_include_directories(d_fltk_ui PUBLIC
  ${FLTK_INCLUDE_DIR}
  ".")

target_link_libraries(d_fltk_ui-ui PRIVATE ${FLTK_LIBRARIES})
