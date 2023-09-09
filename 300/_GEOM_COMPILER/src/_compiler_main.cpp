#include <Geom_Compiler.h>
#include <geom.h>
#include <iostream>

#ifdef __STATIC_LIB	
	// compiles the project into a static library
	#define _DESCRIPTOR_FILEPATH "./descriptor_files/default_descriptor_staticlib.json"
#else
	// compiles the project into an executable
	#define _DESCRIPTOR_FILEPATH "../../_GEOM_COMPILER/descriptor_files/default_descriptor_executable.json"
#endif



// Extracts the raw filename itself
std::string getFileNameWithoutExtension(const std::string& ResourceFilePath)
{
	size_t lastSlash = ResourceFilePath.find_last_of("/\\");
	std::string RawName = ResourceFilePath.substr(lastSlash + 1);
	size_t lastDot = RawName.find_last_of(".");

	if (lastDot != std::string::npos && lastDot > 0) {
		return RawName.substr(0, lastDot);
	}
	else {
		std::cout << "ERROR: Invalid file name \"" << ResourceFilePath << "\n";
		assert(false);
		return RawName;
	}
}


void LoadAndSerializeAllMeshes(_GEOM::DescriptorData& Desc)
{
	// looping through all the filepaths in the descriptor file
	for (; Desc.m_iMeshCurrent < Desc.m_iMeshTotal; ++Desc.m_iMeshCurrent)
	{
		_GEOM::Mesh_Loader meshLoader;
		_GEOM::SkinGeom SkinGeom;
		bool Err = meshLoader.Load(Desc, &SkinGeom);
		assert(Err);

		std::cout << ">>== \t\tSuccessfully Loaded Asset\n";
		std::cout << ">>== \t\tCreating Geom Structure\n";
		_GEOM::Geom geom{};
		SkinGeom.CastToGeomStruct(geom);

		std::string sRawFileName = getFileNameWithoutExtension(Desc.m_Filepaths[Desc.m_iMeshCurrent]);
		std::string sGeomFilepath = Desc.m_sOutputPath + sRawFileName + Desc.m_sOutputFormat;

		_GEOM::Geom::SerializeGeom(sGeomFilepath, geom);
		std::cout << ">>== \t\tSERIALIZATION SUCCESSFUL\n";
		std::cout << ">>== \t\tEnding \t\t==\n";
	}
}


int main(int argc, char* argv[])
{
	std::cout << "================================================================================\n";
	std::cout << ">>\t\tLaunching GEOM Compiler!\n";
	std::cout << "\tAssimp Version: " << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." << aiGetVersionRevision() << std::endl;
	std::cout << "================================================================================\n";

	// Load the descriptor data
	std::cout << ">>\t\tLoading Descriptor Data\n";
	_GEOM::DescriptorData Desc;
	bool Err = _GEOM::DescriptorData::LoadDescriptorData(Desc, _DESCRIPTOR_FILEPATH);
	assert(Err);

	std::cout << "\n================================================================================\n";
	std::cout << ">>\t\tLoading Assets\n";

	// When there is an input filepath argument specified
	if (argc > 1)
	{
		// This is reserved for when I want to implement specific asset loading from the command line
		for (int i{}; i < argc; ++i)
		{
			std::cout << "Argument: " << i << " = " << argv[i] << std::endl;
		}
	}

	else {
		LoadAndSerializeAllMeshes(Desc);
	}

	return 0;
}