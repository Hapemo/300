/*!*****************************************************************************
\file ResourceManager.cpp
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 26-09-2022
\brief
This file contains the class ResourceManager and it's functions declaration.
The ResourceManager class manages the resources, their data and usage.
*******************************************************************************/

#pragma once
#define MultiThread 1 // 1 to multi thread, 0 to not multi thread.

#include <sys/types.h>
#include <sys/stat.h>
#define stat _stat
#include <chrono>
#include <algorithm>
#include "ResourceManager.h"
#include "Serialization.h"
#include "ECSManager.h"

/*!*****************************************************************************
\brief
Loads all the textures in the specified filepath.

\param std::string _filepath
The filepath in string.
*******************************************************************************/
void ResourceManager::LoadTextures(std::string _filepath) {
	const std::filesystem::path tex_dir{ "..\\resources\\Textures\\" + _filepath };
	for (const std::filesystem::path& entry : std::filesystem::directory_iterator(tex_dir)) {
		std::string str = entry.string();
		LoadTexture(str);
	}
}

/*!*****************************************************************************
\brief
Load a specific texture by calling the stbi load function and stores it's data
in a ResourceData struct. Stores the resource in a member vector mResources.

\param const std::string _filepath
The filepath of the texture to be loaded.
*******************************************************************************/
ResourceManager::TextureData ResourceManager::LoadTexture(const std::string _filepath)
{
	struct stat stats;
	ResourceData trackResource;
	const char* filepath = _filepath.c_str();
	//LOG_DEBUG("loading texture:" + _filepath);
	stbi_set_flip_vertically_on_load(true);
	trackResource.texture.data = stbi_load(filepath, &trackResource.texture.width, &trackResource.texture.height, &trackResource.texture.channels, 0);
	trackResource.texture.path = filepath;
	if (stat(trackResource.texture.path.c_str(), &stats) == 0)
		trackResource.lastModified = stats.st_mtime;
	++trackResource.usage;
	mResources.push_back(trackResource);
	spriteManager->InitializeTexture(mResources.back().texture);
	//std::cout << mResources[mResources.size() - 1].texture.path << std::endl;
	return trackResource.texture;
}

ResourceManager::TextureData ResourceManager::LoadTextureWithoutOpenGL(const std::string _filepath) {
	struct stat stats;
	ResourceData trackResource;
	stbi_set_flip_vertically_on_load(true);
	trackResource.texture.data = stbi_load(_filepath.c_str(), &trackResource.texture.width, &trackResource.texture.height, &trackResource.texture.channels, 0);
#if MultiThread
	myLock.lock();
#endif
	trackResource.texture.path = _filepath.c_str();
	if (stat(trackResource.texture.path.c_str(), &stats) == 0)
		trackResource.lastModified = stats.st_mtime;
	++trackResource.usage;
	mResources.push_back(trackResource);
	if (LoadedAll) mReloadedResources.push_back(trackResource);
#if MultiThread
	myLock.unlock();
#endif
	return trackResource.texture;
}

void ResourceManager::InitialiseAllTextures() {
	for (auto& resource : mResources)
		spriteManager->InitializeTexture(resource.texture);
}

void ResourceManager::InitialiseReloadedTextures() {
	for (auto& resource : mReloadedResources)
		spriteManager->InitializeTexture(resource.texture);
	mReloadedResources.clear();
}

/*!*****************************************************************************
\brief
Update a specific texture and it's data by calling the stbi load functon.

\param const size_t _index
The index of the texture to be updated.
*******************************************************************************/
void ResourceManager::UpdateTexture(const size_t _index)
{
	const char* filepath = mResources[_index].texture.path.c_str();
	stbi_set_flip_vertically_on_load(true);
	mResources[_index].texture.data = stbi_load(filepath, &mResources[_index].texture.width, &mResources[_index].texture.height, &mResources[_index].texture.channels, 0);
	//std::cout << mResources[_index].texture.path << std::endl;
}

