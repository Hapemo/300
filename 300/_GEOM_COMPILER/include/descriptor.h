#pragma once

#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include<glm/glm.hpp>
#include <vector>
#include <string>

namespace _GEOM
{
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

		// Descriptor Functions //
		void PrintData();
		static bool LoadDescriptorData(DescriptorData& Desc, std::string filepath) noexcept;
		DescriptorData();
	};
}

#endif // !_DESCRIPTOR_H