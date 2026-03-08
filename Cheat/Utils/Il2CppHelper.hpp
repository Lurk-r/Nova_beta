#pragma once
#include <Unity/IL2CPP.hpp>
#include "Hooks.hpp"
#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <Unity/backend/types.hpp>
#include <thread>
#include <chrono>

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

namespace IL2CPP
{
    struct Vector3
    {
        float x, y, z;
    };

    struct Quaternion
    {
        float x, y, z, w;
    };
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
void MakeAHookHerePls(const std::string& className, const Identifier& methodIdentifier, LPVOID detour, LPVOID* original, const std::string& assembly = "Assembly-CSharp.dll")
{
    auto address = GetMethodPointer(className, methodIdentifier, assembly);
    AttachHook(address, detour, original);
}

template<typename Identifier>
void MakeAHookHereButMappedPls(std::string_view className, const Identifier& methodIdentifier, LPVOID detour, LPVOID* original)
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

template<typename T = IL2CPP::Object*>
T GetInstanceFromStatic(const std::string& className, uint64_t staticInstanceOffset, const std::string& assembly = "Assembly-CSharp.dll")
{
    auto klass = Cache::GetClass(className, assembly);
    if (!klass) return nullptr;

    auto rawKlass = (IL2CPP::IMPORT::Il2CppClass*)klass;

    if (!rawKlass->static_fields) return nullptr;

    return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(rawKlass->static_fields) + staticInstanceOffset);
}

template<typename T>
T GetSingletonField(const std::string& className, uint64_t staticInstanceOffset, uint64_t fieldOffset, const std::string& assembly = "Assembly-CSharp.dll")
{
    auto instance = GetInstanceFromStatic(className, staticInstanceOffset, assembly);
    if (!instance) return T();

    return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(instance) + fieldOffset);
}

template<typename T>
void SetSingletonField(const std::string& className, uint64_t staticInstanceOffset, uint64_t fieldOffset, T value, const std::string& assembly = "Assembly-CSharp.dll")
{
    auto instance = GetInstanceFromStatic(className, staticInstanceOffset, assembly);
    if (instance)
    {
        *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(instance) + fieldOffset) = value;
    }
}

template<typename T>
T GetFieldByOffset(IL2CPP::Object* instance, uint64_t offset)
{
    if (!instance) return T();
    return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(instance) + offset);
}

inline uint32_t GetFieldOffset(const std::string& className, const std::string& fieldName, const std::string& assembly = "Assembly-CSharp.dll")
{
    auto klass = Cache::GetClass(className, assembly);
    if (!klass) return 0;

    try
    {
        auto field = klass->GetField(fieldName, 0);
        if (!field) return 0;

        return field->GetOffset();
    }
    catch (...)
    {
        return 0;
    }
}

#define GET_FIELDOFFSET(className, fieldName) GetFieldOffset(className, fieldName)

namespace Il2CppDumper
{
    static uint64_t il2cpp_base = (uint64_t)GetModuleHandleA("GameAssembly.dll");
    static volatile bool is_dumping = false;

    inline void DumpLog(const char* message) {
        FILE* f = fopen("NovaDump_Debug.log", "a");
        if (f) {
            time_t rawtime;
            struct tm* timeinfo;
            char timeStr[100];

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

            fprintf(f, "[%s] %s\n", timeStr, message);
            fflush(f);
            fclose(f);
        }
    }

    inline bool IsBasicLatin(int32_t x) { return x >= 0x0020 && x <= 0x007F; }

    inline bool IsObfuscated(const std::string& str) {
        if (str.empty()) return true;
        for (char c : str) {
            if (!IsBasicLatin(c)) return true;
        }
        return false;
    }