/*!*****************************************************************************
\brief
Check if files have changed/been updated by checking the last updated time. If
timing is different, then reload the texture. Also check for if the filename
doesn't exist, means the file is deleted, which the function will then remove
the texture from the mResources vector.

\return
Returns a vector of resource id that has been updated.
*******************************************************************************/
std::vector<int> ResourceManager::UpdateTextures() {
	std::vector<int> updatedTextures;
	struct stat stats;
	for (size_t index = 0; index < mResources.size(); ++index) {
		if (stat(mResources[index].texture.path.c_str(), &stats) == 0) {
			std::ifstream fopen(mResources[index].texture.path);
			if (!fopen.is_open()) {
				fopen.close();
				mResources.erase(mResources.begin() + index);
				continue;
			}
			time_t lastModifiedTime = stats.st_mtime;
			if (lastModifiedTime != mResources[index].lastModified) {
				//std::cout << "unloading " << mResources[index].texture.path << std::endl;
				UpdateTexture(index);
				mResources[index].lastModified = lastModifiedTime;
				updatedTextures.push_back((int)index);
			}
			fopen.close();
		}
	}
	return updatedTextures;
}

/*!*****************************************************************************
\brief
Retrieve the texture id of a specific resource. Path starts from "..\\resources\\",

\param const std::string& _texturePath
The filepath of the texture, starting from resources folder's directory

\return
Returns the id of the texture in GLuint.
*******************************************************************************/
GLuint ResourceManager::GetTextureID(const std::string& _texturePath) {
	for (size_t index = 0; index < mResources.size(); ++index)
		if (mResources[index].texture.path == "..\\resources\\" + _texturePath) return mResources[index].texture.id;
	return 0;
}

/*!*****************************************************************************
\brief
Retrieve the texture path of a specific resource.

\param GLint _id
The id of the texture.

\return
Returns the path of the texture in string.
*******************************************************************************/
std::string	ResourceManager::GetTexturePath(GLint _id) {
	std::string path = "";
	for (size_t index = 0; index < mResources.size(); ++index)
		if ((GLint)mResources[index].texture.id == (GLint)_id) {
			path = mResources[index].texture.path;
			path = path.substr(13);
		}
	return path;
}

/*!*****************************************************************************
\brief
Retrieve all the texture ids of the resources.

\return
Returns a vector of the ids in GLuint* of all the texture.
*******************************************************************************/
std::vector<GLuint*> ResourceManager::GetTextureIDs() {
	std::vector<GLuint*> result;
	for (size_t index = 0; index < mResources.size(); ++index)
		result.push_back(&mResources[index].texture.id);
	return result;
}

/*!*****************************************************************************
\brief
Unload a specific texture data by calling stbi image free.

\param void* _data
The void* pointing to the data of the texture to be deleted.
*******************************************************************************/
void ResourceManager::UnloadTexture(void* _data) {
	stbi_image_free(_data);
}

/*!*****************************************************************************
\brief
Gets the aspect ratio of a specific texture.

\param const GLuint _id
The id of the texture.

\return
Returns the aspect ratio of the texture in float.
*******************************************************************************/
float ResourceManager::GetAspectRatio(const GLuint _id) {
	for (size_t index = 0; index < mResources.size(); ++index)
		if (mResources[index].texture.id == _id) return (float)((float)mResources[index].texture.height / (float)mResources[index].texture.width);
	return 0.0f;
}

/*!*****************************************************************************
\brief
Free the resources in the vector.
*******************************************************************************/
//void ResourceManager::FreeResources() {
//	mResources.clear();
//}

// GUID is made with the following format
// GUID is firstly made with total time stamp
// Then replaced the first byte with resource type
// Then replace the second byte with instance of guidCounter

ResourceManager::GUID ResourceManager::GUIDGenerator(std::filesystem::path const& _path) {
	E_RESOURCETYPE resourceType{ CheckResourceType(_path) };
	//ASSERT(resourceType == E_RESOURCETYPE::error, "Unable to determine resource type");
	if (resourceType == E_RESOURCETYPE::error)
		return 0;
	std::chrono::time_point today = std::chrono::system_clock::now();
	std::chrono::duration duration = today.time_since_epoch();
	GUID guid = *static_cast<GUID*>(static_cast<void*>(&duration));
	unsigned char* guidPtr = static_cast<unsigned char*>(static_cast<void*>(&guid));
	*guidPtr = static_cast<unsigned char>(resourceType);
	*(guidPtr + 1) = ++guidCounter; // guidCounter will automatically reset to 0 when overflow, this is intended.
	return guid;
}

