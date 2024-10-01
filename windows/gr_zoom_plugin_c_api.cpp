#include "include/gr_zoom/gr_zoom_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "gr_zoom_plugin.h"

void GrZoomPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  gr_zoom::GrZoomPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
