#include <filesystem>
#include <iostream>
#include <string>

// Forward Declaration
void Init();
void LoadAndSerializeImageFile(const char* filepath, const char* outputFolder);
void CompressImageFile(const char* filepath, const char* outputFolder);
void LoadAndSerializeFontFile(const char* filepath, const char* outputFolder);

int main()
{
	std::filesystem::path directoryTex = "../assets/Textures";
	std::filesystem::path directoryFont = "../assets/Fonts";
	std::filesystem::path directoryIMGUITex = "../_EDITOR/Assets/Textures/";

	Init();

	// Load every texture in the directory ^
	for (const auto& entry : std::filesystem::directory_iterator(directoryTex))
	{
		std::string file = entry.path().filename().string();

		file = directoryTex.string() + "/" + file;

		std::string filetype = file.substr(file.find_last_of("."));

		std::cout << file.c_str() << std::endl;

		if (filetype == ".ttf")
			LoadAndSerializeFontFile(file.c_str(), "../assets/compiled_fonts");
		else
			CompressImageFile(file.c_str(), "../assets/Compressed");
	}

	// Load every IMGUI texture in the directory
	for (const auto& entry : std::filesystem::directory_iterator(directoryIMGUITex))
	{
		std::string file = entry.path().filename().string();

		file = directoryIMGUITex.string() + "/" + file;

		std::string filetype = file.substr(file.find_last_of("."));

		std::cout << file.c_str() << std::endl;

		if (filetype == ".ttf")
			LoadAndSerializeFontFile(file.c_str(), "../assets/compiled_fonts");
		else
			CompressImageFile(file.c_str(), "../_EDITOR/Assets/Textures_Compressed/");
	}

	// Load every font in the directory ^
	for (const auto& entry : std::filesystem::directory_iterator(directoryFont))
	{
		std::string file = entry.path().filename().string();

		file = directoryFont.string() + "/" + file;

		std::string filetype = file.substr(file.find_last_of("."));

		std::cout << file.c_str() << std::endl;

		if (filetype == ".ttf")
			LoadAndSerializeFontFile(file.c_str(), "../assets/compiled_fonts");
		else
			CompressImageFile(file.c_str(), "../assets/Compressed");
	}
}