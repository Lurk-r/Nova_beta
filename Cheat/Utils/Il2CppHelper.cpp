#include "Il2CppHelper.hpp"
#include <Windows.h>

namespace Cache
{
    std::unordered_map<std::string, IL2CPP::Image*> assemblies;
    std::unordered_map<std::pair<std::string, std::string>, IL2CPP::Class*, PairHash> classes;

    IL2CPP::Class* GetClass(const std::string& className, const std::string& assembly)
    {
        auto key = std::make_pair(assembly, className);

        if (auto it = classes.find(key); it != classes.end())
            return it->second;

        auto& assemblyPtr = assemblies.try_emplace(
            assembly,
            IL2CPP::Domain().OpenAssembly(assembly)
        ).first->second;

        auto klass = assemblyPtr->GetClass(className);
        classes[key] = klass;
        return klass;
    }
}

LPVOID GetMappedPointer(std::string_view className, std::string_view methodName)
{
    auto klass = IL2CPP::ClassMapping::GetClass(className.data());
    if (!klass) throw std::runtime_error("Mapped class not found");

    auto method = klass->GetMethod(methodName.data());
    if (!method) throw std::runtime_error("Mapped class method not found");

    return method->GetPointer();
}

LPVOID GetMappedPointer(std::string_view className, uint64_t index)
{
    auto klass = IL2CPP::ClassMapping::GetClass(className.data());
    if (!klass) throw std::runtime_error("Mapped class not found");

    auto method = klass->GetMethod(index);
    if (!method) throw std::runtime_error("Mapped class method not found");

    return method->GetPointer();
}

LPVOID GetMappedPointer(std::string_view className, IL2CPP::SignaturePattern* pattern)
{
    auto klass = IL2CPP::ClassMapping::GetClass(className.data());
    if (!klass) throw std::runtime_error("Mapped class not found");

    auto method = klass->GetMethodByPattern(*pattern);
    if (!method) throw std::runtime_error("Mapped class method not found");

    return method->GetPointer();
}

void HookMethodOffset(uint64_t offset, LPVOID detour, LPVOID* original)
{
    static uint64_t moduleBase = reinterpret_cast<uint64_t>(GetModuleHandleA("GameAssembly.dll"));
    AttachHook(reinterpret_cast<LPVOID>(moduleBase + offset), detour, original);
}