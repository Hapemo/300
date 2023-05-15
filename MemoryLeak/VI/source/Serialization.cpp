/*!*****************************************************************************
\file Serialization.cpp
\author Huang Wei Jhin, Chen Jia Wen
\par DP email: h.weijhin@digipen.edu, c.jiawen@digipen.edu
\par Group: Memory Leak Studios
\date 20-09-2022
\brief
This file contains function definations for a Serialization system that saves and load
-Entities and its Components
-Dialogs
TODO: take note not to change the component registration order. It will break prefab
*******************************************************************************/
#include <Serialization.h>
#include <ECSManager.h>
#include <Scene.h>
#include <GameState.h>
#include "GameStateManager.h"
#include "ResourceManager.h"

using namespace rapidjson;
/*!*****************************************************************************
\brief
	Creats a vec2 using rapidjson value
\param
	vecIn rapidjason vetor 2 value

\return
None.
*******************************************************************************/
Math::Vec2 SerializationManager::GetVec2(Value& vecIn)
{
	Math::Vec2 vecOut;
	vecOut.x = vecIn["X"].GetFloat();
	vecOut.y = vecIn["Y"].GetFloat();
	return vecOut;
}

/*!*****************************************************************************
\brief
	Creates vector of ints using rapidjson value
\param
	vecIn rapidjason vetor  value

\return
None.
*******************************************************************************/
std::vector<int> SerializationManager::GetVectorInt(Value& vecIn)
{
	std::vector<int> data;
	Value dataobj(kArrayType);
	dataobj = vecIn.GetArray();
	for (rapidjson::SizeType index = 0; index < dataobj.Size(); ++index)
	{
		data.push_back(dataobj[index].GetInt());
	}
	return data;
}

/*!*****************************************************************************
\brief
	Loads the scene into ECS

\return
None.
*******************************************************************************/
void SerializationManager::LoadScene(Scene& _sceneData, std::filesystem::path _filename)
{
	//Scene sceneData{};

	//"../resources/Scene/SceneJ.json"
	//std::string path = "../resources/Scene/" + _filename + ".json";
	//std::ifstream ifs(path);
	std::ifstream ifs(_filename.string());
	if (!ifs.good())
	{
		LOG_ERROR("Can't open json file! : " + _filename.string());
		return;
	}
	else
		LOG_INFO("Opening Scene: " + _filename.string());

	_sceneData.mName = _filename.stem().string();
	int same = 0;
	/*for (std::string s : allsceneFilename)
	{
		if (s == _sceneData.mName)
			same++;
	}*/
	for (Scene s : GameStateManager::GetInstance()->mCurrentGameState->mScenes)
	{
		if (s.mName == _sceneData.mName)
			same++;
	}
	allsceneFilename.push_back(_sceneData.mName);
	if (same > 1)
	{
		same = 0;
		for (std::string n : allsceneFilename)
		{
			if (n == _sceneData.mName)
				same++;
		}
		_sceneData.mName += (" (" + std::to_string(same) + ")");
	}
	sceneFilename = _sceneData.mName;
	std::stringstream contents;
	contents << ifs.rdbuf();
	Document doc;
	doc.Parse(contents.str().c_str());
	int x = 1;
	if (_sceneData.mName[0] == 'S' && _sceneData.mName[1] == 'c' && _sceneData.mName[3] == 'n')
		x = 0;
	if (x == 1)
	{
		Value entity(kArrayType);
		entity = doc.GetArray();
		for (rapidjson::SizeType index = 0; index < entity.Size(); ++index)
		{
			Entity e{ ECS::CreateEntity() };
			if (entity[index].HasMember("General"))
			{
				e.AddComponent<General>(getGeneral(entity[index]));
			}
			if (entity[index].HasMember("Lifespan"))
			{
				e.AddComponent<Lifespan>(getLifespan(entity[index]));
			}
			if (entity[index].HasMember("Transform"))
			{
				e.AddComponent<Transform>(getTransform(entity[index]));
			}
			if (entity[index].HasMember("Sprite"))
			{
				e.AddComponent<Sprite>(getSprite(entity[index]));
			}
			if (entity[index].HasMember("Animation"))
			{
				e.AddComponent<Animation>(getAnimation(entity[index]));
			}
			if (entity[index].HasMember("SheetAnimation"))
			{
				e.AddComponent<SheetAnimation>(getSheetAnimation(entity[index]));
			}
			if (entity[index].HasMember("Physics2D"))
			{
				e.AddComponent<Physics2D>(getPhysics2D(entity[index]));
			}
			if (entity[index].HasMember("LayerCollider"))
			{
				e.AddComponent<LayerCollider>(getLayerCollider(entity[index]));
			}
			if (entity[index].HasMember("RectCollider"))
			{
				e.AddComponent<RectCollider>(getRectCollider(entity[index]));
			}
			if (entity[index].HasMember("CircleCollider"))
			{
				e.AddComponent<CircleCollider>(getCircleCollider(entity[index]));
			}
			if (entity[index].HasMember("Edge2DCollider"))
			{
				e.AddComponent<Edge2DCollider>(getEdge2DCollider(entity[index]));
			}
			if (entity[index].HasMember("Point2DCollider"))
			{
				e.AddComponent<Point2DCollider>(getPoint2DCollider(entity[index]));
			}
			if (entity[index].HasMember("Audio"))
			{
				e.AddComponent<Audio>(getAudio(entity[index]));
			}
			if (entity[index].HasMember("AI"))
			{
				e.AddComponent<AI>(getAI(entity[index]));
			}
			if (entity[index].HasMember("Text")) 
			{
				e.AddComponent<Text>(getText(entity[index]));
			}
			if (entity[index].HasMember("Dialogue")) 
			{
				e.AddComponent<Dialogue>(getDialogue(entity[index]));
			}
			if (entity[index].HasMember("Script")) 
			{
				e.AddComponent<Script>(getScript(entity[index]));
			}
			if (entity[index].HasMember("Button")) 
			{
				e.AddComponent<Button>(getButton(entity[index]));
			}
			if (entity[index].HasMember("LightSource")) 
			{
				e.AddComponent<LightSource>(getLightSource(entity[index]));
			}
			if (entity[index].HasMember("ShadowCaster")) 
			{
				e.AddComponent<ShadowCaster>(getShadowCaster(entity[index]));
			}
			if (entity[index].HasMember("Viewport")) 
			{
				e.AddComponent<Viewport>(getViewport(entity[index]));
			}
			if (entity[index].HasMember("MovementAI")) 
			{
				e.AddComponent<MovementAI>(getMovementAI(entity[index]));
			}
			if (entity[index].HasMember("ColorAI"))
			{
				e.AddComponent<ColorAI>(getColorAI(entity[index]));
			}
			if (entity[index].HasMember("PlayerData"))
			{
				e.AddComponent<PlayerData>(getPlayerData(entity[index]));
			}
			if (entity[index].HasMember("ParticleSystem"))
			{
				e.AddComponent<ParticleSystem>(getParticleSystem(entity[index]));
			}
			//mEntities.insert(e);
			_sceneData.mEntities.insert(e);
		}
	}
	else////////////to be deleted REMOVEME
	{
		//int i = 0;
		//for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr <= doc.MemberEnd(); ++itr)
		//{
		//	std::string header = "SceneData";
		//	Value head(header.c_str(), (SizeType)header.size(), doc.GetAllocator());

		//	Entity e{ ECS::CreateEntity() };
		//	Value entity(kObjectType);
		//	std::string str("Entity" + std::to_string(i));
		//	Value index(str.c_str(), (SizeType)str.size(), doc.GetAllocator());
		//	if (!doc.HasMember(index))
		//	{
		//		if (doc.HasMember(head))
		//		{
		//			continue;
		//		}
		//		LOG_INFO("1 Enitity not loaded");

		//		continue;
		//	}
		//	entity = doc[index];

		//	if (entity.HasMember("General"))
		//	{
		//		std::string name = entity["General"]["name"].GetString();
		//		bool isActive = entity["General"]["isActive"].GetBool();
		//		int tag = entity["General"]["tag"].GetInt();
		//		int subtag = entity["General"]["subtag"].GetInt();
		//		e.AddComponent<General>(General{ name, (TAG)tag ,(SUBTAG)subtag, isActive });
		//		//if (tag == (int)TAG::PLAYER)
		//		//	e.AddComponent(PlayerTmp{ 0 });
		//	}
		//	if (entity.HasMember("Lifespan"))
		//	{
		//		float lifetime = entity["Lifespan"]["lifetime"].GetFloat();
		//		float limit = entity["Lifespan"]["limit"].GetFloat();
		//		e.AddComponent<Lifespan>({ lifetime, limit });
		//	}
		//	if (entity.HasMember("Transform"))
		//	{
		//		Math::Vec2 s = GetVec2(entity["Transform"]["scale"]);
		//		float r = (float)entity["Transform"]["rotation"].GetFloat();
		//		Math::Vec2 t = GetVec2(entity["Transform"]["translation"]);

		//		e.AddComponent<Transform>({ s, r, t });
		//	}
		//	if (entity.HasMember("Sprite"))
		//	{
		//		Color c;
		//		c.r = (GLubyte)entity["Sprite"]["color"]["r"].GetInt();
		//		c.g = (GLubyte)entity["Sprite"]["color"]["g"].GetInt();
		//		c.b = (GLubyte)entity["Sprite"]["color"]["b"].GetInt();
		//		c.a = (GLubyte)entity["Sprite"]["color"]["a"].GetInt();
		//		SPRITE s = (SPRITE)entity["Sprite"]["sprite"].GetInt();
		//		GLuint t = (GLuint)spriteManager->GetTextureID(entity["Sprite"]["texture"].GetString());
		//		//GLuint t = (GLuint)entity["Sprite"]["texture"].GetInt();
		//		int l = entity["Sprite"]["layer"].GetInt();
		//		e.AddComponent<Sprite>({ c, s, t ,l });
		//	}
		//	if (entity.HasMember("Animation"))
		//	{
		//		std::vector<GLuint> images;
		//		Value a(kObjectType);
		//		a = entity["Animation"]["images"].GetArray();
		//		for (int j = 0; j < (int)a.Size(); ++j)
		//		{
		//			GLuint tex = spriteManager->GetTextureID(a[j].GetString());
		//			images.push_back(tex);
		//		}
		//		//float timePerImage = entity["Animation"]["timePerImage"].GetFloat();
		//		//float timeToImageSwap = entity["Animation"]["timeToImageSwap"].GetFloat();
		//		//int currentImageIndex = entity["Animation"]["currentImageIndex"].GetInt();
		//		//e.AddComponent<Animation>({ images , timePerImage , timeToImageSwap , currentImageIndex });
		//	}
		//	if (entity.HasMember("SheetAnimation"))
		//	{
		//		short frameCount = (short)entity["SheetAnimation"]["frameCount"].GetInt();
		//		short currentImageIndex = (short)entity["SheetAnimation"]["currFrameIndex"].GetInt();
		//		float timePerImage = entity["SheetAnimation"]["timePerFrame"].GetFloat();
		//		float timeToImageSwap = entity["SheetAnimation"]["timeToFrameSwap"].GetFloat();
		//		e.AddComponent<SheetAnimation>({ frameCount , currentImageIndex , timePerImage , timeToImageSwap });
		//	}
		//	if (entity.HasMember("Physics2D")) {

		//		bool dynamicsEnabled = entity["Physics2D"]["dynamicsEnabled"].GetBool();
		//		float mass = entity["Physics2D"]["mass"].GetFloat();
		//		float inertia = entity["Physics2D"]["inertia"].GetFloat();
		//		float restitution = entity["Physics2D"]["restitution"].GetFloat();
		//		float friction = entity["Physics2D"]["friction"].GetFloat();
		//		float damping = entity["Physics2D"]["damping"].GetFloat();

		//		Math::Vec2 accumulatedForce = GetVec2(entity["Physics2D"]["accumulatedForce"]);
		//		Math::Vec2 velocity = GetVec2(entity["Physics2D"]["velocity"]);
		//		Math::Vec2 acceleration = GetVec2(entity["Physics2D"]["acceleration"]);

		//		float angularVelocity = entity["Physics2D"]["angularVelocity"].GetFloat();
		//		float angularTorque = entity["Physics2D"]["angularTorque"].GetFloat();

		//		//vect force
		//		std::vector<Force> forceList{};
		//		Value a(kObjectType);
		//		a = entity["Physics2D"]["forceList"].GetArray();
		//		for (int j = 0; j < (int)a.Size(); ++j)
		//		{
		//			Force force{};
		//			Value f(kObjectType);
		//			f = a[j].GetObject();
		//			force.lifetimeLimit = f["lifetimeLimit"].GetDouble();
		//			force.age = f["age"].GetDouble();
		//			force.isActive = f["isActive"].GetBool();
		//			force.forceID = f["forceID"].GetInt();
		//			if (force.forceID == 0)
		//			{
		//				force.linearForce.unitDirection = GetVec2(f["linearForce"]["unitDirection"]);
		//				force.linearForce.magnitude = f["linearForce"]["magnitude"].GetFloat();
		//			}
		//			else if (force.forceID == 1)
		//			{
		//				force.rotationalForce.torque = f["rotationalForce"]["torque"].GetFloat();
		//			}
		//			else if (force.forceID == 2)
		//			{
		//				force.dragForce.directionalDrag = f["dragForce"]["directionalDrag"].GetFloat();
		//				force.dragForce.rotationalDrag = f["dragForce"]["rotationalDrag"].GetFloat();
		//			}

		//			forceList.push_back(force);
		//		}

		//		bool renderFlag = entity["Physics2D"]["renderFlag"].GetBool();

		//		e.AddComponent<Physics2D>({ dynamicsEnabled, mass, inertia, restitution, friction, damping, accumulatedForce,velocity, acceleration, angularVelocity, angularTorque, forceList, renderFlag });
		//	}
		//	if (entity.HasMember("RectCollider"))
		//	{
		//		Math::Vec2 centerOffset = GetVec2(entity["RectCollider"]["centerOffset"]);
		//		Math::Vec2	scaleOffset = GetVec2(entity["RectCollider"]["scaleOffset"]);
		//		bool isTrigger = entity["RectCollider"]["isTrigger"].GetBool();
		//		bool renderFlag = entity["RectCollider"]["renderFlag"].GetBool();
		//		e.AddComponent<RectCollider>({ centerOffset , scaleOffset , isTrigger, renderFlag });
		//	}
		//	if (entity.HasMember("CircleCollider"))
		//	{
		//		Math::Vec2 centerOffset = GetVec2(entity["CircleCollider"]["centerOffset"]);
		//		float scaleOffset = entity["CircleCollider"]["scaleOffset"].GetFloat();
		//		bool isTrigger = entity["CircleCollider"]["isTrigger"].GetBool();
		//		bool renderFlag = entity["CircleCollider"]["renderFlag"].GetBool();
		//		e.AddComponent<CircleCollider>({ centerOffset , scaleOffset , isTrigger, renderFlag });
		//	}
		//	if (entity.HasMember("Edge2DCollider"))
		//	{
		//		Math::Vec2 p0Offset = GetVec2(entity["Edge2DCollider"]["p0Offset"]);
		//		float rotationOffset = entity["Edge2DCollider"]["rotationOffset"].GetFloat();
		//		float scaleOffset = entity["Edge2DCollider"]["scaleOffset"].GetFloat();
		//		bool renderFlag = entity["Edge2DCollider"]["renderFlag"].GetBool();

		//		e.AddComponent<Edge2DCollider>({ p0Offset ,rotationOffset, scaleOffset , renderFlag });
		//	}
		//	if (entity.HasMember("Point2DCollider"))
		//	{
		//		Math::Vec2 centerOffset = GetVec2(entity["Point2DCollider"]["centerOffset"]);
		//		bool renderFlag = entity["Point2DCollider"]["renderFlag"].GetBool();
		//		e.AddComponent<Point2DCollider>({ centerOffset ,renderFlag });
		//	}
		//	if (entity.HasMember("Audio"))
		//	{
		//		Sound sound;
		//		sound.path = entity["Audio"]["path"].GetString();
		//		sound.volume = entity["Audio"]["volume"].GetFloat();
		//		sound.volumeMod = entity["Audio"]["volumeMod"].GetFloat();
		//		sound.pitch = entity["Audio"]["pitch"].GetFloat();
		//		sound.isPaused = entity["Audio"]["isPaused"].GetBool();
		//		sound.isMute = entity["Audio"]["isMute"].GetBool();
		//		sound.isLoop = entity["Audio"]["isLoop"].GetBool();
		//		sound.isRandPitch = entity["Audio"]["isRandPitch"].GetBool();

		//		bool isSpacial = entity["Audio"]["isSpacial"].GetBool();
		//		e.AddComponent<Audio>({ sound , isSpacial });
		//	}
		//	if (entity.HasMember("AI"))
		//	{
		//		int colorChange = entity["AI"]["colorChange"].GetInt();
		//		int movement = entity["AI"]["movement"].GetInt();
		//		float speed = entity["AI"]["speed"].GetFloat();
		//		float range = entity["AI"]["range"].GetFloat();

		//		e.AddComponent<AI>({ colorChange ,movement, speed , range });
		//	}
		//	if (entity.HasMember("Text")) {
		//		Text text;
		//		text.fontFile = entity["Text"]["fontFile"].GetString();
		//		text.text = entity["Text"]["text"].GetString();
		//		text.offset = GetVec2(entity["Text"]["offset"]);
		//		text.scale = entity["Text"]["scale"].GetFloat();
		//		text.color.r = (GLubyte)entity["Text"]["color"]["r"].GetInt();
		//		text.color.g = (GLubyte)entity["Text"]["color"]["g"].GetInt();
		//		text.color.b = (GLubyte)entity["Text"]["color"]["b"].GetInt();
		//		text.color.a = (GLubyte)entity["Text"]["color"]["a"].GetInt();
		//		e.AddComponent<Text>(text);
		//	}
		//	if (entity.HasMember("Dialogue")) {
		//		Dialogue dialogue;
		//		dialogue.speakerID = (GLubyte)entity["Dialogue"]["speakerID"].GetInt();
		//		dialogue.selectedID = (GLubyte)entity["Dialogue"]["selectedID"].GetInt();
		//		dialogue.textID = (GLubyte)entity["Dialogue"]["textID"].GetInt();
		//		dialogue.nextTextID = (GLubyte)entity["Dialogue"]["nextTextID"].GetInt();
		//		e.AddComponent<Dialogue>(dialogue);
		//	}
		//	if (entity.HasMember("LightSource")) {
		//		LightSource lightSource;
		//		lightSource.centerOffset = GetVec2(entity["LightSource"]["centerOffset"]);
		//		e.AddComponent<LightSource>(lightSource);
		//	}
		//	if (entity.HasMember("Script")) {
		//		Script script;
		//		script.name = entity["Script"]["name"].GetString();
		//		//script.script = nullptr;

		//		e.AddComponent<Script>(script);
		//	}
		//	//mEntities.insert(e);
		//	_sceneData.mEntities.insert(e);
		//	i++;
		//}
	}

	ifs.close();
}


