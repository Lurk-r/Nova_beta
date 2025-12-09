#include "Hooks.hpp"

bool CheckPointer(void* pointer, const char* context)
{
#ifdef _DEBUG
    if (pointer == nullptr)
    {
        Logger::Error("Error at {}: pointer is null {}", context, pointer);
        return false;
    }
#endif
    return true;
}

bool InitializeMinHook()
{
    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
#ifdef _DEBUG
        Logger::Error("Error initializing MinHook");
#endif
        return false;
    }
    return true;
}

void LogMinHookResult(MH_STATUS status, const char* context, void* pointer)
{
#ifdef _DEBUG
    if (status != MH_OK)
    {
        Logger::Error("Error {} function at {}", context, pointer);
    }
    else
    {
        Logger::Info("Successfully {} function at {}", context, pointer);
    }
#endif
}