bool ResourceManager::FileExist(std::string const& _fileName) {
	std::ifstream file{ _fileName.c_str() };
	return file.good();
}

ResourceManager::GUID ResourceManager::ReadGUIDFromFile(std::string const& _metaPath) {
	std::ifstream file{ _metaPath };
	char buffer [sizeof(GUID)];
	file.read(buffer, sizeof(GUID));
	file.close();
	return *(static_cast<GUID*>(static_cast<void*>(buffer)));
}

void ResourceManager::LoadAllResources() {
	mResources.reserve(1000); // Predict there is going to be at most 1000 textures, for multi threading

	LoadAllResources(std::filesystem::path{resourceFolder});

//#if MultiThread
	for (std::thread& t : mResourceLoadingThreads)
		t.join();
	mResourceLoadingThreads.clear();
//#endif

	// Initialise resources that haven't been initialised before
	if (!LoadedAll) InitialiseAllTextures();
	else InitialiseReloadedTextures();

	LoadedAll = true;
}

void ResourceManager::LoadResource(std::filesystem::path const& entry) {
	// Skip any meta files
	const std::string fileName{ entry.filename().string() };
	if (fileName.find(".meta") != std::string::npos) return;

	// Check for existing meta file
	std::string metaPath{ entry.string() + ".meta" };
	GUID guid{};
	if (FileExist(metaPath))// If exist, get it's guid
		guid = ReadGUIDFromFile(metaPath);
	else {									// else make one and get it's guid
		guid = GUIDGenerator(entry);
		std::ofstream newMetaFile(metaPath);
		char* data = static_cast<char*>(static_cast<void*>(&guid));
		newMetaFile.write(data, sizeof(guid));
		newMetaFile.close();
	}

	// Skip subsequent duplicate resources
	if (LoadedAll) {
		bool fileLoaded{ false };
		for (auto& [_guid, _fileName] : mAllFilePaths) {
			if (_fileName == entry.string()) {
				fileLoaded = true;
				break;
			}
		}

		if (entry.string().find("\\Prefabs\\") != std::string::npos) return;
		if (entry.string().find("\\SceneGUID\\") != std::string::npos) return;

		if (fileLoaded) return;
		LOG_INFO("RESOURCE MANAGER: loading new resources midway through editor: " + entry.string());
	}

	// Open and store resources, then linking them to their guid
	E_RESOURCETYPE resourceType{ CheckResourceType(entry) };
	//ASSERT(resourceType == E_RESOURCETYPE::error, "Unable to determine resource type");
	if (resourceType == E_RESOURCETYPE::error || resourceType == E_RESOURCETYPE::prefab) return;

	void* dataPointer{};

	switch (resourceType) {
	case E_RESOURCETYPE::texture:
		dataPointer = new TextureData;
//#if MultiThread
		*(static_cast<TextureData*>(dataPointer)) = LoadTextureWithoutOpenGL(entry.string());
		//LoadTextureWithoutOpenGL(entry.string());
//#else
		//*(static_cast<TextureData*>(dataPointer)) = LoadTexture(entry.string());
//#endif
		break;

	case E_RESOURCETYPE::audio:
		// entry.string() gives the path of the file. eg. "..\\resources\\Audio\\SHOOT1.wav"
		/*dataPointer = static_cast<void*>*/
		myLock.lock();
		(audioManager->LoadAudio(entry));
		myLock.unlock();
		break;

	case E_RESOURCETYPE::script:

		break;

	case E_RESOURCETYPE::scene:
		//dataPointer = new SceneData;
		//*(static_cast<SceneData*>(dataPointer)) = SerializationManager::LoadSceneData(entry.string());
		break;

	case E_RESOURCETYPE::gamestateEntities:
		//dataPointer = new GameStateData;
		//*(static_cast<GameStateData*>(dataPointer)) = SerializationManager::LoadGameStateData(entry.string());
		break;

	case E_RESOURCETYPE::dialogue:

		break;
	case E_RESOURCETYPE::font:

		break;
	case E_RESOURCETYPE::prefab:

		break;
	}
	(void)dataPointer;
	myLock.lock();
	mAllResources.insert({ guid, dataPointer });
	mAllFilePaths.insert({ guid, entry.string() });
	myLock.unlock();
	std::cout << "GUID: " << guid << " | File: " << entry.filename().string() << '\n';
}

