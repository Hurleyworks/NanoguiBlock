#pragma once

// cinder
#include <cinder/gl/gl.h>
#include <cinder/Clipboard.h>
#include <cinder/Timer.h>
#include <cinder/Filesystem.h>

#include <stdint.h>
#include <array>
#include <chrono>
#include <atomic>
#include <unordered_map>
#include <numeric>
#include <sstream>
#include <regex>

#include "NanoguiModule/nanovg/nanovg.h"

#include "NanoguiModule/nanogui/resources.h"
#include "NanoguiModule/nanogui/entypo.h"
#include "NanoguiModule/nanogui/common.h"
#include "NanoguiModule/util/NanoUtil.h"
#include "NanoguiModule/nanogui/object.h"
#include "NanoguiModule/nanogui/theme.h"
#include "NanoguiModule/nanogui/widget.h"
#include "NanoguiModule/nanogui/label.h"
#include "NanoguiModule/nanogui/screen.h"
#include "NanoguiModule/nanogui/layout.h"
#include "NanoguiModule/nanogui/window.h"
#include "NanoguiModule/nanogui/button.h"
#include "NanoguiModule/nanogui/checkbox.h"
#include "NanoguiModule/nanogui/popup.h"
#include "NanoguiModule/nanogui/popupbutton.h"
#include "NanoguiModule/nanogui/combobox.h"
#include "NanoguiModule/nanogui/messagedialog.h"
#include "NanoguiModule/nanogui/progressbar.h"
#include "NanoguiModule/nanogui/colorwheel.h"
#include "NanoguiModule/nanogui/colorpicker.h"
#include "NanoguiModule/nanogui/textbox.h"
#include "NanoguiModule/nanogui/formhelper.h"
#include "NanoguiModule/nanogui/graph.h"
#include "NanoguiModule/nanogui/imagepanel.h"
#include "NanoguiModule/nanogui/imageview.h"
#include "NanoguiModule/nanogui/toolbutton.h"
#include "NanoguiModule/nanogui/vscrollpanel.h"
#include "NanoguiModule/nanogui/slider.h"
#include "NanoguiModule/util/Performance.h"
#include "NanoguiModule/util/ShaderSource.h"



