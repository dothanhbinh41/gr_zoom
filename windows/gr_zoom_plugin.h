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
    };

} // namespace gr_zoom

#endif // FLUTTER_PLUGIN_GR_ZOOM_PLUGIN_H_
