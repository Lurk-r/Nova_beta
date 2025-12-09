#pragma once
#include "PointerWrapper.hpp"

namespace Pointers
{
	// Dummy Pointers
	extern IL2CPP::Object* SocketInstance;

	// Pointer Wrapper
    namespace Json
    {
        extern Pointer<IL2CPP::Object* (IL2CPP::String* json)> Decode;
        extern Pointer<IL2CPP::String* (IL2CPP::Object* json)> Encode;
    }

	void Init();
}