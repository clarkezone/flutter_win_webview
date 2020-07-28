cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "webviewpopupauth")
project(${PROJECT_NAME} LANGUAGES CXX)

set(PLUGIN_NAME "${PROJECT_NAME}_plugin")

find_program(NUGET_EXE NAMES nuget)
if(NOT NUGET_EXE)
	message("NUGET.EXE not found.")
	message(FATAL_ERROR "Please install this executable, and run CMake again.")
endif()

exec_program(${NUGET_EXE}
    ARGS install "Microsoft.Web.WebView2" -Version 0.9.579 -ExcludeVersion -OutputDirectory ${CMAKE_BINARY_DIR}/packages)
exec_program(${NUGET_EXE}
    ARGS install "Microsoft.Windows.ImplementationLibrary" -Version 1.0.200519.2 -ExcludeVersion -OutputDirectory ${CMAKE_BINARY_DIR}/packages)

add_library(${PLUGIN_NAME} SHARED
  "${PLUGIN_NAME}.cpp"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)

target_link_libraries(${PLUGIN_NAME} PRIVATE ${CMAKE_BINARY_DIR}/packages/Microsoft.Web.WebView2/build/native/Microsoft.Web.WebView2.targets)
target_link_libraries(${PLUGIN_NAME} PRIVATE ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.ImplementationLibrary/build/native/Microsoft.Windows.ImplementationLibrary.targets)

target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_compile_definitions(${PLUGIN_NAME} PRIVATE _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)

# List of absolute paths to libraries that should be bundled with the plugin
set(webviewpopupauth_bundled_libraries
  ""
  PARENT_SCOPE
)