/*!*****************************************************************************
\brief
	Gets each component from rapid json
\param entity
	rapid json object to get from
\return
None.
*******************************************************************************/
General SerializationManager::getGeneral(Value& entity)
{
	General general;
	general.name = entity["General"]["name"].GetString();
	general.isActive = entity["General"]["isActive"].GetBool();
	general.tag = (TAG)entity["General"]["tag"].GetInt();
	general.subtag = (SUBTAG)entity["General"]["subtag"].GetInt();
	general.isPaused = false;
	return general;
}
Lifespan SerializationManager::getLifespan(Value& entity)
{
	Lifespan lifespan;
	lifespan.lifetime = entity["Lifespan"]["lifetime"].GetFloat();
	lifespan.limit = entity["Lifespan"]["limit"].GetFloat();
	return lifespan;
}
Transform SerializationManager::getTransform(Value& entity)
{
	Transform transform;
	transform.scale = GetVec2(entity["Transform"]["scale"]);
	transform.rotation = (float)entity["Transform"]["rotation"].GetFloat();
	transform.translation = GetVec2(entity["Transform"]["translation"]);
	return transform;
}
Color SerializationManager::getColor(Value& entity)
{
	Color color;
	color.r = (GLubyte)entity["color"]["r"].GetInt();
	color.g = (GLubyte)entity["color"]["g"].GetInt();
	color.b = (GLubyte)entity["color"]["b"].GetInt();
	color.a = (GLubyte)entity["color"]["a"].GetInt();
	return color;
}
Sprite SerializationManager::getSprite(Value& entity)
{
	Sprite sprite;
	sprite.color.r = (GLubyte)entity["Sprite"]["color"]["r"].GetInt();
	sprite.color.g = (GLubyte)entity["Sprite"]["color"]["g"].GetInt();
	sprite.color.b = (GLubyte)entity["Sprite"]["color"]["b"].GetInt();
	sprite.color.a = (GLubyte)entity["Sprite"]["color"]["a"].GetInt();
	sprite.sprite = (SPRITE)entity["Sprite"]["sprite"].GetInt();
	if (sprite.sprite == SPRITE::TEXTURE)
		sprite.texture = (GLuint)spriteManager->GetTextureID(entity["Sprite"]["texture"].GetString());
	else
		sprite.texture = 0;
	sprite.layer = entity["Sprite"]["layer"].GetInt();
	return sprite;
}
Animation SerializationManager::getAnimation(Value& entity)
{
	Animation animation;
	Value a(kObjectType);
	if (entity["Animation"].HasMember("sheets"))
	{
	a = entity["Animation"]["sheets"].GetArray();
	for (int j = 0; j < (int)a.Size(); ++j)
	{
		SpriteSheet sheets{};
		sheets.sheet = spriteManager->GetTextureID(a[j]["sheet"].GetString());
		sheets.frameCount = a[j]["frameCount"].GetInt();
		sheets.timePerFrame = a[j]["timePerFrame"].GetFloat();
		animation.sheets.push_back(sheets);
	}
	//animation.timePerImage.push_back(entity["Animation"]["timePerImage"].GetFloat());/////////////!!!!!!!!!!!!!!!!!!!
	//animation.timeToImageSwap = entity["Animation"]["timeToImageSwap"].GetFloat();
	animation.currentImageIndex = entity["Animation"]["currentImageIndex"].GetInt();
	}
	return animation;
}
SheetAnimation SerializationManager::getSheetAnimation(Value& entity)
{
	SheetAnimation sheetAnimation;
	sheetAnimation.frameCount = (short)entity["SheetAnimation"]["frameCount"].GetInt();
	sheetAnimation.currFrameIndex = (short)entity["SheetAnimation"]["currFrameIndex"].GetInt();
	sheetAnimation.timePerFrame = entity["SheetAnimation"]["timePerFrame"].GetFloat();
	sheetAnimation.timeToFrameSwap = entity["SheetAnimation"]["timeToFrameSwap"].GetFloat();
	return sheetAnimation;
}
Physics2D SerializationManager::getPhysics2D(Value& entity)
{
	Physics2D physics2D;
	bool dynamicsEnabled = entity["Physics2D"]["dynamicsEnabled"].GetBool();
	float mass = entity["Physics2D"]["mass"].GetFloat();
	float inertia = entity["Physics2D"]["inertia"].GetFloat();
	float restitution = entity["Physics2D"]["restitution"].GetFloat();
	float friction = entity["Physics2D"]["friction"].GetFloat();
	float damping = entity["Physics2D"]["damping"].GetFloat();

	Math::Vec2 accumulatedForce = GetVec2(entity["Physics2D"]["accumulatedForce"]);
	Math::Vec2 velocity = GetVec2(entity["Physics2D"]["velocity"]);
	Math::Vec2 acceleration = GetVec2(entity["Physics2D"]["acceleration"]);

	float angularVelocity = entity["Physics2D"]["angularVelocity"].GetFloat();
	float angularTorque = entity["Physics2D"]["angularTorque"].GetFloat();

	//vect force
	std::vector<Force> forceList{};
	Value a(kObjectType);
	a = entity["Physics2D"]["forceList"].GetArray();
	for (int j = 0; j < (int)a.Size(); ++j)
	{
		Force force{};
		Value f(kObjectType);
		f = a[j].GetObject();
		force.lifetimeLimit = f["lifetimeLimit"].GetDouble();
		force.age = f["age"].GetDouble();
		force.isActive = f["isActive"].GetBool();
		force.forceID = f["forceID"].GetInt();
		if (force.forceID == 0)
		{
			force.linearForce.unitDirection = GetVec2(f["linearForce"]["unitDirection"]);
			force.linearForce.magnitude = f["linearForce"]["magnitude"].GetFloat();
		}
		else if (force.forceID == 1)
		{
			force.rotationalForce.torque = f["rotationalForce"]["torque"].GetFloat();
		}
		else if (force.forceID == 2)
		{
			force.dragForce.directionalDrag = f["dragForce"]["directionalDrag"].GetFloat();
			force.dragForce.rotationalDrag = f["dragForce"]["rotationalDrag"].GetFloat();
		}

		forceList.push_back(force);
	}

	bool renderFlag = entity["Physics2D"]["renderFlag"].GetBool();

	physics2D = { dynamicsEnabled, mass, inertia, restitution, friction, damping, accumulatedForce,velocity, acceleration, angularVelocity, angularTorque, forceList, renderFlag };

	return physics2D;
}
RectCollider SerializationManager::getRectCollider(Value& entity)
{
	RectCollider rectCollider;
	rectCollider.centerOffset = GetVec2(entity["RectCollider"]["centerOffset"]);
	rectCollider.scaleOffset = GetVec2(entity["RectCollider"]["scaleOffset"]);
	if (entity["RectCollider"].HasMember("isTrigger"))
		rectCollider.isTrigger = entity["RectCollider"]["isTrigger"].GetBool();
	rectCollider.renderFlag = entity["RectCollider"]["renderFlag"].GetBool();
	return rectCollider;
}
LayerCollider SerializationManager::getLayerCollider(Value& entity)
{
	LayerCollider layerCollider;
	layerCollider.centerOffset = GetVec2(entity["LayerCollider"]["centerOffset"]);
	layerCollider.scaleOffset = GetVec2(entity["LayerCollider"]["scaleOffset"]);
	layerCollider.renderFlag = entity["LayerCollider"]["renderFlag"].GetBool();
	return layerCollider;
}
CircleCollider SerializationManager::getCircleCollider(Value& entity)
{
	CircleCollider circleCollider;
	circleCollider.centerOffset = GetVec2(entity["CircleCollider"]["centerOffset"]);
	circleCollider.scaleOffset = entity["CircleCollider"]["scaleOffset"].GetFloat();
	if (entity["CircleCollider"].HasMember("isTrigger"))
		circleCollider.isTrigger = entity["CircleCollider"]["isTrigger"].GetBool();
	circleCollider.renderFlag = entity["CircleCollider"]["renderFlag"].GetBool();
	return circleCollider;
}
Edge2DCollider SerializationManager::getEdge2DCollider(Value& entity)
{
	Edge2DCollider edge2DCollider;
	edge2DCollider.p0Offset = GetVec2(entity["Edge2DCollider"]["p0Offset"]);
	edge2DCollider.rotationOffset = entity["Edge2DCollider"]["rotationOffset"].GetFloat();
	edge2DCollider.scaleOffset = entity["Edge2DCollider"]["scaleOffset"].GetFloat();
	edge2DCollider.renderFlag = entity["Edge2DCollider"]["renderFlag"].GetBool();
	return edge2DCollider;

}
Point2DCollider SerializationManager::getPoint2DCollider(Value& entity)
{
	Point2DCollider point2DCollider;
	point2DCollider.centerOffset = GetVec2(entity["Point2DCollider"]["centerOffset"]);
	point2DCollider.renderFlag = entity["Point2DCollider"]["renderFlag"].GetBool();
	return point2DCollider;
}
Audio SerializationManager::getAudio(Value& entity)
{
	Audio audio;
	Sound sound;
	sound.path = entity["Audio"]["path"].GetString();
	if(entity["Audio"].HasMember("toPlay"))//////////////////////////////////////////remove
		sound.toPlay = entity["Audio"]["toPlay"].GetBool();
	sound.volume = entity["Audio"]["volume"].GetFloat();
	sound.volumeMod = entity["Audio"]["volumeMod"].GetFloat();
	sound.pitch = entity["Audio"]["pitch"].GetFloat();
	sound.isPaused = entity["Audio"]["isPaused"].GetBool();
	sound.isMute = entity["Audio"]["isMute"].GetBool();
	sound.isLoop = entity["Audio"]["isLoop"].GetBool();
	sound.isRandPitch = entity["Audio"]["isRandPitch"].GetBool();
	if(entity["Audio"].HasMember("toPlayOnCollision"))
		sound.toPlayOnCollision = entity["Audio"]["toPlayOnCollision"].GetBool();
	audio.sound = sound;
	audio.isSpacial = entity["Audio"]["isSpacial"].GetBool();
	if (entity["Audio"].HasMember("spacialDistance"))
		audio.spacialDistance = entity["Audio"]["spacialDistance"].GetFloat();
	if (entity["Audio"].HasMember("spacialRatio"))
		audio.spacialRatio = entity["Audio"]["spacialRatio"].GetFloat();
	return audio;
}
AI SerializationManager::getAI(Value& entity)
{
	AI ai;
	ai.colorChange = entity["AI"]["colorChange"].GetInt();
	ai.movement = entity["AI"]["movement"].GetInt();
	ai.speed = entity["AI"]["speed"].GetFloat();
	ai.range = entity["AI"]["range"].GetFloat();
	return ai;
}
Text SerializationManager::getText(Value& entity)
{
	Text text;
	text.fontFile = entity["Text"]["fontFile"].GetString();
	text.text = entity["Text"]["text"].GetString();
	text.offset = GetVec2(entity["Text"]["offset"]);
	text.scale = entity["Text"]["scale"].GetFloat();
	text.color.r = (GLubyte)entity["Text"]["color"]["r"].GetInt();
	text.color.g = (GLubyte)entity["Text"]["color"]["g"].GetInt();
	text.color.b = (GLubyte)entity["Text"]["color"]["b"].GetInt();
	text.color.a = (GLubyte)entity["Text"]["color"]["a"].GetInt();
	if (entity["Text"].HasMember("followCam"))//////remove this if line
		text.followCam = entity["Text"]["followCam"].GetBool();
	if (entity["Text"].HasMember("width"))//////remove this if line
		text.width = entity["Text"]["width"].GetFloat();
	return text;
}
Dialogue SerializationManager::getDialogue(Value& entity)
{
	Dialogue dialogue;
	dialogue.speakerID = (GLubyte)entity["Dialogue"]["speakerID"].GetInt();
	dialogue.selectedID = (GLubyte)entity["Dialogue"]["selectedID"].GetInt();
	dialogue.textID = (GLubyte)entity["Dialogue"]["textID"].GetInt();
	dialogue.nextTextID = (GLubyte)entity["Dialogue"]["nextTextID"].GetInt();
	return dialogue;
}
Script SerializationManager::getScript(Value& entity)
{
	Script script;
	script.name = entity["Script"]["name"].GetString();
	//script.script = nullptr;
	return script;
}
Button SerializationManager::getButton(Value& entity)
{
	Button button;
	button.interactable = entity["Button"]["interactable"].GetBool();
	return button;
}
LightSource SerializationManager::getLightSource(Value& entity)
{
	LightSource lightSource;
	lightSource.centerOffset = GetVec2(entity["LightSource"]["centerOffset"]);
	lightSource.radius = entity["LightSource"]["radius"].GetFloat();
	lightSource.intensity = entity["LightSource"]["intensity"].GetFloat();
	return lightSource;
}
ShadowCaster SerializationManager::getShadowCaster(Value& entity)
{
	ShadowCaster shadowCaster;
	Value a(kObjectType);
	a = entity["ShadowCaster"]["centerOffsets"].GetArray();
	for (int j = 0; j < (int)a.Size(); ++j)
	{
		/*Math::Vec2 point = GetVec2(a[j]["centerOffset"]);
		point.x = (float)((int)point.x);
		point.y = (float)((int)point.y);
		shadowCaster.centerOffset.push_back(point);*/
		shadowCaster.centerOffset.push_back(GetVec2(a[j]["centerOffset"]));
	}
	shadowCaster.renderFlag = entity["ShadowCaster"]["renderFlag"].GetBool();;
	return shadowCaster;
}
Viewport SerializationManager::getViewport(Value& entity)
{
	Viewport viewport;
	viewport.isUI = entity["Viewport"]["isUI"].GetBool();
	viewport.width = entity["Viewport"]["width"].GetInt();
	return viewport;
}
MovementAI SerializationManager::getMovementAI(Value& entity)
{
	MovementAI movementAI;
	movementAI.run = entity["MovementAI"]["run"].GetBool();
	movementAI.next = entity["MovementAI"]["next"].GetBool();
	movementAI.loop = entity["MovementAI"]["loop"].GetBool();
	movementAI.reverse = entity["MovementAI"]["reverse"].GetBool();
	movementAI.cycle = entity["MovementAI"]["cycle"].GetBool();
	movementAI.nextStep = entity["MovementAI"]["nextStep"].GetInt();
	movementAI.acceleration = entity["MovementAI"]["acceleration"].GetFloat();
	if (entity["MovementAI"].HasMember("moveOnHover"))//////remove this if line
	{
		movementAI.moveOnHover = entity["MovementAI"]["moveOnHover"].GetBool();
	}
	else
		movementAI.moveOnHover = false;
	if (entity["MovementAI"].HasMember("moveScale"))//////remove this if line
	{
		movementAI.moveScale = entity["MovementAI"]["moveScale"].GetBool();
		movementAI.moveRotate = entity["MovementAI"]["moveRotate"].GetBool();
		movementAI.moveTranslation = entity["MovementAI"]["moveTranslation"].GetBool();
	}
	else
	{
		movementAI.moveScale = true;
		movementAI.moveRotate = true;
		movementAI.moveTranslation = true;
	}
	if (entity["MovementAI"].HasMember("moveOnCollide"))//////remove this if line
	{
		movementAI.moveOnCollide = entity["MovementAI"]["moveOnCollide"].GetBool();
	}
	else
		movementAI.moveOnCollide = false;
	Value a(kObjectType);
	if (entity["MovementAI"].HasMember("targets"))
	{
		a = entity["MovementAI"]["targets"].GetArray();
		for (int j = 0; j < (int)a.Size(); ++j)
		{
			float time = (float)a[j]["time"].GetFloat();
			Transform trans = getTransform(a[j]);
			movementAI.time.push_back(time);
			movementAI.targetTransforms.push_back(trans);
		}
	}

	return movementAI;
}
ColorAI SerializationManager::getColorAI(Value& entity)
{
	ColorAI colorAI;
	colorAI.run = entity["ColorAI"]["run"].GetBool();
	colorAI.next = entity["ColorAI"]["next"].GetBool();
	colorAI.loop = entity["ColorAI"]["loop"].GetBool();
	colorAI.reverse = entity["ColorAI"]["reverse"].GetBool();
	colorAI.cycle = entity["ColorAI"]["cycle"].GetBool();
	colorAI.nextStep = entity["ColorAI"]["nextStep"].GetInt();
	colorAI.acceleration = entity["ColorAI"]["acceleration"].GetFloat();
	colorAI.spriteORtext = entity["ColorAI"]["spriteORtext"].GetInt();
	colorAI.changeOnHover = entity["ColorAI"]["changeOnHover"].GetBool();
	if (entity["ColorAI"].HasMember("changeOnCollide"))//////remove this if line
	{
		colorAI.changeOnCollide = entity["ColorAI"]["changeOnCollide"].GetBool();
	}
	else
		colorAI.changeOnCollide = false;
	if (entity["ColorAI"].HasMember("changeR"))//////remove this if line
	{
		colorAI.changeR = entity["ColorAI"]["changeR"].GetBool();
		colorAI.changeG = entity["ColorAI"]["changeG"].GetBool();
		colorAI.changeB = entity["ColorAI"]["changeB"].GetBool();
		colorAI.changeA = entity["ColorAI"]["changeA"].GetBool();
	}
	else
	{
		colorAI.changeR = true;
		colorAI.changeG = true;
		colorAI.changeB = true;
		colorAI.changeA = true;
	}
	Value a(kObjectType);
	if (entity["ColorAI"].HasMember("targets"))
	{
		a = entity["ColorAI"]["targets"].GetArray();
		for (int j = 0; j < (int)a.Size(); ++j)
		{
			float time = (float)a[j]["time"].GetFloat();
			Color clr = getColor(a[j]);
			colorAI.time.push_back(time);
			colorAI.targetColors.push_back(clr);
		}
	}

	return colorAI;
}
PlayerData SerializationManager::getPlayerData(Value& entity)
{
	PlayerData playerData;
	(void)entity;
	return playerData;
}
ParticleSystem::ParticleInfo SerializationManager::getParticleInfo(Value& entity)
{
	ParticleSystem::ParticleInfo mParticleInfo;
	mParticleInfo.mScale = entity["mParticleInfo"]["mScale"].GetFloat();
	mParticleInfo.mFacing = entity["mParticleInfo"]["mFacing"].GetFloat();
	mParticleInfo.mLifespan = entity["mParticleInfo"]["mLifespan"].GetFloat();
	mParticleInfo.mScale = entity["mParticleInfo"]["mScale"].GetFloat();
	mParticleInfo.mSprite = getSprite(entity["mParticleInfo"]);
	mParticleInfo.mRotation = entity["mParticleInfo"]["mRotation"].GetFloat();
	mParticleInfo.mSpeed = entity["mParticleInfo"]["mSpeed"].GetFloat();
	mParticleInfo.mExpanding = entity["mParticleInfo"]["mExpanding"].GetFloat();
	mParticleInfo.mFading = entity["mParticleInfo"]["mFading"].GetBool();
	mParticleInfo.mFadeIn = entity["mParticleInfo"]["mFadeIn"].GetBool();
	return mParticleInfo;
}
ParticleSystem SerializationManager::getParticleSystem(Value& entity)
{
	ParticleSystem particleSystem;
	particleSystem.mParticleInfo = getParticleInfo(entity["ParticleSystem"]);
	particleSystem.mDensity = entity["ParticleSystem"]["mDensity"].GetInt();
	particleSystem.mCenter = GetVec2(entity["ParticleSystem"]["mCenter"]);
	particleSystem.mAreaWidth = entity["ParticleSystem"]["mAreaWidth"].GetFloat();
	particleSystem.mDirection = entity["ParticleSystem"]["mDirection"].GetFloat();
	particleSystem.mSpread = entity["ParticleSystem"]["mSpread"].GetFloat();
	particleSystem.mDuration = entity["ParticleSystem"]["mDuration"].GetFloat();
	particleSystem.mIsActive = entity["ParticleSystem"]["mIsActive"].GetBool();
	particleSystem.mSlow = entity["ParticleSystem"]["mSlow"].GetFloat();
	particleSystem.OriginalAlpha() = particleSystem.mParticleInfo.mSprite.color.a;
	return particleSystem;
}


