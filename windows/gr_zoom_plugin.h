#ifndef FLUTTER_PLUGIN_GR_ZOOM_PLUGIN_H_
#define FLUTTER_PLUGIN_GR_ZOOM_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <memory>

namespace gr_zoom
{

    class GrZoomPlugin : public flutter::Plugin
    {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        GrZoomPlugin();

        virtual ~GrZoomPlugin();

        // Disallow copy and assign.
        GrZoomPlugin(const GrZoomPlugin &) = delete;
        GrZoomPlugin &operator=(const GrZoomPlugin &) = delete;

        // Called when a method is called on this plugin's channel from Dart.
        void HandleMethodCall(
            const flutter::MethodCall<flutter::EncodableValue> &method_call,
            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

        template <typename T>
        bool GetValueFromEncodableMap(const flutter::EncodableMap *map,
                                      const std::string &key, T &out);
        void AuthenticateSDK(const std::string &token);
        void JoinMeetingForAPIUser(const std::string &meetingId, const std::string &meetingPassword);
    };

} // namespace gr_zoom

#endif // FLUTTER_PLUGIN_GR_ZOOM_PLUGIN_H_
