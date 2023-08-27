#include "descriptor.h"
#include <iostream>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <filesystem>

#define MAX_FILEPATHS	 50

namespace _GEOM
{
	DescriptorData::DescriptorData()
	{
		m_iMeshTotal = 0;
		m_iMeshCurrent = 0;
		m_sOutputPath = "DEFAULT";
		m_sOutputFormat = "DEFAULT";
		m_Filepaths.resize(MAX_FILEPATHS);

		m_Scale = glm::vec3(1.f);
		m_Rotate = glm::vec3(0.f);
		m_Translate = glm::vec3(0.f);
	}

	bool DescriptorData::LoadDescriptorData(DescriptorData& Desc, std::string filepath) noexcept
	{
		// Read the json data from the file
		std::ifstream file(filepath);
		assert(file.is_open());

		std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Parse the json data using rapidjson
		rapidjson::Document document;
		if (document.Parse(jsonContent.c_str()).HasParseError())
		{
			std::cout << "Error parsing json file: " << filepath << std::endl;
			return false;
		}

		// Access the JSON data
		if (document.HasMember("Asset_Filepaths"))
		{
			rapidjson::Value& assetFilepaths = document["Asset_Filepaths"];
			assert(assetFilepaths.IsString());

			std::filesystem::path folderpath = assetFilepaths.GetString();
			for (const auto& entry : std::filesystem::directory_iterator(folderpath))
			{
				if (std::filesystem::is_regular_file(entry))
				{
					Desc.m_Filepaths[Desc.m_iMeshTotal++] = entry.path().string();
				}
			}
		}

		if (document.HasMember("Asset_OutputPath"))
		{	
			rapidjson::Value& assetOutputPath = document["Asset_OutputPath"];
			assert(assetOutputPath.IsString());
			Desc.m_sOutputPath = assetOutputPath.GetString();
		}

		if (document.HasMember("Compiled_Geom_Format"))
		{
			rapidjson::Value& assetOutputFormat = document["Compiled_Geom_Format"];
			assert(assetOutputFormat.IsString());
			Desc.m_sOutputFormat = assetOutputFormat.GetString();
		}

		if (document.HasMember("Pre_Transform_Scale"))
		{
			rapidjson::Value& preTransformScale = document["Pre_Transform_Scale"];
			assert(preTransformScale.IsArray());
			assert(preTransformScale.Size() == 3);
			Desc.m_Scale.x = preTransformScale[0].GetFloat();
			Desc.m_Scale.y = preTransformScale[1].GetFloat();
			Desc.m_Scale.z = preTransformScale[2].GetFloat();
		}

		if (document.HasMember("Pre_Transform_Rotate"))
		{
			rapidjson::Value& preTransformRotate = document["Pre_Transform_Rotate"];
			assert(preTransformRotate.IsArray());
			assert(preTransformRotate.Size() == 3);
			Desc.m_Rotate.x = preTransformRotate[0].GetFloat();
			Desc.m_Rotate.y = preTransformRotate[1].GetFloat();
			Desc.m_Rotate.z = preTransformRotate[2].GetFloat();
		}

		if (document.HasMember("Pre_Transform_Translate"))
		{
			rapidjson::Value& preTransformTranslate = document["Pre_Transform_Translate"];
			assert(preTransformTranslate.IsArray());
			assert(preTransformTranslate.Size() == 3);
			Desc.m_Translate.x = preTransformTranslate[0].GetFloat();
			Desc.m_Translate.y = preTransformTranslate[1].GetFloat();
			Desc.m_Translate.z = preTransformTranslate[2].GetFloat();
		}

		return true;	// no error
	}
}