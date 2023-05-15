/*!*****************************************************************************
\file MonoMethods.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 04-01-2023
\brief
This file contains the function declarations of the class MonoMethods.
MonoMethods are where all the mono internal calls functions that scripts need
to use from the engine is being linked.
*******************************************************************************/

#include "MonoMethods.h"
#include "Logger.h"

/*!*****************************************************************************
\brief
MonoString conversion methods.
*******************************************************************************/
std::string MonoMethods::ConvertFromMonoString(MonoString* _string) {
	return (_string == nullptr ? "" : mono_string_to_utf8(_string));
}
MonoString* MonoMethods::ConvertToMonoString(std::string _string) {
	return (_string == "" ? nullptr : mono_string_new(mono_domain_get(), _string.c_str()));
}

/*!*****************************************************************************
\brief
Logger function.
*******************************************************************************/
void MonoMethods::LogGame(MonoString* _log) {
	FUNC->LogGame(MONO->ConvertFromMonoString(_log));
}
void MonoMethods::LogTest(MonoString* _log) {
	FUNC->LogTest(MONO->ConvertFromMonoString(_log));
}

/*!*****************************************************************************
\brief
Player data.
*******************************************************************************/
void MonoMethods::LoadPlayerData(MonoString* _name) {
	FUNC->LoadPlayerData(MONO->ConvertFromMonoString(_name));
}
MonoString* MonoMethods::GetPlayerName() {
	return MONO->ConvertToMonoString(FUNC->GetPlayerName());
}

