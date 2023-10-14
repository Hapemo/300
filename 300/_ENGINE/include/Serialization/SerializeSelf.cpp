#include "ECS/ECS_Components.h"

void General::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "name", name);
	Serialize(writer, "tagid", tagid);
	Serialize(writer, "subtag", static_cast<int>(subtag));
	Serialize(writer, "isActive", isActive);
	Serialize(writer, "isPaused", isPaused);
	writer.EndObject();
}

void Transform::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "scale", mScale);
	Serialize(writer, "rotate", mRotate);
	Serialize(writer, "translate", mTranslate);
	writer.EndObject();
}

void RigidBody::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "density", mDensity);
	Serialize(writer, "material", static_cast<int>(mMaterial));
	Serialize(writer, "motion", static_cast<int>(mMotion));
	Serialize(writer, "velocity", mVelocity);
	writer.EndObject();
}

void MeshRenderer::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "vert", mShaderPath.first);
	Serialize(writer, "frag", mShaderPath.second);
	Serialize(writer, "materialinstance", mMaterialInstancePath, 4);
	Serialize(writer, "mesh", mMeshPath);
	Serialize(writer, "texturecont", mTextureCont, 4);
	Serialize(writer, "guid", mGUID);
	writer.EndObject();
}

void BoxCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "scaleoffset", mScaleOffset);
	Serialize(writer, "translateoffset", mTranslateOffset);
	writer.EndObject();
}

void SphereCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "scaleoffset", mScaleOffset);
	Serialize(writer, "translateoffset", mTranslateOffset);
	writer.EndObject();
}

void PlaneCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "normal", mNormal);
	Serialize(writer, "translateoffset", mTranslateOffset);
	writer.EndObject();
}

void Scripts::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "scriptfile", mScriptFile);
	Serialize(writer, "scriptscontainer", scriptsContainer);
	writer.EndObject();
}

void Parent::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "prevsibling", mPrevSibling);
	Serialize(writer, "nextsibling", mNextSibling);
	Serialize(writer, "parent", mParent);
	writer.EndObject();
}

void Children::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "numchildren", mNumChildren);
	Serialize(writer, "firstchild", mFirstChild);
	writer.EndObject();
}

void Audio::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "filename", mFileName);
	Serialize(writer, "audiotype", static_cast<int>(mAudioType));
	Serialize(writer, "playing", mIsPlaying);
	Serialize(writer, "play", mIsPlay);
	writer.EndObject();
}

void Camera::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "positon", mCamera.mPosition);
	Serialize(writer, "target", mCamera.mTarget);
	Serialize(writer, "size", mCamera.mSize);
	Serialize(writer, "near", mCamera.mNear);
	Serialize(writer, "far", mCamera.mFar);
	writer.EndObject();
}

void Prefab::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "prefab", mPrefab);
	writer.EndObject();
}

void PointLight::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	Serialize(writer, "lightcolor", mLightColor);
	Serialize(writer, "attenuation", mAttenuation);
	Serialize(writer, "intensity", mIntensity);
	writer.EndObject();
}