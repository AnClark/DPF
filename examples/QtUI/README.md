# QtUI Example

This example will show how to implement a Qt-based UI to DPF. It's based on EmbedExternalUI example.

DPF supports external UI. If you prefer building UI with other toolkits, and expect to use them with DPF, this example may help.

You can use this example as a template for your own implementation.

## Files

| Filename                               | Description                                                  |
| -------------------------------------- | ------------------------------------------------------------ |
| `CMakeLists.txt`                       | CMake config file for this example. It includes some essential tricks to let DPF build and link with Qt properly. Don't forget to take a look at it. |
| `DistrhoPluginInfo.h`                  | DPF plugin information.                                      |
| `QtExamplePlugin.cpp`                  | Plugin's DSP part. At there it also initializes QApplication instance. |
| `QtExamplePluginUI.cpp`                | Plugin's UI part. At there it manipulates the Qt GUI.        |
| `PluginWindow.cpp`<br>`PluginWindow.h` | C++ code for the Qt GUI.                                     |
| `PluginWindow.ui`                      | Qt Designer UI code for the Qt GUI.                          |

## Notice

This example only supports CMakeLists.txt; Makefile is not supported. Because, handling Qt configurations and dependencies in Makefile means tons of work. CMake is preferred, as it supports Qt well, and builds faster.

Supports for external UI in CMake is required. I (AnClark) have implemented this feature, but it has not been applied to the upstream DPF yet. You can clone my fork, and give it a try.