/*!*****************************************************************************
\brief
	Adds a vector 2 to a rapid jason dom tree

\return
None.
*******************************************************************************/
void SerializationManager::addVectorMember(Document& scene, Value& parent, const char* name, Math::Vec2 data)
{
	Value child(kObjectType);
	child.AddMember(StringRef("X"), data.x, scene.GetAllocator());
	child.AddMember(StringRef("Y"), data.y, scene.GetAllocator());
	parent.AddMember(StringRef(name), child, scene.GetAllocator());
}
/*!*****************************************************************************
\brief
	Adds a string vector to a rapid jason dom tree as a array
\param scene
	rapid jason document to add to
\param parent
	parent onject to add to
\param name
	name of child to add to parant object
\prama data
	data to be added to the child objects

\return
None.
*******************************************************************************/
template<typename T>
void SerializationManager::addVectorArrayStrMember(Document& scene, Value& parent, const char* name, std::vector <T> data)
{
	Value child(kObjectType);
	child.SetArray();
	for (size_t i = 0; i < data.size(); ++i)
	{
		std::string tex = spriteManager->GetTexturePath(data[i]);
		Value texpath(tex.c_str(), (SizeType)tex.size(), scene.GetAllocator());
		child.PushBack(texpath, scene.GetAllocator());
	}
	parent.AddMember(StringRef(name), child, scene.GetAllocator());
}
template<typename T>
void SerializationManager::addVectorArrayForceMember(Document& scene, Value& parent, const char* name, std::vector <T> data)
{
	Value child(kObjectType);
	child.SetArray();
	for (size_t i = 0; i < data.size(); ++i)
	{
		Force force = data[i];
		Value childitems(kObjectType);

		childitems.AddMember(StringRef("lifetimeLimit"), data[i].lifetimeLimit, scene.GetAllocator());
		childitems.AddMember(StringRef("age"), data[i].age, scene.GetAllocator());
		childitems.AddMember(StringRef("isActive"), data[i].isActive, scene.GetAllocator());
		childitems.AddMember(StringRef("forceID"), data[i].forceID, scene.GetAllocator());
		Value grandchilditems(kObjectType);
		if (data[i].forceID == 0)
		{
			addVectorMember(scene, grandchilditems, "unitDirection", data[i].linearForce.unitDirection);
			grandchilditems.AddMember(StringRef("magnitude"), data[i].linearForce.magnitude, scene.GetAllocator());
		}
		else if (data[i].forceID == 1)
		{
			grandchilditems.AddMember(StringRef("torque"), data[i].rotationalForce.torque, scene.GetAllocator());
		}
		else if (data[i].forceID == 2)
		{
			grandchilditems.AddMember(StringRef("directionalDrag"), data[i].dragForce.directionalDrag, scene.GetAllocator());
			grandchilditems.AddMember(StringRef("rotationalDrag"), data[i].dragForce.rotationalDrag, scene.GetAllocator());
			childitems.AddMember(StringRef("color"), grandchilditems, scene.GetAllocator());
		}

		child.PushBack(childitems, scene.GetAllocator());
	}
	parent.AddMember(StringRef(name), child, scene.GetAllocator());


}
/*!*****************************************************************************
\brief
	Adds a vector to a rapid jason dom tree as a array
\param scene
	rapid jason document to add to
\param parent
	parent onject to add to
\param name
	name of child to add to parant object
\prama data
	data to be added to the child objects

\return
None.
*******************************************************************************/
template<typename T>
void SerializationManager::addVectorArrayMember(Document& scene, Value& parent, const char* name, std::vector <T> data)
{
	Value child(kObjectType);
	child.SetArray();
	for (size_t i = 0; i < data.size(); ++i)
	{
		child.PushBack(data.at(i), scene.GetAllocator());
	}
	parent.AddMember(StringRef(name), child, scene.GetAllocator());
}

