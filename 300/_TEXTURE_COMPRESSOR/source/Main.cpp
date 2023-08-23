#include <filesystem>
#include <iostream>
#include <string>

// Forward Declaration
void LoadAndSerializeImageFile(const char* filepath, const char* outputFolder);

int main()
{
	std::filesystem::path directory = "./Textures";

	// Load every file in the directory ^
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		std::string file = entry.path().filename().string();

		file = directory.string() + "/" + file;
		std::cout << file.c_str() << std::endl;

		LoadAndSerializeImageFile(file.c_str(), "Compressed");
	}
}