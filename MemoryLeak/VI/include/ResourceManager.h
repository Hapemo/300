/*!*****************************************************************************
\file ResourceManager.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the function definition of the class ResourceManager.
The ResourceManager class manages the resources, their data and usage.

For Milestone 2:
Revamped resource manager to a more robust resource manager, managing all types
of resources using void*. Added meta file system. Added scene and game state
serialisation. Automatically loading of game state and scene will be implmemented
for game release, but currently it's done manually in game state.

For M4: 
Multithreaded resource loading, speeding up resource loading by several fold.

IMPORTANT NOTE TODO: Right now, all scene data are loaded into the gamestate 
since the start. Next time if we want to load in specific scene data resource 
at specific game state, we have to make a function that load specific scene
using indicated GUIDs.
Currently, gamestates have to be manually unloaded when exiting the gamestate,
cannot call UnloadAllResources. Next time, during game mode instead of editor mode,
UnloadAllResources should be called to unload even the gamestate's data.
The resource manager can only manage static resources. Dynamic resources that will 
have change in data during run-time will not work.
*******************************************************************************/

#pragma once

#include "singleton.h"
#include "pch.h"
#include "Logger.h"
#include "stb_image.h"

#define LOAD_ASSET(...) ResourceManager::GetInstance()->LoadTexture(__VA_ARGS__)
#define UNLOAD_ASSET(...) ResourceManager::GetInstance()->UnloadTexture(__VA_ARGS__)

#define GET_TEXTURE_ID(...) ResourceManager::GetInstance()->GetTextureID(__VA_ARGS__)
#define GET_TEXTURE_IDS(...) ResourceManager::GetInstance()->GetTextureIDs(__VA_ARGS__)
#define GET_TEXTURE_PATH(...) ResourceManager::GetInstance()->GetTexturePath(__VA_ARGS__)
#define GET_TEXTURE_DATA(...) ResourceManager::GetInstance()->GetTextureData(__VA_ARGS__)
#define GET_RESOURCES(...) ResourceManager::GetInstance()->GetResources(__VA_ARGS__)
#define GET_ASPECT_RATIO(...) ResourceManager::GetInstance()->GetAspectRatio(__VA_ARGS__)

#define LOAD_TEXTURES(...) ResourceManager::GetInstance()->LoadTextures(__VA_ARGS__)
#define UPDATE_TEXTURES(...) ResourceManager::GetInstance()->UpdateTextures(__VA_ARGS__)
//#define FREE_RESOURCES(...) ResourceManager::GetInstance()->FreeResources(__VA_ARGS__)
//
//struct GameStateData;
//struct SceneData;
struct Entity;
class ResourceManager : public Singleton<ResourceManager> {
public:
	/*!*****************************************************************************
	Struct to store all the texture data.
	*******************************************************************************/
	struct TextureData {
		std::string	path = "";
		GLuint		id = 0;
		void* data = NULL;
		GLsizei		width = 0;
		GLsizei		height = 0;
		int			channels = 0;
	};

	using GUID = uint64_t;
	
	enum class E_RESOURCETYPE : char {
		error = 0,
		texture,
		audio,
		script,
		scene,
		gamestateEntities,
		dialogue,
		prefab,
		font /////////////////////////////added
	};
private:

	/*!*****************************************************************************
	Struct to store all the data of a resource.
	*******************************************************************************/
	struct ResourceData {
		TextureData	texture;
		time_t		lastModified = time(NULL);
		int			usage = 0;
	};

	/*!*****************************************************************************
	Member vector to store all the loaded resources data.
	*******************************************************************************/
	std::vector<ResourceData> mResources;
	std::vector<ResourceData> mReloadedResources;
	
	const std::filesystem::path resourceFolder = "..\\resources";

	std::map<GUID, void*> mAllResources;
	std::map<GUID, std::string> mAllFilePaths; // Store the GUID mapped to file path. All file paths contains "..\\resources\\".
	unsigned char guidCounter = 0;
	bool LoadedAll = false;		// Editor Mode only
	std::vector<std::thread> mResourceLoadingThreads;
	std::mutex myLock;
	std::set<GUID> mGuidList; // Game Mode only

public:
	/*!*****************************************************************************
	Constructor for ResourceManager.
	*******************************************************************************/
	ResourceManager() = default;

