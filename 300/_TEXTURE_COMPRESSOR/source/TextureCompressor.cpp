#include <stb-master/stb_image.h>

#include <string>
#include <iostream>
#include <fstream>

void LoadAndSerializeImageFile(const char* filepath, const char* outputFolder)
{
	// Image stats
	int width, height, channel;

	// Settings
	stbi_set_flip_vertically_on_load(false);
	
	// Loading of image file with STB
	unsigned char* texData = stbi_load(filepath, &width, &height, &channel, 0);

	///////////////////////////////////////
	// Serialize data into custom file
	///////////////////////////////////////
	std::string filename{ filepath };

	// concatenation of final output custom file
	filename = filename.substr(filename.find_last_of("/"), filename.find_last_of(".") - filename.find_last_of("/"));
	filename = outputFolder + filename + ".texture";

	// Create custom file
	std::ofstream outfile{ filename, std::ios_base::binary };
	if (!outfile)
		throw std::runtime_error("Custom file not created!");

	int dataSize = width * height * channel;
	outfile << width << "\n";
	outfile << height << "\n";
	outfile << channel << "\n";
	outfile << dataSize << "\n";
	outfile.write(reinterpret_cast<char*>(texData), dataSize);

	// Free memory of loaded data
	stbi_image_free(texData);
}