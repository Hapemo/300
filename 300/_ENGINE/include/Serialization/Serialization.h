/*!*************************************************************************
****
\file Serialization.h
\author Charissa Yip
\par DP email: charissa.yip@digipen.edu
\date 28-9-2023
\brief

This file contains declarations of functions to facilitate serialization and
deserialization. The following classes include BaseJSON, EntityJSON (for serialization
of a single object), EntityListJSON (for serialization of a list of objects),
SceneJSON (for serialization of a scene) and SceneListJSON (for serialization of
a list of scenes). Additionally, there are some helper functions to convert
enums to strings and vice versa within the serialization.

****************************************************************************
***/

#pragma once
#include "pch.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ECS/ECS_Components.h"
#include "ECS/ECS.h"
#include "GameState/Scene.h"

/***************************************************************************/
/*

FIND___ENUM FUNCTIONS
\brief
Helper function to convert string into tag by searching through a map
for the string name and finding the associated enumeration.

\param str
The name of the enumeration to search for.

\return
The associated enumeration.

***************************************************************************

FIND___STRING FUNCTIONS
\brief
Helper function to convert tag into string by searching through a map
for the tag enumeration and finding the associated name/string.

\param tag
The enumeration to search for.

\return
The associated name in the form of a string.

*/
/**************************************************************************/
TAG FindTagEnum(std::string str);
std::string FindTagString(TAG tag);

SUBTAG FindSubTagEnum(std::string str);
std::string FindSubTagString(SUBTAG subtag);

MATERIAL FindMaterialEnum(std::string str);
std::string FindMaterialString(MATERIAL tag);

MOTION FindMotionEnum(std::string str);
std::string FindMotionString(MOTION tag);

AUDIOTYPE FindAudioEnum(std::string str);
std::string FindAudioString(AUDIOTYPE tag);

/*******************************/
/*!
	BaseJSON Class
*/
/*******************************/
class BaseJSON
{
public:
/***************************************************************************/
/*

\brief
Reads data from a .json file and reformats into C++ code

\param filename
The file to deserialise

\return
A bool which checks if the file was successfully deserialised, true if
successful, otherwise false.

*/
/**************************************************************************/
	bool DeserializeFile(const std::string& filename);

/***************************************************************************/
/*

\brief
Converts and writes to a .json file from C++ code

\param filename
The file to serialise into

\return
A bool which checks if the file was successfully serialised, true if
successful, otherwise false.

*/
/**************************************************************************/
	bool SerializeFile(const std::string& filename);

/***************************************************************************/
/*

\brief
Converts data from .json format, in the form of a string, into C++ code

\param s
The string of objects to deserialise

\return
A bool which checks if the document was successfully deserialised, true if
successful, otherwise false.

*/
/**************************************************************************/
	virtual bool Deserialize(const std::string& s);

/***************************************************************************/
/*

\brief
Converts data into .json format, in the form of a string, from C++ code

\return
A string of JSON objects

*/
/**************************************************************************/
	virtual std::string Serialize() const;

/***************************************************************************/
/*

\brief
Reads from the object/values that were assigned to the .json keywords.

\param obj
The object to deserialise into the respective components

\return
A bool which checks if the deserialising was successful, true if
successful, otherwise false.

*/
/**************************************************************************/
	virtual bool Deserialize(const rapidjson::Value& obj) = 0;

/***************************************************************************/
/*

\brief
Stores and assigns data to .json keywords, creating an object.

\param writer
Location of where the objects/information will be written into

\return
A bool which checks if the serialising was successful, true if
successful, otherwise false.

*/
/**************************************************************************/
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const = 0;

protected:
/***************************************************************************/
/*

\brief
Parses data into a document with .json format

\param s
The string of data

\param doc
The document to parse the string into so as to convert into a .json object.

\return
A bool which checks if the parsing was successfully parsed into document, true
if successful, otherwise false.

*/
/**************************************************************************/
	bool InitDocument(const std::string& s, rapidjson::Document& doc);
};

