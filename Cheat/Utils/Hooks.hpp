#pragma once

#include <MinHook.hpp>
#include <Logger/Logger.hpp>

bool CheckPointer(void* pointer, const char* context);
bool InitializeMinHook();
void LogMinHookResult(MH_STATUS status, const char* context, void* pointer);

template<typename Func>
void Detour(void* pointer, Func* detour)
{
    if (!CheckPointer(pointer, "detouring"))
        return;

    MH_STATUS status = MH_CreateHook(pointer, (LPVOID)detour, nullptr);
    LogMinHookResult(status, "creating detour", pointer);

    if (status != MH_OK)
        return;

    status = MH_EnableHook(pointer);
    LogMinHookResult(status, "enabling detour", pointer);
}

template<typename Func>
void AttachHook(void* pointer, Func* detour, Func** original)
{
    if (!CheckPointer(pointer, "hooking"))
        return;

    MH_STATUS status = MH_CreateHook(pointer, (LPVOID)detour, (LPVOID*)original);

    if (status != MH_OK)
        return;

    status = MH_EnableHook(pointer);
    LogMinHookResult(status, "enabling hook", pointer);
}