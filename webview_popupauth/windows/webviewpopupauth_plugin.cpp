#include "include/webviewpopupauth/webview_popup_auth.h"

#include <flutter/flutter_view.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <windows.h>

#include <string>
#include <vector>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

namespace {

using flutter::EncodableList;
using flutter::EncodableMap;
using flutter::EncodableValue;

// See channel_controller.dart for documentation.
const char kChannelName[] = "flutter_webview_plugin";
const char kShowOpenAuthWindowMethod[] = "WebviewPopupauth.Show.Open";

// Pointer to WebViewController
static wil::com_ptr<ICoreWebView2Controller> webviewController;

// Pointer to WebView window
static wil::com_ptr<ICoreWebView2> webviewWindow;


// Returns the top-level window that owns |view|.
HWND GetRootWindow(flutter::FlutterView *view) {
  return GetAncestor(view->GetNativeWindow(), GA_ROOT);
}

class WebviewPopupauthPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  // Creates a plugin that communicates on the given channel.
  WebviewPopupauthPlugin(flutter::PluginRegistrarWindows *registrar);

  virtual ~WebviewPopupauthPlugin();

 private:
  // Called when a method is called on the plugin channel;
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  void WebviewInit(std::string url);

  // The registrar for this plugin, for accessing the window.
  flutter::PluginRegistrarWindows *registrar_;
};

// static
void WebviewPopupauthPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), kChannelName,
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<WebviewPopupauthPlugin>(registrar);

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

WebviewPopupauthPlugin::WebviewPopupauthPlugin(flutter::PluginRegistrarWindows *registrar)
    : registrar_(registrar) {}

WebviewPopupauthPlugin::~WebviewPopupauthPlugin() {}

void WebviewPopupauthPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {

  OutputDebugStringA(method_call.method_name().c_str());
  if (method_call.method_name().compare("launch") == 0) {
    if (!method_call.arguments() || !method_call.arguments()->IsMap() || method_call.arguments()->MapValue().empty()) {
      result->Error("Bad Arguments", "Argument map missing or malformed");
      return;
    }
	auto url = method_call.arguments()->MapValue().at(flutter::EncodableValue("url")).StringValue();
	//std::wstring wstr(url.begin(), url.end());
    OutputDebugStringA(url.c_str());

	WebviewInit(url);

    //EncodableValue response("FAKEHASH");

    //result->Success(&response);
  }
  else if(method_call.method_name().compare("close") == 0) {
	  auto hwnd = registrar_->GetView()->GetNativeWindow();
	  CloseWindow(hwnd);
  }
  else {
    result->NotImplemented();
  }
}

}  // namespace

void WebviewPopupauthPlugin::WebviewInit(std::string url) {
	auto hwnd = registrar_->GetView()->GetNativeWindow();

	CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
		Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			[hwnd, url](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

				// Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
				env->CreateCoreWebView2Controller(hwnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
					[hwnd,url](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
						if (controller != nullptr) {
							webviewController = controller;
							webviewController->get_CoreWebView2(&webviewWindow);
						}

						// Add a few settings for the webview
						// The demo step is redundant since the values are the default settings
						ICoreWebView2Settings* Settings;
						webviewWindow->get_Settings(&Settings);
						Settings->put_IsScriptEnabled(TRUE);
						Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
						Settings->put_IsWebMessageEnabled(TRUE);

						// Resize WebView to fit the bounds of the parent window
						RECT bounds;
						GetClientRect(hwnd, &bounds);
						bounds.top += 50;
						bounds.left += 50;
						bounds.right -= 50;
						bounds.bottom -= 50;
						webviewController->put_Bounds(bounds);

						std::wstring wstr(url.begin(), url.end());
						webviewWindow->Navigate(wstr.c_str());

						return S_OK;
					}).Get());
				return S_OK;
			}).Get());

}

void WebviewPopupAuthRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  WebviewPopupauthPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}