/*!*****************************************************************************
\brief
Internal Call functions that uses MonoString or needs overloading.
*******************************************************************************/
// Input
bool MonoMethods::iInput::iButton::s_Clicked(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iInput::iButton::Clicked(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
bool MonoMethods::iInput::iButton::s_Released(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iInput::iButton::Released(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
bool MonoMethods::iInput::iButton::s_Hover(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iInput::iButton::Hover(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
bool MonoMethods::iInput::iButton::Clicked(const int _eId) {
	return VI::iInput::iButton::Clicked(_eId);
}
bool MonoMethods::iInput::iButton::Released(const int _eId) {
	return VI::iInput::iButton::Released(_eId);
}
bool MonoMethods::iInput::iButton::Hover(const int _eId) {
	return VI::iInput::iButton::Hover(_eId);
}

// Physics
void MonoMethods::iPhysics::s_ApplyImpulse(MonoString* _entityName, MonoString* _sceneName, const float _impulseX, const float _impulseY, const float _rotationX, const float _rotationY) {
	VI::iPhysics::ApplyImpulse(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _impulseX, _impulseY, _rotationX, _rotationY);
}
void MonoMethods::iPhysics::ApplyImpulse(const int _eId, const float _impulseX, const float _impulseY, const float _rotationX, const float _rotationY) {
	VI::iPhysics::ApplyImpulse(_eId, _impulseX, _impulseY, _rotationX, _rotationY);
}
bool MonoMethods::iPhysics::s_IsCollided(MonoString* _entityName1, MonoString* _entityName2, MonoString* _sceneName) {
	return VI::iPhysics::IsCollided(MONO->ConvertFromMonoString(_entityName1), MONO->ConvertFromMonoString(_entityName2), MONO->ConvertFromMonoString(_sceneName));
}
bool MonoMethods::iPhysics::IsCollided(const int _eId1, const int _eId2) {
	return VI::iPhysics::IsCollided(_eId1, _eId2);
}
bool MonoMethods::iPhysics::s_CheckCollision(MonoString* _entityName1, MonoString* _entityName2, MonoString* _sceneName, bool _dynamicCheck) {
	return VI::iPhysics::CheckCollision(MONO->ConvertFromMonoString(_entityName1), MONO->ConvertFromMonoString(_entityName2), MONO->ConvertFromMonoString(_sceneName), _dynamicCheck);
}
bool MonoMethods::iPhysics::CheckCollision(const int _eId1, const int _eId2, bool _dynamicCheck) {
	return VI::iPhysics::CheckCollision(_eId1, _eId2, _dynamicCheck);
}

// Dialogue
void MonoMethods::iDialogue::LoadScript(MonoString* _filename) {
	VI::iDialogue::LoadScript(MONO->ConvertFromMonoString(_filename));
}
MonoString* MonoMethods::iDialogue::GetLine(int _id) {
	return MONO->ConvertToMonoString(VI::iDialogue::GetLine(_id));
}
MonoString* MonoMethods::iDialogue::GetCurrentLine() {
	return MONO->ConvertToMonoString(VI::iDialogue::GetLine(VI::iDialogue::GetCurrentId()));
}

bool MonoMethods::iDialogue::HaveChoices() {
	return VI::iDialogue::HaveChoices(VI::iDialogue::GetCurrentId());
}
bool MonoMethods::iDialogue::GetCurrentChoice1() {
	return VI::iDialogue::GetChoice1(VI::iDialogue::GetCurrentId());
}
bool MonoMethods::iDialogue::GetCurrentChoice2() {
	return VI::iDialogue::GetChoice2(VI::iDialogue::GetCurrentId());
}
bool MonoMethods::iDialogue::IsPlayerCurrentSpeaker() {
	return VI::iDialogue::IsPlayerSpeaker(VI::iDialogue::GetCurrentId());
}

int MonoMethods::iDialogue::GetNextId() {
	LOG_INFO("Ran monomethod getnextid");
	return VI::iDialogue::GetNextId(VI::iDialogue::GetCurrentId());
}
MonoString* MonoMethods::iDialogue::GetNext() {
	return MONO->ConvertToMonoString(VI::iDialogue::GetNext(VI::iDialogue::GetCurrentId()));
}
bool MonoMethods::iDialogue::NextHaveChoices() {
	return VI::iDialogue::HaveChoices(MM::iDialogue::GetNextId());
}
bool MonoMethods::iDialogue::GetNextChoice1() {
	return VI::iDialogue::GetChoice1(MM::iDialogue::GetNextId());
}
bool MonoMethods::iDialogue::GetNextChoice2() {
	return VI::iDialogue::GetChoice2(MM::iDialogue::GetNextId());
}
bool MonoMethods::iDialogue::IsPlayerNextSpeaker() {
	return VI::iDialogue::IsPlayerSpeaker(MM::iDialogue::GetNextId());
}

// Gamestate
void MonoMethods::iGameState::Go(MonoString* _name) {
	VI::iGameState::ChangeGameState(MONO->ConvertFromMonoString(_name));
}
MonoString* MonoMethods::iGameState::GetCurrentName() {
	return MONO->ConvertToMonoString(VI::iGameState::GetCurrentGameStateName());
}

// Entity
int MonoMethods::iEntity::GetId(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iEntity::GetId(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
bool MonoMethods::iEntity::s_IsActive(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iEntity::IsActive(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iEntity::s_SetIsActive(MonoString* _entityName, MonoString* _sceneName, bool _active) {
	VI::iEntity::SetIsActive(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _active);
}
void MonoMethods::iEntity::s_Activate(MonoString* _entityName, MonoString* _sceneName) {
	VI::iEntity::Activate(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iEntity::s_Deactivate(MonoString* _entityName, MonoString* _sceneName) {
	VI::iEntity::Deactivate(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
MonoString* MonoMethods::iEntity::s_GetParent(MonoString* _entityName, MonoString* _sceneName) {
	return MONO->ConvertToMonoString(VI::iEntity::GetParent(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName)));
}
int MonoMethods::iEntity::s_GetParentId(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iEntity::GetParentId(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}

bool MonoMethods::iEntity::IsActive(const int _id) {
	return VI::iEntity::IsActive(_id);
}
void MonoMethods::iEntity::SetIsActive(const int _id, bool _active) {
	VI::iEntity::SetIsActive(_id, _active);
}
void MonoMethods::iEntity::Activate(const int _id) {
	VI::iEntity::Activate(_id);
}
void MonoMethods::iEntity::Deactivate(const int _id) {
	VI::iEntity::Deactivate(_id);
}
MonoString* MonoMethods::iEntity::GetParent(const int _id) {
	return MONO->ConvertToMonoString(VI::iEntity::GetParent(_id));
}
int MonoMethods::iEntity::GetParentId(const int _id) {
	return VI::iEntity::GetParentId(_id);
}

// Scene
void MonoMethods::iScene::Pause(MonoString* _name) {
	VI::iScene::Pause(MONO->ConvertFromMonoString(_name));
}
void MonoMethods::iScene::Play(MonoString* _name) {
	VI::iScene::Play(MONO->ConvertFromMonoString(_name));
}
bool MonoMethods::iScene::IsPaused(MonoString* _name) {
	return VI::iScene::IsPaused(MONO->ConvertFromMonoString(_name));
}
void MonoMethods::iScene::SetForceRender(MonoString* _name, bool _render) {
	VI::iScene::SetForceRender(MONO->ConvertFromMonoString(_name), _render);
}
bool MonoMethods::iScene::GetForceRender(MonoString* _name) {
	return VI::iScene::GetForceRender(MONO->ConvertFromMonoString(_name));
}

// Animation
void MonoMethods::iAnimation::s_SetSpeed(MonoString* _entityName, MonoString* _sceneName, float _speed) {
	VI::iAnimation::iSpriteSheet::SetSpeed(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _speed);
}
float MonoMethods::iAnimation::s_GetSpeed(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iAnimation::iSpriteSheet::GetSpeed(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iAnimation::s_SetCurrentFrame(MonoString* _entityName, MonoString* _sceneName, int _index) {
	VI::iAnimation::iSpriteSheet::SetCurrentFrame(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _index);
}
int MonoMethods::iAnimation::s_GetCurrentFrame(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iAnimation::iSpriteSheet::GetCurrentFrame(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
int MonoMethods::iAnimation::s_GetFrameCount(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iAnimation::iSpriteSheet::GetFrameCount(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iAnimation::s_SetFrameCount(MonoString* _entityName, MonoString* _sceneName, int _count) {
	return VI::iAnimation::iSpriteSheet::SetFrameCount(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _count);
}
void MonoMethods::iAnimation::s_SetSheetIndex(MonoString* _entityName, MonoString* _sceneName, int _index) {
	VI::iAnimation::iSpriteSheet::SetSheetIndex(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _index);
}
int MonoMethods::iAnimation::s_GetSheetIndex(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iAnimation::iSpriteSheet::GetSheetIndex(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}

void MonoMethods::iAnimation::SetSpeed(const int _eId, float _speed) {
	VI::iAnimation::iSpriteSheet::SetSpeed(_eId, _speed);
}
float MonoMethods::iAnimation::GetSpeed(const int _eId) {
	return VI::iAnimation::iSpriteSheet::GetSpeed(_eId);
}
void MonoMethods::iAnimation::SetCurrentFrame(const int _eId, int _index) {
	VI::iAnimation::iSpriteSheet::SetCurrentFrame(_eId, _index);
}
int MonoMethods::iAnimation::GetCurrentFrame(const int _eId) {
	return VI::iAnimation::iSpriteSheet::GetCurrentFrame(_eId);
}
int MonoMethods::iAnimation::GetFrameCount(const int _eId) {
	return VI::iAnimation::iSpriteSheet::GetFrameCount(_eId);
}
void MonoMethods::iAnimation::SetFrameCount(const int _eId, int _count) {
	return VI::iAnimation::iSpriteSheet::SetFrameCount(_eId, _count);
}
void MonoMethods::iAnimation::SetSheetIndex(const int _eId, int _index) {
	VI::iAnimation::iSpriteSheet::SetSheetIndex(_eId, _index);
}
int MonoMethods::iAnimation::GetSheetIndex(const int _eId) {
	return VI::iAnimation::iSpriteSheet::GetSheetIndex(_eId);
}

// ColorAI
void MonoMethods::iColorAI::StartAnimation(const int _eId) {
	VI::iColorAI::StartAnimation(_eId);
}
void MonoMethods::iColorAI::AddColor(const int _eId, int _r, int _g, int _b, int _a, float _time) {
	VI::iColorAI::AddColor(_eId, _r, _g, _b, _a, _time);
}
void MonoMethods::iColorAI::AddColorAt(const int _eId, int _r, int _g, int _b, int _a, float _time, int _index) {
	VI::iColorAI::AddColorAt(_eId, _r, _g, _b, _a, _time, _index);
}
void MonoMethods::iColorAI::RemoveColorAt(const int _eId, int _index) {
	VI::iColorAI::RemoveColorAt(_eId, _index);
}
bool MonoMethods::iColorAI::SetNextStep(const int _eId, int _i) { // return ture if sucessful (withing 0 to the vector MAX)
	return VI::iColorAI::SetNextStep(_eId, _i);
}
void MonoMethods::iColorAI::StopAfterThisAnimation(const int _eId, bool _next) {
	VI::iColorAI::StopAfterThisAnimation(_eId, _next);
}
void MonoMethods::iColorAI::StopAfterEndofAnimationLoop(const int _eId, bool _loop) {
	VI::iColorAI::StopAfterEndofAnimationLoop(_eId, _loop);
}
void MonoMethods::iColorAI::ReverseOrderAfterNextAnimation(const int _eId, bool _reverse) {
	VI::iColorAI::ReverseOrderAfterNextAnimation(_eId, _reverse);
}
void MonoMethods::iColorAI::SetAnimationLoopToCycle(const int _eId, bool _cycle) {
	VI::iColorAI::SetAnimationLoopToCycle(_eId, _cycle);
}

void MonoMethods::iColorAI::s_StartAnimation(MonoString* _entityName, MonoString* _sceneName) {
	VI::iColorAI::StartAnimation(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iColorAI::s_AddColor(MonoString* _entityName, MonoString* _sceneName, int _r, int _g, int _b, int _a, float _time) {
	VI::iColorAI::AddColor(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _r, _g, _b, _a, _time);
}
void MonoMethods::iColorAI::s_AddColorAt(MonoString* _entityName, MonoString* _sceneName, int _r, int _g, int _b, int _a, float _time, int _index) {
	VI::iColorAI::AddColorAt(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _r, _g, _b, _a, _time, _index);
}
void MonoMethods::iColorAI::s_RemoveColorAt(MonoString* _entityName, MonoString* _sceneName, int _index) {
	VI::iColorAI::RemoveColorAt(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _index);
}
bool MonoMethods::iColorAI::s_SetNextStep(MonoString* _entityName, MonoString* _sceneName, int _i) { // return ture if sucessful (withing 0 to the vector MAX)
	return VI::iColorAI::SetNextStep(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _i);
}
void MonoMethods::iColorAI::s_StopAfterThisAnimation(MonoString* _entityName, MonoString* _sceneName, bool _next) {
	VI::iColorAI::StopAfterThisAnimation(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _next);
}
void MonoMethods::iColorAI::s_StopAfterEndofAnimationLoop(MonoString* _entityName, MonoString* _sceneName, bool _loop) {
	VI::iColorAI::StopAfterEndofAnimationLoop(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _loop);
}
void MonoMethods::iColorAI::s_ReverseOrderAfterNextAnimation(MonoString* _entityName, MonoString* _sceneName, bool _reverse) {
	VI::iColorAI::ReverseOrderAfterNextAnimation(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _reverse);
}
void MonoMethods::iColorAI::s_SetAnimationLoopToCycle(MonoString* _entityName, MonoString* _sceneName, bool _cycle) {
	VI::iColorAI::SetAnimationLoopToCycle(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _cycle);
}

// Particle system
void MonoMethods::iParticleSystem::iParticleInfo::SetTexture(const int _eId, MonoString* _texture) {
	VI::iParticleSystem::iParticleInfo::SetTexture(_eId, MONO->ConvertFromMonoString(_texture));
}

// Sprite
void MonoMethods::iSprite::s_SetTexture(MonoString* _entityName, MonoString* _sceneName, MonoString* _path) {
	VI::iSprite::SetTexture(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), MONO->ConvertFromMonoString(_path));
}
MonoString* MonoMethods::iSprite::s_GetTexture(MonoString* _entityName, MonoString* _sceneName) {
	return MONO->ConvertToMonoString(VI::iSprite::GetTexture(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName)));
}

void MonoMethods::iSprite::SetTexture(const int _eId, MonoString* _path) {
	VI::iSprite::SetTexture(_eId, MONO->ConvertFromMonoString(_path));
}
MonoString* MonoMethods::iSprite::GetTexture(const int _eId) {
	return MONO->ConvertToMonoString(VI::iSprite::GetTexture(_eId));
}

void MonoMethods::iSprite::s_SetType(MonoString* _entityName, MonoString* _sceneName, int _type) {
	VI::iSprite::SetType(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _type);
}
int MonoMethods::iSprite::s_GetType(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iSprite::GetType(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}

void MonoMethods::iSprite::SetType(const int _eId, int _type) {
	VI::iSprite::SetType(_eId, _type);
}
int MonoMethods::iSprite::GetType(const int _eId) {
	return VI::iSprite::GetType(_eId);
}

int MonoMethods::iSprite::GetLayer(const int _eId) {
	return VI::iSprite::GetLayer(_eId);
}
int MonoMethods::iSprite::s_GetLayer(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iSprite::GetLayer(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iSprite::SetLayer(const int _eId, int _layer) {
	VI::iSprite::SetLayer(_eId, _layer);
}
void MonoMethods::iSprite::s_SetLayer(MonoString* _entityName, MonoString* _sceneName, int _layer) {
	VI::iSprite::SetLayer(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _layer);
}

void MonoMethods::iSprite::s_SetColor(MonoString* _entityName, MonoString* _sceneName, int _r, int _g, int _b, int _a) {
	VI::iSprite::SetColor(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _r, _g, _b, _a);
}
void MonoMethods::iSprite::SetColor(const int _eId, int _r, int _g, int _b, int _a) {
	VI::iSprite::SetColor(_eId, _r, _g, _b, _a);
}

void MonoMethods::iSprite::s_SetAlpha(MonoString* _entityName, MonoString* _sceneName, int _a) {
	VI::iSprite::SetAlpha(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _a);
}
void MonoMethods::iSprite::SetAlpha(const int _eId, int _a) {
	VI::iSprite::SetAlpha(_eId, _a);
}

int MonoMethods::iSprite::s_GetColor(MonoString* _entityName, MonoString* _sceneName, int _rgba) {
	return VI::iSprite::GetColor(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _rgba);
}
int MonoMethods::iSprite::GetColor(const int _eId, int _rgba) {
	return VI::iSprite::GetColor(_eId, _rgba);
}

int MonoMethods::iSprite::s_GetAlpha(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iSprite::GetColor(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), 3);
}
int MonoMethods::iSprite::GetAlpha(const int _eId) {
	return VI::iSprite::GetColor(_eId, 3);
}

// Audio
void MonoMethods::iAudio::s_Play(MonoString* _entityName, MonoString* _sceneName) {
	VI::iAudio::Play(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iAudio::s_PlayNow(MonoString* _entityName, MonoString* _sceneName) {
	VI::iAudio::PlayNow(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iAudio::s_PlayOnLoop(MonoString* _entityName, MonoString* _sceneName) {
	VI::iAudio::PlayOnLoop(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iAudio::s_SetLoop(MonoString* _entityName, MonoString* _sceneName, bool _loop) {
	VI::iAudio::SetLoop(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _loop);
}
void MonoMethods::iAudio::s_Stop(MonoString* _entityName, MonoString* _sceneName) {
	VI::iAudio::Stop(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}

void MonoMethods::iAudio::Play(const int _eId) {
	VI::iAudio::Play(_eId);
}
void MonoMethods::iAudio::PlayNow(const int _eId) {
	VI::iAudio::PlayNow(_eId);
}
void MonoMethods::iAudio::PlayOnLoop(const int _eId) {
	VI::iAudio::PlayOnLoop(_eId);
}
void MonoMethods::iAudio::SetLoop(const int _eId, bool _loop) {
	VI::iAudio::SetLoop(_eId, _loop);
}
void MonoMethods::iAudio::Stop(const int _eId) {
	VI::iAudio::Stop(_eId);
}

void MonoMethods::iAudio::PlayBGM(MonoString* _soundName) {
	VI::iAudio::PlayBGM(MONO->ConvertFromMonoString(_soundName));
}
bool MonoMethods::iAudio::IsBgmPlaying() {
	return VI::iAudio::IsBgmPlaying();
}
//void MonoMethods::iAudio::ForcePlay(MonoString* _soundName) {
//	VI::iAudio::ForcePlay(MONO->ConvertFromMonoString(_soundName));
//}

// Transform
float MonoMethods::iTransform::s_GetPosX(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iTransform::GetPosX(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
float MonoMethods::iTransform::s_GetPosY(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iTransform::GetPosY(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iTransform::s_SetPosX(MonoString* _entityName, MonoString* _sceneName, float _posX) {
	VI::iTransform::SetPosX(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _posX);
}
void MonoMethods::iTransform::s_SetPosY(MonoString* _entityName, MonoString* _sceneName, float _posY) {
	VI::iTransform::SetPosY(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _posY);
}
float MonoMethods::iTransform::s_GetScaleX(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iTransform::GetScaleX(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
float MonoMethods::iTransform::s_GetScaleY(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iTransform::GetScaleY(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iTransform::s_SetScaleX(MonoString* _entityName, MonoString* _sceneName, float _scaleX) {
	VI::iTransform::SetScaleX(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _scaleX);
}
void MonoMethods::iTransform::s_SetScaleY(MonoString* _entityName, MonoString* _sceneName, float _scaleY) {
	VI::iTransform::SetScaleY(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _scaleY);
}
float MonoMethods::iTransform::s_GetRotate(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iTransform::GetRotate(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iTransform::s_SetRotate(MonoString* _entityName, MonoString* _sceneName, float _rotate) {
	VI::iTransform::SetRotate(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _rotate);
}

float MonoMethods::iTransform::GetPosX(const int _eId) {
	return VI::iTransform::GetPosX(Entity(_eId));
}
float MonoMethods::iTransform::GetPosY(const int _eId) {
	return VI::iTransform::GetPosY(Entity(_eId));
}
void MonoMethods::iTransform::SetPosX(const int _eId, float _posX) {
	VI::iTransform::SetPosX(Entity(_eId), _posX);
}
void MonoMethods::iTransform::SetPosY(const int _eId, float _posY) {
	VI::iTransform::SetPosY(Entity(_eId), _posY);
}
float MonoMethods::iTransform::GetScaleX(const int _eId) {
	return VI::iTransform::GetScaleX(Entity(_eId));
}
float MonoMethods::iTransform::GetScaleY(const int _eId) {
	return VI::iTransform::GetScaleY(Entity(_eId));
}
void MonoMethods::iTransform::SetScaleX(const int _eId, float _scaleX) {
	VI::iTransform::SetScaleX(Entity(_eId), _scaleX);
}
void MonoMethods::iTransform::SetScaleY(const int _eId, float _scaleY) {
	VI::iTransform::SetScaleY(Entity(_eId), _scaleY);
}
float MonoMethods::iTransform::GetRotate(const int _eId) {
	return VI::iTransform::GetRotate(Entity(_eId));
}
void MonoMethods::iTransform::SetRotate(const int _eId, float _rotate) {
	VI::iTransform::SetRotate(Entity(_eId), _rotate);
}

// Text
void MonoMethods::iText::s_UpdateText(MonoString* _entityName, MonoString* _sceneName, MonoString* _text) {
	VI::iText::UpdateText(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), MONO->ConvertFromMonoString(_text));
}
void MonoMethods::iText::s_SetOffset(MonoString* _entityName, MonoString* _sceneName, float _xoffset, float _yoffset) {
	VI::iText::SetOffset(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _xoffset, _yoffset);
}
float MonoMethods::iText::s_GetOffsetX(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iText::GetOffsetX(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
float MonoMethods::iText::s_GetOffsetY(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iText::GetOffsetY(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
int MonoMethods::iText::s_GetLineCount(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iText::GetLineCount(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
float MonoMethods::iText::s_GetScale(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iText::GetScale(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void MonoMethods::iText::s_SetScale(MonoString* _entityName, MonoString* _sceneName, float _scale) {
	VI::iText::SetScale(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _scale);
}

void MonoMethods::iText::UpdateText(const int _eId, MonoString* _text) {
	VI::iText::UpdateText(_eId, MONO->ConvertFromMonoString(_text));
}
void MonoMethods::iText::SetOffset(const int _eId, float _xoffset, float _yoffset) {
	VI::iText::SetOffset(_eId, _xoffset, _yoffset);
}
float MonoMethods::iText::GetOffsetX(const int _eId) {
	return VI::iText::GetOffsetX(_eId);
}
float MonoMethods::iText::GetOffsetY(const int _eId) {
	return VI::iText::GetOffsetY(_eId);
}
int MonoMethods::iText::GetLineCount(const int _eId) {
	return VI::iText::GetLineCount(_eId);
}
float MonoMethods::iText::GetScale(const int _eId) {
	return VI::iText::GetScale(_eId);
}
void MonoMethods::iText::SetScale(const int _eId, float _scale) {
	VI::iText::SetScale(_eId, _scale);
}

// LightSource
float MonoMethods::iLightSource::s_GetRadius(MonoString* _entityName, MonoString* _sceneName) {
	return VI::iLightSource::GetRadius(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName));
}
void  MonoMethods::iLightSource::s_SetRadius(MonoString* _entityName, MonoString* _sceneName, float _radius) {
	VI::iLightSource::SetRadius(MONO->ConvertFromMonoString(_entityName), MONO->ConvertFromMonoString(_sceneName), _radius);
}
float MonoMethods::iLightSource::GetRadius(const int _eId) {
	return VI::iLightSource::GetRadius(_eId);
}
void MonoMethods::iLightSource::SetRadius(const int _eId, float _radius) {
	VI::iLightSource::SetRadius(_eId, _radius);
}

/*!*****************************************************************************
\brief
	Registers the internal calls for mono use.
*******************************************************************************/
void MonoMethods::RegisterCalls() {
	// Test
	mono_add_internal_call("VI.Test::ArgString", &FUNC->TestArgString);
	mono_add_internal_call("VI.Test::ReturnString", &FUNC->TestReturnString);
	//mono_add_internal_call("BonVoyage.THIS::IsActive", &MM::iEntity::IsActive);
	
	// Logger
	mono_add_internal_call("LOG::WRITE", &MONO->LogGame);
	mono_add_internal_call("LOG::TEST", &MONO->LogTest);

	// Player data
	mono_add_internal_call("VI.Data::Load", &MONO->LoadPlayerData);
	mono_add_internal_call("VI.Data::Save", &FUNC->SavePlayerData);
	mono_add_internal_call("VI.Data::GetPlayerName", &MONO->GetPlayerName);
	mono_add_internal_call("VI.Data::GetLevelAt", &FUNC->GetLevelAt);
	mono_add_internal_call("VI.Data::SetLevelAt", &FUNC->SetLevelAt);
	mono_add_internal_call("VI.Data::GetData1", &FUNC->GetData1);
	mono_add_internal_call("VI.Data::SetData1", &FUNC->SetData1);
	mono_add_internal_call("VI.Data::GetData2", &FUNC->GetData2);
	mono_add_internal_call("VI.Data::SetData2", &FUNC->SetData2);

	// General
	mono_add_internal_call("VI.General::DeltaTime", &FUNC->GetDeltaTime);

	// Inputs
	mono_add_internal_call("VI.Input/Key::Press", &VI::iInput::iKey::Press);
	mono_add_internal_call("VI.Input/Key::Hold", &VI::iInput::iKey::Hold);
	mono_add_internal_call("VI.Input/Key::Release", &VI::iInput::iKey::Release);
	mono_add_internal_call("VI.Input/Key::Idle", &VI::iInput::iKey::Idle);

	mono_add_internal_call("VI.Input/Button::s_Clicked", &MM::iInput::iButton::s_Clicked);
	mono_add_internal_call("VI.Input/Button::s_Released", &MM::iInput::iButton::s_Released);
	mono_add_internal_call("VI.Input/Button::s_Hover", &MM::iInput::iButton::s_Hover);

	mono_add_internal_call("VI.Input/Button::Clicked", &MM::iInput::iButton::Clicked);
	mono_add_internal_call("VI.Input/Button::Released", &MM::iInput::iButton::Released);
	mono_add_internal_call("VI.Input/Button::Hover", &MM::iInput::iButton::Hover);

	mono_add_internal_call("VI.Input/Button::AllNotHover", &VI::iInput::iButton::AllNotHover);
	mono_add_internal_call("VI.Input/Button::AllNotClick", &VI::iInput::iButton::AllNotClick);

	mono_add_internal_call("VI.Input/Mouse::WorldPosX", &VI::iInput::iMouse::GetWorldPosX);
	mono_add_internal_call("VI.Input/Mouse::WorldPosY", &VI::iInput::iMouse::GetWorldPosY);

	mono_add_internal_call("VI.Input/Mouse::Press", &VI::iInput::iKey::Press);
	mono_add_internal_call("VI.Input/Mouse::Hold", &VI::iInput::iKey::Hold);
	mono_add_internal_call("VI.Input/Mouse::Release", &VI::iInput::iKey::Release);
	mono_add_internal_call("VI.Input/Mouse::Idle", &VI::iInput::iKey::Idle);

	// Physics
	mono_add_internal_call("VI.Physics::s_ApplyImpulse", &MM::iPhysics::s_ApplyImpulse);
	mono_add_internal_call("VI.Physics::s_IsCollided", &MM::iPhysics::s_IsCollided);
	mono_add_internal_call("VI.Physics::s_CheckCollision", &MM::iPhysics::s_CheckCollision);

	mono_add_internal_call("VI.Physics::ApplyImpulse", &MM::iPhysics::ApplyImpulse);
	mono_add_internal_call("VI.Physics::IsCollided", &MM::iPhysics::IsCollided);
	mono_add_internal_call("VI.Physics::CheckCollision", &MM::iPhysics::CheckCollision);

	// Dialogue
	mono_add_internal_call("VI.Dialogue::LoadScript", &MM::iDialogue::LoadScript);
	mono_add_internal_call("VI.Dialogue::GetLine", &MM::iDialogue::GetLine);
	mono_add_internal_call("VI.Dialogue::HaveChoices", &VI::iDialogue::HaveChoices);
	mono_add_internal_call("VI.Dialogue::GetSelectedChoice", &VI::iDialogue::GetSelectedChoice);
	mono_add_internal_call("VI.Dialogue::GetNext", &VI::iDialogue::GetNext);
	mono_add_internal_call("VI.Dialogue::GetNextId", &VI::iDialogue::GetNextId);

	mono_add_internal_call("VI.Dialogue/Current::GetLine", &MM::iDialogue::GetCurrentLine);
	mono_add_internal_call("VI.Dialogue/Current::GetId", &VI::iDialogue::GetCurrentId);
	mono_add_internal_call("VI.Dialogue/Current::HaveChoices", &MM::iDialogue::HaveChoices);
	mono_add_internal_call("VI.Dialogue/Current::SetTo", &VI::iDialogue::SetCurrentId);
	mono_add_internal_call("VI.Dialogue/Current::GetFirstChoice", &MM::iDialogue::GetCurrentChoice1);
	mono_add_internal_call("VI.Dialogue/Current::GetSecondChoice", &MM::iDialogue::GetCurrentChoice2);
	mono_add_internal_call("VI.Dialogue/Current::SetSelectedChoice", &VI::iDialogue::SetSelectedChoice);
	mono_add_internal_call("VI.Dialogue/Current::GetSelectedChoice", &VI::iDialogue::GetCurrentSelectedChoice);
	mono_add_internal_call("VI.Dialogue/current::PlayerSpeaking", &MM::iDialogue::IsPlayerCurrentSpeaker);

	mono_add_internal_call("VI.Dialogue/Next::GetLine", &MM::iDialogue::GetNext);
	mono_add_internal_call("VI.Dialogue/Next::GetId", &VI::iDialogue::GetNextId);
	mono_add_internal_call("VI.Dialogue/Next::HaveChoices", &MM::iDialogue::NextHaveChoices);
	mono_add_internal_call("VI.Dialogue/Next::GetFirstChoice", &MM::iDialogue::GetNextChoice1);
	mono_add_internal_call("VI.Dialogue/Next::GetSecondChoice", &MM::iDialogue::GetNextChoice2);
	mono_add_internal_call("VI.Dialogue/Next::PlayerSpeaking", &MM::iDialogue::IsPlayerNextSpeaker);

	mono_add_internal_call("VI.Dialogue/Choice::First", &VI::iDialogue::GetChoice1);
	mono_add_internal_call("VI.Dialogue/Choice::Second", &VI::iDialogue::GetChoice2);
	mono_add_internal_call("VI.Dialogue/Choice::Selected", &VI::iDialogue::SetSelectedChoice);
	mono_add_internal_call("VI.Dialogue/Speaker::IsPlayer", &VI::iDialogue::IsPlayerSpeaker);

	// Gamestate
	mono_add_internal_call("VI.GameState::Go", &MM::iGameState::Go);
	mono_add_internal_call("VI.GameState::GetName", &MM::iGameState::GetCurrentName);
	mono_add_internal_call("VI.GameState::Exit", &VI::iGameState::GameStateExit);

	// Entity
	mono_add_internal_call("VI.Entity::GetId", &MM::iEntity::GetId);

	mono_add_internal_call("VI.Entity::s_IsActive", &MM::iEntity::s_IsActive);
	mono_add_internal_call("VI.Entity::s_SetActive", &MM::iEntity::s_SetIsActive);
	mono_add_internal_call("VI.Entity::s_Activate", &MM::iEntity::s_Activate);
	mono_add_internal_call("VI.Entity::s_Deactivate", &MM::iEntity::s_Deactivate);
	mono_add_internal_call("VI.Entity/Parent::s_Name", &MM::iEntity::s_GetParent);
	mono_add_internal_call("VI.Entity/Parent::s_Id", &MM::iEntity::s_GetParentId);

	mono_add_internal_call("VI.Entity::IsActive", &MM::iEntity::IsActive);
	mono_add_internal_call("VI.Entity::SetActive", &MM::iEntity::SetIsActive);
	mono_add_internal_call("VI.Entity::Activate", &MM::iEntity::Activate);
	mono_add_internal_call("VI.Entity::Deactivate", &MM::iEntity::Deactivate);
	mono_add_internal_call("VI.Entity/Parent::Name", &MM::iEntity::GetParent);
	mono_add_internal_call("VI.Entity/Parent::Id", &MM::iEntity::GetParentId);

	// Scene
	mono_add_internal_call("VI.Scene::Pause", &MM::iScene::Pause);
	mono_add_internal_call("VI.Scene::Play", &MM::iScene::Play);
	mono_add_internal_call("VI.Scene::IsPaused", &MM::iScene::IsPaused);
	mono_add_internal_call("VI.Scene::SetForceRender", &MM::iScene::SetForceRender);
	mono_add_internal_call("VI.Scene::GetForceRender", &MM::iScene::GetForceRender);

	// Camera
	mono_add_internal_call("VI.Camera/GetScale::X", &VI::iCamera::GetScaleX);
	mono_add_internal_call("VI.Camera/GetScale::Y", &VI::iCamera::GetScaleY);
	mono_add_internal_call("VI.Camera/GetPos::X", &VI::iCamera::GetPosX);
	mono_add_internal_call("VI.Camera/GetPos::Y", &VI::iCamera::GetPosY);
	mono_add_internal_call("VI.Camera/SetScale::X", &VI::iCamera::SetScaleX);
	mono_add_internal_call("VI.Camera/SetScale::Y", &VI::iCamera::SetScaleY);
	mono_add_internal_call("VI.Camera/SetPos::X", &VI::iCamera::SetPosX);
	mono_add_internal_call("VI.Camera/SetPos::Y", &VI::iCamera::SetPosY);

	// Window
	mono_add_internal_call("VI.Window::GetScreenWidth", &VI::iWindow::GetScreenWidth);
	mono_add_internal_call("VI.Window::GetScreenHeight", &VI::iWindow::GetScreenHeight);
	mono_add_internal_call("VI.Window::GetFullScreen", &VI::iWindow::GetFullScreen);
	mono_add_internal_call("VI.Window::SetFullScreen", &VI::iWindow::SetFullScreen);

	// Viewport
	mono_add_internal_call("VI.Viewport::GetWidth", &VI::iViewport::GetWidth);
	mono_add_internal_call("VI.Viewport::SetWidth", &VI::iViewport::SetWidth);
	mono_add_internal_call("VI.Viewport::IsUI", &VI::iViewport::IsUI);
	mono_add_internal_call("VI.Viewport::SetIsUI", &VI::iViewport::SetIsUI);

	// Math
	mono_add_internal_call("VI.Math::SqMagnitude", &VI::iMath::SqMagnitude);
	mono_add_internal_call("VI.Math::Magnitude", &VI::iMath::Magnitude);
	mono_add_internal_call("VI.Math/Normalize::X", &VI::iMath::NormalizeX);
	mono_add_internal_call("VI.Math/Normalize::Y", &VI::iMath::NormalizeY);
	mono_add_internal_call("VI.Math::ArcTangent", &VI::iMath::ArcTangent);
	mono_add_internal_call("VI.Math::Negate", &VI::iMath::Negate);
	mono_add_internal_call("VI.Math::Epsilon", &VI::iMath::Epsilon);
	mono_add_internal_call("VI.Math::Pi", &VI::iMath::Pi);
	mono_add_internal_call("VI.Math::ConvertRadToDeg", &VI::iMath::ConvertRadToDeg);
	mono_add_internal_call("VI.Math::ConvertDegToRad", &VI::iMath::ConvertDegToRad);

	// Animation
	mono_add_internal_call("VI.Animation/SheetIndex::s_Set", &MM::iAnimation::s_SetSheetIndex);
	mono_add_internal_call("VI.Animation/SheetIndex::s_Get", &MM::iAnimation::s_GetSheetIndex);
	mono_add_internal_call("VI.Animation/Speed::s_Set", &MM::iAnimation::s_SetSpeed);
	mono_add_internal_call("VI.Animation/Speed::s_Get", &MM::iAnimation::s_GetSpeed);
	mono_add_internal_call("VI.Animation/CurrentFrame::s_Set", &MM::iAnimation::s_SetCurrentFrame);
	mono_add_internal_call("VI.Animation/CurrentFrame::s_Get", &MM::iAnimation::s_GetCurrentFrame);
	mono_add_internal_call("VI.Animation/FrameCount::s_Set", &MM::iAnimation::s_SetFrameCount);
	mono_add_internal_call("VI.Animation/FrameCount::s_Get", &MM::iAnimation::s_GetFrameCount);

	mono_add_internal_call("VI.Animation/SheetIndex::Set", &MM::iAnimation::SetSheetIndex);
	mono_add_internal_call("VI.Animation/SheetIndex::Get", &MM::iAnimation::GetSheetIndex);
	mono_add_internal_call("VI.Animation/Speed::Set", &MM::iAnimation::SetSpeed);
	mono_add_internal_call("VI.Animation/Speed::Get", &MM::iAnimation::GetSpeed);
	mono_add_internal_call("VI.Animation/CurrentFrame::Set", &MM::iAnimation::SetCurrentFrame);
	mono_add_internal_call("VI.Animation/CurrentFrame::Get", &MM::iAnimation::GetCurrentFrame);
	mono_add_internal_call("VI.Animation/FrameCount::Set", &MM::iAnimation::SetFrameCount);
	mono_add_internal_call("VI.Animation/FrameCount::Get", &MM::iAnimation::GetFrameCount);

	// MovementAI
	mono_add_internal_call("VI.MovementAI::Run", &VI::iMovementAI::Start);
	mono_add_internal_call("VI.MovementAI::ForceStop", &VI::iMovementAI::ForceStop);
	mono_add_internal_call("VI.MovementAI::SetNext", &VI::iMovementAI::SetNext);
	mono_add_internal_call("VI.MovementAI::GoToNext", &VI::iMovementAI::GoToNext);
	mono_add_internal_call("VI.MovementAI::Stop", &VI::iMovementAI::Stop);
	mono_add_internal_call("VI.MovementAI::StopAfterEndLoop", &VI::iMovementAI::StopAfterEndLoop);
	mono_add_internal_call("VI.MovementAI::ReverseOrder", &VI::iMovementAI::ReverseOrder);
	mono_add_internal_call("VI.MovementAI::SetLoopCycle", &VI::iMovementAI::SetLoopCycle);
	mono_add_internal_call("VI.MovementAI::GetCurrentIndex", &VI::iMovementAI::GetCurrentIndex);
	mono_add_internal_call("VI.MovementAI::Remove", &VI::iMovementAI::Remove);
	mono_add_internal_call("VI.MovementAI/Edit::Timing", &VI::iMovementAI::EditTiming);
	mono_add_internal_call("VI.MovementAI/Edit::CurrentTiming", &VI::iMovementAI::EditCurrentTiming);
	mono_add_internal_call("VI.MovementAI/Edit::Current", &VI::iMovementAI::EditCurrent);
	mono_add_internal_call("VI.MovementAI/Edit::CurrentScaleX", &VI::iMovementAI::CurrentScaleX);
	mono_add_internal_call("VI.MovementAI/Edit::CurrentScaleY", &VI::iMovementAI::CurrentScaleY);
	mono_add_internal_call("VI.MovementAI/Edit::CurrentRotate", &VI::iMovementAI::CurrentRotate);
	mono_add_internal_call("VI.MovementAI/Edit::CurrentPosX", &VI::iMovementAI::CurrentPosX);
	mono_add_internal_call("VI.MovementAI/Edit::CurrentPosY", &VI::iMovementAI::CurrentPosY);
	mono_add_internal_call("VI.MovementAI/Get::Timing", &VI::iMovementAI::GetTiming);
	mono_add_internal_call("VI.MovementAI/Get::CurrentTiming", &VI::iMovementAI::GetCurrentTiming);
	mono_add_internal_call("VI.MovementAI/Get::CurrentScaleX", &VI::iMovementAI::GetCurrentScaleX);
	mono_add_internal_call("VI.MovementAI/Get::CurrentScaleY", &VI::iMovementAI::GetCurrentScaleY);
	mono_add_internal_call("VI.MovementAI/Get::CurrentRotate", &VI::iMovementAI::GetCurrentRotate);
	mono_add_internal_call("VI.MovementAI/Get::CurrentPosX", &VI::iMovementAI::GetCurrentPosX);
	mono_add_internal_call("VI.MovementAI/Get::CurrentPosY", &VI::iMovementAI::GetCurrentPosY);
	mono_add_internal_call("VI.MovementAI/Add::Transform", &VI::iMovementAI::AddTransform);
	mono_add_internal_call("VI.MovementAI/Add::TransformAt", &VI::iMovementAI::AddTransformAt);
	mono_add_internal_call("VI.MovementAI/Add::TransformScaleAt", &VI::iMovementAI::TransformScaleAt);
	mono_add_internal_call("VI.MovementAI/Add::TransformRotateAt", &VI::iMovementAI::TransformRotateAt);
	mono_add_internal_call("VI.MovementAI/Add::TransformPosAt", &VI::iMovementAI::TransformPosAt);
	mono_add_internal_call("VI.MovementAI/Add::TransformDifference", &VI::iMovementAI::AddTransformDifference);
	mono_add_internal_call("VI.MovementAI/SetCalculatedTime::FromPosition", &VI::iMovementAI::SetCalculatedTimeFromPosition);
	mono_add_internal_call("VI.MovementAI/SetCalculatedTime::FromRotation", &VI::iMovementAI::SetCalculatedTimeFromRotation);
	mono_add_internal_call("VI.MovementAI/SetCalculatedTime::FromScale", &VI::iMovementAI::SetCalculatedTimeFromScale);
	mono_add_internal_call("VI.MovementAI/AddAtCurrent::Transform", &VI::iMovementAI::AddTransformAtCurrent);
	mono_add_internal_call("VI.MovementAI/AddAtCurrent::TransformScale", &VI::iMovementAI::TransformScaleAtCurrent);
	mono_add_internal_call("VI.MovementAI/AddAtCurrent::TransformRotate", &VI::iMovementAI::TransformRotateAtCurrent);
	mono_add_internal_call("VI.MovementAI/AddAtCurrent::TransformPos", &VI::iMovementAI::TransformPosAtCurrent);

	// ColorAI
	mono_add_internal_call("VI.ColorAI::StartAnimation", &MM::iColorAI::StartAnimation);
	mono_add_internal_call("VI.ColorAI::AddColor", &MM::iColorAI::AddColor);
	mono_add_internal_call("VI.ColorAI::AddColorAt", &MM::iColorAI::AddColorAt);
	mono_add_internal_call("VI.ColorAI::RemoveColorAt", &MM::iColorAI::RemoveColorAt);
	mono_add_internal_call("VI.ColorAI::SetNextStep", &MM::iColorAI::SetNextStep);
	mono_add_internal_call("VI.ColorAI/Stop::AfterThisAnimation", &MM::iColorAI::StopAfterThisAnimation);
	mono_add_internal_call("VI.ColorAI/Stop::AfterEndofAnimationLoop", &MM::iColorAI::StopAfterEndofAnimationLoop);
	mono_add_internal_call("VI.ColorAI::ReverseOrderAfterNextAnimation", &MM::iColorAI::ReverseOrderAfterNextAnimation);
	mono_add_internal_call("VI.ColorAI::SetAnimationLoopToCycle", &MM::iColorAI::SetAnimationLoopToCycle);

	mono_add_internal_call("VI.ColorAI::s_StartAnimation", &MM::iColorAI::s_StartAnimation);
	mono_add_internal_call("VI.ColorAI::s_AddColor", &MM::iColorAI::s_AddColor);
	mono_add_internal_call("VI.ColorAI::s_AddColorAt", &MM::iColorAI::s_AddColorAt);
	mono_add_internal_call("VI.ColorAI::s_RemoveColorAt", &MM::iColorAI::s_RemoveColorAt);
	mono_add_internal_call("VI.ColorAI::s_SetNextStep", &MM::iColorAI::s_SetNextStep);
	mono_add_internal_call("VI.ColorAI/Stop::s_AfterThisAnimation", &MM::iColorAI::s_StopAfterThisAnimation);
	mono_add_internal_call("VI.ColorAI/Stop::s_AfterEndofAnimationLoop", &MM::iColorAI::s_StopAfterEndofAnimationLoop);
	mono_add_internal_call("VI.ColorAI::s_ReverseOrderAfterNextAnimation", &MM::iColorAI::s_ReverseOrderAfterNextAnimation);
	mono_add_internal_call("VI.ColorAI::s_SetAnimationLoopToCycle", &MM::iColorAI::s_SetAnimationLoopToCycle);

	// Particle System
	mono_add_internal_call("VI.ParticleSystem::GetDensity", &VI::iParticleSystem::GetDensity);
	mono_add_internal_call("VI.ParticleSystem::SetDensity", &VI::iParticleSystem::SetDensity);
	mono_add_internal_call("VI.ParticleSystem::GetPosX", &VI::iParticleSystem::GetPosX);
	mono_add_internal_call("VI.ParticleSystem::SetPosX", &VI::iParticleSystem::SetPosX);
	mono_add_internal_call("VI.ParticleSystem::GetPosY", &VI::iParticleSystem::GetPosY);
	mono_add_internal_call("VI.ParticleSystem::SetPosY", &VI::iParticleSystem::SetPosY);
	mono_add_internal_call("VI.ParticleSystem::GetWidth", &VI::iParticleSystem::GetWidth);
	mono_add_internal_call("VI.ParticleSystem::SetWidth", &VI::iParticleSystem::SetWidth);
	mono_add_internal_call("VI.ParticleSystem::GetDir", &VI::iParticleSystem::GetDir);
	mono_add_internal_call("VI.ParticleSystem::SetDir", &VI::iParticleSystem::SetDir);
	mono_add_internal_call("VI.ParticleSystem::GetSpread", &VI::iParticleSystem::GetSpread);
	mono_add_internal_call("VI.ParticleSystem::SetSpread", &VI::iParticleSystem::SetSpread);
	mono_add_internal_call("VI.ParticleSystem::GetDuration", &VI::iParticleSystem::GetDuration);
	mono_add_internal_call("VI.ParticleSystem::SetDuration", &VI::iParticleSystem::SetDuration);
	mono_add_internal_call("VI.ParticleSystem::IsActive", &VI::iParticleSystem::IsActive);
	mono_add_internal_call("VI.ParticleSystem::SetIsActive", &VI::iParticleSystem::SetIsActive);
	mono_add_internal_call("VI.ParticleSystem::GetSlow", &VI::iParticleSystem::GetSlow);
	mono_add_internal_call("VI.ParticleSystem::SetSlow", &VI::iParticleSystem::SetSlow);
	mono_add_internal_call("VI.ParticleSystem::GenerateOnce", &VI::iParticleSystem::GenerateOnce);
	mono_add_internal_call("VI.ParticleSystem::GenerateLoop", &VI::iParticleSystem::GenerateLoop);
	
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::GetScale", &VI::iParticleSystem::iParticleInfo::GetScale);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::GetDirection", &VI::iParticleSystem::iParticleInfo::GetDirection);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::GetLifeSpan", &VI::iParticleSystem::iParticleInfo::GetLifeSpan);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::GetRotation", &VI::iParticleSystem::iParticleInfo::GetRotation);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::GetSpeed", &VI::iParticleSystem::iParticleInfo::GetSpeed);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::GetFading", &VI::iParticleSystem::iParticleInfo::GetFading);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::GetSpriteType", &VI::iParticleSystem::iParticleInfo::GetSpriteType);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::GetLayer", &VI::iParticleSystem::iParticleInfo::GetLayer);

	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetScale", &VI::iParticleSystem::iParticleInfo::SetScale);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetDirection", &VI::iParticleSystem::iParticleInfo::SetDirection);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetLifeSpan", &VI::iParticleSystem::iParticleInfo::SetLifeSpan);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetRotation", &VI::iParticleSystem::iParticleInfo::SetRotation);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetSpeed", &VI::iParticleSystem::iParticleInfo::SetSpeed);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetFading", &VI::iParticleSystem::iParticleInfo::SetFading);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetColour", &VI::iParticleSystem::iParticleInfo::SetColour);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetSpriteType", &VI::iParticleSystem::iParticleInfo::SetSpriteType);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetTexture", &MM::iParticleSystem::iParticleInfo::SetTexture);
	mono_add_internal_call("VI.ParticleSystem/ParticleInfo::SetLayer", &VI::iParticleSystem::iParticleInfo::SetLayer);

	// Sprite
	mono_add_internal_call("VI.Sprite::s_SetTexture", &MM::iSprite::s_SetTexture);
	mono_add_internal_call("VI.Sprite::s_GetTexture", &MM::iSprite::s_GetTexture);
	mono_add_internal_call("VI.Sprite::SetTexture", &MM::iSprite::SetTexture);
	mono_add_internal_call("VI.Sprite::GetTexture", &MM::iSprite::GetTexture);

	mono_add_internal_call("VI.Sprite::GetType", &MM::iSprite::GetType);
	mono_add_internal_call("VI.Sprite::SetType", &MM::iSprite::SetType);
	mono_add_internal_call("VI.Sprite::s_GetType", &MM::iSprite::s_GetType);
	mono_add_internal_call("VI.Sprite::s_SetType", &MM::iSprite::s_SetType);

	mono_add_internal_call("VI.Sprite::GetLayer", &MM::iSprite::GetLayer);
	mono_add_internal_call("VI.Sprite::SetLayer", &MM::iSprite::SetLayer);
	mono_add_internal_call("VI.Sprite::s_GetLayer", &MM::iSprite::s_GetLayer);
	mono_add_internal_call("VI.Sprite::s_SetLayer", &MM::iSprite::s_SetLayer);

	mono_add_internal_call("VI.Sprite::s_SetColor", &MM::iSprite::s_SetColor);
	mono_add_internal_call("VI.Sprite::SetColor", &MM::iSprite::SetColor);
	mono_add_internal_call("VI.Sprite::s_SetAlpha", &MM::iSprite::s_SetAlpha);
	mono_add_internal_call("VI.Sprite::SetAlpha", &MM::iSprite::SetAlpha);

	mono_add_internal_call("VI.Sprite::s_GetColor", &MM::iSprite::s_GetColor);
	mono_add_internal_call("VI.Sprite::GetColor", &MM::iSprite::GetColor);
	mono_add_internal_call("VI.Sprite::s_GetAlpha", &MM::iSprite::s_GetAlpha);
	mono_add_internal_call("VI.Sprite::GetAlpha", &MM::iSprite::GetAlpha);

	// Audio
	mono_add_internal_call("VI.Audio::Play", &MM::iAudio::Play);
	mono_add_internal_call("VI.Audio::PlayNow", &MM::iAudio::PlayNow);
	mono_add_internal_call("VI.Audio::PlayOnLoop", &MM::iAudio::PlayOnLoop);
	mono_add_internal_call("VI.Audio::SetLoop", &MM::iAudio::SetLoop);
	mono_add_internal_call("VI.Audio::Stop", &MM::iAudio::Stop);

	mono_add_internal_call("VI.Audio::PlayBGM", &MM::iAudio::PlayBGM);
	mono_add_internal_call("VI.Audio::IsBgmPlaying", &MM::iAudio::IsBgmPlaying);
	//mono_add_internal_call("VI.Audio::ForcePlay", &MM::iAudio::ForcePlay);

	mono_add_internal_call("VI.Audio::StopAll", &VI::iAudio::StopAll);
	mono_add_internal_call("VI.Audio::StopBGM", &VI::iAudio::StopBGM);
	mono_add_internal_call("VI.Audio::StopSFX", &VI::iAudio::StopSFX);

	mono_add_internal_call("VI.Audio/Volume::SetVolume", &VI::iAudio::SetAllVolume);
	mono_add_internal_call("VI.Audio/Volume::SetBGMVolume", &VI::iAudio::SetBGMVolume);
	mono_add_internal_call("VI.Audio/Volume::SetSFXVolume", &VI::iAudio::SetSFXVolume);
	mono_add_internal_call("VI.Audio/Volume::GetBgmVol", &VI::iAudio::GetBgmVol);
	mono_add_internal_call("VI.Audio/Volume::GetSfxVol", &VI::iAudio::GetSfxVol);

	mono_add_internal_call("VI.Audio::s_Stop", &MM::iAudio::s_Stop);
	mono_add_internal_call("VI.Audio::s_Play", &MM::iAudio::s_Play);
	mono_add_internal_call("VI.Audio::s_PlayNow", &MM::iAudio::s_PlayNow);
	mono_add_internal_call("VI.Audio::s_PlayOnLoop", &MonoMethods::iAudio::s_PlayOnLoop);
	mono_add_internal_call("VI.Audio::s_SetLoop", &MM::iAudio::s_SetLoop);

	// Transform
	mono_add_internal_call("VI.Transform/Position::s_GetX", &MM::iTransform::s_GetPosX);
	mono_add_internal_call("VI.Transform/Position::s_GetY", &MM::iTransform::s_GetPosY);
	mono_add_internal_call("VI.Transform/Position::s_SetX", &MM::iTransform::s_SetPosX);
	mono_add_internal_call("VI.Transform/Position::s_SetY", &MM::iTransform::s_SetPosY);

	mono_add_internal_call("VI.Transform/Position::GetX", &MM::iTransform::GetPosX);
	mono_add_internal_call("VI.Transform/Position::GetY", &MM::iTransform::GetPosY);
	mono_add_internal_call("VI.Transform/Position::SetX", &MM::iTransform::SetPosX);
	mono_add_internal_call("VI.Transform/Position::SetY", &MM::iTransform::SetPosY);

	mono_add_internal_call("VI.Transform/Scale::s_GetX", &MM::iTransform::s_GetScaleX);
	mono_add_internal_call("VI.Transform/Scale::s_GetY", &MM::iTransform::s_GetScaleY);
	mono_add_internal_call("VI.Transform/Scale::s_SetX", &MM::iTransform::s_SetScaleX);
	mono_add_internal_call("VI.Transform/Scale::s_SetY", &MM::iTransform::s_SetScaleY);

	mono_add_internal_call("VI.Transform/Scale::GetX", &MM::iTransform::GetScaleX);
	mono_add_internal_call("VI.Transform/Scale::GetY", &MM::iTransform::GetScaleY);
	mono_add_internal_call("VI.Transform/Scale::SetX", &MM::iTransform::SetScaleX);
	mono_add_internal_call("VI.Transform/Scale::SetY", &MM::iTransform::SetScaleY);

	mono_add_internal_call("VI.Transform/Rotate::s_Get", &MM::iTransform::s_GetRotate);
	mono_add_internal_call("VI.Transform/Rotate::s_Set", &MM::iTransform::s_SetRotate);

	mono_add_internal_call("VI.Transform/Rotate::Get", &MM::iTransform::GetRotate);
	mono_add_internal_call("VI.Transform/Rotate::Set", &MM::iTransform::SetRotate);

	// Text
	mono_add_internal_call("VI.Text::s_Update", &MM::iText::s_UpdateText);
	mono_add_internal_call("VI.Text::s_GetLineCount", &MM::iText::s_GetLineCount);
	mono_add_internal_call("VI.Text/Offset::s_Set", &MM::iText::s_SetOffset);
	mono_add_internal_call("VI.Text/Offset::s_GetX", &MM::iText::s_GetOffsetX);
	mono_add_internal_call("VI.Text/Offset::s_GetY", &MM::iText::s_GetOffsetY);
	mono_add_internal_call("VI.Text/Scale::s_Get", &MM::iText::s_GetScale);
	mono_add_internal_call("VI.Text/Scale::s_Set", &MM::iText::s_SetScale);

	mono_add_internal_call("VI.Text::Update", &MM::iText::UpdateText);
	mono_add_internal_call("VI.Text::GetLineCount", &MM::iText::GetLineCount);
	mono_add_internal_call("VI.Text/Offset::Set", &MM::iText::SetOffset);
	mono_add_internal_call("VI.Text/Offset::GetX", &MM::iText::GetOffsetX);
	mono_add_internal_call("VI.Text/Offset::GetY", &MM::iText::GetOffsetY);
	mono_add_internal_call("VI.Text/Scale::Get", &MM::iText::GetScale);
	mono_add_internal_call("VI.Text/Scale::Set", &MM::iText::SetScale);

	mono_add_internal_call("VI.Text::SetColour", &VI::iText::SetColour);

	// Weather
	mono_add_internal_call("VI.Weather::Init", &VI::iWeather::WeatherInit);
	mono_add_internal_call("VI.Weather::GetCurrent", &VI::iWeather::GetCurrentWeather);

	// LightSource
	mono_add_internal_call("VI.LightSource/Radius::s_Get", &MM::iLightSource::s_GetRadius);
	mono_add_internal_call("VI.LightSource/Radius::s_Set", &MM::iLightSource::s_SetRadius);
	mono_add_internal_call("VI.LightSource/Radius::Get", &MM::iLightSource::GetRadius);
	mono_add_internal_call("VI.LightSource/Radius::Set", &MM::iLightSource::SetRadius);
}
