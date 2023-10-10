/*!*************************************************************************
****
\file			_compiler_main.cpp
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The entrypoint for the geom compiler. This file calls the other supporting
	functions that the geom compiler uses

****************************************************************************
***/

#include <Geom_Compiler.h>
#include <geom.h>
#include <iostream>


/***************************************************************************/
/*!
\brief
	// Extracts the raw filename itself
	Helper function that gets the raw filename without any prefix and postfix
	extensions
*/
/**************************************************************************/
std::string _GEOM::Mesh_Loader::getFileNameWithoutExtension(const std::string& ResourceFilePath)
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

/***************************************************************************/
/*!
\brief
	Helper function that gets the filename with the postfix extentions
*/
/**************************************************************************/
std::string _GEOM::Mesh_Loader::getFileNameWithExtension(const std::string& ResourceFilePath)
{
	size_t lastSlash = ResourceFilePath.find_last_of("/\\");
	std::string RawName = ResourceFilePath.substr(lastSlash + 1);
	return RawName;
}


/***************************************************************************/
/*!
\brief
	Loads and serializes all the meshes
*/
/**************************************************************************/
void LoadAndSerializeAllMeshes(_GEOM::DescriptorData& Desc)
{
	// looping through all the filepaths in the descriptor file
	for (; Desc.m_iMeshCurrent < Desc.m_iMeshTotal; ++Desc.m_iMeshCurrent)
	{
		_GEOM::Mesh_Loader meshLoader;
		_GEOM::SkinGeom SkinGeom;

		meshLoader.getMeshName() = meshLoader.getFileNameWithoutExtension(Desc.m_Filepaths[Desc.m_iMeshCurrent]);
		bool Err = meshLoader.Load(Desc, &SkinGeom);
		assert(Err);

		std::cout << ">>== \t\tSuccessfully Loaded Asset\n";
		std::cout << ">>== \t\tCreating Geom Structure\n";
		_GEOM::Geom geom{};
		SkinGeom.CastToGeomStruct(geom);

		std::string sGeomFilepath = Desc.m_sOutputPath + meshLoader.getMeshName() + Desc.m_sOutputFormat;

		_GEOM::Geom::SerializeGeom(sGeomFilepath, geom);
		std::cout << ">>== \t\tSERIALIZATION SUCCESSFUL\n";
		std::cout << ">>== \t\tEnding \t\t==\n";
	}
}


/***************************************************************************/
/*!
\brief
	entry point to the geom compiler
*/
/**************************************************************************/
int main(int argc, char* argv[])
{
	std::cout << "================================================================================\n";
	std::cout << ">>\t\tLaunching GEOM Compiler!\n";
	std::cout << "\tAssimp Version: " << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." << aiGetVersionRevision() << std::endl;
	std::cout << "================================================================================\n";

	std::string descriptorFilepath{};
	_GEOM::DescriptorData Desc;
	descriptorFilepath = "./descriptor_files/default_descriptor.json";

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

		descriptorFilepath = argv[1];

		// Load the descriptor data
		std::cout << ">>\t\tLoading Descriptor Data For ONE Meshes\n";
		bool Err = _GEOM::DescriptorData::DeserializeGEOM_DescriptorDataFromFile(Desc, descriptorFilepath);
		assert(Err);
	}

	else
	{
		// Load the descriptor data
		std::cout << ">>\t\tLoading Descriptor Data For ALL Meshes\n";
		bool Err = _GEOM::DescriptorData::DeserializeGEOM_DescriptorDataFromFolder(Desc, descriptorFilepath);
		assert(Err);
	}

	// Load and serialize all the meshes
	LoadAndSerializeAllMeshes(Desc);

	return 0;
}