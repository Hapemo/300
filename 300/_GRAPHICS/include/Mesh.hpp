/**-------------------------------------------------------------------------------------/
 * @file
 *  Camera.hpp
 * @author
 *  Lee Fu Sheng Roy, 670000622,	f.lee@digipen.edu
 *  Richmond Choo,					r.choo@digipen.edu
 * @date
 *  2023/09/21
 * @brief
 *  Class definition of Camera.
 * @copyright
 *  Copyright (C) 2023 DigiPen Institute of Technology.
 *-------------------------------------------------------------------------------------*/

#ifndef MESH_HPP
#define MESH_HPP


#include "Math.hpp"
#include "Vao.hpp"
#include "Vbo.hpp"
#include <geom.h>

#include "stb_image.h"


constexpr int MAX_INSTANCES = 100;

// Follows the format in the shader code
/******************************************
	location = 0 --> Pos;
	location = 1 --> VertexColor;
	location = 2 --> LTW;
******************************************/

namespace GFX
{

	class Mesh
	{
	public:
		// -- Called once on startup --
		void LoadFromGeom(const _GEOM::Geom& GeomData, std::vector<vec3>& positions, std::vector<glm::vec2>& uvs, 
						std::vector<unsigned int>& indices, std::vector<vec3>& normals, std::vector<vec3>& tangents);
		void LoadAnimationDataFromGeom(const _GEOM::Geom& GeomData, std::vector<glm::vec4>& boneIDs, std::vector<glm::vec4>& boneWeights);
		void Setup(std::vector<vec3> const& positions, std::vector<unsigned int> const& indices, std::vector<vec2> const& TexCoords = std::vector<vec2>{}, unsigned colorDivisor = 1u);
		void Setup(const _GEOM::Geom& GeomData);

		// -- Used when drawing --
		void BindVao();
		void PrepForDraw();
		void UnbindVao();

		void DrawAllInstances();

		// -- Used after drawing current scene --
		void ClearInstances();

		// -- Getter --
		int GetVertexCount() { return mVertexCount; }
		int GetIndexCount() { return mIndexCount; }

		// Called once on exit --
		void Destroy();

		// Stores the rendering data for each instance of mesh
		std::vector<mat4> mLTW;
		std::vector<vec4> mColors;
		std::vector<vec4> mTexEntID;

		// This mesh may contain multiple different animations. These animations are unique to this mesh.
		std::vector<_GEOM::Animation>	mAnimation{};
		bool							mHasAnimation{ false };
		_GEOM::bbox						mBBOX;


	private:
		// Vertex array object and buffer object for each mesh
		VAO mVao;
		VBO mVbo;
		VBO mEbo;
		VBO mColorVbo;
		VBO mTexCoordVbo;
		VBO mTexEntIDVbo;
		VBO mLTWVbo;
		VBO mTangentVbo;
		VBO mBitTangentVbo;
		VBO mNormalVbo;
		VBO mBoneIDVbo;
		VBO mBoneWeightVbo;

		// Stats of the mesh model
		std::string		mMeshName;
		int				mVertexCount{};
		int				mIndexCount{};
	};


	struct MeshData {
		void* m_pData;
		Mesh meshdata;
	};


	class MeshManager
	{
	public:
		std::vector<Mesh> mSceneMeshes;

	public:
		static MeshManager& GetInstance()
		{
			static MeshManager instance;
			return instance;
		}

		MeshManager() = default;
		MeshManager(MeshManager const&) = delete;
		void Init();

		void operator=(MeshManager const&) = delete;

	private:
		const std::string compiled_geom_path = "../assets/compiled_geom/";
		void SetupMesh(std::string filepath);
	};
}


namespace Deserialization
{
	bool DeserializeGeom(const std::string Filepath, _GEOM::Geom& GeomData) noexcept;
	void DeserializeAssimpNodeData(std::ifstream& infile, _GEOM::AssimpNodeData& Node);
}

#endif
