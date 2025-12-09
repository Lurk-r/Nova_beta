#include "Patterns.hpp"

namespace Patterns
{
#pragma region CLASS
	IL2CPP::FieldPattern ProfileClass = { "String", "Int32", "String", "Boolean", "Boolean", "Nullable`1", "Boolean", "String", "String", "Action" };
#pragma endregion

#pragma region METHOD
	
#pragma endregion

#pragma region IMAGES
	static void AssemblyCSharpQueue()
	{
		// No namespace
		IL2CPP::ClassMapping::AddQueue("ProfileClass", "", &ProfileClass);

		// BestHTTP.JSON
		IL2CPP::ClassMapping::AddQueue("Json", "BestHTTP.JSON");
	}
#pragma endregion

	// Initializate
	void Init()
	{
		// Images
		IL2CPP::ClassMapping::AddImageToScan(IL2CPP::DefaultImage::AssemblyCSharp);

		// Mapping
		AssemblyCSharpQueue();

		IL2CPP::ClassMapping::StartMapping();
	}
}