/*!*****************************************************************************
\brief
	Adds a vector to a rapid jason dom tree as individual child objects
\param scene
	rapid jason document to add to
\param parent
	parent onject to add to
\param name
	name of child to add to parant object
\prama data
	data to be added to the child objects
\return
None.
*******************************************************************************/
template<typename T>
void SerializationManager::addVectorsMember(Document& scene, Value& parent, const char* name, std::vector <T> data)
{
	Value child(kObjectType);
	for (size_t i = 0; i < data.size(); ++i)
	{
		std::string num = std::to_string(i);
		Value number(num.c_str(), (SizeType)num.size(), scene.GetAllocator());
		child.AddMember(number, data[i], scene.GetAllocator());
	}
	parent.AddMember(StringRef(name), child, scene.GetAllocator());
}
/*!*****************************************************************************
\brief
	Saves the data in the ECS system to a json file

\return
None.
*******************************************************************************/
void SerializationManager::SaveScene(Scene& _sceneData)
{
	Document scene;
	auto& allocator = scene.GetAllocator();
	//scene.SetObject();
	scene.SetArray();
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	//int counter = 0;
	GUIDList.clear();
	for (const Entity& e : _sceneData.mEntities)
	{
		if (!e.HasComponent<General>())
			continue;
		Value entity(kObjectType);
		if (e.HasComponent<General>())
		{
			addGeneral(scene, entity, e.GetComponent<General>());
		}
		if (e.HasComponent<Lifespan>())
		{
			addLifespan(scene, entity, e.GetComponent<Lifespan>());
		}
		if (e.HasComponent<Transform>())
		{
			addTransform(scene, entity, e.GetComponent<Transform>());
		}
		if (e.HasComponent<Sprite>())
		{
			addSprite(scene, entity, e.GetComponent<Sprite>());
		}
		if (e.HasComponent<Animation>())
		{
			addAnimation(scene, entity, e.GetComponent<Animation>());
		}
		if (e.HasComponent<SheetAnimation>())
		{
			addSheetAnimation(scene, entity, e.GetComponent<SheetAnimation>());
		}
		if (e.HasComponent<Physics2D>()) {

			addPhysics2D(scene, entity, e.GetComponent<Physics2D>());
		}
		if (e.HasComponent<RectCollider>())
		{
			addRectCollider(scene, entity, e.GetComponent<RectCollider>());
		}
		if (e.HasComponent<LayerCollider>())
		{
			addLayerCollider(scene, entity, e.GetComponent<LayerCollider>());
		}
		if (e.HasComponent<CircleCollider>())
		{
			addCircleCollider(scene, entity, e.GetComponent<CircleCollider>());
		}
		if (e.HasComponent<Edge2DCollider>())
		{
			addEdge2DCollider(scene, entity, e.GetComponent<Edge2DCollider>());
		}
		if (e.HasComponent<Point2DCollider>())
		{
			addPoint2DCollider(scene, entity, e.GetComponent<Point2DCollider>());
		}
		if (e.HasComponent<Audio>())
		{
			addAudio(scene, entity, e.GetComponent<Audio>());
		}
		if (e.HasComponent<AI>())
		{
			addAI(scene, entity, e.GetComponent<AI>());
		}
		if (e.HasComponent<Text>())
		{
			addText(scene, entity, e.GetComponent<Text>());
		}
		if (e.HasComponent<Dialogue>())
		{
			addDialogue(scene, entity, e.GetComponent<Dialogue>());
		}
		if (e.HasComponent<Script>()) 
		{
			addScript(scene, entity, e.GetComponent<Script>());
		}
		if (e.HasComponent<Button>()) 
		{
			addButton(scene, entity, e.GetComponent<Button>());
		}
		if (e.HasComponent<LightSource>())
		{
			addLightSource(scene, entity, e.GetComponent<LightSource>());
		}
		if (e.HasComponent<ShadowCaster>()){
			addShadowCaster(scene, entity, e.GetComponent<ShadowCaster>());
		}
		if (e.HasComponent<Viewport>()) 
		{
			addViewport(scene, entity, e.GetComponent<Viewport>());
		}
		if (e.HasComponent<MovementAI>()) 
		{
			addMovementAI(scene, entity, e.GetComponent<MovementAI>());
		}
		if (e.HasComponent<ColorAI>())
		{
			addColorAI(scene, entity, e.GetComponent<ColorAI>());
		}
		if (e.HasComponent<PlayerData>())
		{
			addPlayerData(scene, entity, e.GetComponent<PlayerData>());
		}
		if (e.HasComponent<ParticleSystem>())
		{
			addParticleSystem(scene, entity, e.GetComponent<ParticleSystem>());
		}
		/*std::string s("Entity" + std::to_string(counter));
		Value index(s.c_str(), (SizeType)s.size(), allocator);
		scene.AddMember(index, entity, allocator);
		counter++;*/

		scene.PushBack(entity, allocator);
	}


	scene.Accept(writer);
	std::string jsonf(buffer.GetString(), buffer.GetSize());
	std::string path = "../resources/Scene/" + _sceneData.mName + ".json";

	// Back up
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "[%d%m%y-%H%M%S]_");
	std::string time = oss.str();
	std::string Backuppath = "../resources/Scene/Backup/" + time + _sceneData.mName +  ".json";
	std::ifstream ifs(path);
	if (ifs.good())
	{
		LOG_INFO(path +" backing up before saving! To : "+ Backuppath);
		std::ofstream backUpofs(Backuppath);
		std::string backupData{};
		while (getline(ifs, backupData)) {
			backUpofs << backupData << "\n";
		}

	}
	else
		LOG_ERROR("Can't backup before saving! : ");
	//
	std::ofstream ofs(path);
	ofs << jsonf;
	if (!ofs.good())
	{
		LOG_ERROR("Unable to save scene to: " + path);
	}
	else
		LOG_INFO("Saved Scene: " + path);
	ofs.close();
	SaveSceneGUID(_sceneData.mName);
	GUIDList.clear();
}
void SerializationManager::SaveSceneGUID(std::string sceneName)
{
	Document scene;
	auto& allocator = scene.GetAllocator();
	//scene.SetObject();
	scene.SetArray();
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	for (const ResourceManager::GUID g : GUIDList)
	//for(int g =0; g< GUIDList.size(); ++g)
	{
		//Value guid(g, (SizeType)g.size(), scene.GetAllocator());
		//uint64_t guid = static_cast<uint64_t>(g);
		scene.PushBack(g, allocator);
	}
	scene.Accept(writer);
	std::string jsonf(buffer.GetString(), buffer.GetSize());
	std::string path = "../resources/SceneGUID/" + sceneName + "_GUIDList.json";
	std::ofstream ofs(path);
	ofs << jsonf;
	if (!ofs.good())
	{
		LOG_ERROR("Unable to save scene to: " + path);
	}
	else
		LOG_INFO("Saved Scene: " + path);
}
/*!*****************************************************************************
\brief
	Saves each component into rapid json
\param scene
	rapid jason document to add to
\param entity
	parent onject to add to
\prama general
	data to be added to the child objects
\return
None.
*******************************************************************************/
void SerializationManager::addGeneral(Document& scene, Value& entity, General general)
{
	Value tmp(kObjectType);
	Value vname(general.name.c_str(), (SizeType)general.name.size(), scene.GetAllocator());
	tmp.AddMember(StringRef("name"), vname, scene.GetAllocator());
	tmp.AddMember(StringRef("tag"), (int)general.tag, scene.GetAllocator());
	tmp.AddMember(StringRef("subtag"), (int)general.subtag, scene.GetAllocator());
	tmp.AddMember(StringRef("isActive"), general.isActive, scene.GetAllocator());
	entity.AddMember(StringRef("General"), tmp, scene.GetAllocator());
}
void SerializationManager::addLifespan(Document& scene, Value& entity, Lifespan lifespan)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("limit"), lifespan.limit, scene.GetAllocator());
	tmp.AddMember(StringRef("lifetime"), lifespan.lifetime, scene.GetAllocator());
	entity.AddMember(StringRef("Lifespan"), tmp, scene.GetAllocator());
}
void SerializationManager::addTransform(Document& scene, Value& entity, Transform transform)
{
	Value tmp(kObjectType);
	addVectorMember(scene, tmp, "scale", transform.scale);
	tmp.AddMember(StringRef("rotation"), transform.rotation, scene.GetAllocator());
	addVectorMember(scene, tmp, "translation", transform.translation);
	entity.AddMember(StringRef("Transform"), tmp, scene.GetAllocator());
}
void SerializationManager::addColor(Document& scene, Value& entity, Color color)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("r"), (int)color.r, scene.GetAllocator());
	tmp.AddMember(StringRef("g"), (int)color.g, scene.GetAllocator());
	tmp.AddMember(StringRef("b"), (int)color.b, scene.GetAllocator());
	tmp.AddMember(StringRef("a"), (int)color.a, scene.GetAllocator());
	entity.AddMember(StringRef("color"), tmp, scene.GetAllocator());
}
void SerializationManager::addSprite(Document& scene, Value& entity, Sprite sprite)
{
	Value tmp(kObjectType);
	Value tmpc(kObjectType);
	tmpc.AddMember(StringRef("r"), (int)sprite.color.r, scene.GetAllocator());
	tmpc.AddMember(StringRef("g"), (int)sprite.color.g, scene.GetAllocator());
	tmpc.AddMember(StringRef("b"), (int)sprite.color.b, scene.GetAllocator());
	tmpc.AddMember(StringRef("a"), (int)sprite.color.a, scene.GetAllocator());
	tmp.AddMember(StringRef("color"), tmpc, scene.GetAllocator());
	tmp.AddMember(StringRef("sprite"), (int)sprite.sprite, scene.GetAllocator());
	std::string tex = spriteManager->GetTexturePath(sprite.texture);
	Value vtexture(tex.c_str(), (SizeType)tex.size(), scene.GetAllocator());
	tmp.AddMember(StringRef("texture"), vtexture, scene.GetAllocator());
	tmp.AddMember(StringRef("layer"), sprite.layer, scene.GetAllocator());
	entity.AddMember(StringRef("Sprite"), tmp, scene.GetAllocator());
	GUIDList.push_back(ResourceManager::GetInstance()->GetFileGUID(tex));
}
void SerializationManager::addAnimation(Document& scene, Value& entity, Animation animation)
{
	Value tmp(kObjectType);
	//addVectorArrayStrMember(scene, tmp, "images", animation.images);
	//tmp.AddMember(StringRef("timeToImageSwap"), animation.timeToImageSwap, scene.GetAllocator());
	Value child(kObjectType);
	child.SetArray();
	for (size_t i = 0; i < animation.sheets.size(); ++i)
	{
		Value sheets(kObjectType);
		std::string tex = spriteManager->GetTexturePath(animation.sheets[i].sheet);
		Value texpath(tex.c_str(), (SizeType)tex.size(), scene.GetAllocator());
		sheets.AddMember(StringRef("sheet"), texpath, scene.GetAllocator());
		sheets.AddMember(StringRef("frameCount"), animation.sheets[i].frameCount, scene.GetAllocator());
		sheets.AddMember(StringRef("timePerFrame"), animation.sheets[i].timePerFrame, scene.GetAllocator());
		child.PushBack(sheets, scene.GetAllocator());
		GUIDList.push_back(ResourceManager::GetInstance()->GetFileGUID(tex));
	}
	tmp.AddMember(StringRef("sheets"), child, scene.GetAllocator());
	
	
	tmp.AddMember(StringRef("currentImageIndex"), animation.currentImageIndex, scene.GetAllocator());
	entity.AddMember(StringRef("Animation"), tmp, scene.GetAllocator());
}
void SerializationManager::addSheetAnimation(Document& scene, Value& entity, SheetAnimation sheetAnimation)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("frameCount"), sheetAnimation.frameCount, scene.GetAllocator());
	tmp.AddMember(StringRef("currFrameIndex"), sheetAnimation.currFrameIndex, scene.GetAllocator());
	tmp.AddMember(StringRef("timePerFrame"), sheetAnimation.timePerFrame, scene.GetAllocator());
	tmp.AddMember(StringRef("timeToFrameSwap"), sheetAnimation.timeToFrameSwap, scene.GetAllocator());
	entity.AddMember(StringRef("SheetAnimation"), tmp, scene.GetAllocator());
}
void SerializationManager::addPhysics2D(Document& scene, Value& entity, Physics2D physics2D)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("dynamicsEnabled"), physics2D.dynamicsEnabled, scene.GetAllocator());
	tmp.AddMember(StringRef("mass"), physics2D.mass, scene.GetAllocator());
	tmp.AddMember(StringRef("inertia"), physics2D.inertia, scene.GetAllocator());
	tmp.AddMember(StringRef("restitution"), physics2D.restitution, scene.GetAllocator());
	tmp.AddMember(StringRef("friction"), physics2D.friction, scene.GetAllocator());
	tmp.AddMember(StringRef("damping"), physics2D.damping, scene.GetAllocator());

	addVectorMember(scene, tmp, "accumulatedForce", physics2D.accumulatedForce);
	addVectorMember(scene, tmp, "velocity", physics2D.velocity);
	addVectorMember(scene, tmp, "acceleration", physics2D.acceleration);

	tmp.AddMember(StringRef("angularVelocity"), physics2D.angularVelocity, scene.GetAllocator());
	tmp.AddMember(StringRef("angularTorque"), physics2D.angularTorque, scene.GetAllocator());

	addVectorArrayForceMember(scene, tmp, "forceList", physics2D.forceList);


	tmp.AddMember(StringRef("renderFlag"), physics2D.renderFlag, scene.GetAllocator());
	entity.AddMember(StringRef("Physics2D"), tmp, scene.GetAllocator());
}
void SerializationManager::addRectCollider(Document& scene, Value& entity, RectCollider rectCollider)
{
	Value tmp(kObjectType);
	addVectorMember(scene, tmp, "centerOffset", rectCollider.centerOffset);
	addVectorMember(scene, tmp, "scaleOffset", rectCollider.scaleOffset);
	tmp.AddMember(StringRef("isTrigger"), rectCollider.isTrigger, scene.GetAllocator());
	tmp.AddMember(StringRef("renderFlag"), rectCollider.renderFlag, scene.GetAllocator());
	entity.AddMember(StringRef("RectCollider"), tmp, scene.GetAllocator());
}
void SerializationManager::addLayerCollider(Document& scene, Value& entity, LayerCollider layerCollider)
{
	Value tmp(kObjectType);
	addVectorMember(scene, tmp, "centerOffset", layerCollider.centerOffset);
	addVectorMember(scene, tmp, "scaleOffset", layerCollider.scaleOffset);
	tmp.AddMember(StringRef("renderFlag"), layerCollider.renderFlag, scene.GetAllocator());
	entity.AddMember(StringRef("LayerCollider"), tmp, scene.GetAllocator());
}
void SerializationManager::addCircleCollider(Document& scene, Value& entity, CircleCollider circleCollider)
{
	Value tmp(kObjectType);
	addVectorMember(scene, tmp, "centerOffset", circleCollider.centerOffset);
	tmp.AddMember(StringRef("scaleOffset"), circleCollider.scaleOffset, scene.GetAllocator());
	tmp.AddMember(StringRef("isTrigger"), circleCollider.isTrigger, scene.GetAllocator());
	tmp.AddMember(StringRef("renderFlag"), circleCollider.renderFlag, scene.GetAllocator());
	entity.AddMember(StringRef("CircleCollider"), tmp, scene.GetAllocator());
}
void SerializationManager::addEdge2DCollider(Document& scene, Value& entity, Edge2DCollider edge2DCollider)
{
	Value tmp(kObjectType);
	addVectorMember(scene, tmp, "p0Offset", edge2DCollider.p0Offset);
	tmp.AddMember(StringRef("rotationOffset"), edge2DCollider.rotationOffset, scene.GetAllocator());
	tmp.AddMember(StringRef("scaleOffset"), edge2DCollider.scaleOffset, scene.GetAllocator());
	tmp.AddMember(StringRef("renderFlag"), edge2DCollider.renderFlag, scene.GetAllocator());
	entity.AddMember(StringRef("Edge2DCollider"), tmp, scene.GetAllocator());
}
void SerializationManager::addPoint2DCollider(Document& scene, Value& entity, Point2DCollider point2DCollider)
{
	Value tmp(kObjectType);
	addVectorMember(scene, tmp, "centerOffset", point2DCollider.centerOffset);
	tmp.AddMember(StringRef("renderFlag"), point2DCollider.renderFlag, scene.GetAllocator());
	entity.AddMember(StringRef("Point2DCollider"), tmp, scene.GetAllocator());
}
void SerializationManager::addAudio(Document& scene, Value& entity, Audio audio)
{
	Value tmp(kObjectType);
	Value vpath(audio.sound.path.c_str(), (SizeType)audio.sound.path.size(), scene.GetAllocator());
	tmp.AddMember(StringRef("path"), vpath, scene.GetAllocator());
	tmp.AddMember(StringRef("toPlay"), audio.sound.toPlay, scene.GetAllocator());
	tmp.AddMember(StringRef("volume"), audio.sound.volume, scene.GetAllocator());
	tmp.AddMember(StringRef("volumeMod"), audio.sound.volumeMod, scene.GetAllocator());
	tmp.AddMember(StringRef("pitch"), audio.sound.pitch, scene.GetAllocator());
	tmp.AddMember(StringRef("isPaused"), audio.sound.isPaused, scene.GetAllocator());
	tmp.AddMember(StringRef("isMute"), audio.sound.isMute, scene.GetAllocator());
	tmp.AddMember(StringRef("isLoop"), audio.sound.isLoop, scene.GetAllocator());
	tmp.AddMember(StringRef("isRandPitch"), audio.sound.isRandPitch, scene.GetAllocator());
	tmp.AddMember(StringRef("toPlayOnCollision"), audio.sound.toPlayOnCollision, scene.GetAllocator());
	tmp.AddMember(StringRef("isSpacial"), audio.isSpacial, scene.GetAllocator());
	tmp.AddMember(StringRef("spacialDistance"), audio.spacialDistance, scene.GetAllocator());
	tmp.AddMember(StringRef("spacialRatio"), audio.spacialRatio, scene.GetAllocator());
	entity.AddMember(StringRef("Audio"), tmp, scene.GetAllocator());
	std::string path = "\\Audio\\SFX\\" + audio.sound.path;
	GUIDList.push_back(ResourceManager::GetInstance()->GetFileGUID(path));
}
void SerializationManager::addAI(Document& scene, Value& entity, AI ai)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("colorChange"), ai.colorChange, scene.GetAllocator());
	tmp.AddMember(StringRef("movement"), ai.movement, scene.GetAllocator());
	tmp.AddMember(StringRef("speed"), ai.speed, scene.GetAllocator());
	tmp.AddMember(StringRef("range"), ai.range, scene.GetAllocator());
	entity.AddMember(StringRef("AI"), tmp, scene.GetAllocator());
}
void SerializationManager::addText(Document& scene, Value& entity, Text text)
{
	Value tmp(kObjectType);
	Value tmpc(kObjectType);
	Value vfontFile(text.fontFile.c_str(), (SizeType)text.fontFile.size(), scene.GetAllocator());
	tmp.AddMember(StringRef("fontFile"), vfontFile, scene.GetAllocator());
	Value vtext(text.text.c_str(), (SizeType)text.text.size(), scene.GetAllocator());
	tmp.AddMember(StringRef("text"), vtext, scene.GetAllocator());
	addVectorMember(scene, tmp, "offset", text.offset);
	tmp.AddMember(StringRef("scale"), text.scale, scene.GetAllocator());
	tmpc.AddMember(StringRef("r"), text.color.r, scene.GetAllocator());
	tmpc.AddMember(StringRef("g"), text.color.g, scene.GetAllocator());
	tmpc.AddMember(StringRef("b"), text.color.b, scene.GetAllocator());
	tmpc.AddMember(StringRef("a"), text.color.a, scene.GetAllocator());
	tmp.AddMember(StringRef("color"), tmpc, scene.GetAllocator());
	tmp.AddMember(StringRef("followCam"), text.followCam, scene.GetAllocator());
	tmp.AddMember(StringRef("width"), text.width, scene.GetAllocator());

	entity.AddMember(StringRef("Text"), tmp, scene.GetAllocator());
	std::string path = "\\Font\\" + text.fontFile;
	GUIDList.push_back(ResourceManager::GetInstance()->GetFileGUID(path));
}
void SerializationManager::addDialogue(Document& scene, Value& entity, Dialogue dialogue)
{
	Value tmp(kObjectType);
	//tmp.AddMember(StringRef("filename"), dialogue.filename, scene.GetAllocator());
	tmp.AddMember(StringRef("speakerID"), dialogue.speakerID, scene.GetAllocator());
	tmp.AddMember(StringRef("selectedID"), dialogue.selectedID, scene.GetAllocator());
	tmp.AddMember(StringRef("textID"), dialogue.textID, scene.GetAllocator());
	tmp.AddMember(StringRef("nextTextID"), dialogue.nextTextID, scene.GetAllocator());
	entity.AddMember(StringRef("Dialogue"), tmp, scene.GetAllocator());
	std::string path = "\\Dialogs\\" + dialogue.filename;
	GUIDList.push_back(ResourceManager::GetInstance()->GetFileGUID(path));
}
void SerializationManager::addScript(Document& scene, Value& entity, Script script)
{
	Value tmp(kObjectType);
	Value spath(script.name.c_str(), (SizeType)script.name.size(), scene.GetAllocator());
	tmp.AddMember(StringRef("name"), spath, scene.GetAllocator());
	entity.AddMember(StringRef("Script"), tmp, scene.GetAllocator());
	/*std::string path = "\\Scripts\\" + script.name;
	GUIDList.push_back(ResourceManager::GetInstance()->GetFileGUID(path));*/
}
void SerializationManager::addButton(Document& scene, Value& entity, Button button)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("interactable"), button.interactable, scene.GetAllocator());
	entity.AddMember(StringRef("Button"), tmp, scene.GetAllocator());
}
void SerializationManager::addLightSource(Document& scene, Value& entity, LightSource lightSource)
{
	Value tmp(kObjectType);
	addVectorMember(scene, tmp, "centerOffset", lightSource.centerOffset);
	tmp.AddMember(StringRef("radius"), lightSource.radius, scene.GetAllocator());
	tmp.AddMember(StringRef("intensity"), lightSource.intensity, scene.GetAllocator());
	entity.AddMember(StringRef("LightSource"), tmp, scene.GetAllocator());
}
void SerializationManager::addShadowCaster(Document& scene, Value& entity, ShadowCaster shadowCaster)
{
	Value tmp(kObjectType);
	Value child(kObjectType);
	child.SetArray();
	for (int i = 0; i < shadowCaster.centerOffset.size(); i++)
	{
		Value ctrOffset(kObjectType);
		addVectorMember(scene, ctrOffset, "centerOffset", shadowCaster.centerOffset[i]);
		child.PushBack(ctrOffset, scene.GetAllocator());
	}
	tmp.AddMember(StringRef("centerOffsets"), child, scene.GetAllocator());
	tmp.AddMember(StringRef("renderFlag"), shadowCaster.renderFlag, scene.GetAllocator());
	entity.AddMember(StringRef("ShadowCaster"), tmp, scene.GetAllocator());
}

