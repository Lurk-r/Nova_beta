#include "Backend/Backend.hpp"
#include "Utils/Utils.hpp"
#include "Cheat.hpp"

#include <Obfusheader.hpp>
/*
static void PrintLogo()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);

	std::cout << " ________   ________  ___      ___ ________     " << std::endl;
	std::cout << "|\\   ___  \\|\\   __  \\|\\  \\    /  /|\\   __  \\    " << std::endl;
	std::cout << "\\ \\  \\\\ \\  \\ \\  \\|\\  \\ \\  \\  /  / | \\  \\|\\  \\   " << std::endl;
	std::cout << " \\ \\  \\\\ \\  \\ \\  \\\\\\  \\ \\  \\/  / / \\ \\   __  \\  " << std::endl;
	std::cout << "  \\ \\  \\\\ \\  \\ \\  \\\\\\  \\ \\    / /   \\ \\  \\ \\  \\ " << std::endl;
	std::cout << "   \\ \\__\\\\ \\__\\ \\_______\\ \\__/ /     \\ \\__\\ \\__\\" << std::endl;
	std::cout << "    \\|__| \\|__|\\|_______|\\|__|/       \\|__|\\|__|" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
}
*/
static DWORD WINAPI MainThread(LPVOID param)
{
	Features.Load();
	Cheat::Init();
  /*
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitleA(OBF("-Nova-"));
	const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	std::wcout.imbue(std::locale("en_US.UTF-8"));

	PrintLogo();
	printf("Press Right Control / Right Shift / Insert / F1 to open menu\n");
  */
	return 0;
}

extern "C" __declspec(dllexport)
BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case 1:
		HANDLE hMainThread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, nullptr);
		if (hMainThread)
			CloseHandle(hMainThread);
		break;
	}

	return TRUE;
}