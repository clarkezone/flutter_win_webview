#ifndef PLUGINS_WEBVIEW_POPUPAUTH_WINDOWS_WEBVIEW_POPUPAUTH_H_
#define PLUGINS_WEBVIEW_POPUPAUTH_WINDOWS_WEBVIEW_POPUPAUTH_H_

// A plugin to allow resizing the window.

#include <flutter_plugin_registrar.h>

#ifdef FLUTTER_PLUGIN_IMPL
#define FLUTTER_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FLUTTER_PLUGIN_EXPORT __declspec(dllimport)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

FLUTTER_PLUGIN_EXPORT void FileChooserPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar);

#if defined(__cplusplus)
}  // extern "C"
#endif

#endif  // PLUGINS_WEBVIEW_POPUPAUTH_WINDOWS_WEBVIEW_POPUPAUTH_H_