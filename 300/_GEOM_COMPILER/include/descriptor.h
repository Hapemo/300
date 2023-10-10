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
	Contains the functionalities and data stored from the descriptor files
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
			Default constructor for the descriptordata struct
		*/
		/**************************************************************************/
		DescriptorData();
	};

	struct FBX_DescriptorData
	{
		FBX_DescriptorData(unsigned int guid) : m_GUID(guid) {}

		unsigned int m_GUID;

		static void SerializeFBX_DescriptorFile(std::string fbxFilepath, const FBX_DescriptorData& FBX_DescriptorFile) noexcept;
		static void LoadFBX_DescriptorFile(std::string fbxFilepath, FBX_DescriptorData& FBX_DescriptorFile) noexcept;
	};


}

#endif // !_DESCRIPTOR_H