void SerializationManager::addViewport(Document& scene, Value& entity, Viewport Viewport)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("isUI"), Viewport.isUI, scene.GetAllocator());
	tmp.AddMember(StringRef("width"), Viewport.width, scene.GetAllocator());
	entity.AddMember(StringRef("Viewport"), tmp, scene.GetAllocator());
}
void SerializationManager::addMovementAI(Document& scene, Value& entity, MovementAI movementAI)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("run"), movementAI.run, scene.GetAllocator());
	tmp.AddMember(StringRef("next"), movementAI.next, scene.GetAllocator());
	tmp.AddMember(StringRef("loop"), movementAI.loop, scene.GetAllocator());
	tmp.AddMember(StringRef("reverse"), movementAI.reverse, scene.GetAllocator());
	tmp.AddMember(StringRef("cycle"), movementAI.cycle, scene.GetAllocator());
	tmp.AddMember(StringRef("nextStep"), movementAI.nextStep, scene.GetAllocator());
	tmp.AddMember(StringRef("acceleration"), movementAI.acceleration, scene.GetAllocator());
	tmp.AddMember(StringRef("moveOnHover"), movementAI.moveOnHover, scene.GetAllocator());
	tmp.AddMember(StringRef("moveOnCollide"), movementAI.moveOnCollide, scene.GetAllocator());
	tmp.AddMember(StringRef("moveScale"), movementAI.moveScale, scene.GetAllocator());
	tmp.AddMember(StringRef("moveRotate"), movementAI.moveRotate, scene.GetAllocator());
	tmp.AddMember(StringRef("moveTranslation"), movementAI.moveTranslation, scene.GetAllocator());
	Value child(kObjectType);
	child.SetArray();
	for (int i = 0; i < movementAI.targetTransforms.size(); ++i)
	{
		Value trans(kObjectType);
		trans.AddMember(StringRef("time"), movementAI.time[i], scene.GetAllocator());
		addTransform(scene, trans, movementAI.targetTransforms[i]);
		child.PushBack(trans, scene.GetAllocator());
	}
	tmp.AddMember(StringRef("targets"), child, scene.GetAllocator());
	entity.AddMember(StringRef("MovementAI"), tmp, scene.GetAllocator());
}
void SerializationManager::addColorAI(Document& scene, Value& entity, ColorAI colorAI)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("run"), colorAI.run, scene.GetAllocator());
	tmp.AddMember(StringRef("next"), colorAI.next, scene.GetAllocator());
	tmp.AddMember(StringRef("loop"), colorAI.loop, scene.GetAllocator());
	tmp.AddMember(StringRef("reverse"), colorAI.reverse, scene.GetAllocator());
	tmp.AddMember(StringRef("cycle"), colorAI.cycle, scene.GetAllocator());
	tmp.AddMember(StringRef("nextStep"), colorAI.nextStep, scene.GetAllocator());
	tmp.AddMember(StringRef("acceleration"), colorAI.acceleration, scene.GetAllocator());
	tmp.AddMember(StringRef("spriteORtext"), colorAI.spriteORtext, scene.GetAllocator());
	tmp.AddMember(StringRef("changeOnHover"), colorAI.changeOnHover, scene.GetAllocator());
	tmp.AddMember(StringRef("changeOnCollide"), colorAI.changeOnCollide, scene.GetAllocator());
	tmp.AddMember(StringRef("changeR"), colorAI.changeR, scene.GetAllocator());
	tmp.AddMember(StringRef("changeG"), colorAI.changeG, scene.GetAllocator());
	tmp.AddMember(StringRef("changeB"), colorAI.changeB, scene.GetAllocator());
	tmp.AddMember(StringRef("changeA"), colorAI.changeA, scene.GetAllocator());
	Value child(kObjectType);
	child.SetArray();
	for (int i = 0; i < colorAI.targetColors.size(); ++i)
	{
		Value trans(kObjectType);
		trans.AddMember(StringRef("time"), colorAI.time[i], scene.GetAllocator());
		addColor(scene, trans, colorAI.targetColors[i]);
		child.PushBack(trans, scene.GetAllocator());
	}
	tmp.AddMember(StringRef("targets"), child, scene.GetAllocator());
	entity.AddMember(StringRef("ColorAI"), tmp, scene.GetAllocator());
}
void SerializationManager::addPlayerData(Document& scene, Value& entity, PlayerData playerData)
{
	Value tmp(kObjectType);
	
	entity.AddMember(StringRef("PlayerData"), tmp, scene.GetAllocator());
}
void SerializationManager::addParticleInfo(Document& scene, Value& entity, ParticleSystem::ParticleInfo particleInfo)
{
	Value tmp(kObjectType);
	tmp.AddMember(StringRef("mScale"), particleInfo.mScale, scene.GetAllocator());
	tmp.AddMember(StringRef("mFacing"), particleInfo.mFacing, scene.GetAllocator());
	tmp.AddMember(StringRef("mLifespan"), particleInfo.mLifespan, scene.GetAllocator());
	addSprite(scene, tmp, particleInfo.mSprite);
	tmp.AddMember(StringRef("mRotation"), particleInfo.mRotation, scene.GetAllocator());
	tmp.AddMember(StringRef("mSpeed"), particleInfo.mSpeed, scene.GetAllocator());
	tmp.AddMember(StringRef("mExpanding"), particleInfo.mExpanding, scene.GetAllocator());
	tmp.AddMember(StringRef("mFading"), particleInfo.mFading, scene.GetAllocator());
	tmp.AddMember(StringRef("mFadeIn"), particleInfo.mFadeIn, scene.GetAllocator());
	entity.AddMember(StringRef("mParticleInfo"), tmp, scene.GetAllocator());
}
void SerializationManager::addParticleSystem(Document& scene, Value& entity, ParticleSystem particleSystem)
{
	Value tmp(kObjectType);
	addParticleInfo(scene, tmp, particleSystem.mParticleInfo);
	tmp.AddMember(StringRef("mDensity"), particleSystem.mDensity, scene.GetAllocator());
	addVectorMember(scene, tmp, "mCenter", particleSystem.mCenter);
	tmp.AddMember(StringRef("mAreaWidth"), particleSystem.mAreaWidth, scene.GetAllocator());
	tmp.AddMember(StringRef("mDirection"), particleSystem.mDirection, scene.GetAllocator());
	tmp.AddMember(StringRef("mSpread"), particleSystem.mSpread, scene.GetAllocator());
	tmp.AddMember(StringRef("mDuration"), particleSystem.mDuration, scene.GetAllocator());
	tmp.AddMember(StringRef("mIsActive"), particleSystem.mIsActive, scene.GetAllocator());
	tmp.AddMember(StringRef("mSlow"), particleSystem.mSlow, scene.GetAllocator());
	entity.AddMember(StringRef("ParticleSystem"), tmp, scene.GetAllocator());


}




