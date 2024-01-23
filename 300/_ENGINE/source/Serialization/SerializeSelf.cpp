/*!*************************************************************************
****
\file		   SerializeSelf.cpp
\author(s)	   Charissa Yip, Lor Xaun Yun Michelle, Kew Yu Jun
\par DP email:
charissa.yip\@digipen.edu, xaunyunmichelle.lor\@digipen.edu
\date		   4-11-2023
\brief
This file contains declarations of functions for each component class.
****************************************************************************/
#include "ECS/ECS_Components.h"
#include "ResourceManagerTy.h"
#include "Guid.h"

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
	//std::cout << name << std::endl;
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
	mPreviousPosition = mTranslate;
}

void MeshRenderer::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("meshrenderer");
	writer.StartObject();
	Serialize(writer, "vert", mShaderPath.first);
	Serialize(writer, "frag", mShaderPath.second);
	Serialize(writer, "meshrefid", mMeshRef.data_uid);
	Serialize(writer, "instancecolor", mInstanceColor);
	Serialize(writer, "bloomthreshold", mBloomThreshold);
	Serialize(writer, "materialinstance", mMaterialInstancePath, 6);
	Serialize(writer, "mesh", mMeshPath);
	//Serialize(writer, "texturecont", mTextureCont, 5);
	writer.Key("texturerefid");
	writer.StartArray();
	for (int i = 0; i < 6; ++i)
	{
		Serialize(writer, nullptr, mTextureRef[i].data_uid);
	}
	writer.EndArray();
	writer.EndObject();
}

void MeshRenderer::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "vert", mShaderPath.first);
	Deserialize(reader, "frag", mShaderPath.second);
	Deserialize(reader, "meshrefid", mMeshRef.data_uid);
	Deserialize(reader, "instancecolor", mInstanceColor);
	Deserialize(reader, "bloomthreshold", mBloomThreshold);
	Deserialize(reader, "materialinstance", mMaterialInstancePath, 6);

	for (int i{}; i < 6; ++i)
	{
		// deserialize the texture descriptor files
		if (mMaterialInstancePath[i] == " ")
			continue;

		// store the texture descriptor data to meshrenderer struct
		std::string texturedescFilepath = mMaterialInstancePath[i] + ".desc";
		_GEOM::Texture_DescriptorData::DeserializeTEXTURE_DescriptorDataFromFile(mTextureDescriptorData[i], texturedescFilepath);
	}

	Deserialize(reader, "mesh", mMeshPath);
	//Deserialize(reader, "texturecont", mTextureCont, 5);
	for (int i = 0; i < 6; ++i)
	{
		Deserialize(reader["texturerefid"][i], nullptr, mTextureRef[i].data_uid);
	}
	//mTextureRef->data = reinterpret_cast<void*>(systemManager->mResourceTySystem->getMaterialInstance(mTextureRef->data_uid));
}

void UIrenderer::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("uirenderer");
	writer.StartObject();
	Serialize(writer, "texpath", mTexPath);
	Serialize(writer, "textureref", mTextureRef.data_uid);
	Serialize(writer, "degree", mDegree);
	Serialize(writer, "color", mColor);
	Serialize(writer, "layer", mLayer);
	Serialize(writer, "worldtransform", mWorldTransform);
	writer.EndObject();
}

void UIrenderer::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "texpath", mTexPath);
	Deserialize(reader, "textureref", mTextureRef.data_uid);
	Deserialize(reader, "degree", mDegree);
	Deserialize(reader, "color", mColor);
	Deserialize(reader, "layer", mLayer);
	Deserialize(reader, "worldtransform", mWorldTransform);
}

void RigidBody::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("rigidbody");
	writer.StartObject();
	Serialize(writer, "density", mDensity);
	Serialize(writer, "material", mMaterial);
	Serialize(writer, "motion", mMotion);
	Serialize(writer, "velocity", mVelocity);
	Serialize(writer, "rotationconstraints", mRotationConstraints);
	Serialize(writer, "gravity", mGravity);
	writer.EndObject();
}

void RigidBody::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "density", mDensity);
	Deserialize(reader, "material", mMaterial);
	Deserialize(reader, "motion", mMotion);
	Deserialize(reader, "velocity", mVelocity);
	Deserialize(reader, "rotationconstraints", mRotationConstraints);
	Deserialize(reader, "gravity", mGravity);
}

void BoxCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("boxcollider");
	writer.StartObject();
	Serialize(writer, "scaleoffset", mScaleOffset);
	Serialize(writer, "translateoffset", mTranslateOffset);
	Serialize(writer, "istrigger", mIsTrigger);
	writer.EndObject();
}

void BoxCollider::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "scaleoffset", mScaleOffset);
	Deserialize(reader, "translateoffset", mTranslateOffset);
	Deserialize(reader, "istrigger", mIsTrigger);
}

void SphereCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("spherecollider");
	writer.StartObject();
	Serialize(writer, "scaleoffset", mScaleOffset);
	Serialize(writer, "translateoffset", mTranslateOffset);
	Serialize(writer, "istrigger", mIsTrigger);
	writer.EndObject();
}

void SphereCollider::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "scaleoffset", mScaleOffset);
	Deserialize(reader, "translateoffset", mTranslateOffset);
	Deserialize(reader, "istrigger", mIsTrigger);

}

void CapsuleCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("capsulecollider");
	writer.StartObject();
	Serialize(writer, "translateoffset", mTranslateOffset);
	Serialize(writer, "radius", mRadius);
	Serialize(writer, "halfheight", mHalfHeight);
	Serialize(writer, "istrigger", mIsTrigger);
	writer.EndObject();
}

