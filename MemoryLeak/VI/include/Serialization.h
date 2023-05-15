/*!*****************************************************************************
\file Serialization.h
\author Huang Wei Jhin
\par DP email: h.weijhin@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function declarations for a Serialization system that modifies
Entities and its Components.
*******************************************************************************/
#pragma once
#include <ECS_systems.h>
#include "ECS_items.h"
#include "ECS_components.h"
#include <ECS_tools.h>
#include "ResourceManager.h"
#include <vec2.h>

#include "document.h"
#include "writer.h"
#include <istreamwrapper.h>
#include "stringbuffer.h"
#include <prettywriter.h>

class Scene;
class GameState;
using namespace rapidjson;
/*!*****************************************************************************
\brief
	This class encapsulates the functions for Serialization manager
*******************************************************************************/
class SerializationManager : public System
{
public:
	void LoadScene(Scene& _sceneData, std::filesystem::path _filename);
	void SaveScene(Scene& _sceneData);
	void LoadGameState(GameState& _gameState, std::filesystem::path _filename);
	void SaveGameState(GameState& _gameState);


	void LoadPrefab(std::string _filename = "NewPrefab");
	void SavePrefab(std::string _filename = "NewPrefab");
	void LoadDialogs(std::string _filename = "Dialog1");
	void SaveDialogs(std::string _filename = "Dialog1");

	void LoadPlayerData(std::string _filename = "Dialog1");
	void SavePlayerData(std::string _filename = "Dialog1");

	void SaveSceneGUID(std::string sceneName);
	void GetGUIDList(std::filesystem::path _filename, std::set<ResourceManager::GUID>& GUIDList);

	//getters
	std::string GetSceneFilename();
	std::string GetGameStateFilename();
	std::string GetPrefabFilename();
	std::string GetDialogueFilename();
	std::vector<std::string> GetAllsceneFilename();
	std::vector<std::string> GetAllgameStateFilename();
	std::vector<std::string> GetAllprefabFilename();
	std::vector<std::string> GetAlldialogueFilename();



private:
	std::string sceneFilename;
	std::string gameStateFilename;
	std::string prefabFilename;
	std::string dialogueFilename;
	std::vector<std::string> allsceneFilename;
	std::vector<std::string> allgameStateFilename;
	std::vector<std::string> allprefabFilename;
	std::vector<std::string> alldialogueFilename;
	std::vector<ResourceManager::GUID> GUIDList;
	//helper functions
	static Math::Vec2 GetVec2(Value& vecIn);
	static std::vector<int> GetVectorInt(Value& vecIn);
	static void addVectorMember(Document& scene, Value& parent, const char* name, Math::Vec2 data);
	template<typename T>
	static void addVectorArrayStrMember(Document& scene, Value& parent, const char* name, std::vector <T> data);
	template<typename T>
	static void addVectorArrayForceMember(Document& scene, Value& parent, const char* name, std::vector <T> data);
	template<typename T>
	static void addVectorArrayMember(Document& scene, Value& parent, const char* name, std::vector <T> data);
	template<typename T>
	static void addVectorsMember(Document& scene, Value& parent, const char* name, std::vector <T> data);
	void addColor(Document& scene, Value& entity, Color color);
	Color getColor(Value& entity);

	//load components
	General getGeneral(Value& entity);
	Lifespan getLifespan(Value& entity);
	Transform getTransform(Value& entity);
	Sprite getSprite(Value& entity);
	Animation getAnimation(Value& entity);
	SheetAnimation getSheetAnimation(Value& entity);
	Physics2D getPhysics2D(Value& entity);
	RectCollider getRectCollider(Value& entity);
	LayerCollider getLayerCollider(Value& entity);
	CircleCollider getCircleCollider(Value& entity);
	Edge2DCollider getEdge2DCollider(Value& entity);
	Point2DCollider getPoint2DCollider(Value& entity);
	Audio getAudio(Value& entity);
	AI getAI(Value& entity);
	Text getText(Value& entity);
	Dialogue getDialogue(Value& entity);
	Script getScript(Value& entity);
	Button getButton(Value& entity);
	LightSource getLightSource(Value& entity);
	ShadowCaster getShadowCaster(Value& entity);
	Viewport getViewport(Value& entity);
	MovementAI getMovementAI(Value& entity);
	ColorAI getColorAI(Value& entity);
	PlayerData getPlayerData(Value& entity);
	ParticleSystem::ParticleInfo getParticleInfo(Value& entity);
	ParticleSystem getParticleSystem(Value& entity);

	//save components 
	void addGeneral(Document& scene, Value& entity, General general);
	void addLifespan(Document& scene, Value& entity, Lifespan lifespan);
	void addTransform(Document& scene, Value& entity, Transform transform);
	void addSprite(Document& scene, Value& entity, Sprite sprite);
	void addAnimation(Document& scene, Value& entity, Animation animation);
	void addSheetAnimation(Document& scene, Value& entity, SheetAnimation sheetAnimation);
	void addPhysics2D(Document& scene, Value& entity, Physics2D physics2D);
	void addRectCollider(Document& scene, Value& entity, RectCollider rectCollider);
	void addLayerCollider(Document& scene, Value& entity, LayerCollider layerCollider);
	void addCircleCollider(Document& scene, Value& entity, CircleCollider circleCollider);
	void addEdge2DCollider(Document& scene, Value& entity, Edge2DCollider edge2DCollider);
	void addPoint2DCollider(Document& scene, Value& entity, Point2DCollider point2DCollider);
	void addAudio(Document& scene, Value& entity, Audio audio);
	void addAI(Document& scene, Value& entity, AI ai);
	void addText(Document& scene, Value& entity, Text text);
	void addDialogue(Document& scene, Value& entity, Dialogue dialogue);
	void addScript(Document& scene, Value& entity, Script script);
	void addButton(Document& scene, Value& entity, Button button);
	void addLightSource(Document& scene, Value& entity, LightSource lightSource);
	void addShadowCaster(Document& scene, Value& entity, ShadowCaster shadowCaster);
	void addViewport(Document& scene, Value& entity, Viewport Viewport);
	void addMovementAI(Document& scene, Value& entity, MovementAI movementAI);
	void addColorAI(Document& scene, Value& entity, ColorAI colorAI);
	void addPlayerData(Document& scene, Value& entity, PlayerData playerData);
	void addParticleInfo(Document& scene, Value& entity, ParticleSystem::ParticleInfo particleInfo);
	void addParticleSystem(Document& scene, Value& entity, ParticleSystem particleSystem);
};



