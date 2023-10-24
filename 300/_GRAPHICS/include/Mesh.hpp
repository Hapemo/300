/*!*****************************************************************************
\file Mesh.hpp
\author Lee Fu Sheng Roy
\par DP email: f.lee@digipen.edu
\par Group: Pepe Production
\date 28-09-2023
\brief
Mesh class implementation. Consists of the loading of serialized geom data,
creation of the required VBOs and VAO.
*******************************************************************************/

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
/*!*****************************************************************************
Loads the positions, texture coordinates, indices, normals and tangents from
the geom data
*******************************************************************************/
		void LoadFromGeom(const _GEOM::Geom& GeomData, std::vector<vec3>& positions, std::vector<glm::vec2>& uvs, 
						std::vector<unsigned int>& indices, std::vector<vec3>& normals, std::vector<vec3>& tangents);
		
/*!*****************************************************************************
Loads the animation data such as bone IDs and bone weight from the geom data
*******************************************************************************/
		void LoadAnimationDataFromGeom(const _GEOM::Geom& GeomData, std::vector<glm::vec4>& boneIDs, std::vector<glm::vec4>& boneWeights);
		
/*!*****************************************************************************
Creates the mesh by initializing the required VAO and VBOs for the mesh
*******************************************************************************/
		void Setup(std::vector<vec3> const& positions, std::vector<unsigned int> const& indices, std::vector<vec2> const& TexCoords = std::vector<vec2>{}, unsigned colorDivisor = 1u);
		
/*!*****************************************************************************
Creates the mesh by initializing the required VAO and VBOs for the mesh
*******************************************************************************/
		void Setup(const _GEOM::Geom& GeomData);

/*!*****************************************************************************
Creates the mesh by initializing the required VAO and VBOs for the mesh to 
to render 2D images. Essentially a quad mesh
*******************************************************************************/
		void Setup2DImageMesh();

/*!*****************************************************************************
Binds the VAO to the openGL context to be used for rendering
*******************************************************************************/
		void BindVao();

/*!*****************************************************************************
Prepares the mesh to be drawn by attaching the render data onto the mesh's VBO
*******************************************************************************/
		void PrepForDraw();

/*!*****************************************************************************
Unbinds the VAO from the openGL context
*******************************************************************************/
		void UnbindVao();

/*!*****************************************************************************
Draw all instances of the mesh
*******************************************************************************/
		void DrawAllInstances();

/*!*****************************************************************************
Clears all render of the mesh
*******************************************************************************/
		void ClearInstances();

		
/*!*****************************************************************************
Gets the vertex count of the mesh
*******************************************************************************/
		int GetVertexCount() { return mVertexCount; }
		
/*!*****************************************************************************
Gets the index count of the mesh
*******************************************************************************/
		int GetIndexCount() { return mIndexCount; }

/*!*****************************************************************************
Destroys the mesh by deleting the VAO and VBOs allocated on th GPU's memory
*******************************************************************************/
		void Destroy();

		// Stores the rendering data for each instance of mesh
		_GEOM::bbox						mBBOX;
		std::vector<mat4>				mLTW;
		std::vector<vec4>				mColors;
		std::vector<vec4>				mTexEntID;

		// This mesh may contain multiple different animations. These animations are unique to this mesh.
		std::vector<_GEOM::Animation>	mAnimation{};
		bool							mHasAnimation{ false };

		// Stores the descriptor data for this mesh
		_GEOM::DescriptorData			mMeshDescriptorData;
		_GEOM::Texture_DescriptorData	mTextureDescriptorData;
		std::string						mMeshDescriptorPath;
		std::string						mTextureDescriptorPath;

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
		std::string		mMeshName{};
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