void CapsuleCollider::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "translateoffset", mTranslateOffset);
	Deserialize(reader, "radius", mRadius);
	Deserialize(reader, "halfheight", mHalfHeight);
	Deserialize(reader, "istrigger", mIsTrigger);
}

void MeshCollider::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("meshcollider");
	writer.StartObject();
	Serialize(writer, "ismeshcollider", mIsMeshCollider);
	writer.EndObject();
}

void MeshCollider::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "ismeshcollider", mIsMeshCollider);
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
	for (int i = 0; i < scriptsContainer.size(); ++i)
		Deserialize(reader, scriptsContainer[i]->scriptFile.c_str(), scriptsContainer[i]->variables);
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
	Serialize(writer, "filepath", mFilePath);
	Serialize(writer, "filename", mFileName);
	Serialize(writer, "fullpath", mFullPath);
	Serialize(writer, "playonawake", mPlayonAwake);
	Serialize(writer, "islooping", mIsLooping);
	Serialize(writer, "audiotype", mAudioType);
	Serialize(writer, "volume", mVolume);
	Serialize(writer, "retainaudio", mGameStateRetain);
	writer.EndObject();
}

void Audio::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "filepath", mFilePath);
	Deserialize(reader, "filename", mFileName);
	Deserialize(reader, "fullpath", mFullPath);
	Deserialize(reader, "playonawake", mPlayonAwake);
	Deserialize(reader, "islooping", mIsLooping);
	Deserialize(reader, "audiotype", mAudioType);
	Deserialize(reader, "volume", mVolume);
	Deserialize(reader, "retainaudio", mGameStateRetain);
}

void AudioListener::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("audiolistener");
	writer.StartObject();
	writer.EndObject();
}

void AudioListener::DeserializeSelf(rapidjson::Value& reader)
{

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
	Serialize(writer, "linearfalloff", mLinearFalloff);
	Serialize(writer, "quadraticfalloff", mQuadraticFalloff);
	Serialize(writer, "intensity", mIntensity);
	writer.EndObject();
}

void PointLight::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "lightcolor", mLightColor);
	Deserialize(reader, "linearfalloff", mLinearFalloff);
	Deserialize(reader, "quadraticfalloff", mQuadraticFalloff);
	Deserialize(reader, "intensity", mIntensity);
}

void VFX::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("vfx");
	writer.StartObject();
	Serialize(writer, "bloomthreshold", mBloomThreshold);
	writer.EndObject();
}

void VFX::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "bloomthreshold", mBloomThreshold);
}

void AISetting::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("aisetting");
	writer.StartObject();
	Serialize(writer, "movementtype", mMovementType);
	Serialize(writer, "spreadout", mSpreadOut);
	Serialize(writer, "stayaway", mStayAway);
	Serialize(writer, "elevation", mElevation);
	Serialize(writer, "targetname", mTargetName);
	Serialize(writer, "graphdataname", mGraphDataName);
	writer.EndObject();
}

void AISetting::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "movementtype", mMovementType);
	Deserialize(reader, "spreadout", mSpreadOut);
	Deserialize(reader, "stayaway", mStayAway);
	Deserialize(reader, "elevation", mElevation);
	Deserialize(reader, "targetname", mTargetName);
	Deserialize(reader, "graphdataname", mGraphDataName);
}

void Crosshair::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("crosshair");
	writer.StartObject();
	Serialize(writer, "thickness", mThickness);
	Serialize(writer, "inner", mInner);
	Serialize(writer, "outer", mOuter);
	Serialize(writer, "color", mColor);
	writer.EndObject();
}

void Crosshair::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "thickness", mThickness);
	Deserialize(reader, "inner", mInner);
	Deserialize(reader, "outer", mOuter);
	Deserialize(reader, "color", mColor);
}

void Healthbar::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("healthbar");
	writer.StartObject();
	Serialize(writer, "healthcolor", mHealthColor);
	Serialize(writer, "backcolor", mBackColor);
	Serialize(writer, "position", mPosition);
	Serialize(writer, "maxhealth", mMaxHealth);
	Serialize(writer, "width", mWidth);
	Serialize(writer, "height", mHeight);
	writer.EndObject();
}

void Healthbar::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "healthcolor", mHealthColor);
	Deserialize(reader, "backcolor", mBackColor);
	Deserialize(reader, "position", mPosition);
	Deserialize(reader, "maxhealth", mMaxHealth);
	Deserialize(reader, "maxhealth", mHealth);
	Deserialize(reader, "width", mWidth);
	Deserialize(reader, "height", mHeight);
}

void Button::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("button");
	writer.StartObject();
	Serialize(writer, "interactable", mInteractable);
	Serialize(writer, "hover", mIsHover);
	Serialize(writer, "click", mIsClick);
	Serialize(writer, "activated", mActivated);
	Serialize(writer, "renderflag", mRenderFlag);
	writer.EndObject();
}

void Button::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "interactable", mInteractable);
	Deserialize(reader, "hover", mIsHover);
	Deserialize(reader, "click", mIsClick);
	Deserialize(reader, "activated", mActivated);
	Deserialize(reader, "renderflag", mRenderFlag);
}

void Spotlight::SerializeSelf(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
	writer.Key("spotlight");
	writer.StartObject();
	Serialize(writer, "direction", mDirection);
	Serialize(writer, "color", mColor);
	Serialize(writer, "cutoff", mCutoff);
	Serialize(writer, "intensity", mIntensity);
	writer.EndObject();
}

void Spotlight::DeserializeSelf(rapidjson::Value& reader)
{
	Deserialize(reader, "direction", mDirection);
	Deserialize(reader, "color", mColor);
	Deserialize(reader, "cutoff", mCutoff);
	Deserialize(reader, "intensity", mIntensity);

}