void ResourceManager::LoadAllResources(std::filesystem::path const& _folder) {
	std::cout << "------- File Directory: " << _folder << std::endl;

	for (const std::filesystem::path& entry : std::filesystem::directory_iterator(_folder)) {
		// If it's a folder, go into it
		if (std::filesystem::is_directory(entry)) {
			LoadAllResources(entry);
			continue;
		}
#if MultiThread
		mResourceLoadingThreads.push_back(std::thread([this,entry] {LoadResource(entry); }));
#else
		LoadResource(entry);
#endif
	}
	
}

void ResourceManager::UnloadAllResources() {
	for (auto& [guid, dataPtr] : mAllResources) {
		E_RESOURCETYPE resourceType{ static_cast<E_RESOURCETYPE>(guid) };

		switch (resourceType) {
		case E_RESOURCETYPE::texture:
			delete static_cast<TextureData*>(dataPtr);
			break;

		case E_RESOURCETYPE::audio:
			//static_cast<FMOD::Sound*>(dataPtr)->release();
			break;

		case E_RESOURCETYPE::script:

			break;

		case E_RESOURCETYPE::scene: {
			//SceneData* data = static_cast<SceneData*>(dataPtr);
			//if (!data) break;
			//ECS::DestroySomeEntites(data->mEntities);
			//delete data;
			break;
		}

		case E_RESOURCETYPE::gamestateEntities: {
			//GameStateData* data = static_cast<GameStateData*>(dataPtr);
			//if (!data) break;
			//ECS::DestroySomeEntites(data->mEntities);
			//delete data;
			break;
		}

		case E_RESOURCETYPE::dialogue:

			break;
		case E_RESOURCETYPE::font:

			break;
		case E_RESOURCETYPE::prefab:

			break;
		}
	}
	mAllResources.clear();
	mAllFilePaths.clear();
}

void ResourceManager::CheckWrongAudioFile(std::filesystem::path const& _path) {
	if (_path.string().find(".wav") != std::string::npos || 
			_path.string().find(".mp3") != std::string::npos) return; // Nothing wrong with wav file

	// If something wrong with audio file read in, code under here
	LOG_ERROR("Attempted to load in invalid audio file: " + _path.string());
}

void ResourceManager::CheckWrongTextureFile(std::filesystem::path const& _path) {
	if (_path.string().find(".png") != std::string::npos ||
			_path.string().find(".jpg") != std::string::npos) return; // Nothing wrong with tex file

	// If something wrong with audio file read in, code under here
	LOG_ERROR("Attempted to load in invalid texture file: " + _path.string());
}

ResourceManager::E_RESOURCETYPE ResourceManager::CheckResourceType(std::filesystem::path const& _path) {
	if (_path.string().find("\\Audio\\") != std::string::npos) {
		CheckWrongAudioFile(_path);
		return E_RESOURCETYPE::audio;
	}
	else if (_path.string().find("\\Dialogs\\") != std::string::npos) return E_RESOURCETYPE::dialogue;
	else if (_path.string().find("\\Scene\\") != std::string::npos) return E_RESOURCETYPE::scene;
	else if (_path.string().find("\\Textures\\") != std::string::npos) {
		CheckWrongTextureFile(_path);
		return E_RESOURCETYPE::texture;
	}
	else if (_path.string().find("\\Scripts\\") != std::string::npos) return E_RESOURCETYPE::script;
	else if (_path.string().find("\\GameStates\\") != std::string::npos) return E_RESOURCETYPE::gamestateEntities;
	else if (_path.string().find("\\Fonts\\") != std::string::npos) return E_RESOURCETYPE::font;
	else if (_path.string().find("\\Prefabs\\") != std::string::npos) return E_RESOURCETYPE::prefab;
	else if (_path.string().find("\\SceneGUID\\") != std::string::npos) return E_RESOURCETYPE::error;

	LOG_WARN("Attempted to load unknown resource into resource manager. Resource: " + _path.string());
	return E_RESOURCETYPE::error;
}

