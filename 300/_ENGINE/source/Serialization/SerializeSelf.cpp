#include "ECS/ECS_Components.h"

void General::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("general");
	writer.StartObject();
	Serialize(writer, "name", name);
	Serialize(writer, "tagid", tagid);
	Serialize(writer, "subtag", subtag);
	Serialize(writer, "isActive", isActive);
	Serialize(writer, "isPaused", isPaused);
	writer.EndObject();
}

void General::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "name", name);
	Deserialize(reader, "tagid", tagid);
	Deserialize(reader, "subtag", subtag);
	Deserialize(reader, "isActive", isActive);
	Deserialize(reader, "isPaused", isPaused);
}

void Transform::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("transform");
	writer.StartObject();
	Serialize(writer, "scale", mScale);
	Serialize(writer, "rotate", mRotate);
	Serialize(writer, "translate", mTranslate);
	writer.EndObject();
}

void Transform::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "scale", mScale);
	Deserialize(reader, "rotate", mRotate);
	Deserialize(reader, "translate", mTranslate);
}

void RigidBody::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("rigidbody");
	writer.StartObject();
	Serialize(writer, "density", mDensity);
	Serialize(writer, "material", mMaterial);
	Serialize(writer, "motion", mMotion);
	Serialize(writer, "velocity", mVelocity);
	writer.EndObject();
}

void RigidBody::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "density", mDensity);
	Deserialize(reader, "material", mMaterial);
	Deserialize(reader, "motion", mMotion);
	Deserialize(reader, "velocity", mVelocity);
}

void MeshRenderer::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("meshrenderer");
	writer.StartObject();
	Serialize(writer, "vert", mShaderPath.first);
	Serialize(writer, "frag", mShaderPath.second);
	Serialize(writer, "materialinstance", mMaterialInstancePath, 4);
	Serialize(writer, "mesh", mMeshPath);
	Serialize(writer, "texturecont", mTextureCont, 4);
	Serialize(writer, "guid", mGUID);
	writer.EndObject();
}

void MeshRenderer::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "vert", mShaderPath.first);
	Deserialize(reader, "frag", mShaderPath.second);
	Deserialize(reader, "materialinstance", mMaterialInstancePath, 4);
	Deserialize(reader, "mesh", mMeshPath);
	Deserialize(reader, "texturecont", mTextureCont, 4);
	Deserialize(reader, "guid", mGUID);
}

void BoxCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("boxcollider");
	writer.StartObject();
	Serialize(writer, "scaleoffset", mScaleOffset);
	Serialize(writer, "translateoffset", mTranslateOffset);
	writer.EndObject();
}

void BoxCollider::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "scaleoffset", mScaleOffset);
	Deserialize(reader, "translateoffset", mTranslateOffset);
}

void SphereCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("spherecollider");
	writer.StartObject();
	Serialize(writer, "scaleoffset", mScaleOffset);
	Serialize(writer, "translateoffset", mTranslateOffset);
	writer.EndObject();
}

void SphereCollider::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "scaleoffset", mScaleOffset);
	Deserialize(reader, "translateoffset", mTranslateOffset);
}

void PlaneCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("planecollider");
	writer.StartObject();
	Serialize(writer, "normal", mNormal);
	Serialize(writer, "translateoffset", mTranslateOffset);
	writer.EndObject();
}

void PlaneCollider::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "normal", mNormal);
	Deserialize(reader, "translateoffset", mTranslateOffset);
}

void Scripts::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("scripts");
	writer.StartObject();
	Serialize(writer, "scriptscontainer", scriptsContainer);
	writer.EndObject();
}

void Scripts::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "scriptscontainer", scriptsContainer);
}

void Parent::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("parent");
	writer.StartObject();
	Serialize(writer, "prevsibling", mPrevSibling);
	Serialize(writer, "nextsibling", mNextSibling);
	Serialize(writer, "parent", mParent);
	writer.EndObject();
}

void Parent::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "prevsibling", mPrevSibling);
	Deserialize(reader, "nextsibling", mNextSibling);
	Deserialize(reader, "parent", mParent);
}

void Children::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("children");
	writer.StartObject();
	Serialize(writer, "numchildren", mNumChildren);
	Serialize(writer, "firstchild", mFirstChild);
	writer.EndObject();
}

void Children::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "numchildren", mNumChildren);
	Deserialize(reader, "firstchild", mFirstChild);
}

void Audio::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("audio");
	writer.StartObject();
	Serialize(writer, "filename", mFileName);
	Serialize(writer, "audiotype", mAudioType);
	Serialize(writer, "playing", mIsPlaying);
	Serialize(writer, "play", mIsPlay);
	writer.EndObject();
}

void Audio::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "filename", mFileName);
	Deserialize(reader, "audiotype", mAudioType);
	Deserialize(reader, "playing", mIsPlaying);
	Deserialize(reader, "play", mIsPlay);
}

void Camera::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("camera");
	writer.StartObject();
	Serialize(writer, "positon", mCamera.mPosition);
	Serialize(writer, "target", mCamera.mTarget);
	Serialize(writer, "size", mCamera.mSize);
	Serialize(writer, "near", mCamera.mNear);
	Serialize(writer, "far", mCamera.mFar);
	writer.EndObject();
}

void Camera::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "positon", mCamera.mPosition);
	Deserialize(reader, "target", mCamera.mTarget);
	Deserialize(reader, "size", mCamera.mSize);
	Deserialize(reader, "near", mCamera.mNear);
	Deserialize(reader, "far", mCamera.mFar);
}

void Prefab::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("prefab");
	writer.StartObject();
	Serialize(writer, "prefab", mPrefab);
	writer.EndObject();
}

void Prefab::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "prefab", mPrefab);
}

void PointLight::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("pointlight");
	writer.StartObject();
	Serialize(writer, "lightcolor", mLightColor);
	Serialize(writer, "attenuation", mAttenuation);
	Serialize(writer, "intensity", mIntensity);
	writer.EndObject();
}

void PointLight::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "lightcolor", mLightColor);
	Deserialize(reader, "attenuation", mAttenuation);
	Deserialize(reader, "intensity", mIntensity);
}