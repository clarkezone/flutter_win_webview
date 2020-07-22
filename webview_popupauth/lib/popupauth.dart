import 'package:flutter/services.dart';

/// The result of a popup web authentication operation.
class PopupAuthResult {
  /// Creates a new result object with the given state and paths.
  const PopupAuthResult({this.token, this.canceled});

  /// Whether or not the authentication operation was canceled by the user.
  final bool canceled;

  /// A token returned from authentication request. If [canceled] is true, it
  /// should always be empty.
  final String token;
}

/// A singleton object that controls web authentication interactions with windows.
class PopupAuthChannelController {
  PopupAuthChannelController._();

  /// The platform channel used to manage popup auth
  final _channel = new MethodChannel("flutter/webviewpopupauth");

  /// A reference to the singleton instance of the class.
  static final PopupAuthChannelController instance =
      new PopupAuthChannelController._();

  /// Shows a Webauth popup, returning a
  /// [PopupAuthResult] when complete.
  Future<PopupAuthResult> show(
    String url,
  ) async {
    final methodName = "WebviewPopupauth.Show.Open";
    final token = await _channel.invokeMethod<String>(methodName);
    return PopupAuthResult(token: token ?? [], canceled: token == null);
  }
}

/// Shows a modal web authentication dialog.
///
/// A number of configuration options are available:
/// - [url] URL to point browser to.
Future<PopupAuthResult> showOpenPanel(String url) async {
  return PopupAuthChannelController.instance.show(url);
}