/*!*****************************************************************************
\brief
	Load the saved gamestate data
\param _gameState
	gameState to load to
\param _filename
   path to load from
\return
None.
*******************************************************************************/
void SerializationManager::LoadGameState(GameState& _gameState, std::filesystem::path _filename)
{
	//GameState gameStateData{};
	//std::vector<std::string> scenefilename{};
	//std::string path = "../resources/GameStates/" + _filename + ".json";
	//std::ifstream ifs(path);

	std::ifstream ifs(_filename.string());
	if (!ifs.good())
	{
		LOG_ERROR("Can't open json file! : " + _filename.stem().string());
		return;
	}
	else
		LOG_INFO("Opening game state: " + _filename.stem().string());
	_gameState.mName = _filename.stem().string();

	int same = 0;
	//for (std::string s : allgameStateFilename)
	for (GameState& g : GameStateManager::GetInstance()->mGameStates)
	{
		if (g.mName == _gameState.mName)
			same++;
	}
	allgameStateFilename.push_back(_gameState.mName);
	if (same > 1)//gamestate already open
	{
		same = 0; //count number of open GS
		for (std::string n : allgameStateFilename)
		{
			if (n == _gameState.mName)
				same++;
		}
		_gameState.mName += (" (" + std::to_string(same) + ")");
	}

	gameStateFilename = _gameState.mName;
	std::stringstream contents;
	contents << ifs.rdbuf();
	Document doc;
	doc.Parse(contents.str().c_str());

	Value entity(kArrayType);
	entity = doc.GetArray();
	for (rapidjson::SizeType index = 0; index < entity.Size(); ++index)
	{
		Scene sceneData(entity[index]["SceneName"].GetString());
		//sceneData.mName = entity[index]["SceneName"].GetString();
		if (entity[index].HasMember("Transform"))
		{
			_gameState.mCamera = getTransform(entity[index]);
			sceneData.mIsUI = false;
		}
		else
		{
			sceneData.mIsUI = true;
		}
		sceneData.mIsPause = entity[index]["isActive"].GetBool();
		if(entity[index].HasMember("mForceRender"))
			sceneData.mForceRender = entity[index]["mForceRender"].GetBool();
		sceneData.mLayer = entity[index]["layer"].GetInt();
		sceneData.mOrder = entity[index]["order"].GetInt();


		_gameState.mScenes.push_back(sceneData);
	}
	ifs.close();
	//return gameStateData;
}
/*!*****************************************************************************
\brief
	Save the gamestate data
\param _gameState
	gameState to save from
\return
None.
*******************************************************************************/
void SerializationManager::SaveGameState(GameState& _gameState)
{

	//scave each scene
	for (int s = 0; s < _gameState.mScenes.size(); s++)
	{
		SaveScene(_gameState.mScenes[s]);
		LOG_INFO("Saved a scene");
	}
	Document gamestate;
	auto& allocator = gamestate.GetAllocator();
	//scene.SetObject();
	gamestate.SetArray();
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	//int counter = 0;
	for (int s = 0; s < _gameState.mScenes.size(); s++)
	{

		Value scene(kObjectType);
		Value sceneName(_gameState.mScenes[s].mName.c_str(), (SizeType)_gameState.mScenes[s].mName.size(), allocator);
		scene.AddMember(StringRef("SceneName"), sceneName, gamestate.GetAllocator());
		if(!_gameState.mScenes[s].mIsUI)
			addTransform(gamestate, scene, _gameState.mCamera);
		scene.AddMember(StringRef("isActive"), _gameState.mScenes[s].mIsPause, gamestate.GetAllocator());
		scene.AddMember(StringRef("mForceRender"), _gameState.mScenes[s].mForceRender, gamestate.GetAllocator());
		scene.AddMember(StringRef("layer"), _gameState.mScenes[s].mLayer, gamestate.GetAllocator());
		scene.AddMember(StringRef("order"), _gameState.mScenes[s].mOrder, gamestate.GetAllocator());
		gamestate.PushBack(scene, allocator);
	}

	gamestate.Accept(writer);
	std::string jsonf(buffer.GetString(), buffer.GetSize());
	std::string path = "../resources/GameStates/" + _gameState.mName + ".json";

	// Back up
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "[%d%m%y-%H%M%S]_");
	std::string time = oss.str();
	std::string Backuppath = "../resources/GameStates/Backup/" + time + _gameState.mName  + ".json";
	std::ifstream ifs(path);
	if (ifs.good())
	{
		LOG_INFO(path + " backing up before saving! To : " + Backuppath);
		std::ofstream backUpofs(Backuppath);
		std::string backupData{};
		while (getline(ifs, backupData)) {
			backUpofs << backupData << "\n";
		}

	}
	else
		LOG_ERROR("Can't backup before saving! : ");
	//

	std::ofstream ofs(path);

	ofs << jsonf;
	if (!ofs.good())
	{
		LOG_ERROR("Unable to save Game State to: " + path);
	}
	else
		LOG_INFO("Saved Game State: " + path);



}



/*!*****************************************************************************
\brief
	Loads the dialogs from a json file to the dialog manager

\return
None.
*******************************************************************************/
void SerializationManager::LoadDialogs(std::string _filename)
{
	std::ifstream ifs("../resources/Dialogs/" + _filename + ".json");
	if (!ifs.good())
		LOG_ERROR("Can't open " + _filename + ".json file!");
	else
	{
		LOG_INFO("Opened dialog json file: " + _filename);

		dialogueFilename = _filename;
		alldialogueFilename.push_back(_filename);

		std::stringstream contents;
		contents << ifs.rdbuf();
		Document json;
		json.Parse(contents.str().c_str());
		dialogManager->Clear();
		int dialogId;
		Dialog dialog;
		Value dialogObj(kArrayType);
		dialogObj = json.GetArray();
		for (rapidjson::SizeType index = 0; index < dialogObj.Size(); ++index) {
			dialogId = dialogObj[index]["Dialogue ID"].GetInt();
			dialog.text = dialogObj[index]["Dialogue Text"].GetString();
			dialog.speaker = dialogObj[index]["Speaker"].GetInt();
			dialog.next = dialogObj[index]["Next Dialogue"].GetInt();
			dialog.next2 = dialogObj[index]["Next Dialogue(2)"].GetInt();
			dialogManager->LoadDialog(dialogId, dialog);
		}
	}
}
/*!*****************************************************************************
\brief
	Saves the data in the dialog manager to a json file

\return
None.
*******************************************************************************/
void SerializationManager::SaveDialogs(std::string _filename)
{
	Document json;
	auto& allocator = json.GetAllocator();
	json.SetArray();
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	Value temp(kObjectType);
	for (std::pair<int, Dialog> dialog : dialogManager->GetDialogs()) {
		Value dialogObj(kObjectType);
		std::string text = dialog.second.text;
		Value dText(text.c_str(), (SizeType)text.size(), allocator);
		dialogObj.AddMember(StringRef("Dialogue ID"), (int)dialog.first, allocator);
		dialogObj.AddMember(StringRef("Dialogue Text"), dText, allocator);
		dialogObj.AddMember(StringRef("Speaker"), (int)dialog.second.speaker, allocator);
		dialogObj.AddMember(StringRef("Next Dialogue"), (int)dialog.second.next, allocator);
		dialogObj.AddMember(StringRef("Next Dialogue(2)"), (int)dialog.second.next2, allocator);
		json.PushBack(dialogObj, allocator);
	}
	json.Accept(writer);
	std::string jsonf(buffer.GetString(), buffer.GetSize());
	std::string path = "../resources/Dialogs/" + _filename + ".json";
	std::ofstream ofs(path);
	ofs << jsonf;
	if (!ofs.good()) LOG_ERROR("Unable to save Dialogs file to: " + path);
	else LOG_INFO("Saved Dialogs file: " + path);
}





/*!*****************************************************************************
\brief
	Loads the PlayerData from a json file to the dialog manager

\return
None.
*******************************************************************************/
void SerializationManager::LoadPlayerData(std::string _filename)
{
	PlayerData playerData;
	std::ifstream ifs("../resources/PlayerData/" + _filename + ".json");
	if (!ifs.good())
	{
		LOG_ERROR("New Player  " + _filename + "!");
		playerData.name = _filename;
		playerDataManager->SetPlayerData(playerData);
	}
	else
	{
		LOG_INFO("Opened PlayerData json file: " + _filename);


		std::stringstream contents;
		contents << ifs.rdbuf();
		Document json;
		json.Parse(contents.str().c_str());		
		
		Value playerDataObj(kObjectType);
		playerData.name = playerDataObj["name"].GetString();
		playerData.levelAt = playerDataObj["levelAt"].GetInt();
		playerData.data1 = GetVectorInt(playerDataObj["data1"]);
		playerData.data1 = GetVectorInt(playerDataObj["data2"]);
		playerDataManager->SetPlayerData(playerData);
	}
}
/*!*****************************************************************************
\brief
	Saves the data in the PlayerData manager to a json file

\return
None.
*******************************************************************************/
void SerializationManager::SavePlayerData(std::string _filename)
{
	Document json;
	auto& allocator = json.GetAllocator();
	json.SetArray();
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	PlayerData playerdata = playerDataManager->GetPlayerData();

	Value playerDataObj(kObjectType);
	std::string text = playerdata.name;
	Value dText(text.c_str(), (SizeType)text.size(), allocator);
	playerDataObj.AddMember(StringRef("name"), dText, allocator);
	playerDataObj.AddMember(StringRef("levelAt"), playerdata.levelAt, allocator);
	addVectorArrayMember(json, playerDataObj, "data1", playerdata.data1);
	addVectorArrayMember(json, playerDataObj, "data2", playerdata.data2);
	json.Accept(writer);
	std::string jsonf(buffer.GetString(), buffer.GetSize());
	std::string path = "../resources/PlayerData/" + _filename + ".json";
	std::ofstream ofs(path);
	ofs << jsonf;
	if (!ofs.good()) LOG_ERROR("Unable to save PlayerData file to: " + path);
	else LOG_INFO("Saved PlayerData file: " + path);
}


/*!*****************************************************************************
\brief
	Load the saved prefab data
\param _filename
	filename to load from
\return
None.
*******************************************************************************/
void SerializationManager::LoadPrefab(std::string _filename)
{
	LOG_INFO("Loading prefab: " + _filename);
	std::string path = "../resources/Prefabs/" + _filename + ".json";
	std::ifstream ifs(path);
	//std::ifstream ifs(filename);
	if (!ifs.good())
	{
		LOG_ERROR("Can't open json file! : " + path);
		return;
	}
	else
		LOG_INFO("Loading prefab: " + path);

	prefabFilename = _filename;
	allprefabFilename.push_back(_filename);

	std::stringstream contents;
	contents << ifs.rdbuf();
	Document doc;
	doc.Parse(contents.str().c_str());

	if (!doc.HasMember("General"))
		return;

	for (auto p : PrefabManager::GetInstance()->GetPrefab())
	{
		if (p->Name() == _filename)
		{
			LOG_INFO("Prefab already Loaded");
			return;
		}
	}

	PrefabManager::PrefabPtr p = PrefabManager::GetInstance()->CreatePrefab();
	p->Name() = _filename;

	if (doc.HasMember("General"))
	{
		//std::string name = doc["General"]["name"].GetString();   /REMOVEME
		//bool isActive = doc["General"]["isActive"].GetBool();
		//int tag = doc["General"]["tag"].GetInt();
		//int subtag = doc["General"]["subtag"].GetInt();
		////p->AddComponent<General>(General{ name, (TAG)tag ,(SUBTAG)subtag, isActive }); ///Causes memory leaks
		p->UpdateComponent<General>(getGeneral(doc));
	}
	if (doc.HasMember("Lifespan"))
	{
		p->AddComponent<Lifespan>(getLifespan(doc));
	}
	if (doc.HasMember("Transform"))
	{
		p->AddComponent<Transform>(getTransform(doc));
	}
	if (doc.HasMember("Sprite"))
	{
		p->AddComponent<Sprite>(getSprite(doc));
	}
	if (doc.HasMember("Animation"))
	{
		p->AddComponent<Animation>(getAnimation(doc));
	}
	if (doc.HasMember("SheetAnimation"))
	{
		p->AddComponent<SheetAnimation>(getSheetAnimation(doc));
	}
	if (doc.HasMember("Physics2D"))
	{
		p->AddComponent<Physics2D>(getPhysics2D(doc));
	}
	if (doc.HasMember("RectCollider"))
	{
		p->AddComponent<RectCollider>(getRectCollider(doc));
	}
	if (doc.HasMember("CircleCollider"))
	{
		p->AddComponent<CircleCollider>(getCircleCollider(doc));
	}
	if (doc.HasMember("Edge2DCollider"))
	{
		p->AddComponent<Edge2DCollider>(getEdge2DCollider(doc));
	}
	if (doc.HasMember("Point2DCollider"))
	{
		p->AddComponent<Point2DCollider>(getPoint2DCollider(doc));
	}
	if (doc.HasMember("Audio"))
	{
		p->AddComponent<Audio>(getAudio(doc));
	}
	if (doc.HasMember("AI"))
	{
		p->AddComponent<AI>(getAI(doc));
	}
	if (doc.HasMember("Text"))
	{
		p->AddComponent<Text>(getText(doc));
	}
	if (doc.HasMember("Dialogue")) {
		p->AddComponent<Dialogue>(getDialogue(doc));
	}
	if (doc.HasMember("LightSource")) {
		p->AddComponent<LightSource>(getLightSource(doc));
	}
	if (doc.HasMember("Script")) {
		p->AddComponent<Script>(getScript(doc));
	}

	ifs.close();
}

/*!*****************************************************************************
\brief
	save the prefab data
\param _filename
	filename to save to
\return
None.
*******************************************************************************/
void SerializationManager::SavePrefab(std::string _filename)
{
	PrefabManager::PrefabPtr p = PrefabManager::GetInstance()->GetPrefab(_filename);
	Document prefab;
	//auto& allocator = prefab.GetAllocator();
	prefab.SetObject();

	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	if (p->HasComponent<General>())
	{
		addGeneral(prefab, prefab, p->GetComponent<General>());
	}
	if (p->HasComponent<Lifespan>())
	{
		addLifespan(prefab, prefab, p->GetComponent<Lifespan>());
	}
	if (p->HasComponent<Transform>())
	{
		addTransform(prefab, prefab, p->GetComponent<Transform>());
	}
	if (p->HasComponent<Sprite>())
	{
		addSprite(prefab, prefab, p->GetComponent<Sprite>());
	}
	if (p->HasComponent<Animation>())
	{
		addAnimation(prefab, prefab, p->GetComponent<Animation>());
	}
	if (p->HasComponent<SheetAnimation>())
	{
		addSheetAnimation(prefab, prefab, p->GetComponent<SheetAnimation>());
	}
	if (p->HasComponent<Physics2D>()) {

		addPhysics2D(prefab, prefab, p->GetComponent<Physics2D>());
	}
	if (p->HasComponent<RectCollider>())
	{
		addRectCollider(prefab, prefab, p->GetComponent<RectCollider>());
	}
	if (p->HasComponent<CircleCollider>())
	{
		addCircleCollider(prefab, prefab, p->GetComponent<CircleCollider>());
	}
	if (p->HasComponent<Edge2DCollider>())
	{
		addEdge2DCollider(prefab, prefab, p->GetComponent<Edge2DCollider>());
	}
	if (p->HasComponent<Point2DCollider>())
	{
		addPoint2DCollider(prefab, prefab, p->GetComponent<Point2DCollider>());
	}
	if (p->HasComponent<Audio>())
	{
		addAudio(prefab, prefab, p->GetComponent<Audio>());
	}
	if (p->HasComponent<AI>())
	{
		addAI(prefab, prefab, p->GetComponent<AI>());
	}
	if (p->HasComponent<Text>()) {

		addText(prefab, prefab, p->GetComponent<Text>());
	}
	if (p->HasComponent<Dialogue>()) {

		addDialogue(prefab, prefab, p->GetComponent<Dialogue>());
	}
	if (p->HasComponent<LightSource>()) {

		addLightSource(prefab, prefab, p->GetComponent<LightSource>());
	}
	if (p->HasComponent<Script>()) {

		addScript(prefab, prefab, p->GetComponent<Script>());
	}

	prefab.Accept(writer);
	std::string jsonf(buffer.GetString(), buffer.GetSize());
	std::string path = "../resources/Prefabs/" + _filename + ".json";
	std::ofstream ofs(path);
	ofs << jsonf;
	if (!ofs.good())
	{
		LOG_ERROR("Unable to save prefab to: " + path);
	}
	else
		LOG_INFO("Saved Prefab: " + path);
}







