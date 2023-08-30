#include <filesystem>
#include <iostream>
#include <string>

#define TEST_COMPRESSOR 1

// Forward Declaration
void Init();
void LoadAndSerializeImageFile(const char* filepath, const char* outputFolder);
void CompressImageFile(const char* filepath, const char* outputFolder);

int main()
{
	std::filesystem::path directory = "../assets/Textures";

	Init();

	// Load every file in the directory ^
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		std::string file = entry.path().filename().string();

		file = directory.string() + "/" + file;
		std::cout << file.c_str() << std::endl;

#if TEST_COMPRESSOR
		CompressImageFile(file.c_str(), "../assets/Compressed");
#else
		LoadAndSerializeImageFile(file.c_str(), "../assets/Compressed");
#endif
	}
}