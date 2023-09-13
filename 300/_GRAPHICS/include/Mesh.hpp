#ifndef MESH_HPP
#define MESH_HPP


#include "Math.hpp"
#include "Vao.hpp"
#include "Vbo.hpp"
#include <geom.h>
#include "Resource/Guid.h"

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
		void LoadFromGeom(const _GEOM::Geom& GeomData, std::vector<vec3>& positions, std::vector<glm::vec2>& uvs, std::vector<unsigned int>& indices);
		void Setup(std::vector<vec3> const& positions, std::vector<unsigned int> const& indices, std::vector<vec2> const& TexCoords = std::vector<vec2>{});
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

		// This mesh may contain multiple different animations. These animations are unique to this mesh.
		std::vector<_GEOM::Animation>	mAnimation{};
		bool							mHasAnimation{ false };


	private:
		// Vertex array object and buffer object for each mesh
		VAO mVao;
		VBO mVbo;
		VBO mEbo;
		VBO mColorVbo;
		VBO mTexCoordVbo;
		VBO mLTWVbo;
		VBO mBoneIDVbo;
		VBO mBoneWeightVbo;

		// Stats of the mesh model
		std::string mMeshName;
		int mVertexCount{};
		int mIndexCount{};

		// TODO: Should be able to know what shader to use for this mesh

		// TODO: Should be able to know what textures to use for this mesh
	};


	struct MeshData {
		void* m_pData;
		Mesh meshdata;
	};


	class MeshManager
	{
	public:
		//constexpr static int  MAX_RESOURCE = 2400;

		std::vector<Mesh> mSceneMeshes;
		//std::unordered_map<unsigned, MeshData*> mSceneMeshes;
		//MeshData* m_pInfoBufferEmptyHead{ nullptr };
		//std::array<MeshData, MAX_RESOURCE>         m_Meshbuffer;

		//MeshData& AllocRscInfo();
		//void ReleaseRscInfo(MeshData& RscInfo);

	public:
		static MeshManager& GetInstance()
		{
			static MeshManager instance;
			return instance;
		}

		MeshManager() = default;
		MeshManager(MeshManager const&) = delete;
		void Init();
		//void Destroy();

		void operator=(MeshManager const&) = delete;

	private:
		const std::string compiled_geom_path = "../compiled_geom/";
		void SetupMesh(std::string filepath);
	};
}


namespace Deserialization
{
	bool DeserializeGeom(const std::string Filepath, _GEOM::Geom& GeomData) noexcept;
	void DeserializeAssimpNodeData(std::ifstream& infile, _GEOM::AssimpNodeData& Node);
}

#endif
