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

#include <descriptor.h>
#include <Guid.h>

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

		if (document.HasMember("GUID"))
		{
			rapidjson::Value& assetFilepaths = document["GUID"];
			assert(assetFilepaths.IsUint());

			Desc.m_GUID = assetFilepaths.GetUint();
		}

		return true;	// no error
	}


	bool DescriptorData::SerializeGEOM_DescriptorDataToFile(std::string geomFilepath, const DescriptorData& GEOM_DescriptorFile) noexcept
	{
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

		rapidjson::Value fbxfilepath(GEOM_DescriptorFile.m_Filepaths[0].c_str(), allocator);
		doc.AddMember("Asset_Filepaths", fbxfilepath, allocator);

		doc.AddMember("GUID", GEOM_DescriptorFile.m_GUID, allocator);
		doc.AddMember("Asset_OutputPath", "../assets/compiled_geom/", allocator);
		doc.AddMember("Compiled_Geom_Format", ".geom", allocator);

		rapidjson::Value sclvecObj(rapidjson::kArrayType);
		rapidjson::Value rotvecObj(rapidjson::kArrayType);
		rapidjson::Value trnsvecObj(rapidjson::kArrayType);

		sclvecObj.PushBack(GEOM_DescriptorFile.m_Scale.x , allocator);
		sclvecObj.PushBack(GEOM_DescriptorFile.m_Scale.y , allocator);
		sclvecObj.PushBack(GEOM_DescriptorFile.m_Scale.z , allocator);

		rotvecObj.PushBack(GEOM_DescriptorFile.m_Rotate.x, allocator);
		rotvecObj.PushBack(GEOM_DescriptorFile.m_Rotate.y, allocator);
		rotvecObj.PushBack(GEOM_DescriptorFile.m_Rotate.z, allocator);

		trnsvecObj.PushBack(GEOM_DescriptorFile.m_Translate.x, allocator);
		trnsvecObj.PushBack(GEOM_DescriptorFile.m_Translate.y, allocator);
		trnsvecObj.PushBack(GEOM_DescriptorFile.m_Translate.z, allocator);

		doc.AddMember("Pre_Transform_Scale", sclvecObj, allocator);
		doc.AddMember("Pre_Transform_Rotate", rotvecObj, allocator);
		doc.AddMember("Pre_Transform_Translate", trnsvecObj, allocator);

		// Serialize to a file
		std::ofstream file(geomFilepath.c_str());
		if (file.is_open()) 
		{
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			doc.Accept(writer);

			file << buffer.GetString() << std::endl;
			file.close();

			std::cout << "JSON data serialized to " << geomFilepath << std::endl;
		}
		else {
			std::cerr << "Failed to open the file for writing." << std::endl;
			return false;
		}

		return true;
	}



	bool DescriptorData::CheckGUID(std::string geomFilepath, const Asset_DescriptorData& GeomDescriptorFile) noexcept
	{
		// Read the json data from the file
		std::ifstream file(geomFilepath);
		assert(file.is_open());

		std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Parse the json data using rapidjson
		rapidjson::Document document;
		if (document.Parse(jsonContent.c_str()).HasParseError())
		{
			std::cout << "Error parsing json file: " << geomFilepath << std::endl;
			return false;
		}

		//!< Access the JSON data
		if (document.HasMember("GUID"))
		{
			rapidjson::Value& assetFilepaths = document["GUID"];
			assert(assetFilepaths.IsUint());
			if (GeomDescriptorFile.m_GUID == assetFilepaths.GetUint())
				return true;
		}

		// no match is found
		return false;
	}


	unsigned GetGUID(std::string geomFilepath) noexcept
	{
		// Read the json data from the file
		std::ifstream file(geomFilepath);
		assert(file.is_open());

		std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Parse the json data using rapidjson
		rapidjson::Document document;
		if (document.Parse(jsonContent.c_str()).HasParseError())
		{
			std::cout << "Error parsing json file: " << geomFilepath << std::endl;
			return -1;
		}

		//!< Access the JSON data
		if (document.HasMember("GUID"))
		{
			rapidjson::Value& assetFilepaths = document["GUID"];
			assert(assetFilepaths.IsUint());

			return assetFilepaths.GetUint();
		}

		// no GUID is found
		return -1;
	}


	bool CheckAndCreateDescriptorFileMESH(std::string fbxfilepath, std::string& GEOM_Descriptor_Filepath)
	{
		auto getFilename = [](const std::string& str) -> std::string
		{
			size_t found = str.find_last_of("\\");

			if (found == std::string::npos)
			{
				found = str.find_last_of("/");
			}

			return str.substr(found + 1);
		};

		// GUID is generated here
		uid meshguid(fbxfilepath);
		_GEOM::Asset_DescriptorData fbxDescriptor(meshguid.id);

		//>> 1.0: Check if the FBX Desc file is present using filename
		std::string fbxFilepath = fbxfilepath.substr(0, fbxfilepath.find_last_of("/"));
		std::string FBX_DescFile = getFilename(fbxfilepath) + ".desc";
		bool descFilePresent = false;

		std::filesystem::path FBXfolderpath = fbxFilepath;
		for (const auto& FBXentry : std::filesystem::directory_iterator(FBXfolderpath))
		{
			//std::cout << "FBXentry: " << getFilename(FBXentry.path().string()) << std::endl;
			if (getFilename(FBXentry.path().string()) == FBX_DescFile)
			{
				// FBX Descriptor file is present
				descFilePresent = true;
				break;
			}
		}

		//>> 2.0: The FBX Descriptor file is not present. We have to create one to serialize the GUID
		if (!descFilePresent)
		{
			std::string descFilepath = fbxFilepath + "/" + FBX_DescFile;
			_GEOM::Asset_DescriptorData::SerializeASSET_DescriptorFile(descFilepath, fbxDescriptor);
		}

		// 2.1: if the FBX descriptor is present, we deserialize the GUID from the file
		else
		{
			// >> CHECK: Do we really need this step? will the guid just be the same as the one generated above?
			std::string descFilepath = fbxFilepath + "/" + FBX_DescFile;
			_GEOM::Asset_DescriptorData::DeSerializeASSET_DescriptorFile(descFilepath, fbxDescriptor);
		}

		//>> 3.0: Run through the GEOM descriptor files to find a matching GUID
		std::filesystem::path GEOMfolderpath = _GEOM::mGeomFolderpath;
		descFilePresent = false;
		for (const auto& GEOMentry : std::filesystem::directory_iterator(GEOMfolderpath))
		{
			// 3.1: Check the GEOM Descriptor whether the GUID is the same
			std::string geomdesccheck = GEOMentry.path().string().substr(GEOMentry.path().string().find_last_of("."));
			if (geomdesccheck != ".desc")
				continue;

			//std::cout << "GEOMentry: " << getFilename(GEOMentry.path().string()) << std::endl;
			if (_GEOM::DescriptorData::CheckGUID(GEOMentry.path().string(), fbxDescriptor))
			{
				GEOM_Descriptor_Filepath = GEOMentry.path().string();
				descFilePresent = true;
				break;
			}
		}

		// 3.1: If the GEOM descriptor file is not present
		_GEOM::DescriptorData geomDescriptor;
		geomDescriptor.m_GUID = fbxDescriptor.m_GUID;
		geomDescriptor.m_Filepaths.emplace_back(fbxfilepath);

		// If the descriptor file is not present, then load it
		if (!descFilePresent)
		{
			//>> Create a default geom descriptor file
			std::string geomdescName = getFilename(fbxfilepath).substr(0, getFilename(fbxfilepath).find_last_of("."));
			GEOM_Descriptor_Filepath = _GEOM::mGeomFolderpath + geomdescName + ".geom.desc";
			_GEOM::DescriptorData::SerializeGEOM_DescriptorDataToFile(GEOM_Descriptor_Filepath, geomDescriptor);
		}

		return descFilePresent;
	}

	bool CheckAndCreateDescriptorFileTEXTURE(std::string uncompressedfilepath, std::string& CompressedTexture_Descriptor_Filepath, const std::string& guidstr)
	{
		auto getFilename = [](const std::string& str) -> std::string
		{
			size_t found = str.find_last_of("\\");

			if (found == std::string::npos)
			{
				found = str.find_last_of("/");
			}

			return str.substr(found + 1);
		};

		// GUID is generated here
		uid guid(guidstr);
		_GEOM::Asset_DescriptorData uncompressedDesc(guid.id);

		//>> 1.0: Check if the UNCOMPRESSED Desc file is present using filename
		std::string UncompressedFilepath = uncompressedfilepath.substr(0, uncompressedfilepath.find_last_of("/"));
		std::string Uncompressed_Filename = getFilename(uncompressedfilepath);
		std::string Uncompressed_DescFile = Uncompressed_Filename + ".desc";
		bool descFilePresent = false;

		std::filesystem::path TextureFolderpath = UncompressedFilepath;
		for (const auto& UNCOMPRESSEDentry : std::filesystem::directory_iterator(TextureFolderpath))
		{
			//std::cout << "UNCOMPRESSEDentry: " << getFilename(UNCOMPRESSEDentry.path().string()) << std::endl;
			if (getFilename(UNCOMPRESSEDentry.path().string()) == Uncompressed_DescFile)
			{
				// FBX Descriptor file is present
				descFilePresent = true;
				break;
			}
		}

		//>> 2.0: The FBX Descriptor file is not present. We have to create one to serialize the GUID
		if (!descFilePresent)
		{
			std::string descFilepath = UncompressedFilepath + "/" + Uncompressed_DescFile;
			_GEOM::Asset_DescriptorData::SerializeASSET_DescriptorFile(descFilepath, uncompressedDesc);
		}

		// 2.1: if the FBX descriptor is present, we use the deserialized GUID from the file
		else
		{
			std::string descFilepath = UncompressedFilepath + "/" + Uncompressed_DescFile;
			_GEOM::Asset_DescriptorData::DeSerializeASSET_DescriptorFile(descFilepath, uncompressedDesc);
		}

		//>> 3.0: Run through the GEOM descriptor files to find a matching GUID
		std::filesystem::path COMPRESSEDfolderpath = _GEOM::mCompressedTextureFolderpath;
		descFilePresent = false;	// reset the boolean
		for (const auto& COMPRESSEDentry : std::filesystem::directory_iterator(COMPRESSEDfolderpath))
		{
			// 3.1: Check the COMPRESSED Descriptor whether the GUID is the same
			std::string compdesccheck = COMPRESSEDentry.path().string().substr(COMPRESSEDentry.path().string().find_last_of("."));
			if (compdesccheck != ".desc")
				continue;

			if (_GEOM::DescriptorData::CheckGUID(COMPRESSEDentry.path().string(), uncompressedDesc))
			{
				CompressedTexture_Descriptor_Filepath = COMPRESSEDentry.path().string();
				descFilePresent = true;
				break;
			}
		}

		// Populate the local texture descriptor data with default data
		_GEOM::Texture_DescriptorData TextureDesc;
		TextureDesc.mGUID = uncompressedDesc.m_GUID;
		TextureDesc.mCompressionType = CompressionType::SRGB;

		// 3.1: If the COMPRESSED descriptor file is not present, then load it
		if (!descFilePresent)
		{
			//>> Create a default geom descriptor file
			std::string Texture_name = Uncompressed_Filename.substr(0, Uncompressed_Filename.find_last_of("."));
			CompressedTexture_Descriptor_Filepath = _GEOM::mCompressedTextureFolderpath + Texture_name + ".ctexture.desc";
			_GEOM::Texture_DescriptorData::SerializeTEXTURE_DescriptorDataToFile(CompressedTexture_Descriptor_Filepath, TextureDesc);
		}

		return descFilePresent;
	}

	void Asset_DescriptorData::SerializeASSET_DescriptorFile(std::string fbxFilepath, const Asset_DescriptorData& FBX_DescriptorFile) noexcept
	{
		rapidjson::Document doc;
		doc.SetObject();
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

		doc.AddMember("GUID", FBX_DescriptorFile.m_GUID, allocator);

		// Serialize to a file
		std::ofstream file(fbxFilepath.c_str());
		if (file.is_open()) {
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			doc.Accept(writer);

			file << buffer.GetString() << std::endl;
			file.close();

			std::cout << "JSON data serialized to " << fbxFilepath << std::endl;
		}
		else {
			std::cerr << "Failed to open the file for writing." << std::endl;
		}

	}


	void Asset_DescriptorData::DeSerializeASSET_DescriptorFile(std::string fbxFilepath, Asset_DescriptorData& FBX_DescriptorFile) noexcept
	{
		// Read the json data from the file
		std::ifstream file(fbxFilepath);
		assert(file.is_open());

		std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Parse the json data using rapidjson
		rapidjson::Document document;
		if (document.Parse(jsonContent.c_str()).HasParseError())
		{
			std::cout << "Error parsing json file: " << fbxFilepath << std::endl;
			return;
		}

		//!< Access the JSON data
		if (document.HasMember("GUID"))
		{
			rapidjson::Value& assetFilepaths = document["GUID"];
			assert(assetFilepaths.IsUint());
			FBX_DescriptorFile.m_GUID = assetFilepaths.GetUint();
		}
	}


	bool Texture_DescriptorData::isGammaSpace(std::string descriptorfilepath) noexcept
	{
		std::ifstream file(descriptorfilepath);
		assert(file.is_open());

		std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Parse the json data using rapidjson
		rapidjson::Document document;
		if (document.Parse(jsonContent.c_str()).HasParseError())
		{
			std::cout << "Error parsing json file: " << descriptorfilepath << std::endl;
			return false;
		}

		if (document.HasMember("CompressionType"))
		{
			rapidjson::Value& assetFilepaths = document["CompressionType"];
			assert(assetFilepaths.IsString());
			std::string comp = assetFilepaths.GetString();
			if (comp == "SRGB")
				return true;
			return false;
		}

		return false;
	}


	bool Texture_DescriptorData::DeserializeTEXTURE_DescriptorDataFromFile(Texture_DescriptorData& Desc, std::string textureFilepath) noexcept
	{
		// Read the json data from the file
		std::ifstream file(textureFilepath);
		if (!file.is_open())
			return false;

		std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// Parse the json data using rapidjson
		rapidjson::Document document;
		if (document.Parse(jsonContent.c_str()).HasParseError())
		{
			std::cout << "Error parsing json file: " << textureFilepath << std::endl;
			return false;	// error
		}

		//!< Access the JSON data
		if (document.HasMember("GUID"))
		{
			rapidjson::Value& assetFilepaths = document["GUID"];
			assert(assetFilepaths.IsUint());
			Desc.mGUID = assetFilepaths.GetUint();
		}

		if (document.HasMember("CompressionType"))
		{
			rapidjson::Value& assetFilepaths = document["CompressionType"];
			assert(assetFilepaths.IsString());
			std::string comp = assetFilepaths.GetString();
			if (comp == "SRGB")
				Desc.mCompressionType = CompressionType::SRGB;
			else if (comp == "RGB")
				Desc.mCompressionType = CompressionType::RGB;
		}

		if (document.HasMember("Descriptor_Filepath"))
		{
			rapidjson::Value& assetFilepaths = document["Descriptor_Filepath"];
			assert(assetFilepaths.IsString());
			Desc.mDescFilepath = assetFilepaths.GetString();
		}

		return true; // success
	}

	bool Texture_DescriptorData::SerializeTEXTURE_DescriptorDataToFile(std::string textureFilepath, const Texture_DescriptorData& textureDesc) noexcept
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

		std::string comp;
		switch (textureDesc.mCompressionType)
		{
		case CompressionType::SRGB:
			comp = "SRGB";
			break;
			
		case CompressionType::RGB:
			comp = "RGB";
			break;
		}

		doc.SetObject();
		rapidjson::Value compressionType(comp.c_str(), allocator);
		rapidjson::Value textureFilepathValue(textureFilepath.c_str(), allocator);

		doc.AddMember("GUID", textureDesc.mGUID, allocator);						// allocate GUID
		doc.AddMember("CompressionType", compressionType, allocator);				// allocate CompressionType (SRGB or RGB
		doc.AddMember("Descriptor_Filepath", textureFilepathValue, allocator);		// allocate Descriptor_Filepath (the filepath to the .ctexture.desc file

		// Serialize to a file
		std::ofstream file(textureFilepath.c_str());
		if (file.is_open())
		{
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			doc.Accept(writer);

			file << buffer.GetString() << std::endl;
			file.close();

			std::cout << "JSON data serialized to: " << textureFilepath << std::endl;
		}
		else {
			std::cerr << "Failed to open the file for writing." << std::endl;
			return false;
		}

		return true;
	}

}