/*******************************/
/*!
	EntityJSON Class
*/
/*******************************/
class EntityJSON : public BaseJSON
{
public:
/***************************************************************************/
/*

\brief
Default constructor of EntityJSON

*/
/**************************************************************************/
	EntityJSON();

/***************************************************************************/
/*

\brief
Deconstructor of EntityJSON

*/
/**************************************************************************/
	virtual ~EntityJSON();

/***************************************************************************/
/*

\brief
Overloaded function that reads from the object/values that were assigned to
the .json keywords.

\param obj
The object to deserialise

\return
A bool which checks if the deserialization was successful, true if successful,
otherwise false.

*/
/**************************************************************************/
	virtual bool Deserialize(const rapidjson::Value& obj);

/***************************************************************************/
/*

\brief
Overloaded function that writes all existing object data into the writer.

\param writer
The location to write all objects into

\return
A bool which checks if the serialization was successful, true if successful,
otherwise false.

*/
/**************************************************************************/
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

/***************************************************************************/
/*

\brief
The following functions are setters and getters that return/set the respective
private variables:

1. EntityID
2. General
3. Transform
4. RigidBody
5. MeshRenderer
6. BoxCollider
7. SphereCollider
8. PlaneCollider
9. Scripts
10. Parent
11. Children
12. Audio
13. Camera
14. PointLight

The getters facilitates deserialization while the setters help to set the data
to the respective private variables so as to facilitate serialization. There
are booleans set within the setter functions so as to help in the checking
of the existence of the components.

*/
/**************************************************************************/
	const entt::entity GetIDJSON() const
	{
		return mID.id;
	}

	void SetIDJSON(const entt::entity id)
	{
		mID.id = id;
		mid_t = true;
	}

	const General GetGeneralJSON() const
	{
		return mGJ;
	}

	void SetGeneralJSON(const General gj)
	{
		mGJ = gj;
		mg_t = true;
	}

	const Transform GetTransformJSON() const
	{
		return mTJ;
	}

	void SetTransformJSON(const Transform tj)
	{
		mTJ = tj;
		mt_t = true;
	}

	const RigidBody GetRBJSON() const
	{
		return mRBJ;
	}

	void SetRBJSON(const RigidBody rbj)
	{
		mRBJ = rbj;
		mrb_t = true;
	}

	void SetMRJSON(const MeshRenderer mrj)
	{
		mMRJ = mrj;
		mmr_t = true;
	}

	const MeshRenderer GetMRJSON() const
	{
		return mMRJ;
	}

	void SetBCJSON(const BoxCollider bcj)
	{
		mBCJ = bcj;
		mbc_t = true;
	}

	const BoxCollider GetBCJSON() const
	{
		return mBCJ;
	}

	const SphereCollider GetSCJSON() const
	{
		return mSCJ;
	}

	void SetSCJSON(const SphereCollider scj)
	{
		mSCJ = scj;
		msc_t = true;
	}

	const PlaneCollider GetPCJSON() const
	{
		return mPCJ;
	}

	void SetPCJSON(const PlaneCollider pcj)
	{
		mPCJ = pcj;
		mpc_t = true;
	}

	const Scripts GetSJSON() const
	{
		return mSJ;
	}

	void SetSJSON(const Scripts sj)
	{
		mSJ = sj;
		ms_t = true;
	}

	const Parent GetPJSON() const
	{
		return mPJ;
	}

	void SetPJSON(const Parent pj)
	{
		mPJ = pj;
		mp_t = true;
	}

	const Children GetCJSON() const
	{
		return mCJ;
	}

	void SetCJSON(const Children cj)
	{
		mCJ = cj;
		mc_t = true;
	}

	const Audio GetAJSON() const
	{
		return mAJ;
	}

	void SetAJSON(const Audio aj)
	{
		mAJ = aj;
		ma_t = true;
	}

	const Camera GetCAMJSON() const
	{
		return mCamJ;
	}

	void SetCAMJSON(const Camera camj)
	{
		mCamJ = camj;
		mcam_t = true;
	}

	const PointLight GetPLJSON() const
	{
		return mPLJ;
	}

	void SetPLJSON(const PointLight plj)
	{
		mPLJ = plj;
		mpl_t = true;
	}