	/*!*****************************************************************************
	Destructor for ResourceManager.
	*******************************************************************************/
	~ResourceManager() = default;

	// Delete the copy constructor
	ResourceManager(const ResourceManager&) = delete;
	const ResourceManager& operator=(const ResourceManager&) = delete;

	/*!*****************************************************************************
	Loads all the textures in the specified filepath.

	\param std::string _filepath
	The filepath in string.
	*******************************************************************************/
	void LoadTextures(std::string _filepath);

	/*!*****************************************************************************
	Check if files have changed/been updated by checking the last updated time. If
	timing is different, then reload the texture. Also check for if the filename
	doesn't exist, means the file is deleted, which the function will then remove
	the texture from the mResources vector.

	\return
	Returns a vector of resource id that has been updated.
	*******************************************************************************/
	std::vector<int> UpdateTextures();

	/*!*****************************************************************************
	Free the resources in the vector.
	*******************************************************************************/
	//void FreeResources();

	/*!*****************************************************************************
	Update a specific texture and it's data by calling the stbi load functon.

	\param const size_t _index
	The index of the texture to be updated.
	*******************************************************************************/
	void UpdateTexture(const size_t _index);

	/*!*****************************************************************************
	Load a specific texture by calling the stbi load function and stores it's data
	in a ResourceData struct. Stores the resource in a member vector mResources.

	\param const std::string _filepath
	The filepath of the texture to be loaded.
	*******************************************************************************/
	TextureData LoadTexture(const std::string _filepath);

	//==============================================================================
	// For multi threading
	
	/*!*****************************************************************************
	Does the same thing as LoadTexture, but it does not use sprite manager to
	initialise the texture, because openGL is not thread safe

	\param const std::string _filepath
	The filepath of the texture to be loaded.
	*******************************************************************************/
	TextureData LoadTextureWithoutOpenGL(const std::string _filepath);

	/*!*****************************************************************************
	Initialise all textures in openGL side.
	*******************************************************************************/
	void InitialiseAllTextures();

	/*!*****************************************************************************
	Initialise all textures that is load into the game midway through running the
	editor.
	*******************************************************************************/
	void InitialiseReloadedTextures();


	//==============================================================================

	/*!*****************************************************************************
	Unload a specific texture data by calling stbi image free.

	\param void* _data
	The void* pointing to the data of the texture to be deleted.
	*******************************************************************************/
	void UnloadTexture(void* _data);

	/*!*****************************************************************************
	Retrieve the texture data of a specific texture.

	\param size_t _index
	The index of the texture.

	\return
	Returns the data of the texture in the member vector mResources.
	*******************************************************************************/
	TextureData& GetTextureData(size_t _index) { return mResources[_index].texture; }

	/*!*****************************************************************************
	Retrieve the loaded resources stored in the member vector mResources.

	\return
	Returns the member vector mResources.
	*******************************************************************************/
	std::vector<ResourceData>& GetResources() { return mResources; }

	/*!*****************************************************************************
	Gets the aspect ratio of a specific texture.

	\param const GLuint _id
	The id of the texture.

	\return
	Returns the aspect ratio of the texture in float.
	*******************************************************************************/
	float GetAspectRatio(const GLuint _id);

	/*!*****************************************************************************
	Retrieve the texture id of a specific resource.

	\param const std::string& _texturePath
	The filepath of the texture.

	\param GLuint
	The id of the texture.
	*******************************************************************************/
	GLuint GetTextureID(const std::string& _texture_path);

	/*!*****************************************************************************
	Retrieve all the texture ids of the resources.

	\return
	Returns a vector of the ids in GLuint* of all the texture.
	*******************************************************************************/
	std::vector<GLuint*> GetTextureIDs();

	/*!*****************************************************************************
	Retrieve the texture path of a specific resource.

	\param GLint _id
	The id of the texture.

	\return
	Returns the path of the texture in string.
	*******************************************************************************/
	std::string	GetTexturePath(GLint _id);

	/*!*****************************************************************************
	Generate a new guid

	\param std::filesystem::path const&
	- File path of file to generate guid for

	\return GUID
	- Newly generated guid
	*******************************************************************************/
	GUID GUIDGenerator(std::filesystem::path const&);

