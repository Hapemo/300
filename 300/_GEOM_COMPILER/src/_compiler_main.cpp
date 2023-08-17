#include <Geom_Compiler.h>
#include <geom.h>
#include <iostream>

int main()
{
	std::string lDescriptorFilepath = "./descriptor_files/model_descriptors.json";
	std::cout << "================================================================================\n";
	std::cout << ">>\t\tLaunching GEOM Compiler!\n";
	std::cout << "================================================================================\n";

	// Load the descriptor data
	std::cout << ">>\t\tLoading Descriptor Data\n";
	_GEOM::DescriptorData Desc;
	bool Err = _GEOM::DescriptorData::LoadDescriptorData(Desc, lDescriptorFilepath);
	assert(Err);
	  
	// extracts the raw filename itself
	auto getFileNameWithoutExtension = [](const std::string& ResourceFilePath) -> std::string
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
	};

	std::cout << "\n================================================================================\n";
	std::cout << ">>\t\tLoading Assets\n";

	// looping through all the filepaths in the descriptor file
	for (; Desc.m_iMeshCurrent < Desc.m_iMeshTotal; ++Desc.m_iMeshCurrent)
	{
		_GEOM::Mesh_Loader meshLoader;
		_GEOM::SkinGeom SkinGeom;
		Err = meshLoader.Load(Desc, &SkinGeom);
		assert(Err);

		std::cout << ">>== \t\tSuccessfully Loaded Asset\n";
		std::cout << ">>== \t\tCreating Geom Structure\n";
		_GEOM::Geom geom{};
		SkinGeom.CastToGeomStruct(geom, meshLoader.m_PosCompressionScale, meshLoader.m_UVCompressionScale);

		std::string sRawFileName = getFileNameWithoutExtension(Desc.m_Filepaths[Desc.m_iMeshCurrent]);
		std::string sGeomFilepath = Desc.m_sOutputPath + sRawFileName + Desc.m_sOutputFormat;

		_GEOM::Geom::SerializeGeom(sGeomFilepath, Desc, geom);
		std::cout << ">>== \t\tSERIALIZATION SUCCESSFUL\n";
		std::cout << ">>== \t\tEnding \t\t==\n";
	}

	return 0;
}