std::string ResourceManager::GetFilePath(GUID const& _guid) {
	std::string filePath{};
	try {
		filePath = mAllFilePaths.at(_guid);
	} catch (std::out_of_range) {
		LOG_WARN("Attempting to get non-existing filepath from resource manager, using guid: " + std::to_string(_guid));
	}

	return filePath;
}

typename ResourceManager::GUID ResourceManager::GetFileGUID(std::string const& _path) const {
	std::string actualPath{ resourceFolder.string() + "\\" + _path};
	for (auto& [guid, path] : mAllFilePaths)
		if (path == actualPath) return guid;
	LOG_WARN("Unable to identify GUID value of " + _path);
	return GUID();
}


std::filesystem::path ResourceManager::FileTypePath(E_RESOURCETYPE _type) {
	std::string path = resourceFolder.string();
	switch (_type) {
	case E_RESOURCETYPE::texture:
		path += "\\Textures\\";
		break;
	case E_RESOURCETYPE::audio:
		path += "\\Audio\\";
		break;
	case E_RESOURCETYPE::script:
		path += "\\Scripts\\";
		break;
	case E_RESOURCETYPE::scene:
		path += "\\Scene\\";
		break;
	case E_RESOURCETYPE::gamestateEntities:
		path += "\\GameStates\\";
		break;
	case E_RESOURCETYPE::dialogue:
		path += "\\Dialogs\\";
		break;
	case E_RESOURCETYPE::font:
		path += "\\Fonts\\";
		break;
	case E_RESOURCETYPE::prefab:
		path += "\\Prefabs\\";
		break;
	}
	return std::filesystem::path(path);
}


//-----------------------------------------
// For Game Mode
//-----------------------------------------

