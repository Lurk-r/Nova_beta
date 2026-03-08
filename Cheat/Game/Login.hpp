#pragma once
#include <Unity/IL2CPP.hpp>

namespace Login
{
	void HandleTokenOutput();
	void HandleAuthInfoOutput();
	void HandleLoginWithToken();
	void HandleLoginWithIDSecret();

	void Init();
}