void SerializationManager::GetGUIDList(std::filesystem::path _filename, std::set<ResourceManager::GUID>& sGUIDList)
{
	std::ifstream ifs(_filename.string());
	if (!ifs.good())
	{
		LOG_ERROR("Can't open json file! : " + _filename.stem().string());
		return;
	}
	else
		LOG_INFO("Opening game state: " + _filename.stem().string());
	
	std::stringstream contents;
	contents << ifs.rdbuf();
	Document doc;
	doc.Parse(contents.str().c_str());

	Value entity(kArrayType);
	entity = doc.GetArray();
	std::string sceneName;
	for (rapidjson::SizeType index = 0; index < entity.Size(); ++index)
	{
		sceneName = entity[index]["SceneName"].GetString();
		std::string path = "../resources/SceneGUID/" + sceneName + "_GUIDList.json";
		std::ifstream ifsGUID(path);
		if (!ifsGUID.good())
		{
			LOG_ERROR("Can't open GUID json file! : " + sceneName);
			return;
		}
		else
			LOG_INFO("Opening SceneGUID: " + sceneName);

		std::stringstream contentsGUID;
		contentsGUID << ifsGUID.rdbuf();
		Document docGUID;
		docGUID.Parse(contentsGUID.str().c_str());
		Value guid(kArrayType);
		guid = docGUID.GetArray();
		for (rapidjson::SizeType indx = 0; indx < guid.Size(); ++indx)
		{
			sGUIDList.insert(guid[indx].GetInt64());
		}
		ifsGUID.close();
	}
	ifs.close();

}








/*!*****************************************************************************
\brief
	Loaded file name getters
\return
None.
*******************************************************************************/
std::string SerializationManager::GetSceneFilename()
{
	return sceneFilename;
}
std::string SerializationManager::GetGameStateFilename()
{
	return gameStateFilename;
}
std::string SerializationManager::GetPrefabFilename()
{
	return prefabFilename;
}
std::string SerializationManager::GetDialogueFilename()
{
	return dialogueFilename;
}
std::vector<std::string> SerializationManager::GetAllsceneFilename()
{
	return allsceneFilename;
}
std::vector<std::string> SerializationManager::GetAllgameStateFilename()
{
	return allgameStateFilename;
}
std::vector<std::string> SerializationManager::GetAllprefabFilename()
{
	return allprefabFilename;
}
std::vector<std::string> SerializationManager::GetAlldialogueFilename()
{
	return alldialogueFilename;
}

























