#include "Pointers.hpp"
#include "../../Utils/Il2CppHelper.hpp"

namespace Pointers
{
    template <typename F> 
    void CallOnce(F&& fn) {
        static bool initialized = false; 
        if (initialized) return; 
        initialized = true; 
        fn(); 
    }

	// Dummy Pointers
	IL2CPP::Object* SocketInstance = nullptr;

    // Pointer Wrapper
    namespace Json
    {
        Pointer<IL2CPP::Object* (IL2CPP::String* json)> Decode("Json", "Decode");
        Pointer<IL2CPP::String* (IL2CPP::Object* json)> Encode("Json", "Encode");

        void Init()
        {
            CallOnce([] {
                Decode.Define();
                Encode.Define();
            });
        }
    }

	// Initializate
	void Init()
	{
        Json::Init();
	}
}