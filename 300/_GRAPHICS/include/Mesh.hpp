#ifndef MESH_HPP
#define MESH_HPP

#define _ASSIMP_LOADING 0

#include "Math.hpp"
#include "Vao.hpp"
#include "Vbo.hpp"
#include <geom.h>
#include "Resource/Guid.h"

#include "stb_image.h"

#if _ASSIMP_LOADING
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment( lib, "../lib/assimp/BINARIES/Win32/lib/Release/assimp-vc142-mt.lib")
#endif


constexpr int MAX_INSTANCES = 1000;

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

		// -- Used when drawing --
		void BindVao();
		void PrepForDraw();
		void UnbindVao();

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

#if _ASSIMP_LOADING
		// assimp loaded mesh variables
		std::vector<glm::vec3>		mPositions;
		std::vector<unsigned int>	mIndices;
		static std::vector<Mesh> assimpLoadedMeshes;	
#endif

	private:
		// Vertex array object and buffer object for each mesh
		VAO mVao;
		VBO mVbo;
		VBO mEbo;
		VBO mColorVbo;
		VBO mTexCoordVbo;
		VBO mLTWVbo;

		// Stats of the mesh model
		std::string mMeshName;
		int mVertexCount{};
		int mIndexCount{};
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
	};
}


namespace Deserialization
{
	bool DeserializeGeom(const std::string Filepath, _GEOM::Geom& GeomData) noexcept;
	void DeserializeAssimpNodeData(std::ifstream& infile, _GEOM::AssimpNodeData& Node);
}


#if _ASSIMP_LOADING
namespace AssimpImporter
{
	void loadModel(const std::string& filepath);
	void processnode(aiNode* node, const aiScene* scene);
	GFX::Mesh processmesh(aiMesh* mesh, const aiScene* scene);
}
#endif

#endif