	// trackers to disable serialization if component does not exist
	bool mid_t = false, mg_t = false, mt_t = false, mmr_t = false,
		mrb_t = false, mbc_t = false, msc_t = false,
		mpc_t = false, ms_t = false, mp_t = false,
		mc_t = false, ma_t = false, mcam_t = false, mpl_t = false;

private:
	Entity mID;
	General mGJ;
	Transform mTJ;
	RigidBody mRBJ;
	MeshRenderer mMRJ;
	BoxCollider mBCJ;
	SphereCollider mSCJ;
	PlaneCollider mPCJ;
	Scripts mSJ;
	Parent mPJ;
	Children mCJ;
	Audio mAJ;
	Camera mCamJ;
	PointLight mPLJ;
};

/*******************************/
/*!
	EntityListJSON Class
*/
/*******************************/
class EntityListJSON : public BaseJSON
{
public:
	virtual ~EntityListJSON() {};

/***************************************************************************/
/*

\brief
Overloaded function that reads from the object/values that were assigned to
the .json keywords.

\param s
The string to deserialise

\return
A bool which checks if the deserialization was successful, true if successful,
otherwise false.

*/
/**************************************************************************/
	virtual bool Deserialize(const std::string& s);
	virtual bool Deserialize(const rapidjson::Value&) { return false; }

/***************************************************************************/
/*

\brief
Overloaded function that writes all existing object data into the writer.

\param writer
The location to write all objects into

\return
A bool which checks if the serialization was successful, true if successful,
otherwise false.

*/
/**************************************************************************/
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

/***************************************************************************/
/*

\brief
Getter function that returns the list of entities.

\return ents
The list of objects

*/
/**************************************************************************/
	std::list<EntityJSON>& EntitiesList() { return ents; }

private:
	std::list<EntityJSON> ents;
};

/*******************************/
/*!
	SceneJSON Class
*/
/*******************************/
class SceneJSON : public BaseJSON
{
public:

/***************************************************************************/
/*

\brief
Default constructor of SceneJSON

*/
/**************************************************************************/
	SceneJSON();

/***************************************************************************/
/*

\brief
Deconstructor of SceneJSON

*/
/**************************************************************************/
	virtual ~SceneJSON();

/***************************************************************************/
/*

\brief
Overloaded function that reads from the object/values that were assigned to
the .json keywords.

\param obj
The object to deserialise

\return
A bool which checks if the deserialization was successful, true if successful,
otherwise false.

*/
/**************************************************************************/
	virtual bool Deserialize(const rapidjson::Value& obj);

/***************************************************************************/
/*

\brief
Overloaded function that writes all existing object data into the writer.

\param writer
The location to write all objects into

\return
A bool which checks if the serialization was successful, true if successful,
otherwise false.

*/
/**************************************************************************/
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

/***************************************************************************/
/*

\brief
Getter and Setter for SceneJSON.

*/
/**************************************************************************/
	void SetSceneJSON(const Scene sj)
	{
		mScene = sj;
	}

	const Scene GetSceneJSON() const
	{
		return mScene;
	}

private:
	Scene mScene;
};

/*******************************/
/*!
	SceneListJSON Class
*/
/*******************************/
class SceneListJSON : public BaseJSON
{
public:
	virtual ~SceneListJSON() {};

/***************************************************************************/
/*

\brief
Overloaded function that reads from the object/values that were assigned to
the .json keywords.

\param s
The string to deserialise

\return
A bool which checks if the deserialization was successful, true if successful,
otherwise false.

*/
/**************************************************************************/
	virtual bool Deserialize(const std::string& s);
	virtual bool Deserialize(const rapidjson::Value&) { return false; }

/***************************************************************************/
/*

\brief
Overloaded function that writes all existing object data into the writer.

\param writer
The location to write all objects into

\return
A bool which checks if the serialization was successful, true if successful,
otherwise false.

*/
/**************************************************************************/
	virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

/***************************************************************************/
/*

\brief
Getter function that returns the list of scenes.

\return scenes
The list of scenes in the form of objects

*/
/**************************************************************************/
	std::list<SceneJSON>& SceneList() { return scenes; }

private:
	std::list<SceneJSON> scenes;
};