//
//SceneData SerializationManager::LoadSceneData(ResourceManager::GUID const& _guid) {
//	return LoadSceneData(ResourceManager::GetInstance()->GetFilePath(_guid));
//}
//
//
//SceneData SerializationManager::LoadSceneData(std::string const& _filePath) {
//	std::ifstream ifs(_filePath);
//	if (!ifs.good()) {
//		LOG_ERROR("Can't open scene file: " + _filePath);
//		return SceneData();
//	} else LOG_INFO("Opening scene file: " + _filePath);
//	std::stringstream contents;
//	contents << ifs.rdbuf();
//	Document doc;
//	doc.Parse(contents.str().c_str());
//
//	//std::cout << contents.str() << '\n';
//	SceneData sceneData{};
//
//	// Getting scene data from json
//	std::string sceneDataName{ "SceneData" };
//	Value sceneDataJson(sceneDataName.c_str(), (rapidjson::SizeType)sceneDataName.size(), doc.GetAllocator());
//	if (!doc.HasMember(sceneDataJson))
//	{
//		std::cout << "No sceneData found in scene file!\n";
//		sceneData.isActive = true;
//	}
//	else
//	{
//		Value sceneDataValue(kObjectType);
//		sceneDataValue = doc[sceneDataJson];
//		sceneData.isActive = sceneDataValue["isActive"].GetBool();
//	}
//	int i = 0;
//	int runOnce{};
//	for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr <= doc.MemberEnd(); ++itr) {
//		if (!runOnce++) continue;
//
//		Entity e{ ECS::CreateEntity() };
//		Value entity(kObjectType);
//		std::string str("Entity" + std::to_string(i));
//		Value index(str.c_str(), (SizeType)str.size(), doc.GetAllocator());
//		if (!doc.HasMember(index))
//		{
//			LOG_INFO("1 Enitity not loaded");
//
//			continue;
//		}
//		entity = doc[index];
//
//		if (entity.HasMember("General")) {
//			std::string name = entity["General"]["name"].GetString();
//			bool isActive = entity["General"]["isActive"].GetBool();
//			int tag = entity["General"]["tag"].GetInt();
//			int subtag = entity["General"]["subtag"].GetInt();
//			bool isPaused = sceneData.isActive;
//			e.AddComponent<General>(General{ name, (TAG)tag ,(SUBTAG)subtag, isActive, isPaused });
//			if (tag == (int)TAG::PLAYER)
//				e.AddComponent(PlayerTmp{ 0 });
//		}
//		if (entity.HasMember("Lifespan")) {
//			float lifetime = entity["Lifespan"]["lifetime"].GetFloat();
//			float limit = entity["Lifespan"]["limit"].GetFloat();
//			e.AddComponent<Lifespan>({ lifetime, limit });
//		}
//		if (entity.HasMember("Transform")) {
//			Math::Vec2 s = GetVec2(entity["Transform"]["scale"]);
//			float r = (float)entity["Transform"]["rotation"].GetFloat();
//			Math::Vec2 t = GetVec2(entity["Transform"]["translation"]);
//
//			e.AddComponent<Transform>({ s, r, t });
//		}
//		if (entity.HasMember("Sprite")) {
//			Color c;
//			c.r = (GLubyte)entity["Sprite"]["color"]["r"].GetInt();
//			c.g = (GLubyte)entity["Sprite"]["color"]["g"].GetInt();
//			c.b = (GLubyte)entity["Sprite"]["color"]["b"].GetInt();
//			c.a = (GLubyte)entity["Sprite"]["color"]["a"].GetInt();
//			SPRITE s = (SPRITE)entity["Sprite"]["sprite"].GetInt();
//			GLuint t = (GLuint)spriteManager->GetTextureID(entity["Sprite"]["texture"].GetString());
//			//GLuint t = (GLuint)entity["Sprite"]["texture"].GetInt();
//			int l = entity["Sprite"]["layer"].GetInt();
//			e.AddComponent<Sprite>({ c, s, t ,l });
//		}
//		if (entity.HasMember("Animation")) {
//			std::vector<GLuint> images;
//			Value a(kObjectType);
//			a = entity["Animation"]["images"].GetArray();
//			for (int j = 0; j < (int)a.Size(); ++j) {
//				GLuint tex = spriteManager->GetTextureID(a[j].GetString());
//				images.push_back(tex);
//			}
//			float timePerImage = entity["Animation"]["timePerImage"].GetFloat();
//			float timeToImageSwap = entity["Animation"]["timeToImageSwap"].GetFloat();
//			int currentImageIndex = entity["Animation"]["currentImageIndex"].GetInt();
//			e.AddComponent<Animation>({ images , timePerImage , timeToImageSwap , currentImageIndex });
//		}
//		if (entity.HasMember("SheetAnimation")) {
//			short frameCount = (short)entity["SheetAnimation"]["frameCount"].GetInt();
//			short currentImageIndex = (short)entity["SheetAnimation"]["currFrameIndex"].GetInt();
//			float timePerImage = entity["SheetAnimation"]["timePerFrame"].GetFloat();
//			float timeToImageSwap = entity["SheetAnimation"]["timeToFrameSwap"].GetFloat();
//			e.AddComponent<SheetAnimation>({ frameCount , currentImageIndex , timePerImage , timeToImageSwap });
//		}
//		if (entity.HasMember("Physics2D")) {
//
//			bool dynamicsEnabled = entity["Physics2D"]["dynamicsEnabled"].GetBool();
//			float mass = entity["Physics2D"]["mass"].GetFloat();
//			float inertia = entity["Physics2D"]["inertia"].GetFloat();
//			float restitution = entity["Physics2D"]["restitution"].GetFloat();
//			float friction = entity["Physics2D"]["friction"].GetFloat();
//			float damping = entity["Physics2D"]["damping"].GetFloat();
//
//			Math::Vec2 accumulatedForce = GetVec2(entity["Physics2D"]["accumulatedForce"]);
//			Math::Vec2 velocity = GetVec2(entity["Physics2D"]["velocity"]);
//			Math::Vec2 acceleration = GetVec2(entity["Physics2D"]["acceleration"]);
//
//			float angularVelocity = entity["Physics2D"]["angularVelocity"].GetFloat();
//			float angularTorque = entity["Physics2D"]["angularTorque"].GetFloat();
//
//			//vect force
//			std::vector<Force> forceList{};
//			Value a(kObjectType);
//			a = entity["Physics2D"]["forceList"].GetArray();
//			for (int j = 0; j < (int)a.Size(); ++j)
//			{
//				Force force{};
//				Value f(kObjectType);
//				f = a[j].GetObject();
//				force.lifetimeLimit = f["lifetimeLimit"].GetDouble();
//				force.age = f["age"].GetDouble();
//				force.isActive = f["isActive"].GetBool();
//				force.forceID = f["forceID"].GetInt();
//				if (force.forceID == 0)
//				{
//					force.linearForce.unitDirection = GetVec2(f["linearForce"]["unitDirection"]);
//					force.linearForce.magnitude = f["linearForce"]["magnitude"].GetFloat();
//				}
//				else if (force.forceID == 1)
//				{
//					force.rotationalForce.torque = f["rotationalForce"]["torque"].GetFloat();
//				}
//				else if (force.forceID == 2)
//				{
//					force.dragForce.directionalDrag = f["dragForce"]["directionalDrag"].GetFloat();
//					force.dragForce.rotationalDrag = f["dragForce"]["rotationalDrag"].GetFloat();
//				}
//
//				forceList.push_back(force);
//			}
//
//			bool renderFlag = entity["Physics2D"]["renderFlag"].GetBool();
//
//			e.AddComponent<Physics2D>({ dynamicsEnabled, mass, inertia, restitution, friction, damping, accumulatedForce,velocity, acceleration, angularVelocity, angularTorque, forceList, renderFlag });
//		}
//		if (entity.HasMember("RectCollider"))
//		{
//			Math::Vec2 centerOffset = GetVec2(entity["RectCollider"]["centerOffset"]);
//			Math::Vec2	scaleOffset = GetVec2(entity["RectCollider"]["scaleOffset"]);
//			bool renderFlag = entity["RectCollider"]["renderFlag"].GetBool();
//			e.AddComponent<RectCollider>({ centerOffset , scaleOffset , renderFlag });
//		}
//		if (entity.HasMember("CircleCollider"))
//		{
//			Math::Vec2 centerOffset = GetVec2(entity["CircleCollider"]["centerOffset"]);
//			float scaleOffset = entity["CircleCollider"]["scaleOffset"].GetFloat();
//			bool renderFlag = entity["CircleCollider"]["renderFlag"].GetBool();
//			e.AddComponent<CircleCollider>({ centerOffset , scaleOffset , renderFlag });
//		}
//		if (entity.HasMember("Edge2DCollider"))
//		{
//			Math::Vec2 p0Offset = GetVec2(entity["Edge2DCollider"]["p0Offset"]);
//			float rotationOffset = entity["Edge2DCollider"]["rotationOffset"].GetFloat();
//			float scaleOffset = entity["Edge2DCollider"]["scaleOffset"].GetFloat();
//			bool renderFlag = entity["Edge2DCollider"]["renderFlag"].GetBool();
//
//			e.AddComponent<Edge2DCollider>({ p0Offset ,rotationOffset, scaleOffset , renderFlag });
//		}
//		if (entity.HasMember("Point2DCollider"))
//		{
//			Math::Vec2 centerOffset = GetVec2(entity["Point2DCollider"]["centerOffset"]);
//			bool renderFlag = entity["Point2DCollider"]["renderFlag"].GetBool();
//			e.AddComponent<Point2DCollider>({ centerOffset ,renderFlag });
//		}
//		if (entity.HasMember("Audio"))
//		{
//			Sound sound;
//			sound.path = entity["Audio"]["path"].GetString();
//			sound.volume = entity["Audio"]["volume"].GetFloat();
//			sound.volumeMod = entity["Audio"]["volumeMod"].GetFloat();
//			sound.pitch = entity["Audio"]["pitch"].GetFloat();
//			sound.isPaused = entity["Audio"]["isPaused"].GetBool();
//			sound.isMute = entity["Audio"]["isMute"].GetBool();
//			sound.isLoop = entity["Audio"]["isLoop"].GetBool();
//			sound.isRandPitch = entity["Audio"]["isRandPitch"].GetBool();
//
//			bool isSpacial = entity["Audio"]["isSpacial"].GetBool();
//			e.AddComponent<Audio>({ sound , isSpacial });
//		}
//		if (entity.HasMember("AI"))
//		{
//			int colorChange = entity["AI"]["colorChange"].GetInt();
//			int movement = entity["AI"]["movement"].GetInt();
//			float speed = entity["AI"]["speed"].GetFloat();
//			float range = entity["AI"]["range"].GetFloat();
//
//			e.AddComponent<AI>({ colorChange ,movement, speed , range });
//		}
//		if (entity.HasMember("Text")) {
//			Text text;
//			text.fontFile = entity["Text"]["fontFile"].GetString();
//			text.text = entity["Text"]["text"].GetString();
//			text.offset = GetVec2(entity["Text"]["offset"]);
//			text.scale = entity["Text"]["scale"].GetFloat();
//			text.color.r = (GLubyte)entity["Text"]["color"]["r"].GetInt();
//			text.color.g = (GLubyte)entity["Text"]["color"]["g"].GetInt();
//			text.color.b = (GLubyte)entity["Text"]["color"]["b"].GetInt();
//			text.color.a = (GLubyte)entity["Text"]["color"]["a"].GetInt();
//			e.AddComponent<Text>(text);
//		}
//		if (entity.HasMember("Dialogue")) {
//			Dialogue dialogue;
//			dialogue.speakerID = (GLubyte)entity["Dialogue"]["speakerID"].GetInt();
//			dialogue.selectedID = (GLubyte)entity["Dialogue"]["selectedID"].GetInt();
//			dialogue.textID = (GLubyte)entity["Dialogue"]["textID"].GetInt();
//			dialogue.nextTextID = (GLubyte)entity["Dialogue"]["nextTextID"].GetInt();
//			e.AddComponent<Dialogue>(dialogue);
//		}
//		if (entity.HasMember("LightSource")) {
//			LightSource lightSource;
//			lightSource.centreOffset = GetVec2(entity["LightSource"]["centerOffset"]);
//			e.AddComponent<LightSource>(lightSource);
//		}
//		if (entity.HasMember("Script")) {
//			Script script;
//			script.name = entity["Script"]["name"].GetString();
//			//script.script = nullptr;
//			e.AddComponent<Script>(script);
//		}
//		sceneData.mEntities.insert(e);
//		i++;
//	}
//	logicSystem->Init();
//	return sceneData;
//}
//
//GameStateData SerializationManager::LoadGameStateData(ResourceManager::GUID const& _guid) {
//	return LoadGameStateData(ResourceManager::GetInstance()->GetFilePath(_guid));
//}
//
//GameStateData SerializationManager::LoadGameStateData(std::string const& _filePath) {
//	std::ifstream ifs(_filePath);
//	if (!ifs.good()) {
//		LOG_ERROR("Can't open scene file: " + _filePath);
//		return GameStateData();
//	} else LOG_INFO("Opening scene file: " + _filePath);
//	std::stringstream contents;
//	contents << ifs.rdbuf();
//	Document doc;
//	doc.Parse(contents.str().c_str());
//
//	//std::cout << contents.str() << '\n';
//	GameStateData gamestateData{};
//
//	// Getting gamestate data from json
//	std::string gamestateDataName{ "GameStateData" };
//	Value gamestateDataJson(gamestateDataName.c_str(), (rapidjson::SizeType)gamestateDataName.size(), doc.GetAllocator());
//	if (!doc.HasMember(gamestateDataJson)) {
//		std::cout << "No gamestateData found in scene file!\n";
//		gamestateData.mGUIDs.push_back(0);
//	}
//	else
//	{
//		Value gamestateDataValue(kObjectType);
//		gamestateDataValue = doc[gamestateDataJson];
//
//		Value a(kObjectType);
//		a = gamestateDataValue["Scenes"].GetArray();
//		for (int j = 0; j < (int)a.Size(); ++j) {
//			ResourceManager::GUID guid = a[j].GetUint64();
//			gamestateData.mGUIDs.push_back(guid);
//		}
//	}
//	
//	logicSystem->Init();
//	return gamestateData;
//}
//
//
//void SerializationManager::SaveSceneData(ResourceManager::GUID const& _guid) {
//	SceneData sceneData = ResourceManager::GetInstance()->GetResource<SceneData>(_guid);
//
//	Document scene;
//	auto& allocator = scene.GetAllocator();
//	scene.SetObject();
//
//	StringBuffer buffer;
//	//Writer<StringBuffer> writer(buffer);
//	PrettyWriter<StringBuffer> writer(buffer);
//	int counter = 0;
//
//	// Add all scene data
//	Value sceneDataValue(kObjectType);
//	sceneDataValue.AddMember(StringRef("isActive"), sceneData.isActive, allocator);
//
//	// Add all entities in the scene
//	for (const Entity& e : sceneData.mEntities) {
//		if (!e.HasComponent<General>())
//			continue;
//		Value entity(kObjectType);
//		if (e.HasComponent<General>()) {
//			Value tmp(kObjectType);
//			tmp.AddMember(StringRef("name"), StringRef(e.GetComponent<General>().name.c_str()), allocator);
//			tmp.AddMember(StringRef("tag"), (int)e.GetComponent<General>().tag, allocator);
//			tmp.AddMember(StringRef("subtag"), (int)e.GetComponent<General>().subtag, allocator);
//			tmp.AddMember(StringRef("isActive"), e.GetComponent<General>().isActive, allocator);
//			entity.AddMember(StringRef("General"), tmp, allocator);
//		}
//		if (e.HasComponent<Transform>()) {
//			Value tmp(kObjectType);
//			addVectorMember(scene, tmp, "scale", e.GetComponent<Transform>().scale);
//			tmp.AddMember(StringRef("rotation"), e.GetComponent<Transform>().rotation, allocator);
//			addVectorMember(scene, tmp, "translation", e.GetComponent<Transform>().translation);
//			entity.AddMember(StringRef("Transform"), tmp, allocator);
//		}
//		if (e.HasComponent<Sprite>()) {
//			Value tmp(kObjectType);
//			Value tmpc(kObjectType);
//			tmpc.AddMember(StringRef("r"), e.GetComponent<Sprite>().color.r, allocator);
//			tmpc.AddMember(StringRef("g"), e.GetComponent<Sprite>().color.g, allocator);
//			tmpc.AddMember(StringRef("b"), e.GetComponent<Sprite>().color.b, allocator);
//			tmpc.AddMember(StringRef("a"), e.GetComponent<Sprite>().color.a, allocator);
//			tmp.AddMember(StringRef("color"), tmpc, allocator);
//			tmp.AddMember(StringRef("sprite"), (int)e.GetComponent<Sprite>().sprite, allocator);
//			std::string tex = spriteManager->GetTexturePath(spriteManager->GetTexture(e));
//			Value texpath(tex.c_str(), (SizeType)tex.size(), allocator);
//			tmp.AddMember(StringRef("texture"), texpath, allocator);
//			tmp.AddMember(StringRef("layer"), e.GetComponent<Sprite>().layer, allocator);
//			entity.AddMember(StringRef("Sprite"), tmp, allocator);
//		}
//		if (e.HasComponent<Animation>()) {
//			Value tmp(kObjectType);
//			addVectorArrayStrMember(scene, tmp, "images", e.GetComponent<Animation>().images);
//			tmp.AddMember(StringRef("timePerImage"), e.GetComponent<Animation>().timePerImage, allocator);
//			tmp.AddMember(StringRef("timeToImageSwap"), e.GetComponent<Animation>().timeToImageSwap, allocator);
//			tmp.AddMember(StringRef("currentImageIndex"), e.GetComponent<Animation>().currentImageIndex, allocator);
//			entity.AddMember(StringRef("Animation"), tmp, allocator);
//		}
//		if (e.HasComponent<SheetAnimation>()) {
//			Value tmp(kObjectType);
//			tmp.AddMember(StringRef("frameCount"), e.GetComponent<SheetAnimation>().frameCount, allocator);
//			tmp.AddMember(StringRef("currFrameIndex"), e.GetComponent<SheetAnimation>().currFrameIndex, allocator);
//			tmp.AddMember(StringRef("timePerFrame"), e.GetComponent<SheetAnimation>().timePerFrame, allocator);
//			tmp.AddMember(StringRef("timeToFrameSwap"), e.GetComponent<SheetAnimation>().timeToFrameSwap, allocator);
//			entity.AddMember(StringRef("SheetAnimation"), tmp, allocator);
//		}
//		if (e.HasComponent<Physics2D>()) {
//			Value tmp(kObjectType);
//			tmp.AddMember(StringRef("dynamicsEnabled"), e.GetComponent<Physics2D>().dynamicsEnabled, allocator);
//			tmp.AddMember(StringRef("mass"), e.GetComponent<Physics2D>().mass, allocator);
//			tmp.AddMember(StringRef("inertia"), e.GetComponent<Physics2D>().inertia, allocator);
//			tmp.AddMember(StringRef("restitution"), e.GetComponent<Physics2D>().restitution, allocator);
//			tmp.AddMember(StringRef("friction"), e.GetComponent<Physics2D>().friction, allocator);
//			tmp.AddMember(StringRef("damping"), e.GetComponent<Physics2D>().damping, allocator);
//
//			addVectorMember(scene, tmp, "accumulatedForce", e.GetComponent<Physics2D>().accumulatedForce);
//			addVectorMember(scene, tmp, "velocity", e.GetComponent<Physics2D>().velocity);
//			addVectorMember(scene, tmp, "acceleration", e.GetComponent<Physics2D>().acceleration);
//
//			tmp.AddMember(StringRef("angularVelocity"), e.GetComponent<Physics2D>().angularVelocity, allocator);
//			tmp.AddMember(StringRef("angularTorque"), e.GetComponent<Physics2D>().angularTorque, allocator);
//			addVectorArrayForceMember(scene, tmp, "forceList", e.GetComponent<Physics2D>().forceList);
//			tmp.AddMember(StringRef("renderFlag"), e.GetComponent<Physics2D>().renderFlag, allocator);
//			entity.AddMember(StringRef("Physics2D"), tmp, allocator);
//		}
//		if (e.HasComponent<RectCollider>()) {
//			Value tmp(kObjectType);
//			addVectorMember(scene, tmp, "centerOffset", e.GetComponent<RectCollider>().centerOffset);
//			addVectorMember(scene, tmp, "scaleOffset", e.GetComponent<RectCollider>().scaleOffset);
//			tmp.AddMember(StringRef("renderFlag"), e.GetComponent<RectCollider>().renderFlag, allocator);
//			entity.AddMember(StringRef("RectCollider"), tmp, allocator);
//		}
//		if (e.HasComponent<CircleCollider>()) {
//			Value tmp(kObjectType);
//			addVectorMember(scene, tmp, "centerOffset", e.GetComponent<CircleCollider>().centerOffset);
//			tmp.AddMember(StringRef("scaleOffset"), e.GetComponent<CircleCollider>().scaleOffset, allocator);
//			tmp.AddMember(StringRef("renderFlag"), e.GetComponent<CircleCollider>().renderFlag, allocator);
//			entity.AddMember(StringRef("CircleCollider"), tmp, allocator);
//		}
//		if (e.HasComponent<Edge2DCollider>()) {
//			Value tmp(kObjectType);
//			addVectorMember(scene, tmp, "p0Offset", e.GetComponent<Edge2DCollider>().p0Offset);
//			tmp.AddMember(StringRef("rotationOffset"), e.GetComponent<Edge2DCollider>().rotationOffset, allocator);
//			tmp.AddMember(StringRef("scaleOffset"), e.GetComponent<Edge2DCollider>().scaleOffset, allocator);
//			tmp.AddMember(StringRef("renderFlag"), e.GetComponent<Edge2DCollider>().renderFlag, allocator);
//			entity.AddMember(StringRef("Edge2DCollider"), tmp, allocator);
//		}
//		if (e.HasComponent<Point2DCollider>()) {
//			Value tmp(kObjectType);
//			addVectorMember(scene, tmp, "centerOffset", e.GetComponent<Point2DCollider>().centerOffset);
//			tmp.AddMember(StringRef("renderFlag"), e.GetComponent<Point2DCollider>().renderFlag, allocator);
//			entity.AddMember(StringRef("Point2DCollider"), tmp, allocator);
//		}
//		if (e.HasComponent<Audio>()) {
//			Value tmp(kObjectType);
//			tmp.AddMember(StringRef("path"), StringRef(e.GetComponent<Audio>().sound.path.c_str()), allocator);
//			tmp.AddMember(StringRef("volume"), e.GetComponent<Audio>().sound.volume, allocator);
//			tmp.AddMember(StringRef("volumeMod"), e.GetComponent<Audio>().sound.volumeMod, allocator);
//			tmp.AddMember(StringRef("pitch"), e.GetComponent<Audio>().sound.pitch, allocator);
//			tmp.AddMember(StringRef("isPaused"), e.GetComponent<Audio>().sound.isPaused, allocator);
//			tmp.AddMember(StringRef("isMute"), e.GetComponent<Audio>().sound.isMute, allocator);
//			tmp.AddMember(StringRef("isLoop"), e.GetComponent<Audio>().sound.isLoop, allocator);
//			tmp.AddMember(StringRef("isRandPitch"), e.GetComponent<Audio>().sound.isRandPitch, allocator);
//			tmp.AddMember(StringRef("isSpacial"), e.GetComponent<Audio>().isSpacial, allocator);
//			entity.AddMember(StringRef("Audio"), tmp, allocator);
//		}
//		if (e.HasComponent<AI>()) {
//			Value tmp(kObjectType);
//			tmp.AddMember(StringRef("colorChange"), e.GetComponent<AI>().colorChange, allocator);
//			tmp.AddMember(StringRef("movement"), e.GetComponent<AI>().movement, allocator);
//			tmp.AddMember(StringRef("speed"), e.GetComponent<AI>().speed, allocator);
//			tmp.AddMember(StringRef("range"), e.GetComponent<AI>().range, allocator);
//			entity.AddMember(StringRef("AI"), tmp, allocator);
//		}
//		if (e.HasComponent<Text>()) {
//			Value tmp(kObjectType);
//			Value tmpc(kObjectType);
//			tmp.AddMember(StringRef("fontFile"), StringRef(e.GetComponent<Text>().fontFile.c_str()), allocator);
//			tmp.AddMember(StringRef("text"), StringRef(e.GetComponent<Text>().text.c_str()), allocator);
//			addVectorMember(scene, tmp, "offset", e.GetComponent<Text>().offset);
//			tmp.AddMember(StringRef("scale"), e.GetComponent<Text>().scale, allocator);
//			tmpc.AddMember(StringRef("r"), e.GetComponent<Text>().color.r, allocator);
//			tmpc.AddMember(StringRef("g"), e.GetComponent<Text>().color.g, allocator);
//			tmpc.AddMember(StringRef("b"), e.GetComponent<Text>().color.b, allocator);
//			tmpc.AddMember(StringRef("a"), e.GetComponent<Text>().color.a, allocator);
//			tmp.AddMember(StringRef("color"), tmpc, allocator);
//			entity.AddMember(StringRef("Text"), tmp, allocator);
//		}
//		if (e.HasComponent<Dialogue>()) {
//			Value tmp(kObjectType);
//			tmp.AddMember(StringRef("speakerID"), e.GetComponent<Dialogue>().speakerID, allocator);
//			tmp.AddMember(StringRef("selectedID"), e.GetComponent<Dialogue>().selectedID, allocator);
//			tmp.AddMember(StringRef("textID"), e.GetComponent<Dialogue>().textID, allocator);
//			tmp.AddMember(StringRef("nextTextID"), e.GetComponent<Dialogue>().nextTextID, allocator);
//			entity.AddMember(StringRef("Dialogue"), tmp, allocator);
//		}
//		std::string s("Entity" + std::to_string(counter));
//		Value index(s.c_str(), (SizeType)s.size(), allocator);
//		scene.AddMember(index, entity, allocator);
//		counter++;
//	}
//
//	scene.Accept(writer);
//	std::string jsonf(buffer.GetString(), buffer.GetSize());
//	std::string path = ResourceManager::GetInstance()->GetFilePath(_guid);
//	std::ofstream ofs(path);
//	ofs << jsonf;
//	if (!ofs.good()) {
//		LOG_ERROR("Unable to save scene to: " + path);
//	} else
//		LOG_INFO("Saved Scene: " + path);
//}