void ResourceManager::LoadGameStateResources(std::filesystem::path const& _gamestatePath) {
	mResources.reserve(1000); // Predict there is going to be at most 1000 textures, for multi threading
	(void)_gamestatePath;
	serializationManager->GetGUIDList(_gamestatePath, mGuidList);
	//mGuidList.insert({ 16665530911558913, 16665530911559169, 16665530911624961, 16665530911625217, 16747534957347073, 16747534957347329, 16747534957413121, 16747534957413377, 16669651548307713, 16669651548570113, 16669651548766977, 16669651549029377, 16669651549226241, 16669651549423105, 16669651549751041, 16669651550341121, 16747534957806849, 16681741603832065, 16682022752420097, 16679133496279297, 16679183972630785, 16753364657635585, 16753364657636353, 16753364657635841, 16679151735668993, 16753364657636097, 16683718373083393, 16693037898268929, 16693037898269185, 16693037898269441, 16693037898335233, 16693037898335489, 16693037898335745, 16693037898336001, 16693037898401793, 16693037898402049, 16693037898402305, 16693037898468097, 16693037898468353, 16693037898468609, 16693037898534401, 16693037898534657, 16693037898534913, 16693037898600705, 16693037898600961, 16693037898601217, 16693037898667009, 16693037898667265, 16693037898667521, 16693037898733313, 16693037898733569, 16692964149232641, 16692964149298433, 16692964149298689, 16692964149364481, 16692964149430273, 16692964149430529, 16692964149496321, 16692964149496577, 16692964149562369, 16747942542967041, 16679151735734785, 16679133496279553, 16679133496345345, 16679151735931649, 16683718373673473, 16683718373739265, 16683718373805057, 16683718373870849, 16683718373936641, 16683718373936897, 16683718374002689, 16683718374068481, 16683718374068737, 16683718374134529, 16683718374200321, 16683718374266113, 16683718374594305, 16747320180545537, 16682022752616961, 16679133496411137, 16679133496476929, 16679133496542721, 16679133496542977, 16693093095833857, 16679151736062977, 16679151736128769, 16679133496608769, 16679151736194561, 16679133496609025, 16679151736260353, 16679133496674817, 16679133496675073, 16671515798471169, 16669651553094145, 16669651553094401, 16683718375643137, 16683718375643393, 14757395258967641, 16669651553160193, 16689637057036545, 16665530912085249, 16747534960952833, 16747534960953345, 16747534960953089, 16747534961019137, 16747534961019393, 16747534961085185, 16747534961085697, 16747534961085441, 16747534961085953, 16669651554405633, 16669651555061249, 16747534961217281, 16747534961414401, 16747534961414657, 16747534961612033, 16747534961612289, 16747534961545985, 16747534961546241, 16747534961283073, 16683718377609985, 16683718377675777, 16669651555782401, 16741782613262593, 16683718378134785, 16665530912609793, 16669651557945601, 16669651557945857, 16669651558011649, 16669651558011905, 16665530913265409, 16671351933436161, 16665530914510849, 16671351934157313, 16665530915887361, 16671351934878465, 16671351935075329, 16671351935337729, 16693194617848065, 16691742289560577, 16691742289626369, 16691742289692161, 16749186023686401, 16691742289757953, 16747534962792193, 16691742290151425, 16691742290151681, 16691742290217473, 16691742290217729, 16665530918443777, 16665530919492609, 16665530920148225, 16665530921197057, 16665530921918209, 16665530922901505, 16665530923622657, 16665530924737025, 16665530925392641, 16665530926375937, 16665530927031553, 16665530927687169, 16665530928801537, 16665530930243585, 16665530931685633, 16665530933127681, 16671387269988609, 16665530934438657, 16691998620975361, 16691998621827585, 16665530935553025, 16695313745641729, 16665530936798465, 16665530938240513, 16665530939748097, 14757395258960776, 14757395258961366, 14757395258961956, 14757395258962677, 14757395258963202, 14757395258963661, 14757395258964447, 14757395258965103, 14757395258965496, 14757395258966021, 14757395258966480, 16747568241443073, 16747534965873409, 16747534965873665, 16747534966070785, 16747534966070529, 16747534966071041, 16747534966071297, 16747534965610497, 16747534965741825, 16747534966071553, 16747534966401025, 16747534966401537, 16747534966202881, 16747534966203137, 16747534966334977, 16747534966401281, 16747534966334465, 16747534966334721, 16747534966400769, 14757395258967004, 16691998647714561, 16691998649680897, 16691998651122945, 16691998652368385, 16691998653810433, 16665530940731393, 16665530942042369, 16665530943287809, 16689398516547841, 16693623341121793, 16693623341908481, 16693623342170881, 16693623342367745, 16693623342499073, 16693623342761473, 16693623342958337, 16693623343089665, 16720718955086081, 16720718955610625, 16720718956069633, 16720718956528641, 16720718956791041, 16720718957250049, 16720718957709057, 16720718957971457, 16764502807806721, 16764502807150849, 16764502807151105, 16689398516745473, 16689398516876801, 16764502808462337, 16689398516942593, 16689398517139457, 16665530944008961, 16665530944009217, 16689398517270785, 16689398517467649, 16689398517795585, 16689398518057985, 16689398518320385, 16689398518648321, 16689398518910721, 16683718410051585, 16683718412476673, 16683718414770689, 16683718416999169, 16683718419031041, 16720718980253953, 16691149651509505, 16691149668811265, 16691149676610305, 16750120184643841, 16750120184775169, 16750120184775425, 16750120184775681, 16720719006534145, 16691149684540417, 16691149698499841, 16691149699679745, 16750120185366529, 16750120185366017, 16750120185366273, 16750120185365761, 16728197186781441, 16728197186978305, 16728197187175169, 16728197187372033, 16685760444956929, 16685760445088257, 16685760445285121, 16685760445481985, 16685760445613313, 16685760445744641, 16728197188486401, 16728197188879873, 16728197189273345, 16728197189666817, 16728197190060289, 16728197190453761, 16728197190847233, 16728197191306241, 16728197191765249, 16728197192158721, 16728197192486657, 16728197192880129, 16728197193273601, 16728197193667073, 16728197194126081, 16728197194585089, 16728197195044097, 16728197195437569, 16728197195831041, 16728197196290049, 16728197196683521, 16728212152254721, 16728197197536001, 16728197197929473, 16728197198257409, 16728197198650881, 16728197199044353, 16728197199437825, 16728197199831297, 16728197200224769, 16728197200552705, 16728197200880641, 16728197201274113, 16728197201602049, 16728197201995521, 16687557551980801, 16687557552112129, 16691787165335809, 16687557552243457, 16691787165663745, 16687557552374785, 16687557552506113, 16720719039958017, 16728197206321153, 16683718432796161, 16689398527496193, 16689398528479489, 16689398529659393, 16689398530708225, 16689398531888129, 16689398532936961, 16689398536476161, 16689398537393921, 16689398537984001, 16689398538901761, 16693194779918849, 16747534975511809, 16747534975512321, 16747534975511553, 16747534975512065, 16747534975512577, 16747534975512833, 16747534975513089, 16747534975511297, 16689398540409601, 16665530944009473, 16665530944009729, 16665530944009985 });
	// mGuidList = /*Serialisation manager's function*/; TODO

	SelectiveLoadAllResources(resourceFolder);

	for (std::thread& t : mResourceLoadingThreads)
		t.join();
	
	InitialiseAllTextures();
	
	mResourceLoadingThreads.clear();
	mGuidList.clear();
}

