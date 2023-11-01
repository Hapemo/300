#include <stb_image.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>		// OpenGL math library functions
#include <map>
#include <algorithm>
#include <vector>
#include <ft2build.h>		// Freetype
#include <TextureCompressor.h>

#include FT_FREETYPE_H

#define FONT_LOAD_SIZE 50	// Default font loading size

struct Character
{
	glm::vec2	mAdvance;	// Offset to advance to the next glyph
	glm::vec2	mSize;		// width and height of bitmap
	float		mLeft;		// bitmap_left
	float		mTop;		// bitmap_top

	float		mTextCoord;	// x offset of glyph in texture coordinates
};

void Init()
{
	// Initialize glfw
	if (glfwInit() == 0)
	{
		throw std::runtime_error("Failed to initialize GLFW!");
	}

	GLFWwindow* window;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);		// hide window
	window = glfwCreateWindow(100, 100, "Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		throw std::runtime_error("Failed to create a window!");
	}

	glfwMakeContextCurrent(window);

	// Set up GLEW
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Failed to initialize GLEW!");
	}
}

void SerializeCompressedImage(int width, int height, int channels, int size, unsigned char* pixelData, int internalFormat, const std::string& filepath)
{
	// Create custom file
	std::ofstream outfile{ filepath, std::ios_base::binary };
	if (!outfile)
		throw std::runtime_error("Custom file not created!");

	outfile << width << "\n";
	outfile << height << "\n";
	outfile << channels << "\n";
	outfile << internalFormat << "\n";
	outfile << size << "\n";
	outfile.write(reinterpret_cast<char*>(pixelData), size);
}

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

void CompressImageFile(const char* filepath, const char* outputFolder, bool gammaSpace)
{
	// Image stats
	int width, height, channel;

	// Settings
	stbi_set_flip_vertically_on_load(true);

	// Loading of image file with STB
	unsigned char* texData = stbi_load(filepath, &width, &height, &channel, 0);

	///////////////////////////////////////
	// Compressing of the image data
	///////////////////////////////////////
	GLuint tex;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// Default format supports 4 channels
	//GLint internalFormat{ GL_COMPRESSED_RGBA_ARB };
	GLint internalFormat{ GL_COMPRESSED_RGBA };
	GLint format{ GL_RGBA };

	if (gammaSpace)
		internalFormat = GL_COMPRESSED_SRGB_ALPHA;

	if (channel == 1)		// RED, 1 component
	{
		internalFormat = GL_COMPRESSED_RED;
		format = GL_RED;
	}
	else if (channel == 3)		// RGB
	{
		if (gammaSpace)
			internalFormat = GL_COMPRESSED_SRGB;
		else
			internalFormat = GL_COMPRESSED_RGB;
		format = GL_RGB;
	}

	// Compress with opengl
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, texData);

	// Validate compression
	GLint result{};
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &result);

	GLint compressedSize{};
	if (result == GL_TRUE)		// Compression successful
	{
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);				// Get compressed internal format
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize);		// Get compressed image size

		// Allocate buffer to store pixel data of compressed image
		unsigned char* pixelData = new unsigned char[compressedSize];

		glGetCompressedTexImage(GL_TEXTURE_2D, 0, pixelData);

		///////////////////////////////////////
		// Serialize data into custom file
		///////////////////////////////////////
		std::string filename{ filepath };

		// concatenation of final output custom file
		filename = filename.substr(filename.find_last_of("/"), filename.find_last_of(".") - filename.find_last_of("/"));
		filename = outputFolder + filename + ".ctexture";

		SerializeCompressedImage(width, height, channel, compressedSize, pixelData, internalFormat, filename);

		// Free buffer memory
		delete[] pixelData;
	}
	else
		std::cout << "Failed to compress: " << filepath << "\n";

	// Free memory of loaded data in stb
	stbi_image_free(texData);
	glDeleteTextures(1, &tex);
}

