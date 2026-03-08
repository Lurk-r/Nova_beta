#include "Backend/Backend.hpp"
#include "Utils/Utils.hpp"
#include "Cheat.hpp"

#include <Obfusheader.hpp>

static void PrintLogo()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);

	std::cout << " _____ _               _   _ _   _ _     " << std::endl;
	std::cout << "/  __ \\ |             | | | | | (_) |    " << std::endl;
	std::cout << "| /  \\/ | __ _ _ __   | | | | |_ _| |___ " << std::endl;
	std::cout << "| |   | |/ _` | '_ \\  | | | | __| | / __|" << std::endl;
	std::cout << "| \\__/\\ | (_| | | | | | |_| | |_| | \\__ \\" << std::endl;
	std::cout << " \\____/_|\\__,_|_| |_|  \\___/ \\__|_|_|___/" << std::endl;
	std::cout << std::endl;
	//todo make this say nova but im too lazy to rn
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
}

static DWORD WINAPI MainThread(LPVOID param)
{
	Features.Load();

	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitleA(OBF("-Nova-"));
	const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	std::wcout.imbue(std::locale("en_US.UTF-8"));

	PrintLogo();
	Cheat::Init();
	printf("Press Right Control to open the menu\n");

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