    inline std::string get_method_modifier(uint32_t flags) {
        std::string result;
        result.reserve(64);

        auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;
        switch (access) {
        case METHOD_ATTRIBUTE_PRIVATE: result += "private "; break;
        case METHOD_ATTRIBUTE_PUBLIC: result += "public "; break;
        case METHOD_ATTRIBUTE_FAMILY: result += "protected "; break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM: result += "internal "; break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM: result += "protected internal "; break;
        }
        if (flags & METHOD_ATTRIBUTE_STATIC) result += "static ";
        if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
            result += "abstract ";
            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) result += "override ";
        }
        else if (flags & METHOD_ATTRIBUTE_FINAL) {
            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) result += "sealed override ";
        }
        else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
            if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) result += "virtual ";
            else result += "override ";
        }
        if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) result += "extern ";
        return result;
    }

    inline std::string get_method_modifier_clean(uint32_t flags) {
        std::string s = get_method_modifier(flags);
        if (!s.empty() && s.back() == ' ') s.pop_back();
        return s;
    }

    inline std::string GetFullType(const IL2CPP::IMPORT::Il2CppType* type) {
        if (!type) return "void";

        std::string result;
        result.reserve(128);

        try {
            if (type->type == IL2CPP::IMPORT::Il2CppTypeEnum::IL2CPP_TYPE_GENERICINST) {
                auto genericClass = type->data.generic_class;
                if (genericClass && genericClass->cached_class) {
                    result = genericClass->cached_class->name;
                    if (result.size() >= 2) {
                        result.pop_back();
                        result.pop_back();
                    }
                    result += "<";
                    auto classInst = genericClass->context.class_inst;
                    if (classInst) {
                        for (uint32_t i = 0; i < classInst->type_argc; ++i) {
                            const auto argType = classInst->type_argv[i];
                            auto argClass = IL2CPP::IMPORT::il2cpp_class_from_type(argType);
                            if (argClass) result += GetFullType(argType);
                            else result += "UnknownType";
                            if (i < classInst->type_argc - 1) result += ", ";
                        }
                    }
                    result += ">";
                }
            }
            else {
                auto typeClass = IL2CPP::IMPORT::il2cpp_class_from_type(type);
                if (typeClass && typeClass->name) result += typeClass->name;
                else result += "Unknown";
            }
        }
        catch (...) {
            result = "ErrorType";
        }
        return result;
    }

    inline bool _il2cpp_type_is_byref(const IL2CPP::IMPORT::Il2CppType* type) {
        auto byref = type->byref;
        if (IL2CPP::IMPORT::il2cpp_type_is_byref) {
            byref = IL2CPP::IMPORT::il2cpp_type_is_byref(type);
        }
        return byref;
    }

    inline void dump_field(std::stringstream& outPut, IL2CPP::IMPORT::Il2CppClass* klass) {
        try {
            outPut << "\n\t";

            IL2CPP::IMPORT::il2cpp_runtime_class_init(klass);

            void* iter = nullptr;
            void* iterPattern = nullptr;
            bool hasFields = false;

            outPut << "\t// FieldPattern: {";
            int fieldCount = 0;
            int totalFields = klass->field_count;

            while (auto field = IL2CPP::IMPORT::il2cpp_class_get_fields(klass, &iterPattern)) {
                auto field_type = IL2CPP::IMPORT::il2cpp_field_get_type(field);
                auto field_class = IL2CPP::IMPORT::il2cpp_class_from_type(field_type);
                std::string name = (field_class && field_class->name) ? field_class->name : "Unknown";

                if (fieldCount > 0) outPut << ", ";
                if (!IsObfuscated(name)) outPut << "\"" << name << "\"";
                else outPut << "nullptr";

                fieldCount++;
            }
            outPut << "};\n\n";

            uintptr_t fieldIndex = 0;
            auto is_enum = IL2CPP::IMPORT::il2cpp_class_is_enum(klass);

            while (auto field = IL2CPP::IMPORT::il2cpp_class_get_fields(klass, &iter)) {
                hasFields = true;

                outPut << "\t";
                auto attrs = IL2CPP::IMPORT::il2cpp_field_get_flags(field);
                auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
                switch (access) {
                case FIELD_ATTRIBUTE_PRIVATE:
                    outPut << "private ";
                    break;
                case FIELD_ATTRIBUTE_PUBLIC:
                    outPut << "public ";
                    break;
                case FIELD_ATTRIBUTE_FAMILY:
                    outPut << "protected ";
                    break;
                case FIELD_ATTRIBUTE_ASSEMBLY:
                case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                    outPut << "internal ";
                    break;
                case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                    outPut << "protected internal ";
                    break;
                }
                if (attrs & FIELD_ATTRIBUTE_LITERAL) {
                    outPut << "const ";
                }
                else {
                    if (attrs & FIELD_ATTRIBUTE_STATIC) {
                        outPut << "static ";
                    }
                    if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                        outPut << "readonly ";
                    }
                }

                auto field_type = IL2CPP::IMPORT::il2cpp_field_get_type(field);
                const char* fName = IL2CPP::IMPORT::il2cpp_field_get_name(field);
                uint32_t offset = IL2CPP::IMPORT::il2cpp_field_get_offset(field);

                outPut << GetFullType(field_type) << " " << (fName ? fName : "Unknown");

                if (attrs & FIELD_ATTRIBUTE_LITERAL && is_enum) {
                    uint64_t val = 0;
                    IL2CPP::IMPORT::il2cpp_field_static_get_value(field, &val);
                    outPut << " = " << std::dec << val;
                }

                outPut << "; // Index: 0x" << std::hex << fieldIndex << " Offset: 0x" << offset << std::dec << "\n";
                fieldIndex++;
            }

            if (!hasFields) {
                outPut << "\n";
            }
        }
        catch (...) {
            outPut << "\t\n";
        }
    }

    inline void dump_property(std::stringstream& outPut, IL2CPP::IMPORT::Il2CppClass* klass) {
        try {
            outPut << "\n\t";
            void* iter = nullptr;
            bool hasProps = false;

            while (auto prop_const = IL2CPP::IMPORT::il2cpp_class_get_properties(klass, &iter)) {
                if (!hasProps) {
                    outPut << "\n";
                    hasProps = true;
                }

                try {
                    auto prop = const_cast<IL2CPP::IMPORT::PropertyInfo*>(prop_const);

                    outPut << "\t";
                    auto get = IL2CPP::IMPORT::il2cpp_property_get_get_method(prop);
                    auto set = IL2CPP::IMPORT::il2cpp_property_get_set_method(prop);
                    auto prop_name = IL2CPP::IMPORT::il2cpp_property_get_name(prop);

                    IL2CPP::IMPORT::Il2CppClass* prop_class = nullptr;
                    uint32_t iflags = 0;

                    if (get) {
                        outPut << get_method_modifier(IL2CPP::IMPORT::il2cpp_method_get_flags(get, &iflags));
                        prop_class = IL2CPP::IMPORT::il2cpp_class_from_type(IL2CPP::IMPORT::il2cpp_method_get_return_type(get));
                    }
                    else if (set) {
                        outPut << get_method_modifier(IL2CPP::IMPORT::il2cpp_method_get_flags(set, &iflags));
                        auto param = IL2CPP::IMPORT::il2cpp_method_get_param(set, 0);
                        prop_class = IL2CPP::IMPORT::il2cpp_class_from_type(param);
                    }

                    if (prop_class) {
                        const auto prop_type = IL2CPP::IMPORT::il2cpp_class_get_type(prop_class);
                        outPut << GetFullType(prop_type) << " " << (prop_name ? prop_name : "Unknown") << " { ";
                        if (get) outPut << "get; ";
                        if (set) outPut << "set; ";
                        outPut << "}\n";
                    }
                }
                catch (...) {
                    outPut << "\t\n";
                }
            }
        }
        catch (...) {
            outPut << "\t\n";
        }
    }

    inline void dump_method(std::stringstream& outPut, IL2CPP::IMPORT::Il2CppClass* klass) {
        try {
            outPut << "\n\t";

            void* iter = nullptr;
            uintptr_t index = 0;

            while (auto method = IL2CPP::IMPORT::il2cpp_class_get_methods(klass, &iter)) {
                try {
                    if (method->methodPointer) {
                        uint32_t iflags = 0;
                        auto flags = IL2CPP::IMPORT::il2cpp_method_get_flags(method, &iflags);
                        std::string modif = get_method_modifier_clean(flags);

                        auto returnType = IL2CPP::IMPORT::il2cpp_method_get_return_type(method);
                        auto returnClass = IL2CPP::IMPORT::il2cpp_class_from_type(returnType);
                        std::string returnTypeName = (returnClass && returnClass->name) ? returnClass->name : "void";
                        std::string methodName = IL2CPP::IMPORT::il2cpp_method_get_name(method);
                        if (methodName.empty()) methodName = "Unknown";

                        outPut << "\t// SignaturePattern: {\"" << modif << "\", ";
                        outPut << (IsObfuscated(returnTypeName) ? "nullptr" : ("\"" + returnTypeName + "\"")) << ", ";
                        outPut << (IsObfuscated(methodName) ? "nullptr" : ("\"" + methodName + "\"")) << ", {";

                        int paramCount = IL2CPP::IMPORT::il2cpp_method_get_param_count(method);
                        for (int i = 0; i < paramCount; i++) {
                            if (i > 0) outPut << ", ";
                            auto param = IL2CPP::IMPORT::il2cpp_method_get_param(method, i);
                            auto paramClass = IL2CPP::IMPORT::il2cpp_class_from_type(param);
                            std::string pName = (paramClass && paramClass->name) ? paramClass->name : "void";

                            if (paramClass && IL2CPP::IMPORT::il2cpp_class_is_enum(paramClass)) outPut << "\"ENUM\"";
                            else if (IsObfuscated(pName)) outPut << "nullptr";
                            else outPut << "\"" << pName << "\"";
                        }
                        outPut << "}};\n";

                        uint64_t offset = (uint64_t)method->methodPointer - il2cpp_base;
                        outPut << "\t// Index: 0x" << std::hex << index << " Offset: 0x" << offset << " VA: 0x" << (uint64_t)method->methodPointer << std::dec << "\n";
                    }
                    else {
                        outPut << "\t\n";
                    }

                    outPut << "\t";
                    uint32_t iflags = 0;
                    auto flags = IL2CPP::IMPORT::il2cpp_method_get_flags(method, &iflags);
                    outPut << get_method_modifier(flags);

                    auto return_type = IL2CPP::IMPORT::il2cpp_method_get_return_type(method);
                    if (_il2cpp_type_is_byref(return_type)) outPut << "ref ";

                    const char* mName = IL2CPP::IMPORT::il2cpp_method_get_name(method);
                    outPut << GetFullType(return_type) << " " << (mName ? mName : "Unknown") << "(";

                    auto param_count = IL2CPP::IMPORT::il2cpp_method_get_param_count(method);
                    for (int i = 0; i < param_count; ++i) {
                        auto param_type = IL2CPP::IMPORT::il2cpp_method_get_param(method, i);
                        auto attrs = param_type->attrs;
                        if (_il2cpp_type_is_byref(param_type)) {
                            if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN)) outPut << "out ";
                            else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT)) outPut << "in ";
                            else outPut << "ref ";
                        }
                        else {
                            if (attrs & PARAM_ATTRIBUTE_IN) outPut << "[In] ";
                            if (attrs & PARAM_ATTRIBUTE_OUT) outPut << "[Out] ";
                        }
                        const char* pName = IL2CPP::IMPORT::il2cpp_method_get_param_name(method, i);
                        outPut << GetFullType(param_type) << " " << (pName ? pName : "unk") << ", ";
                    }
                    if (param_count > 0) outPut.seekp(-2, outPut.cur);

                    outPut << ") { }\n\n";
                }
                catch (...) {
                    outPut << "\t\n\n";
                }

                index++;
            }
        }
        catch (...) {
            outPut << "\t\n";
        }
    }

    inline void dump_type(std::stringstream& outPut, const IL2CPP::IMPORT::Il2CppClass* klass_const) {
        try {
            auto klass = const_cast<IL2CPP::IMPORT::Il2CppClass*>(klass_const);

            const char* ns = IL2CPP::IMPORT::il2cpp_class_get_namespace(klass);
            const char* name = IL2CPP::IMPORT::il2cpp_class_get_name(klass);
            auto dataSize = IL2CPP::IMPORT::il2cpp_class_get_data_size(klass);
            auto fieldCount = klass->field_count;
            auto methodCount = klass->method_count;

            outPut << "\n// Namespace: ";
            if (ns && ns[0] != '\0') {
                outPut << ns;
            }
            else {
                outPut << "none";
            }
            outPut << "  ClassDataSize: " << dataSize << "  FieldCount: " << fieldCount << "  MethodCount: " << methodCount << "\n";

            auto flags = IL2CPP::IMPORT::il2cpp_class_get_flags(klass);
            if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
                outPut << "[Serializable]\n";
            }

            auto is_valuetype = IL2CPP::IMPORT::il2cpp_class_is_valuetype(klass);
            auto is_enum = IL2CPP::IMPORT::il2cpp_class_is_enum(klass);
            auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;

            switch (visibility) {
            case TYPE_ATTRIBUTE_PUBLIC:
            case TYPE_ATTRIBUTE_NESTED_PUBLIC:
                outPut << "public ";
                break;
            case TYPE_ATTRIBUTE_NOT_PUBLIC:
            case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
            case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
                outPut << "internal ";
                break;
            case TYPE_ATTRIBUTE_NESTED_PRIVATE:
                outPut << "private ";
                break;
            case TYPE_ATTRIBUTE_NESTED_FAMILY:
                outPut << "protected ";
                break;
            case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
                outPut << "protected internal ";
                break;
            }

            if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
                outPut << "static ";
            }
            else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
                outPut << "abstract ";
            }
            else if (!is_valuetype && !is_enum && flags & TYPE_ATTRIBUTE_SEALED) {
                outPut << "sealed ";
            }

            if (flags & TYPE_ATTRIBUTE_INTERFACE) {
                outPut << "interface ";
            }
            else if (is_enum) {
                outPut << "enum ";
            }
            else if (is_valuetype) {
                outPut << "struct ";
            }
            else {
                outPut << "class ";
            }

            outPut << (name ? name : "Unknown");

            std::vector<std::string> extends;
            auto parent = IL2CPP::IMPORT::il2cpp_class_get_parent(klass);
            if (!is_valuetype && !is_enum && parent) {
                auto parent_type = IL2CPP::IMPORT::il2cpp_class_get_type(parent);
                if (parent_type->type != IL2CPP::IMPORT::Il2CppTypeEnum::IL2CPP_TYPE_OBJECT) {
                    extends.emplace_back(IL2CPP::IMPORT::il2cpp_class_get_name(parent));
                }
            }

            void* iter = nullptr;
            while (auto itf = IL2CPP::IMPORT::il2cpp_class_get_interfaces(klass, &iter)) {
                extends.emplace_back(IL2CPP::IMPORT::il2cpp_class_get_name(itf));
            }

            if (!extends.empty()) {
                outPut << " : " << extends[0];
                for (size_t i = 1; i < extends.size(); ++i) {
                    outPut << ", " << extends[i];
                }
            }

            outPut << "\n{";
            dump_field(outPut, klass);
            dump_property(outPut, klass);
            dump_method(outPut, klass);
            outPut << "}\n";
        }
        catch (...) {
            outPut << "\n";
        }
    }

    inline void Dump(const char* fileName) {
        DumpLog("======================================");
        DumpLog("Dump called SYNCHRONOUSLY");

        if (is_dumping) {
            DumpLog("Dump already in progress, aborting");
            return;
        }

        DumpLog("Setting is_dumping flag");
        is_dumping = true;

        try {
            DumpLog("Getting IL2CPP domain...");
            auto domain = IL2CPP::IMPORT::il2cpp_domain_get();
            if (!domain) {
                DumpLog("ERROR: Failed to get IL2CPP domain!");
                is_dumping = false;
                return;
            }
            DumpLog("Domain obtained successfully");

            DumpLog("Checking current thread...");
            auto thread = IL2CPP::IMPORT::il2cpp_thread_current();
            if (!thread) {
                DumpLog("No current thread, attempting to attach...");
                thread = IL2CPP::IMPORT::il2cpp_thread_attach(domain);
                if (!thread) {
                    DumpLog("ERROR: Failed to attach thread to IL2CPP domain!");
                    is_dumping = false;
                    return;
                }
                DumpLog("Thread attached successfully");
            }
            else {
                DumpLog("Thread already attached");
            }

            DumpLog("Getting assemblies...");
            size_t size = 0;
            auto assemblies = IL2CPP::IMPORT::il2cpp_domain_get_assemblies(domain, &size);
            if (!assemblies || size == 0) {
                DumpLog("ERROR: No assemblies found or assemblies pointer is null!");
                is_dumping = false;
                return;
            }

            char buffer[256];
            sprintf(buffer, "Found %zu assemblies", size);
            DumpLog(buffer);

            DumpLog("Creating string buffer...");
            std::stringstream ssbuffer;
            DumpLog("Buffer created successfully");

            std::stringstream imageOutput;
            for (size_t i = 0; i < size; ++i) {
                auto image = IL2CPP::IMPORT::il2cpp_assembly_get_image(assemblies[i]);
                if (!image) continue;
                const char* imgName = IL2CPP::IMPORT::il2cpp_image_get_name(image);
                imageOutput << "// Image " << i << ": " << (imgName ? imgName : "Unknown") << "\n";
            }
            ssbuffer << imageOutput.str();

            for (size_t i = 0; i < size; ++i) {
                try {
                    sprintf(buffer, "Processing assembly %zu / %zu", i + 1, size);
                    DumpLog(buffer);

                    auto image = IL2CPP::IMPORT::il2cpp_assembly_get_image(assemblies[i]);
                    if (!image) {
                        sprintf(buffer, "Assembly %zu has null image, skipping", i);
                        DumpLog(buffer);
                        continue;
                    }

                    const char* imgName = IL2CPP::IMPORT::il2cpp_image_get_name(image);
                    std::string imageName = (imgName ? imgName : "Unknown");
                    sprintf(buffer, "Assembly image name: %s", imageName.c_str());
                    DumpLog(buffer);
                    ssbuffer << "\n// Dll : " << imageName << "\n";

                    auto classCount = IL2CPP::IMPORT::il2cpp_image_get_class_count(image);
                    sprintf(buffer, "Assembly has %d classes", classCount);
                    DumpLog(buffer);

                    for (int j = 0; j < classCount; ++j) {
                        try {
                            if (j % 100 == 0) {
                                sprintf(buffer, "Processing class %d / %d", j, classCount);
                                DumpLog(buffer);
                            }

                            auto klass = IL2CPP::IMPORT::il2cpp_image_get_class(image, j);
                            if (klass) {
                                ssbuffer << "// Class Number " << j << "\n";
                                dump_type(ssbuffer, klass);
                            }
                        }
                        catch (...) {
                            sprintf(buffer, "EXCEPTION processing class %d", j);
                            DumpLog(buffer);
                            continue;
                        }
                    }

                    sprintf(buffer, "Completed assembly %zu", i + 1);
                    DumpLog(buffer);
                }
                catch (...) {
                    sprintf(buffer, "EXCEPTION processing assembly %zu", i);
                    DumpLog(buffer);
                    continue;
                }
            }

            DumpLog("All assemblies processed, writing to file...");
            sprintf(buffer, "Output file: %s", fileName);
            DumpLog(buffer);

            std::ofstream out(fileName, std::ios::binary);
            if (out.is_open()) {
                DumpLog("File opened successfully, writing buffer...");
                out << ssbuffer.str();
                out.close();
                DumpLog("File written and closed successfully!");
            }
            else {
                DumpLog("ERROR: Failed to open output file!");
            }
        }
        catch (...) {
            DumpLog("CRITICAL EXCEPTION in dump!");
        }

        DumpLog("Setting is_dumping to false");
        is_dumping = false;
        DumpLog("Dump completed");
        DumpLog("======================================");
    }

    inline void DumpAsync(const char* fileName) {
        Dump(fileName);
    }
}