void CreateFontTextureAtlas(unsigned int* id, unsigned int width, unsigned height)
{
	// Creating and binding texture
	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Specifying texture size
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void FillUpFontPixels(std::map<char, Character>& charMap, unsigned int texID, FT_Face face, unsigned width)
{
	int x = 0;
	// Load every glyph into image store
	for (int i = 32; i < 128; ++i)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			continue;
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		// Filling in bitmap information of each char
		Character c;
		c.mAdvance.x = (float)(face->glyph->advance.x >> 6);
		c.mAdvance.y = (float)(face->glyph->advance.y >> 6);

		c.mSize.x = (float)(face->glyph->bitmap.width);
		c.mSize.y = (float)(face->glyph->bitmap.rows);

		c.mLeft = (float)(face->glyph->bitmap_left);
		c.mTop = (float)(face->glyph->bitmap_top);

		c.mTextCoord = (float)x / width;

		charMap[(char)i] = c;

		x += face->glyph->bitmap.width;
	}
}

void SerializeFont(std::map<char, Character>& charMap, unsigned int texID, unsigned width, unsigned height, const char* outputPath)
{
	// Create custom file
	std::ofstream outfile{ outputPath, std::ios_base::binary };
	if (!outfile)
		throw std::runtime_error("Custom file not created!");

	int dataSize = width * height * 4;
	int numOfChars = (int)charMap.size();

	// Fonts stats
	outfile << width << "\n";
	outfile << height << "\n";
	outfile << dataSize << "\n";
	outfile << numOfChars << "\n";

	// Each character's data
	for (auto it = charMap.begin(); it != charMap.end(); ++it)
	{
		char ch = it->first;
		Character chData = it->second;

		// char, mAdvance.x, mAdvance.y, mSize.x mSize.y, mLeft, mTop, mTexCoord
		outfile << (int)ch << " " << chData.mAdvance.x << " " << chData.mAdvance.y << " ";
		outfile << chData.mSize.x << " " << chData.mSize.y << " ";
		outfile << chData.mLeft << " " << chData.mTop << " " << chData.mTextCoord << "\n";
	}

	int bufferSize{}, redSize{}, texWidth{}, texHeight{};
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BUFFER_SIZE, &bufferSize);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_RED_TYPE, &redSize);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);

	// Allocate memory to store pixel data of texture atlas
	unsigned char* pixelData = new unsigned char[dataSize];

	// Retrieve pixel data
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, pixelData);

	// Write pixel data to output file
	outfile.write(reinterpret_cast<char*>(pixelData), dataSize);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Free from GPU
	glDeleteTextures(1, &texID);

	delete[] pixelData;
}

void LoadAndSerializeFontFile(const char* filepath, const char* outputFolder)
{
	std::map<char, Character> charMap;
	FT_Library font_lib;
	unsigned int atlasWidth{}, atlasHeight{};

	// Initialize a new FreeType library object
	if (FT_Init_FreeType(&font_lib))	// 0 means success
	{
		throw std::runtime_error("ERROR: Unable to initialize FreeType Library");
	}

	// Load font as a face in FreeType
	FT_Face face;
	if (FT_New_Face(font_lib, filepath, 0, &face))		// 0 means success
	{
		throw std::runtime_error("ERROR: Failed to load font");
	}

	// face, pixel_width, pixel_height
	FT_Set_Pixel_Sizes(face, 0, FONT_LOAD_SIZE);	// Setting width to 0 lets the face dynamically
													// calculate the width based on given height

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Disable byte-alignment restriction
	
	// Calculate the total size of texture atlas to create
	for (int i = 32; i < 128; ++i)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			throw std::runtime_error("ERROR: Failed to load char");
		}

		atlasWidth += face->glyph->bitmap.width;
		atlasHeight = std::max(face->glyph->bitmap.rows, atlasHeight);
	}

	unsigned int texID{};
	CreateFontTextureAtlas(&texID, atlasWidth, atlasHeight);	// Create Texture in GPU
	FillUpFontPixels(charMap, texID, face, atlasWidth);			// Add pixel data to the texture

	glBindTextureUnit(0, texID);

	// Clearing FreeType's resource after glyphs has been processed
	FT_Done_Face(face);				// Free face
	FT_Done_FreeType(font_lib);		// Free lib

	// Serialize fonts data into custom file
	// concatenation of final output custom file
	std::string filename{ filepath };
	filename = filename.substr(filename.find_last_of("/"), filename.find_last_of(".") - filename.find_last_of("/"));
	filename = outputFolder + filename + ".cfont";

	glBindTextureUnit(0, 0);

	SerializeFont(charMap, texID, atlasWidth, atlasHeight, filename.c_str());
}