void ResourceManager::SelectiveLoadAllResources(std::filesystem::path const& _folderDirectory) {
	std::cout << "------- File Directory: " << _folderDirectory << std::endl;
	for (const std::filesystem::path& entry : std::filesystem::directory_iterator(_folderDirectory)) {
		// If it's a folder, go into it
		if (std::filesystem::is_directory(entry)) {
			SelectiveLoadAllResources(entry);
			continue;
		}
#if MultiThread
		mResourceLoadingThreads.push_back(std::thread([this, entry] {SelectiveLoadResource(entry); }));
#else
		LoadResource(entry);
#endif
	}
}

void ResourceManager::SelectiveLoadResource(std::filesystem::path const& entry) {
	// Skip any non-meta files
	const std::string fileName{ entry.filename().string() };
	if (fileName.find(".meta") == std::string::npos) return;

	// Check meta file to know if resource should be loaded
	GUID guid = ReadGUIDFromFile(entry.string());
	if (mGuidList.find(guid) == mGuidList.end()) return;

	/*bool approvedGUID{ false };
	for (GUID neededGUID : mGuidList)
		if (guid == neededGUID) {
			approvedGUID = true;
			break;
		}
	if (!approvedGUID) return;*/


	// Get resource path
	uint64_t fileNameLength{ entry.string().length() - std::strlen(".meta") };
	const std::filesystem::path resourcePath{ entry.string().substr(0, fileNameLength) };
	//if ("Props_Boat_E_Spritesheet.png" == resourcePath.filename().string()) {
	//	std::cout << "hello\n";
	//}
	//if ("Props_Boat_N_Spritesheet.png" == resourcePath.filename().string()) {
	//	std::cout << "hello\n";
	//}
	// Open and store resources, then linking them to their guid
	E_RESOURCETYPE resourceType{ CheckResourceType(resourcePath) };
	if (resourceType == E_RESOURCETYPE::error || resourceType == E_RESOURCETYPE::prefab) return;

	void* dataPointer{};

	switch (resourceType) {
	case E_RESOURCETYPE::texture:
		dataPointer = new TextureData;
		*(static_cast<TextureData*>(dataPointer)) = LoadTextureWithoutOpenGL(resourcePath.string());
		break;

	case E_RESOURCETYPE::audio:
		myLock.lock();
		(audioManager->LoadAudio(resourcePath));
		myLock.unlock();
		break;

	case E_RESOURCETYPE::script:
		break;

	case E_RESOURCETYPE::scene:
		break;

	case E_RESOURCETYPE::gamestateEntities:
		break;

	case E_RESOURCETYPE::dialogue:
		break;

	case E_RESOURCETYPE::font:
		break;

	case E_RESOURCETYPE::prefab:
		break;
	}
	(void)dataPointer;
	myLock.lock();
	mAllResources.insert({ guid, dataPointer });
	mAllFilePaths.insert({ guid, resourcePath.string() });
	myLock.unlock();
	std::cout << "GUID: " << guid << " | File: " << entry.string() << '\n';
}

void ResourceManager::SelectiveUnloadAllResources() {
	//audioManager->Unload();
	spriteManager->FreeTextures();
	mResources.clear();
}
