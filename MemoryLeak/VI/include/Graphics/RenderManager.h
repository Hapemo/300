/*!*****************************************************************************
\file RenderManager.h
\author Kew Yu Jun
\par DP email: k.yujun\@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for the class RenderManager, which 
operates on Entities with Sprite and Transform Components.
*******************************************************************************/
#pragma once
#include "Graphics/GLAllocator.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"
#include "Graphics/GLShader.h"
#include "PerformanceVisualiser.h"
#include "Graphics/FBO.h"
#include "FontManager.h"
#include "Camera.h"
#include <stdarg.h>
#include "VIzmo.h"

/*!*****************************************************************************
\brief
Enum class with states that check which framebuffer to render to
*******************************************************************************/
enum class RENDER_STATE { WORLD, GAME, ANIMATOR };

/*!*****************************************************************************
\brief
Struct that encapsulates what is needed when rendering a texture in OpenGL.
*******************************************************************************/
struct TextureInfo
{
	int mTextureUnit;
	std::vector<Vertex> mVertices;
	std::vector<GLushort> mIndices;
};

/*!*****************************************************************************
\brief
RenderManager Class that handles rendering of Entities with Sprite and Transform
Component.
*******************************************************************************/
class RenderManager : public System
{
public:
	/*!*****************************************************************************
	\brief
	Default Constructor for RenderManager class.
	*******************************************************************************/
	RenderManager();

	/*!*****************************************************************************
	\brief
	Initializes the RenderManager class.

	\param int* _windowWidth
	Pixel width of the window.

	\param int* _windowHeight
	Pixel height of the window.
	*******************************************************************************/
	void Init(int* _windowWidth, int* _windowHeight);

	/*!*****************************************************************************
	\brief
	Render Entities with Sprite and Transform Component.
	*******************************************************************************/
	void Render();

	/*!*****************************************************************************
	\brief
	Clears the background. Mainly used for switching states.
	*******************************************************************************/
	void Clear();

	/*!*****************************************************************************
	\brief
	Sets the debug mode of rendering.

	\param bool
	Renders debug drawings if debug mode is set to true.
	*******************************************************************************/
	void SetDebug(bool _debug) { mDebug = _debug; }

	/*!*****************************************************************************
	\brief
	Sets the modifier for the length of the debug vector.

	\param float
	Modifier for the length of the vector.
	*******************************************************************************/
	void SetVectorLengthModifier(float);

	/*!*****************************************************************************
	\brief
	Returns the color attachment to the frame buffer.

	\return
	Returns the color attachment to the frame buffer.
	*******************************************************************************/
	GLuint GetWorldFBO() ;

	/*!*****************************************************************************
	\brief
	Returns the color attachment to the frame buffer.

	\return
	Returns the color attachment to the frame buffer.
	*******************************************************************************/
	GLuint GetGameFBO() ;

	/*!*****************************************************************************
	\brief
	Creates the shadow overlay.

	\param glm::vec4
	The color of the shadow.
	*******************************************************************************/
	void CreateShadows(const glm::vec4& clr);

	/*!*****************************************************************************
	\brief
	Returns the color attachment to the Animator buffer, for displaying the 
	Animator editor.

	\return
	Returns the color attachment to the Animator buffer, for displaying the 
	Animator editor.
	*******************************************************************************/
	GLuint GetAnimatorFBO();

	/*!*****************************************************************************
	\brief
	Rendering will be done to the screen instead of the FBO.
	*******************************************************************************/
	void RenderToScreen() { mRenderGameToScreen = true; }

	/*!*****************************************************************************
	\brief
	Rendering will be done to the FBO instead of the screen.
	*******************************************************************************/
	void RenderToFrameBuffer() { mRenderGameToScreen = false; }

	/*!*****************************************************************************
	\brief
	Rendering will be done to the FBO instead of the screen.
	*******************************************************************************/
	bool GetRenderGameToScreen() { return mRenderGameToScreen; }

