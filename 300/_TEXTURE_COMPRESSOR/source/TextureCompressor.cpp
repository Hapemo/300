#include <stb-master/stb_image.h>

#include <string>
#include <iostream>
#include <fstream>
#include <glew.h>
#include <glfw3.h>

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

void CompressImageFile(const char* filepath, const char* outputFolder)
{
	// Image stats
	int width, height, channel;

	// Settings
	stbi_set_flip_vertically_on_load(false);

	// Loading of image file with STB
	unsigned char* texData = stbi_load(filepath, &width, &height, &channel, 0);

	///////////////////////////////////////
	// Compressing of the image data
	///////////////////////////////////////
	GLuint tex;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// Default format supports 4 channels
	GLint internalFormat{ GL_COMPRESSED_RGBA_ARB };
	GLint format{ GL_RGBA };

	if (channel == 3)		// RGB
	{
		internalFormat = GL_COMPRESSED_RGB_ARB;
		format = GL_RGB;
	}

	// Compress with opengl
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, texData);

	// Validate compression
	GLint result{};
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_ARB, &result);

	GLint compressedSize{};
	if (result == GL_TRUE)		// Compression successful
	{
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);				// Get compressed internal format
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB, &compressedSize);		// Get compressed image size

		// Allocate buffer to store pixel data of compressed image
		unsigned char* pixelData = new unsigned char[compressedSize];

		glGetCompressedTexImageARB(GL_TEXTURE_2D, 0, pixelData);

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
}
