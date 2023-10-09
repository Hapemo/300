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
		static bool LoadDescriptorDataFromFolder(DescriptorData& Desc, std::string folderpath) noexcept;

		/***************************************************************************/
		/*!
		\brief
			loads the descriptor data from the provided descriptor filepath.
			This loads the fbx from the filepath provided in the descriptor file
		*/
		/**************************************************************************/
		static bool LoadDescriptorDataFromFile(DescriptorData& Desc, std::string filepath) noexcept;


		/***************************************************************************/
		/*!
		\brief
			Default constructor for the descriptordata struct
		*/
		/**************************************************************************/
		DescriptorData();
	};
}

#endif // !_DESCRIPTOR_H