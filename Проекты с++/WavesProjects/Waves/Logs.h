#pragma once
#include "imgui/imgui.h"

void logClear();

void logPrint(const char* format, ...);

ImGuiTextBuffer& getLogText();