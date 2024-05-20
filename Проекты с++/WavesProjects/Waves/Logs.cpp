#include "Logs.h"

ImGuiTextBuffer logText;

void logClear()
{
    logText.clear();
}

void logPrint(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    logText.appendfv(format, args);
}

ImGuiTextBuffer& getLogText()
{
    return logText;
}
