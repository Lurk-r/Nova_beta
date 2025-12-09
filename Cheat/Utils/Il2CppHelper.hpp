#pragma once
#include <Unity/IL2CPP.hpp>
#include "Hooks.hpp"
#include <unordered_map>
#include <string>
#include <string_view>

namespace Cache
{
    struct PairHash
    {
        size_t operator()(const std::pair<std::string, std::string>& p) const noexcept
        {
            return std::hash<std::string>{}(p.first) ^ (std::hash<std::string>{}(p.second) << 1);
        }
    };

    extern std::unordered_map<std::string, IL2CPP::Image*> assemblies;
    extern std::unordered_map<std::pair<std::string, std::string>, IL2CPP::Class*, PairHash> classes;

    IL2CPP::Class* GetClass(const std::string& className, const std::string& assembly = "Assembly-CSharp.dll");
}

template<typename T>
LPVOID GetMethodPointer(const std::string& className, const T& methodIdentifier, const std::string& assembly = "Assembly-CSharp.dll")
{
    auto targetClass = Cache::GetClass(className, assembly);
    if (!targetClass) throw std::runtime_error("Class not found");

    if constexpr (std::is_integral_v<T>)
    {
        auto method = targetClass->GetMethod(methodIdentifier);
        if (method) return method->GetPointer();
    }
    else if constexpr (std::is_pointer_v<T>)
    {
        auto method = targetClass->GetMethodByPattern(*methodIdentifier);
        if (method) return method->GetPointer();
    }
    else if constexpr (std::is_convertible_v<T, std::string>)
    {
        auto method = targetClass->GetMethod(methodIdentifier);
        if (method) return method->GetPointer();
    }

    throw std::runtime_error("Failed to get method pointer");
}

LPVOID GetMappedPointer(std::string_view className, std::string_view methodName);
LPVOID GetMappedPointer(std::string_view className, uint64_t index);
LPVOID GetMappedPointer(std::string_view className, IL2CPP::SignaturePattern* pattern);

template<typename Identifier>
void HookMethod(const std::string& className, const Identifier& methodIdentifier, LPVOID detour, LPVOID* original, const std::string& assembly = "Assembly-CSharp.dll")
{
    auto address = GetMethodPointer(className, methodIdentifier, assembly);
    AttachHook(address, detour, original);
}

template<typename Identifier>
void HookMappedMethod(std::string_view className, const Identifier& methodIdentifier, LPVOID detour, LPVOID* original)
{
    auto klass = IL2CPP::ClassMapping::GetClass(className.data());
    if (!klass) throw std::runtime_error("Mapped class not found");

    if constexpr (std::is_convertible_v<Identifier, std::string_view>)
    {
        auto method = klass->GetMethod(methodIdentifier);
        if (method) { AttachHook(method->GetPointer(), detour, original); return; }
    }
    else if constexpr (std::is_integral_v<Identifier>)
    {
        auto method = klass->GetMethod(methodIdentifier);
        if (method) { AttachHook(method->GetPointer(), detour, original); return; }
    }
    else if constexpr (std::is_pointer_v<Identifier>)
    {
        auto method = klass->GetMethodByPattern(*methodIdentifier);
        if (method) { AttachHook(method->GetPointer(), detour, original); return; }
    }

    throw std::runtime_error("Method not found in mapped class");
}

void HookMethodOffset(uint64_t offset, LPVOID detour, LPVOID* original);

template<typename T>
void SetField(IL2CPP::Object* instance, std::string_view fieldName, T value)
{
    if (!instance) throw std::invalid_argument("Null object instance");
    auto& field = instance->GetFieldRef<T>(fieldName.data());
    field = value;
}