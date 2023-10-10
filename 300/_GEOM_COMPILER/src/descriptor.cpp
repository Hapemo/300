/*!*************************************************************************
****
\file			Descriptor.cpp
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	The function definition for the descriptor data class, and its
	supporting functionalities

****************************************************************************
***/


#include <RAPID-JSON/writer.h>
#include "RAPID-JSON/document.h"
#include "RAPID-JSON/stringbuffer.h"
#include "RAPID-JSON/filereadstream.h"
#include "RAPID-JSON/filewritestream.h"

#include "descriptor.h"
#include <iostream>
#include <fstream>

#include <document.h>
#include <writer.h>
#include <stringbuffer.h>

#include <filesystem>

namespace _GEOM
{

/***************************************************************************/
/*!
\brief
	default constructor
*/
/**************************************************************************/
	DescriptorData::DescriptorData()
	{
		m_iMeshTotal = 0;
		m_iMeshCurrent = 0;
		m_sOutputPath = "DEFAULT";
		m_sOutputFormat = "DEFAULT";

		m_Scale = glm::vec3(1.f);
		m_Rotate = glm::vec3(0.f);
		m_Translate = glm::vec3(0.f);
	}

/***************************************************************************/
/*!
\brief
	loads the descriptor data from the provided descriptordata filepath
*/
/**************************************************************************/
	bool DescriptorData::DeserializeGEOM_DescriptorDataFromFolder(DescriptorData& Desc, std::string descriptorFilepath) noexcept
	{
		// Read the json data from the file
		std::ifstream file(descriptorFilepath);
		assert(file.is_open());

		std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Parse the json data using rapidjson
		rapidjson::Document document;
		if (document.Parse(jsonContent.c_str()).HasParseError())
		{
			std::cout << "Error parsing json file: " << descriptorFilepath << std::endl;
			return false;
		}

		// Access the JSON data
		if (document.HasMember("Asset_Filepaths"))
		{
			rapidjson::Value& assetFilepaths = document["Asset_Filepaths"];
			assert(assetFilepaths.IsString());

			// Loads all the files in the folder
			std::filesystem::path folderpath = assetFilepaths.GetString();
			for (const auto& entry : std::filesystem::directory_iterator(folderpath))
			{
				if (std::filesystem::is_regular_file(entry))
				{
					++Desc.m_iMeshTotal;
					Desc.m_Filepaths.push_back(entry.path().string());
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



/***************************************************************************/
/*!
\brief
	loads the descriptor data from the provided descriptordata filepath
*/
/**************************************************************************/
	bool DescriptorData::DeserializeGEOM_DescriptorDataFromFile(DescriptorData& Desc, std::string descriptorFilepath) noexcept
	{
		// Read the json data from the file
		std::ifstream file(descriptorFilepath);
		assert(file.is_open());

		std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Parse the json data using rapidjson
		rapidjson::Document document;
		if (document.Parse(jsonContent.c_str()).HasParseError())
		{
			std::cout << "Error parsing json file: " << descriptorFilepath << std::endl;
			return false;
		}


		//!< Access the JSON data
		if (document.HasMember("Asset_Filepaths"))
		{
			rapidjson::Value& assetFilepaths = document["Asset_Filepaths"];
			assert(assetFilepaths.IsString());
			++Desc.m_iMeshTotal;
			Desc.m_Filepaths.push_back(assetFilepaths.GetString());
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


	bool DescriptorData::SerializeGEOM_DescriptorDataToFile(std::string geomFilepath, const DescriptorData& GEOM_DescriptorFile) noexcept
	{
		return true;
	}


	void FBX_DescriptorData::SerializeFBX_DescriptorFile(std::string fbxFilepath, const FBX_DescriptorData& FBX_DescriptorFile) noexcept
	{
		//rapidjson::Document document;
		//document.SetObject(); // Create an empty object

		//// Add data to the object
		//rapidjson::Value person;
		//person.SetObject();
		//person.AddMember("name", "John", document.GetAllocator());
		//person.AddMember("age", 30, document.GetAllocator());
		//person.AddMember("isStudent", false, document.GetAllocator());

		//// Add the "person" object to the main object
		//document.AddMember("person", person, document.GetAllocator());

		//// Specify the file path where you want to save the JSON data
		//// Open the file for writing
		//FILE* fp = fopen(fbxFilepath.c_str(), "w");
		//if (!fp) {
		//	std::cerr << "Failed to open file for writing." << std::endl;
		//	return;
		//}

		//// Create a FileWriteStream to write JSON to the file
		//char buffer[4096];
		//rapidjson::FileWriteStream fileStream(fp, buffer, sizeof(buffer));
		//rapidjson::Writer<rapidjson::FileWriteStream> writer(fileStream);

		//// Serialize the document to the file
		//document.Accept(writer);

		//// Close the file
		//fclose(fp);

		std::cout << "JSON data written to " << fbxFilepath << std::endl;
	}


	void FBX_DescriptorData::LoadFBX_DescriptorFile(std::string fbxFilepath, FBX_DescriptorData& FBX_DescriptorFile) noexcept
	{

	}

}