	/*!*****************************************************************************
	Helper function to check if a file exists

	\param std::string const&
	- File path of file

	\return bool
	- True if the file exists, otherwise false
	*******************************************************************************/
	static bool FileExist(std::string const&);

	// Load all resources to resource manager
	/*!*****************************************************************************
	Load all resouces from one resource folder stated in the resource manager class.
	Calls another function overload of LoadAllResources to load all files. 

	NOTE: This function is an EDITOR-ONLY function. Launch of game should include a 
	different structure
	*******************************************************************************/
	void LoadAllResources();

	/*!*****************************************************************************
	Load all files in a folder/file path. If it's a folder, enter it and find files
	to load in. NOTE: This function is an EDITOR-ONLY function. Launch of game 
	should include a different structure
	New addition: This function can now load new resources added. The first run,
	this function loads all resources that can be found. Subsequent run will only 
	load resouces that are newly added. (Different name files are counted as new)

	\param std::filesystem::path const&
	- Folder/file path to load
	*******************************************************************************/
	void LoadAllResources(std::filesystem::path const&);

	/*!*****************************************************************************
	Loads a resource specified by the director inputted. 
	This function is EDITOR ONLY.

	\param std::filesystem::path const&
	- File path to load
	*******************************************************************************/
	void LoadResource(std::filesystem::path const&);

	/*!*****************************************************************************
	Unload all resources in resource manager. (mAllResources)
	*******************************************************************************/
	void UnloadAllResources();

	/*!*****************************************************************************
	Read guid from meta file

	\param std::string const& _metaPath
	- File path of the guid file

	\return GUID
	- GUID extracted
	*******************************************************************************/
	GUID ReadGUIDFromFile(std::string const& _metaPath);

	/*!*****************************************************************************
	Check resource type of a resource file and return an enum of it

	\param std::filesystem::path const&
	- File path of file to check

	\return E_RESOURCETYPE
	- Resource type of file 
	*******************************************************************************/
	E_RESOURCETYPE CheckResourceType(std::filesystem::path const&);

	/*!*****************************************************************************
	Get resource data with guid

	\param GUID const&
	- Guid of the resource data

	\return T&
	- Resource data
	*******************************************************************************/
	template<typename T>
	T& GetResource(GUID const&);

	/*!*****************************************************************************
	Get the file path of a resource using it's GUID

	\param GUID const&
	- Guid of the file

	\return std::string
	- File path of the file
	*******************************************************************************/
	std::string GetFilePath(GUID const&);

	/*!*****************************************************************************
	Get file GUID from resource file path. File path's directory should start from
	resource folder directory.

	\param GUID const&
	- Guid of the file, starting from resource folder directory.

	\return std::string
	- File path of the file. Return 0 and logs warning if guid not found.
	*******************************************************************************/
	GUID GetFileGUID(std::string const&) const;

	/*!*****************************************************************************
	Load game state json file. Load in all the data of gamestate and load the
	entities in to the ECS.

	\param GUID const&
	- Guid of the game state

	\return GameStateData
	- Data of a game state
	*******************************************************************************/
	//GameStateData LoadGameState(GUID const&);

	std::filesystem::path FileTypePath(E_RESOURCETYPE);

	void LoadGameStateResources(std::filesystem::path const&);
	void SelectiveLoadAllResources(std::filesystem::path const&);
	void SelectiveLoadResource(std::filesystem::path const& entry);
	void SelectiveUnloadAllResources();

	//------------------------------------
	// Helper function 
	//------------------------------------
	/*!*****************************************************************************
	Checks if the audio file is of wrong format. Logs warning if it is wrong.

	\param std::filesystem::path const&
	- File path of the audio file
	*******************************************************************************/
	void CheckWrongAudioFile(std::filesystem::path const&);

	/*!*****************************************************************************
	Checks if the texture file is of wrong format. Logs warning if it is wrong.

	\param std::filesystem::path const&
	- File path of the texture file
	*******************************************************************************/
	void CheckWrongTextureFile(std::filesystem::path const&);
};

//------------------------------------
// Template function definitions
//------------------------------------
template<typename T>
T& ResourceManager::GetResource(GUID const& _guid) {
	return *(static_cast<T*>(mAllResources[_guid]));
}