	/*!*****************************************************************************
	\brief
	Function that returns the transform of an Entity for use with ImGuizmo

	\param const Entity& _e
	Entity to get the Transformation matrix of.

	\return std::vector<float>
	Transformation matrix of the Entity.
	*******************************************************************************/
	std::vector<float> GetImGuizmoMat4(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Returns the Editor camera object.

	\return
	Returns the Editor camera object.
	*******************************************************************************/
	Camera& GetWorldCamera() { return mWorldCam; }

	/*!*****************************************************************************
	\brief
	Returns the Game camera object.

	\return
	Returns the Game camera object.
	*******************************************************************************/
	Camera& GetGameCamera() { return mGameCam; }

	/*!*****************************************************************************
	\brief
	Returns the Animator camera object.

	\return
	Returns the Animator camera object.
	*******************************************************************************/
	Camera& GetAnimatorCamera() { return mAnimatorCam; }

	/*!*****************************************************************************
	\brief
	Sets the color to be cleared every frame.

	\param const Color& _clr
	The color to be cleared with.
	*******************************************************************************/
	void SetClearColor(const Color& _clr);

	/*!*****************************************************************************
	\brief
	Resets all the cameras in RenderManager.
	*******************************************************************************/
	void ResetCameras();

	/*!*****************************************************************************
	\brief
	Returns the Gizmo object.

	\return
	A reference to the Gizmo object.
	*******************************************************************************/
	VIzmo& GetGizmo() { return mGizmo; }

	/*!*****************************************************************************
	\brief
	Used when an entity is selected in the editor. Draws a box around the selected 
	entity.

	\param const Entity& _e
	The entity to be selected.
	*******************************************************************************/
	void SelectEntity(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Select multiple entities.

	\param std::vector<Entity>const& _es
	The vector of entities to be selected.
	*******************************************************************************/
	void SelectEntities(std::vector<Entity>const& _es);

	void AddSelectedEntity(const Entity& _e);
	/*!*****************************************************************************
	\brief
	Unselect an entity. (not drawing box around it)

	\param const Entity& _e
	The entity to be unselected.
	*******************************************************************************/
	void UnselectEntity(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Unselect all entities. (nothing selected)
	*******************************************************************************/
	void ClearSelectedEntities();

	/*!*****************************************************************************
	\brief
	Checks if entity should be rendered.

	\param const Entity& e

	\return bool
	true if it should be culled, false otherwise.
	*******************************************************************************/
	bool ShouldCull(const Entity& e);

	/*!*****************************************************************************
	\brief
	Adds a new layer to the render layers.

	\param int
	The layer number to add.
	*******************************************************************************/
	void NewLayer(int _layer);

	/*!*****************************************************************************
	\brief
	Returns the entity with the lightsource component attached.

	\param
	The entity with the lightsource component.
	*******************************************************************************/
	Entity GetLightSource() { return lightsource; }

	/*!*****************************************************************************
	\brief
	Get the number of lines of text in a text component.
	\param Entity
	The entity with text component to check for.
	\return
	The number of lines.
	*******************************************************************************/
	int GetTextLines(Entity _e);

private:
	void* gs;
	bool mIsCurrSceneUI;
	float mIsCurrSceneMinimap;
	RENDER_STATE mCurrRenderPass;
	Camera mWorldCam, mGameCam, mAnimatorCam;
	std::unordered_map<std::string, FontRenderer> mFontRenderers;
	bool mRenderGameToScreen;
	float mVectorLengthModifier;
	FBO mWorldFBO, mGameFBO, mAnimatorFBO, mLightMapFBO, mMinimapFBO;
	int* mWindowWidth;
	int* mWindowHeight;
	GLShader mDefaultProgram;
	GLShader mTextureProgram;
	GLAllocator mAllocator;
	std::vector<Vertex> mTextureVertices;
	std::vector<GLushort> mTextureIndices;
	std::map<int, std::vector<Vertex>> mVertices;
	std::map<int, std::vector<GLushort>> mIndices;
	std::vector<Vertex> mDebugPoints;
	std::vector<Vertex> mDebugVertices;
	std::vector<GLushort> mDebugIndices;
	std::vector<Entity> mEditorSelectedEntities;
	std::vector<Vertex> mLightVertices;
	std::vector<GLushort> mLightIndices;

	VIzmo mGizmo;
	int mPrevWidth;
	int mInitialWidth, mInitialHeight;
	bool mDebug;
	std::vector<int> mRenderLayers;
	Color mClearColor;
	Entity lightsource;

	/*!*****************************************************************************
	\brief
	Renders debug drawings.
	*******************************************************************************/
	void RenderDebug();
	
	/*!*****************************************************************************
	\brief
	Binds a Texture Unit to an image.

	\param GLuint& _texID
	Texture ID of the texture to be rendered.

	\param TextureInfo& _texInfo
	Struct containing texture unit ID, vertices and indices.

	\param std::vector<int>& _texUnits
	Current texture units that are in use.
	*******************************************************************************/
	void BindTextureUnit(const GLuint& _texID, TextureInfo& _texInfo, std::vector<int>& _texUnits);

	void BatchRenderLayers(std::map<size_t, std::map<GLuint, TextureInfo>>& _texinfo);
	/*!*****************************************************************************
	\brief
	Batch renders textures, shapes and font based on their respective layers.

	\param std::map<size_t, std::map<GLuint, TextureInfo>>& 
	The texture info with vertices.
	*******************************************************************************/
	void BatchRenderLayers(std::map<size_t, std::map<GLuint, TextureInfo>>& _texinfo, std::map<size_t, std::map<GLuint, TextureInfo>>& _cvpinfo);
	

	/*!*****************************************************************************
	\brief
	Creates the visibility polygon for lighting.

	\param const std::vector<Math::Vec2>&
	The ray endpoints to create the polygon from.
	*******************************************************************************/
	void CreateVisibilityPolygon(const std::vector<Math::Vec2>& _vertices);

	/*!*****************************************************************************
	\brief
	Interpolates color for the lightsource and shadow.

	\param const glm::vec4&
	The original color.

	\param const glm::vec4&
	The target color.

	\param float
	The distanct to the target color from the original.

	\param float
	The distance from the target.
	*******************************************************************************/
	glm::vec4 InterpolateColor(const glm::vec4& original, const glm::vec4& target, float distance, float actual);

	/*!*****************************************************************************
	\brief
	Renders the visibility polygon.
	*******************************************************************************/
	void RenderVisibilityPolygon();

	/*!*****************************************************************************
	\brief
	Batch Rendering of vertices.

	\param GLenum _mode
	Mode of rendering (GL_LINES, GL_TRIANGLES, etc..)

	\param const std::vector<Vertex>& _vertices
	Vertices array to be rendered.
	*******************************************************************************/
	void BatchRenderArrays(GLenum _mode, const std::vector<Vertex>& _vertices);

	/*!*****************************************************************************
	\brief
	Batch Rendering of vertices with Element Buffer Object.

	\param GLenum _mode
	Mode of rendering (GL_LINES, GL_TRIANGLES, etc..)

	\param const std::vector<Vertex>& _vertices
	Vertices array to be rendered.

	\param const std::vector<GLushort>& _indices
	Indices of the vertex array.
	*******************************************************************************/
	void BatchRenderElements(GLenum _mode, const std::vector<Vertex>& _vertices, const std::vector<GLushort>& _indices);

	/*!*****************************************************************************
	\brief
	Batch Rendering of vertices with Element Buffer Object.

	\param const std::vector<Vertex>& _vertices
	Vertices array to be rendered.

	\param const std::vector<GLushort>& _indices
	Indices of the vertex array.

	\param std::vector<int>& _texUnits
	Current texture units that are in use.
	*******************************************************************************/
	void BatchRenderTextures(int& _texCount, std::vector<int>& _texUnits);

	/*!*****************************************************************************
	\brief
	Renders all text on a specific layer.

	\param int
	The layer to render.
	*******************************************************************************/
	void RenderText(int _layer);

	/*!*****************************************************************************
	\brief
	Creating vertices from the ECS.
	*******************************************************************************/
	void CreateVertices(std::map<size_t, std::map<GLuint, TextureInfo>>& _texinfo, std::map<size_t, std::map<GLuint, TextureInfo>>& _cvpinfo);
	void CreateVerticesVP(std::map<size_t, std::map<GLuint, TextureInfo>>& _cvpinfo);
	void CreateMinimap(std::map<size_t, std::map<GLuint, TextureInfo>>& _cvpinfo);
	void CreateMinimapVertices(const Entity& _e, int _layer, std::vector<Vertex>& _vertices, std::vector<GLushort>& _indices, GLuint texid);

	/*!*****************************************************************************
	\brief
	Renders all text on a specific layer.

	\param int
	The layer to render.
	*******************************************************************************/
	void CreateVerticesAnimator(std::map<size_t, std::map<GLuint, TextureInfo>>& _texInfo);

	/*!*****************************************************************************
	\brief
	Create the light map.
	*******************************************************************************/
	void CreateLightMap();

	/*!*****************************************************************************
	\brief
	Creates a debug point based on Transform and Sprite Component, or Physics point
	collider.

	\param const Transform& _t
	The transform component.

	\param const Color& _c
	The color component.
	*******************************************************************************/
	void CreateDebugPoint(const Transform& _t, const Color& _clr);
	
	/*!*****************************************************************************
	\brief
	Rendering of textures
	*******************************************************************************/
	void RenderTextures(std::map<GLuint, TextureInfo>& _texInfo, bool _cvp);

	/*!*****************************************************************************
	\brief
	Rendering of shapes
	*******************************************************************************/
	void RenderShapes(int _layer);

	/*!*****************************************************************************
	\brief
	Creates a square or texture based on Transform and Sprite Component.

	\param const Entity& _e
	The entity containing Transform and Sprite component.

	\param std::vector<Vertex>& _vertices
	Vertices array for new vertices to be pushed to.

	\param std::vector<GLushort>& _indices
	Indices array for new indices to be pushed to.
	*******************************************************************************/
	void CreateSquare(const Entity& _e, int layer, std::vector<Vertex>& _vertices, std::vector<GLushort>& _indices);

	void CreateSquareParticle(GLuint texid, int layer, const Transform& xform, Color clr, std::vector<Vertex>& _vertices, std::vector<GLushort>& _indices);

	/*!*****************************************************************************
	\brief
	Create vertices for the shadow filter.
	\param int
	The layer of the shadow.
	\param std::vector<Vertex>&
	The veritices of the shadow filter.
	\param std::vector<GLushort>&
	The indices of the shadow filter.
	*******************************************************************************/
	void CreateLightFilter(int _shadowLayer, std::vector<Vertex>& _vertices, std::vector<GLushort>& _indices);

	/*!*****************************************************************************
	\brief
	Creates a circle based on Transform and Sprite Component.

	\param const Entity& _e
	The entity containing Transform and Sprite component.
	*******************************************************************************/
	void CreateCircle(const Entity& _e, int layer);

	/*!*****************************************************************************
	\brief
	Creates a circle based on Transform, Color and layer.

	\param const Transform& _t
	The transform component.

	\param const Color& _cltr&
	The color component.

	\param float
	layer to render at.
	*******************************************************************************/
	void CreateCircle(const Transform& _xform, const Color& _clr, int layer);

	/*!*****************************************************************************
	\brief
	Creates a circle based on Transform, Color and layer.

	\param const Transform& _t
	The transform component.

	\param const Color& _cltr&
	The color component.

	\param int
	Layer to render at.
	*******************************************************************************/
	void CreateSquare(const Transform& _xform, const Color& _clr, int layer);

	/*!*****************************************************************************
	\brief
	Creates a debug point based on Transform and Sprite Component, or Physics point
	collider.

	\param const Entity& _e
	The entity containing Transform and Sprite component.
	*******************************************************************************/
	void CreateDebugPoint(const Entity& _e);


	/*!*****************************************************************************
	\brief
	Creates a debug line based on Transform and Sprite Component, or Physics Edge
	Collider.

	\param const Entity& _e
	The entity containing Transform and Sprite component.
	*******************************************************************************/
	void CreateDebugLine(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Creates a debug line based on Transform and Sprite Component, or Physics Edge
	Collider.

	\param const Transform& _t
	The transform component.

	\param const Color& _c
	The color component.
	*******************************************************************************/
	void CreateDebugLine(const Transform& _t, const Color& _clr);

	/*!*****************************************************************************
	\brief
	Creates a debug Square based on Transform and Sprite Component, or Physics Rect
	Collider.

	\param const Entity& _e
	The entity containing Transform and Sprite component.
	*******************************************************************************/
	void CreateDebugSquare(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Creates a debug Square based on Transform and Sprite Component, or Physics Rect
	Collider.

	\param const Transform& _t
	The transform component.

	\param const Color& _c
	The color component.
	*******************************************************************************/
	void CreateDebugSquare(const Transform& _t, const Color& _clr);

	/*!*****************************************************************************
	\brief
	Creates a debug circle based on Transform and Sprite Component, or Physics Circle
	Collider.

	\param const Entity& _e
	The entity containing Transform and Sprite component.
	*******************************************************************************/
	void CreateDebugCircle(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Creates a debug circle based on Transform and Sprite Component, or Physics Circle
	Collider.

	\param const Transform& _t
	The transform component.

	\param const Color& _c
	The color component.
	*******************************************************************************/
	void CreateDebugCircle(const Transform& _t, const Color& _clr);

	/*!*****************************************************************************
	\brief
	Creates a debug arrow based on Transform and Sprite Component, or Physics2D 
	component.

	\param const Entity& _e
	The entity containing Transform and Sprite component.
	*******************************************************************************/
	void CreateDebugArrow(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Creates a debug arrow based on Transform and Sprite Component, or Physics2D 
	component.

	\param const Transform& _t
	The transform component.

	\param const Color& _c
	The color component.
	*******************************************************************************/
	void CreateDebugArrow(const Transform& _t, const Color& _clr);

	/*!*****************************************************************************
	\brief
	Get the transform for where to render the gizmo.

	\param const Transform& _xform
	The transform for the object.

	\return
	The transform matrix.
	*******************************************************************************/
	Math::Mat3 GetGizmoTransform(const Transform& _xform);

	/*!*****************************************************************************
	\brief
	Creates a circle for the gizmo.

	\param const Transform& _t
	The transform for the circle.

	\const Color& _clr
	The color of the circle.
	*******************************************************************************/
	void CreateGizmoCircle(const Transform& _t, const Color& _clr);

	/*!*****************************************************************************
	\brief
	Creates a line for the gizmo.

	\param const Transform& _t
	The transform for the circle.

	\const Color& _clr
	The color of the circle.
	*******************************************************************************/
	void CreateGizmoDebugLine(const Transform& _t, const Color& _clr);

	/*!*****************************************************************************
	\brief
	Creates a debug circle for the gizmo. (outline only)

	\param const Transform& _t
	The transform for the circle.

	\const Color& _clr
	The color of the circle.
	*******************************************************************************/
	void CreateGizmoDebugCircle(const Transform& _t, const Color& _clr);

	/*!*****************************************************************************
	\brief
	Helper function for returning the color of the Sprite Component in OpenGL.

	\param const Entity& _e
	The entity containing Transform and Sprite component.

	\return
	Returns the color of the Sprite.
	*******************************************************************************/
	glm::vec4 GetColor(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Helper function for returning a Color in OpenGL.

	\param GLubyte _r
	The redness of the color.

	\param GLubyte _g
	The greenness of the color.

	\param GLubyte _b
	The blueness of the color.

	\param GLubyte _a
	The transparency of the color.

	\return
	Returns the color.
	*******************************************************************************/
	glm::vec4 GetColor(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a);

	/*!*****************************************************************************
	\brief
	Helper function for returning the Transformation matrix of a Component.

	\param const Entity& _e
	The entity containing Transform and Sprite component.

	\return
	The Transformation matrix.
	*******************************************************************************/
	Math::Mat3 GetTransform(const Entity& _e);

	/*!*****************************************************************************
	\brief
	Helper function for returning the Transformation matrix given scale, rotate, and
	translate.

	\param const Math::Vec2& _scale
	The scale for the transformation.

	\param float _rotate
	The rotation for the transformation

	\param const Math::Vec2& _translate
	The translatetion for the transformation

	\return
	The Transformation matrix.
	*******************************************************************************/
	Math::Mat3 GetTransform(const Math::Vec2& _scale, float _rotate, const Math::Vec2& _translate);

	/*!*****************************************************************************
	\brief
	Sends text into the FontManager to be rendered.

	\param const Entity& _e
	The entity with the Text component.
	*******************************************************************************/
	void CreateText(const Entity& _e, int _layer);

	/*!*****************************************************************************
	\brief
	Initializes the Shader program.
	*******************************************************************************/
	void InitializeShaders();

	/*!*****************************************************************************
	\brief
	Helper function for concatenating the vector of indices.

	\param std::vector<GLushort>& _first
	The vector for indices to be pushed into.

	\param std::vector<GLushort>& _second
	The vector for indices to be taken from.
	*******************************************************************************/
	void ConcatIndices(std::vector<GLushort>& _first, std::vector<GLushort>& _second);

	/*!*****************************************************************************
	\brief
	Creates the gizmo.
	*******************************************************************************/
	void CreateGizmo();

};