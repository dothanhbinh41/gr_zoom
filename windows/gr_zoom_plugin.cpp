#include "gr_zoom_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream> 

namespace gr_zoom
{

  // static
  void GrZoomPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "plugins.webcare/zoom_channel",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<GrZoomPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  GrZoomPlugin::GrZoomPlugin() {}

  GrZoomPlugin::~GrZoomPlugin() {}

  void GrZoomPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("getPlatformVersion") == 0)
    {
      std::ostringstream version_stream;
      version_stream << "Windows ";
      if (IsWindows10OrGreater())
      {
        version_stream << "10+";
      }
      else if (IsWindows8OrGreater())
      {
        version_stream << "8";
      }
      else if (IsWindows7OrGreater())
      {
        version_stream << "7";
      }
      result->Success(flutter::EncodableValue(version_stream.str()));
    }
    else if (method_call.method_name().compare("initZoom") == 0)
    {

      // const auto args = method_call.arguments();
      // const auto *mapArgs = std::get_if<flutter::EncodableMap>(args);
      // if (!mapArgs)
      // {
      //   result->Error("Record", "Call missing parameters");
      //   return;
      // }

      // std::string token;
      // GetValueFromEncodableMap(mapArgs, "jwtToken", token);
      // AuthenticateSDK(token);
      result->Success();
    }
    else if (method_call.method_name().compare("joinMeeting") == 0)
    {
      // const auto args = method_call.arguments();
      // const auto *mapArgs = std::get_if<flutter::EncodableMap>(args);
      // if (!mapArgs)
      // {
      //   result->Error("Record", "Call missing parameters");
      //   return;
      // }

      // std::string meetingId, meetingPassword;
      // GetValueFromEncodableMap(mapArgs, "meetingId", meetingId);
      // GetValueFromEncodableMap(mapArgs, "meetingPassword", meetingPassword);
      // JoinMeetingForAPIUser(meetingId, meetingPassword);
      result->Success();
    }
    else
    {
      result->NotImplemented();
    }
  }  
} // namespace gr_zoom
