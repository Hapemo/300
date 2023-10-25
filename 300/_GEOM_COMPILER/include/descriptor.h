/*!*************************************************************************
****
\file			Descriptor.h
\author			Richmond
\par DP email:	r.choo@digipen.edu
\date			28/9/23
\brief
	This class contains the descriptor data that is loaded from the descriptor
	files.

****************************************************************************
***/

#pragma once

#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include<glm/glm.hpp>
#include <vector>
#include <string>

namespace _GEOM
{

/***************************************************************************/
/*!
\brief
	Contains the functionalities and data stored in the FBX descriptor files
*/
/**************************************************************************/
	struct Asset_DescriptorData
	{
		Asset_DescriptorData(unsigned int guid) : m_GUID(guid) {}

		unsigned int m_GUID;

		static void SerializeASSET_DescriptorFile(std::string AssetFilepath, const Asset_DescriptorData& Asset_DescriptorFile) noexcept;
		static void DeSerializeASSET_DescriptorFile(std::string AssetFilepath, Asset_DescriptorData& Asset_DescriptorFile) noexcept;
	};


/***************************************************************************/
/*!
\brief
	Contains the functionalities and data stored in the Texture descriptor
	files
*/
/**************************************************************************/
	enum class CompressionType : int
	{
		SRGB = 0,
		RGB
	};

	struct Texture_DescriptorData
	{
		CompressionType	mCompressionType;
		unsigned int	mGUID;

		static bool DeserializeTEXTURE_DescriptorDataFromFile(Texture_DescriptorData& Desc, std::string textureFilepath) noexcept;
		static bool SerializeTEXTURE_DescriptorDataToFile(std::string textureFilepath, const Texture_DescriptorData& textureDesc) noexcept;
	};


/***************************************************************************/
/*!
\brief
	Contains the functionalities and data stored in the GEOM descriptor files
*/
/**************************************************************************/
	struct DescriptorData
	{
		// Import Mesh //
		std::vector<std::string> m_Filepaths;
		
		// Pre Transforms //
		glm::vec3 m_Scale;			
		glm::vec3 m_Rotate;
		glm::vec3 m_Translate;

		// Import Mesh Cout //
		int m_iMeshTotal;
		int m_iMeshCurrent;

		// Export Mesh //
		std::string m_sOutputPath;
		std::string m_sOutputFormat;

		// GUID //
		unsigned int m_GUID;


		/***************************************************************************/
		/*!
		\brief
			Helper function to print data
		*/
		/**************************************************************************/
		void PrintData();

		/***************************************************************************/
		/*!
		\brief
			loads the descriptor data from the provided descriptor data folderpath.
			This loads all the fbx in the folder
		*/
		/**************************************************************************/
		static bool DeserializeGEOM_DescriptorDataFromFolder(DescriptorData& Desc, std::string folderpath) noexcept;

		/***************************************************************************/
		/*!
		\brief
			loads the descriptor data from the provided descriptor filepath.
			This loads the fbx from the filepath provided in the descriptor file
		*/
		/**************************************************************************/
		static bool DeserializeGEOM_DescriptorDataFromFile(DescriptorData& Desc, std::string filepath) noexcept;


		/***************************************************************************/
		/*!
		\brief
			Serializes the GEOM descriptor data to the provided filepath
		*/
		/**************************************************************************/
		static bool SerializeGEOM_DescriptorDataToFile(std::string geomFilepath, const DescriptorData& GEOM_DescriptorFile) noexcept;


		/***************************************************************************/
		/*!
		\brief
			Checks the GUID within the provided GEOM desc filepath
		*/
		/**************************************************************************/
		static bool CheckGUID(std::string geomFilepath, const Asset_DescriptorData& GeomDescriptorFile) noexcept;


		/***************************************************************************/
		/*!
		\brief
			Default constructor for the descriptordata struct
		*/
		/**************************************************************************/
		DescriptorData();
	};

	bool CheckAndCreateDescriptorFileMESH(std::string fbxfilepath, std::string& GEOM_Descriptor_Filepath);
	bool CheckAndCreateDescriptorFileTEXTURE(std::string uncompressedfilepath, std::string& Texture_Descriptor_Filepath, const std::string& guid);

	/***************************************************************************/
	/*!
	\brief
		returns the GUID within the provided GEOM desc filepath
	*/
	/**************************************************************************/
	unsigned GetGUID(std::string geomFilepath) noexcept;

	static const std::string mGeomFolderpath = "../assets/compiled_geom/";
	static const std::string mCompressedTextureFolderpath = "../assets/Compressed/";
}

#endif // !_DESCRIPTOR_H