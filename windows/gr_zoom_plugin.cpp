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
#include "zoom_sdk.h"
#include "meeting_service_interface.h"
#include "auth_service_interface.h"

namespace gr_zoom
{

  // static
  void GrZoomPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "gr_zoom",
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

      const auto args = method_call.arguments();
      const auto *mapArgs = std::get_if<flutter::EncodableMap>(args);
      if (!mapArgs)
      {
        result->Error("Record", "Call missing parameters");
        return;
      }

      std::string token;
      GetValueFromEncodableMap(mapArgs, "jwtToken", token);
      AuthenticateSDK(token);
      result->Success();
    }
    else if (method_call.method_name().compare("joinMeeting") == 0)
    {
      const auto args = method_call.arguments();
      const auto *mapArgs = std::get_if<flutter::EncodableMap>(args);
      if (!mapArgs)
      {
        result->Error("Record", "Call missing parameters");
        return;
      }

      std::string meetingId, meetingPassword;
      GetValueFromEncodableMap(mapArgs, "meetingId", meetingId);
      GetValueFromEncodableMap(mapArgs, "meetingPassword", meetingPassword);
      JoinMeetingForAPIUser(meetingId, meetingPassword);
      result->Success();
    }
    else
    {
      result->NotImplemented();
    }
  }

  void AuthenticateSDK(const std::string &token)
  {
    std::wstring jwtToken = std::wstring(token.begin(), token.end());
    const wchar_t *jwtTokenChar = jwtToken.c_str();
    // Initialize SDK with InitParam object
    ZOOM_SDK_NAMESPACE::InitParam initParam;
    ZOOM_SDK_NAMESPACE::SDKError initReturnVal(ZOOM_SDK_NAMESPACE::SDKERR_UNINITIALIZE);
    // Set web domain to zoom.us
    initParam.strWebDomain = L"https://zoom.us";
    initReturnVal = ZOOM_SDK_NAMESPACE::InitSDK(initParam);
    // Check if InitSDK call succeeded
    if (initReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
    {
      // Create IAuthService object to perform Auth actions
      ZOOM_SDK_NAMESPACE::IAuthService *authService;
      ZOOM_SDK_NAMESPACE::SDKError authServiceInitReturnVal = ZOOM_SDK_NAMESPACE::CreateAuthService(&authService);
      if (authServiceInitReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
      {
        // Create IAuthServiceEvent object to listen for Auth events from SDK
        // ZOOM_SDK_NAMESPACE::IAuthServiceEvent *authListener;
        // Auth SDK with AuthContext object
        ZOOM_SDK_NAMESPACE::AuthContext authContext;
        ZOOM_SDK_NAMESPACE::SDKError authCallReturnValue(ZOOM_SDK_NAMESPACE::SDKERR_UNAUTHENTICATION);
        // Call SetEvent to assign your IAuthServiceEvent listener
        // yourAuthServiceEventListener = new YourAuthServiceEventListener();
        // authListener = yourAuthServiceEventListener;
        // authService->SetEvent(authListener);
        // Provide your JWT to the AuthContext object
        authContext.jwt_token = jwtTokenChar;
        authCallReturnValue = authService->SDKAuth(authContext);
        if (authCallReturnValue == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
        {
          // SDK Auth in progress
        }
      }
    }
  }
 

  // void yourAuthServiceEventListener::onAuthenticationReturn(ZOOM_SDK_NAMESPACE::AuthResult ret)
  // {
  //   if (ret == ZOOM_SDK_NAMESPACE::AuthResult::AUTHRET_JWTTOKENWRONG)
  //   {
  //     // SDK Auth call failed because the JWT token is invalid.
  //   }
  //   else if (ret == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
  //   {
  //     // SDK Authenticated successfully
  //   }
  // }

  void JoinMeetingForAPIUser(const std::string &meetingId, const std::string &meetingPassword)
  {

    ZOOM_SDK_NAMESPACE::IMeetingService *meetingService;
    ZOOM_SDK_NAMESPACE::SDKError meetingServiceInitReturnVal = ZOOM_SDK_NAMESPACE::CreateMeetingService(&meetingService);
    if (meetingServiceInitReturnVal == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
    {
      // MeetingService was created successfully, it is recommended to store this value for use across the application
    }

    UINT64 meetingIdLong = std::stoull(meetingId);
    std::wstring meetingPasswordStr = std::wstring(meetingPassword.begin(), meetingPassword.end());
    const wchar_t *meetingPasswordChar = meetingPasswordStr.c_str();

    // Join meeting for end user with JoinParam object
    ZOOM_SDK_NAMESPACE::JoinParam joinMeetingParam;
    // Provide meeting credentials for end user using JoinParam4NormalUser
    ZOOM_SDK_NAMESPACE::JoinParam4NormalUser joinMeetingForNormalUserLoginParam;

    joinMeetingParam.userType = ZOOM_SDK_NAMESPACE::SDK_UT_NORMALUSER;

    joinMeetingForNormalUserLoginParam.meetingNumber = meetingIdLong;
    joinMeetingForNormalUserLoginParam.psw = meetingPasswordChar;
    joinMeetingForNormalUserLoginParam.userName = L"Display name for user";

    joinMeetingParam.param.normaluserJoin = joinMeetingForNormalUserLoginParam;

    ZOOM_SDK_NAMESPACE::SDKError joinMeetingCallReturnValue(ZOOM_SDK_NAMESPACE::SDKERR_UNKNOWN);
    joinMeetingCallReturnValue = meetingService->Join(joinMeetingParam);
    if (joinMeetingCallReturnValue == ZOOM_SDK_NAMESPACE::SDKError::SDKERR_SUCCESS)
    {
      // Join meeting call succeeded, listen for join meeting result using the onMeetingStatusChanged callback
    }
  }

  template <typename T>
  bool GetValueFromEncodableMap(const flutter::EncodableMap *map,
                                const char *key, T &out)
  {
    auto iter = map->find(flutter::EncodableValue(key));
    if (iter != map->end() && !iter->second.IsNull())
    {
      if (auto *value = std::get_if<T>(&iter->second))
      {
        out = *value;
        return true;
      }
    }
    return false;
  }

} // namespace gr_zoom
