/*!*****************************************************************************
\file InternalCalls.h
\author Chen Jia Wen
\par DP email: c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 22-11-2022
\brief
This file contains the function declarations of the class InternalCalls.
InternalCalls are where all functions that scripts need to use from the engine
is being stored.
*******************************************************************************/

#include "InternalCalls.h"
#include "ECSManager.h"
#include "GameStateManager.h"
#include "Helper.h"
#include "logger.h"

/*!*****************************************************************************
\brief
Test internal function for string.
*******************************************************************************/
void InternalCalls::TestArgString(MonoString* _thingToPrint) {
	std::cout << "TestArgString: " << mono_string_to_utf8(_thingToPrint) << "\n";
	LOG_INFO(mono_string_to_utf8(_thingToPrint));
}
MonoString* InternalCalls::TestReturnString() {
	std::cout << "Calling internal call TestReturnString() success!\n";
	return mono_string_new(mono_domain_get(), "Hello from TestReturnString()!");
}

/*!*****************************************************************************
\brief
Error logger helper function.
*******************************************************************************/
void InternalCalls::MissingComponent(std::string _entityName, std::string _componentName) {
	LOG_GAME("Entity " + _entityName + " does not have a " + _componentName + " component!");
}

/*!*****************************************************************************
\brief
Logger function.
*******************************************************************************/
void InternalCalls::LogGame(std::string _log) {
	LOG_GAME(_log);
}
void InternalCalls::LogTest(std::string _log) {
	LOG_TEST(_log);
}

/*!*****************************************************************************
\brief
Gets the delta time in double.
*******************************************************************************/
double InternalCalls::GetDeltaTime() {
	return FPSManager::dt;
}

/*!*****************************************************************************
\brief
Player data.
*******************************************************************************/
void InternalCalls::LoadPlayerData(std::string _name) {
	playerDataManager->LoadPlayerData(_name);
}
void InternalCalls::SavePlayerData() {
	playerDataManager->SavePlayerData();
}

std::string InternalCalls::GetPlayerName() {
	return playerDataManager->GetPlayerName();
}
int InternalCalls::GetLevelAt() {
	return playerDataManager->GetLevelAt();
}
void InternalCalls::SetLevelAt(int _level) {
	playerDataManager->SetLevelAt(_level);
}

int InternalCalls::GetData1(int _index) {
	return playerDataManager->GetData1(_index);
}
void InternalCalls::SetData1(int _index, int _value) {
	playerDataManager->SetData1(_index, _value);
}
int InternalCalls::GetData2(int _index) {
	return playerDataManager->GetData2(_index);
}
void InternalCalls::SetData2(int _index, int _value) {
	playerDataManager->SetData2(_index, _value);
}

/*!*****************************************************************************
\brief
Checks for the state of certain key
\param STATE
- State of the key pressed
\param KEY
- Key to check the state of
\return bool
- Returns true if inputted state matches current state of specified key
*******************************************************************************/
bool InternalCalls::iInput::CheckKey(E_STATE _state, E_KEY _key) {
	return Input::CheckKey(_state, _key);
}
bool InternalCalls::iInput::iKey::Press(int _key) {
	return Input::CheckKey(E_STATE::PRESS, (E_KEY)_key);
}
bool InternalCalls::iInput::iKey::Hold(int _key) {
	return Input::CheckKey(E_STATE::HOLD, (E_KEY)_key);
}
bool InternalCalls::iInput::iKey::Release(int _key) {
	return Input::CheckKey(E_STATE::RELEASE, (E_KEY)_key);
}
bool InternalCalls::iInput::iKey::Idle(int _key) {
	return Input::CheckKey(E_STATE::NOTPRESS, (E_KEY)_key);
}

/*!*****************************************************************************
\brief
This is for checking button activity.
*******************************************************************************/
bool InternalCalls::iInput::iButton::Clicked(const Entity& _e) {
	if (!_e.HasComponent<Button>()) {
		MissingComponent(_e.GetComponent<General>().name, "Button");
		return false;
	}
	return _e.GetComponent<Button>().isClick;
}
bool InternalCalls::iInput::iButton::Clicked(const int _eId) {
	return VI::iInput::iButton::Clicked(VI::iEntity::GetEntity(_eId));
}
bool InternalCalls::iInput::iButton::Clicked(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iInput::iButton::Clicked(VI::iEntity::GetEntity(_entityName, _sceneName));
}
bool InternalCalls::iInput::iButton::Released(const Entity& _e) {
	if (!_e.HasComponent<Button>()) {
		MissingComponent(_e.GetComponent<General>().name, "Button");
		return false;
	}
	return _e.GetComponent<Button>().activated;
}
bool InternalCalls::iInput::iButton::Released(const int _eId) {
	return VI::iInput::iButton::Released(VI::iEntity::GetEntity(_eId));
}
bool InternalCalls::iInput::iButton::Released(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iInput::iButton::Released(VI::iEntity::GetEntity(_entityName, _sceneName));
}
bool InternalCalls::iInput::iButton::Hover(const Entity& _e) {
	if (!_e.HasComponent<Button>()) {
		MissingComponent(_e.GetComponent<General>().name, "Button");
		return false;
	}
	return _e.GetComponent<Button>().isHover;
}
bool InternalCalls::iInput::iButton::Hover(const int _eId) {
	return VI::iInput::iButton::Hover(VI::iEntity::GetEntity(_eId));
}
bool InternalCalls::iInput::iButton::Hover(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iInput::iButton::Hover(VI::iEntity::GetEntity(_entityName, _sceneName));
}
bool InternalCalls::iInput::iButton::AllNotHover() {
	return buttonManager->AllNotHover();
}
bool InternalCalls::iInput::iButton::AllNotClick() {
	return buttonManager->AllNotClick();
}

/*!*****************************************************************************
\brief
Get world mouse position.
*******************************************************************************/
Math::Vec2 InternalCalls::iInput::iMouse::GetWorldPos() {
#ifdef _EDITOR
	return editorManager->GetEditorWorldMousePos();
#else 
	Math::Vec2 cursorPos = Math::Vec2(Input::CursorPos().x, -Input::CursorPos().y) + Math::Vec2(-Helper::GetScreenWidth() / 2.f, Helper::GetScreenHeight() / 2.f);
	//if (Input::CheckKey(PRESS, M_BUTTON_L)) std::cout << "cursorPos: " << cursorPos << '\n';
	return cursorPos;
#endif 
}
float InternalCalls::iInput::iMouse::GetWorldPosX() {
	return VI::iInput::iMouse::GetWorldPos().x;
}
float InternalCalls::iInput::iMouse::GetWorldPosY() {
	return VI::iInput::iMouse::GetWorldPos().y;
}

/*!*****************************************************************************
\brief
ApplyImpulse function that adds a velocity impulse to the entity
\param const Entity &
A reference to a read-only Entity to
\param const Math::Vec2 &
A reference to a read-only variable containing the velocity impulse
\param const Math::Vec2 &
A reference to a read-only variable containing the rotation impulse
calculation
\return void
NULL
*******************************************************************************/
void InternalCalls::iPhysics::ApplyImpulse(const Entity& _e, const Math::Vec2& _impulse, const Math::Vec2& _rotation) {
	physics2DManager->ApplyImpulse(_e, _impulse, _rotation);
}
void InternalCalls::iPhysics::ApplyImpulse(const int _eId, const float _impulseX, const float _impulseY, const float _rotationX, const float _rotationY) {
	Math::Vec2 impulse = { _impulseX, _impulseY };
	Math::Vec2 rotate = { _rotationX, _rotationY };
	VI::iPhysics::ApplyImpulse(VI::iEntity::GetEntity(_eId), impulse, rotate);
}
void InternalCalls::iPhysics::ApplyImpulse(std::string const& _entityName, std::string const& _sceneName, const float _impulseX, const float _impulseY, const float _rotationX, const float _rotationY) {
	Math::Vec2 impulse = { _impulseX, _impulseY };
	Math::Vec2 rotate = { _rotationX, _rotationY };
	VI::iPhysics::ApplyImpulse(VI::iEntity::GetEntity(_entityName, _sceneName), impulse, rotate);
}

/*!*****************************************************************************
\brief
IsCollided function that checks if two given entities have collided by
checking whether if a contact with the two entities exists
\param const Entity &
A reference to a read-only entity to compare with
\param const Entity &
A reference to a read-only entity to compare against
\return bool
Evaluated result of whether a collision happened between the two given entities
*******************************************************************************/
bool InternalCalls::iPhysics::IsCollided(const Entity& _e1, const Entity& _e2) {
	return collision2DManager->EntitiesCollided(_e1, _e2);
}
bool InternalCalls::iPhysics::IsCollided(const int _eId1, const int _eId2) {
	return VI::iPhysics::IsCollided(VI::iEntity::GetEntity(_eId1), VI::iEntity::GetEntity(_eId2));
}
bool InternalCalls::iPhysics::IsCollided(std::string const& _entityName1, std::string const& _entityName2, std::string const& _sceneName) {
	return VI::iPhysics::IsCollided(VI::iEntity::GetEntity(_entityName1, _sceneName), VI::iEntity::GetEntity(_entityName2, _sceneName));
}
bool InternalCalls::iPhysics::CheckCollision(const Entity& _e1, const Entity& _e2, bool const& _dynamicCheck) {
	return collision2DManager->CheckCollision(_e1, _e2, _dynamicCheck);
}
bool InternalCalls::iPhysics::CheckCollision(const int _eId1, const int _eId2, bool const& _dynamicCheck) {
	return VI::iPhysics::CheckCollision(VI::iEntity::GetEntity(_eId1), VI::iEntity::GetEntity(_eId2), _dynamicCheck);
}
bool InternalCalls::iPhysics::CheckCollision(std::string const& _entityName1, std::string const& _entityName2, std::string const& _sceneName, bool const& _dynamicCheck) {
	return VI::iPhysics::CheckCollision(VI::iEntity::GetEntity(_entityName1, _sceneName), VI::iEntity::GetEntity(_entityName2, _sceneName), _dynamicCheck);
}

/*!*****************************************************************************
\brief
	Loads the dialogs from a json file to the dialog manager

\return
None.
*******************************************************************************/
void InternalCalls::iDialogue::LoadScript(std::string _filename) {
	LOG_INFO("Loading Dialogue File: " + _filename);
	serializationManager->LoadDialogs(_filename);
}

/*!*****************************************************************************
\brief
Retrieves the dialog text of the dialog with a specific id using function from
DialogManager.

\param int _id
The id of the dialog to get.

\return
Returns the dialog text in string.
*******************************************************************************/
std::string InternalCalls::iDialogue::GetLine(int _id) {
	return dialogManager->GetLine(_id);
}

/*!*****************************************************************************
\brief
Retrieves the next dialog id using function from DialogManager.

\param int _id
The id of the current dialog.

\return
Returns the next dialog id.
*******************************************************************************/
int InternalCalls::iDialogue::GetNextId(int _id) {
	return dialogManager->GetNext(_id);
}
std::string InternalCalls::iDialogue::GetNext(int _id) {
	return dialogManager->GetLine(dialogManager->GetNext(_id));
}

/*!*****************************************************************************
\brief
Checks if/Get the dialog with a specific id has choices using function from
DialogManager.

\param int _id
The id of the dialog to be checked.

\return
Returns true if the dialog has choices else returns false.
*******************************************************************************/
bool InternalCalls::iDialogue::HaveChoices(int _id) {
	return dialogManager->HaveChoices(_id);
}
std::pair<int, int> InternalCalls::iDialogue::GetChoices(int _id) {
	return dialogManager->GetChoices(_id);
}
int InternalCalls::iDialogue::GetChoice1(int _id) {
	return dialogManager->GetNext(_id);
}
int InternalCalls::iDialogue::GetChoice2(int _id) {
	return dialogManager->GetNext2(_id);
}

/*!*****************************************************************************
\brief
Set/Get the selected choice using function from DialogManager.

\param int _id
The id of the current dialog.

\param int _selectedChoice
The selected choice.
*******************************************************************************/
void InternalCalls::iDialogue::SetSelectedChoice(int _selectedChoice) {
	dialogManager->SetSelectedChoice(VI::iDialogue::GetCurrentId(), _selectedChoice);
}
int InternalCalls::iDialogue::GetSelectedChoice(int _id) {
	return dialogManager->GetSelectedChoice(_id);
}
int InternalCalls::iDialogue::GetCurrentSelectedChoice() {
	return dialogManager->GetSelectedChoice(VI::iDialogue::GetCurrentId());
}

/*!*****************************************************************************
\brief
Get/Set current dialogue id using function from DialogManager.

\param int _id
The id of the current dialog.
*******************************************************************************/
bool InternalCalls::iDialogue::SetCurrentId(int _id) {
	return dialogManager->SetCurrentDialogueID(_id);
}
int InternalCalls::iDialogue::GetCurrentId() {
	return dialogManager->GetCurrentDialogueID();
}

/*!*****************************************************************************
\brief
Returns true if speaker is player using function from DialogManager.

\param int _id
The id of the current dialog.
*******************************************************************************/
bool InternalCalls::iDialogue::IsPlayerSpeaker(int _id) {
	return dialogManager->GetSpeaker(_id);
}

/*!*****************************************************************************
\brief
This function is called by user, to change the next game state
*******************************************************************************/
void InternalCalls::iGameState::ChangeGameState(std::string const& _name) {
	GameStateManager::GetInstance()->ChangeGameState(_name);
}

/*!*****************************************************************************
\brief
Get current game state
*******************************************************************************/
GameState& InternalCalls::iGameState::CurrentGameState() {
	return *GameStateManager::GetInstance()->mCurrentGameState;
}
std::string InternalCalls::iGameState::GetCurrentGameStateName() {
	return GameStateManager::GetInstance()->mCurrentGameState->mName;
}

/*!*****************************************************************************
\brief
Exits the game.
*******************************************************************************/
void InternalCalls::iGameState::GameStateExit() {
#ifdef _EDITOR
	editorManager->SetScenePaused(true);
	editorManager->SetFullScreen(false);
#else
	GameStateManager::GetInstance()->GameStateExit();
#endif // _EDITOR
}

/*!*****************************************************************************
\brief
Gets entity id.
*******************************************************************************/
int InternalCalls::iEntity::GetId(const Entity& _e) {
	return _e.id;
}
int InternalCalls::iEntity::GetId(std::string _entityName, std::string _sceneName) {
	return GameStateManager::GetInstance()->GetEntity(_entityName, _sceneName).id;
}

/*!*****************************************************************************
\brief
Gets an entity.
*******************************************************************************/
Entity InternalCalls::iEntity::GetEntity(const int _eId) {
	if (!ECS::EntityExists(_eId)) {
		std::string error = "There is no Entity with id " + std::to_string(_eId) + "!";
		LOG_GAME(error);
		LOG_ERROR(error);
	}
	return Entity(_eId);
}
Entity InternalCalls::iEntity::GetEntity(std::string const& _entityName, std::string const& _sceneName) {
	/*if (!ECS::EntityExists(_eId)) {
		std::string error = "There is no Entity with id " + std::to_string(_eId) + "!";
		LOG_GAME(error);
		LOG_ERROR(error);
	}*/
	return GameStateManager::GetInstance()->GetEntity(_entityName, _sceneName);
}

/*!*****************************************************************************
\brief
Checks if/Sets an entity is active.
*******************************************************************************/
bool InternalCalls::iEntity::IsActive(const Entity& _e) {
	if (!_e.HasComponent<General>()) {
		LOG_GAME("Entity does not have a General component!");
		LOG_ERROR("Entity does not have a General component!");
		return false;
	}
	return _e.GetComponent<General>().isActive;
}
bool InternalCalls::iEntity::IsActive(const int _eId) {
	return VI::iEntity::IsActive(VI::iEntity::GetEntity(_eId));
}
bool InternalCalls::iEntity::IsActive(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iEntity::IsActive(VI::iEntity::GetEntity(_entityName, _sceneName));
}

void InternalCalls::iEntity::SetIsActive(const Entity& _e, bool _active) {
	_e.GetComponent<General>().isActive = _active;
}
void InternalCalls::iEntity::SetIsActive(const int _eId, bool _active) {
	VI::iEntity::SetIsActive(VI::iEntity::GetEntity(_eId), _active);
}
void InternalCalls::iEntity::SetIsActive(std::string const& _entityName, std::string const& _sceneName, bool _active) {
	VI::iEntity::SetIsActive(VI::iEntity::GetEntity(_entityName, _sceneName), _active);
}

/*!*****************************************************************************
\brief
Activate/Deactivate an entity from scene.
*******************************************************************************/
void InternalCalls::iEntity::Activate(const Entity& _e) {
	_e.Activate();
}
void InternalCalls::iEntity::Activate(const int _eId) {
	VI::iEntity::Activate(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iEntity::Activate(std::string const& _entityName, std::string const& _sceneName) {
	VI::iEntity::Activate(VI::iEntity::GetEntity(_entityName, _sceneName));
}

void InternalCalls::iEntity::Deactivate(const Entity& _e) {
	_e.Deactivate();
}
void InternalCalls::iEntity::Deactivate(const int _eId) {
	VI::iEntity::Deactivate(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iEntity::Deactivate(std::string const& _entityName, std::string const& _sceneName) {
	VI::iEntity::Deactivate(VI::iEntity::GetEntity(_entityName, _sceneName));
}

/*!*****************************************************************************
\brief
Gets parent name/id of an entity from scene.
*******************************************************************************/
std::string InternalCalls::iEntity::GetParent(const Entity& _e) {
	return _e.GetComponent<General>().parent.GetComponent<General>().name;
}
std::string InternalCalls::iEntity::GetParent(const int _eId) {
	return VI::iEntity::GetParent(VI::iEntity::GetEntity(_eId));
}
std::string InternalCalls::iEntity::GetParent(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iEntity::GetParent(VI::iEntity::GetEntity(_entityName, _sceneName));
}

int InternalCalls::iEntity::GetParentId(const Entity& _e) {
	return _e.GetComponent<General>().parent.id;
}
int InternalCalls::iEntity::GetParentId(const int _eId) {
	return VI::iEntity::GetParentId(VI::iEntity::GetEntity(_eId));
}
int InternalCalls::iEntity::GetParentId(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iEntity::GetParentId(VI::iEntity::GetEntity(_entityName, _sceneName));
}

/*!*****************************************************************************
\brief
Gets scene to pause or unpause the scene.
*******************************************************************************/
Scene& InternalCalls::iScene::Select(std::string const& _name) {
	return GameStateManager::GetInstance()->SelectScene(_name);
}
void InternalCalls::iScene::Pause(std::string const& _name) {
	GameStateManager::GetInstance()->SelectScene(_name).Pause(true);
}
void InternalCalls::iScene::Play(std::string const& _name) {
	GameStateManager::GetInstance()->SelectScene(_name).Pause(false);
}
bool InternalCalls::iScene::IsPaused(std::string const& _name) {
	return GameStateManager::GetInstance()->SelectScene(_name).mIsPause;
}
void InternalCalls::iScene::SetForceRender(std::string const& _name, bool _render) {
	GameStateManager::GetInstance()->SelectScene(_name).mForceRender = _render;
}
bool InternalCalls::iScene::GetForceRender(std::string const& _name) {
	return GameStateManager::GetInstance()->SelectScene(_name).mForceRender;
}

/*!*****************************************************************************
\brief
Get/Set current game state's camera
*******************************************************************************/
Transform& InternalCalls::iCamera::CurrentCamera() {
	return VI::iGameState::CurrentGameState().mCamera;
}
float InternalCalls::iCamera::GetScaleX() {
	return VI::iGameState::CurrentGameState().mCamera.scale.x;
}
float InternalCalls::iCamera::GetScaleY() {
	return VI::iGameState::CurrentGameState().mCamera.scale.y;
}
float InternalCalls::iCamera::GetPosX() {
	return VI::iGameState::CurrentGameState().mCamera.translation.x;
}
float InternalCalls::iCamera::GetPosY() {
	return VI::iGameState::CurrentGameState().mCamera.translation.y;
}
void InternalCalls::iCamera::SetScaleX(float _x) {
	VI::iGameState::CurrentGameState().mCamera.scale.x = _x;
}
void InternalCalls::iCamera::SetScaleY(float _y) {
	VI::iGameState::CurrentGameState().mCamera.scale.y = _y;
}
void InternalCalls::iCamera::SetPosX(float _x) {
	VI::iGameState::CurrentGameState().mCamera.translation.x = _x;
}
void InternalCalls::iCamera::SetPosY(float _y) {
	VI::iGameState::CurrentGameState().mCamera.translation.y = _y;
}

/*!*****************************************************************************
\brief
Get main window's screen width and height
*******************************************************************************/
int InternalCalls::iWindow::GetScreenWidth() {
	return Helper::GetScreenWidth();
}
int InternalCalls::iWindow::GetScreenHeight() {
	return Helper::GetScreenHeight();
}

/*!*****************************************************************************
\brief
Set the main window to fullscreen according to bool
*******************************************************************************/
bool InternalCalls::iWindow::GetFullScreen() {
	return Helper::GetFullScreenFlag();
}

void InternalCalls::iWindow::SetFullScreen(bool _fullscreen) {
	Helper::SetFullScreen(_fullscreen);
}

/*!*****************************************************************************
\brief
Viewport stuff
*******************************************************************************/
int InternalCalls::iViewport::GetWidth(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<Viewport>()) {
		MissingComponent(e.GetComponent<General>().name, "Viewport");
		return -1;
	}
	return e.GetComponent<Viewport>().width;
}
void InternalCalls::iViewport::SetWidth(const int _eId, int _width) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<Viewport>()) {
		MissingComponent(e.GetComponent<General>().name, "Viewport");
		return;
	}
	e.GetComponent<Viewport>().width = _width;
}
bool InternalCalls::iViewport::IsUI(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<Viewport>()) {
		MissingComponent(e.GetComponent<General>().name, "Viewport");
		return false;
	}
	return VI::iEntity::GetEntity(_eId).GetComponent<Viewport>().isUI;
}
void InternalCalls::iViewport::SetIsUI(const int _eId, bool _ui) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<Viewport>()) {
		MissingComponent(e.GetComponent<General>().name, "Viewport");
		return;
	}
	e.GetComponent<Viewport>().isUI = _ui;
}

/*!*****************************************************************************
\brief
	Math vector functions
*******************************************************************************/
float InternalCalls::iMath::SqMagnitude(float _x, float _y) {
	Math::Vec2 temp = { _x, _y };
	return temp.SqMagnitude();
}
float InternalCalls::iMath::Magnitude(float _x, float _y) {
	Math::Vec2 temp = { _x, _y };
	return temp.Magnitude();
}
float InternalCalls::iMath::NormalizeX(float _x, float _y) {
	Math::Vec2 temp = { _x, _y };
	return temp.Normalize().x;
}
float InternalCalls::iMath::NormalizeY(float _x, float _y) {
	Math::Vec2 temp = { _x, _y };
	return temp.Normalize().y;
}
float InternalCalls::iMath::ArcTangent(float _x, float _y) {
	return atan2f(_x, _y);
}
float InternalCalls::iMath::Negate(float _value) {
	return (_value < 0 ? -_value : _value);
}
float InternalCalls::iMath::Epsilon() {
	return Math::EPSILON;
}
double InternalCalls::iMath::Pi() {
	return Math::PI;
}
float InternalCalls::iMath::ConvertRadToDeg(float _value) {
	return _value * (180 / (float)Math::PI);
}
float InternalCalls::iMath::ConvertDegToRad(float _value) {
	return _value * ((float)Math::PI / 180);
}

/*!*****************************************************************************
\brief
Set/Get current animation image speed
*******************************************************************************/
void InternalCalls::iAnimation::iSpriteSheet::SetSpeed(const Entity& _e, float _speed) {
	if (!_e.HasComponent<SheetAnimation>()) {
		MissingComponent(_e.GetComponent<General>().name, "SheetAnimation");
		return;
	}
	_e.GetComponent<SheetAnimation>().timePerFrame = _speed;
}
float InternalCalls::iAnimation::iSpriteSheet::GetSpeed(const Entity& _e) {
	if (!_e.HasComponent<SheetAnimation>()) {
		MissingComponent(_e.GetComponent<General>().name, "SheetAnimation");
		return -1.f;
	}
	return _e.GetComponent<SheetAnimation>().timePerFrame;
}
void InternalCalls::iAnimation::iSpriteSheet::SetSpeed(const int _eId, float _speed) {
	VI::iAnimation::iSpriteSheet::SetSpeed(VI::iEntity::GetEntity(_eId), _speed);
}
float InternalCalls::iAnimation::iSpriteSheet::GetSpeed(const int _eId) {
	return VI::iAnimation::iSpriteSheet::GetSpeed(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iAnimation::iSpriteSheet::SetSpeed(std::string const& _entityName, std::string const& _sceneName, float _speed) {
	VI::iAnimation::iSpriteSheet::SetSpeed(VI::iEntity::GetEntity(_entityName, _sceneName), _speed);
}
float InternalCalls::iAnimation::iSpriteSheet::GetSpeed(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iAnimation::iSpriteSheet::GetSpeed(VI::iEntity::GetEntity(_entityName, _sceneName));
}

/*!*****************************************************************************
\brief
Set/Get current sprite sheet image index
*******************************************************************************/
void InternalCalls::iAnimation::iSpriteSheet::SetSheetIndex(const Entity& _e, int _index) {
	if (!_e.HasComponent<Animation>()) {
		MissingComponent(_e.GetComponent<General>().name, "Animation");
		return;
	}
	animator->SetCurrentImageIndex(_e, _index);
}
int InternalCalls::iAnimation::iSpriteSheet::GetSheetIndex(const Entity& _e) {
	if (!_e.HasComponent<Animation>()) {
		MissingComponent(_e.GetComponent<General>().name, "Animation");
		return -1;
	}
	return animator->GetCurrentImageIndex(_e);
}
void InternalCalls::iAnimation::iSpriteSheet::SetSheetIndex(const int _eId, int _index) {
	VI::iAnimation::iSpriteSheet::SetSheetIndex(VI::iEntity::GetEntity(_eId), _index);
}
int InternalCalls::iAnimation::iSpriteSheet::GetSheetIndex(const int _eId) {
	return VI::iAnimation::iSpriteSheet::GetSheetIndex(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iAnimation::iSpriteSheet::SetSheetIndex(std::string const& _entityName, std::string const& _sceneName, int _index) {
	VI::iAnimation::iSpriteSheet::SetSheetIndex((VI::iEntity::GetEntity(_entityName, _sceneName)), _index);
}
int InternalCalls::iAnimation::iSpriteSheet::GetSheetIndex(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iAnimation::iSpriteSheet::GetSheetIndex(VI::iEntity::GetEntity(_entityName, _sceneName));
}

/*!*****************************************************************************
\brief
Set current animation image index
*******************************************************************************/
void InternalCalls::iAnimation::iSpriteSheet::SetCurrentFrame(const Entity& _e, int _index) {
	if (!_e.HasComponent<SheetAnimation>()) {
		MissingComponent(_e.GetComponent<General>().name, "SheetAnimation");
		return;
	}
	_e.GetComponent<SheetAnimation>().currFrameIndex = _index;
}
int InternalCalls::iAnimation::iSpriteSheet::GetCurrentFrame(const Entity& _e) {
	if (!_e.HasComponent<SheetAnimation>()) {
		MissingComponent(_e.GetComponent<General>().name, "SheetAnimation");
		return -1;
	}
	return _e.GetComponent<SheetAnimation>().currFrameIndex;
}
void InternalCalls::iAnimation::iSpriteSheet::SetCurrentFrame(const int _eId, int _index) {
	VI::iAnimation::iSpriteSheet::SetCurrentFrame(VI::iEntity::GetEntity(_eId), _index);
}
int InternalCalls::iAnimation::iSpriteSheet::GetCurrentFrame(const int _eId) {
	return VI::iAnimation::iSpriteSheet::GetCurrentFrame(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iAnimation::iSpriteSheet::SetCurrentFrame(std::string const& _entityName, std::string const& _sceneName, int _index) {
	VI::iAnimation::iSpriteSheet::SetCurrentFrame(VI::iEntity::GetEntity(_entityName, _sceneName), _index);
}
int InternalCalls::iAnimation::iSpriteSheet::GetCurrentFrame(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iAnimation::iSpriteSheet::GetCurrentFrame(VI::iEntity::GetEntity(_entityName, _sceneName));
}

/*!*****************************************************************************
\brief
Get/Set current animation total frame count
*******************************************************************************/
void InternalCalls::iAnimation::iSpriteSheet::SetFrameCount(const Entity& _e, int _count) {
	if (!_e.HasComponent<SheetAnimation>()) {
		MissingComponent(_e.GetComponent<General>().name, "SheetAnimation");
		return;
	}
	_e.GetComponent<SheetAnimation>().frameCount = _count;
}
int InternalCalls::iAnimation::iSpriteSheet::GetFrameCount(const Entity& _e) {
	if (!_e.HasComponent<SheetAnimation>()) {
		MissingComponent(_e.GetComponent<General>().name, "SheetAnimation");
		return -1;
	}
	return _e.GetComponent<SheetAnimation>().frameCount;
}
void InternalCalls::iAnimation::iSpriteSheet::SetFrameCount(const int _eId, int _count) {
	VI::iAnimation::iSpriteSheet::SetFrameCount(VI::iEntity::GetEntity(_eId), _count);
}
int InternalCalls::iAnimation::iSpriteSheet::GetFrameCount(const int _eId) {
	return VI::iAnimation::iSpriteSheet::GetFrameCount(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iAnimation::iSpriteSheet::SetFrameCount(std::string const& _entityName, std::string const& _sceneName, int _count) {
	VI::iAnimation::iSpriteSheet::SetFrameCount(VI::iEntity::GetEntity(_entityName, _sceneName), _count);
}
int InternalCalls::iAnimation::iSpriteSheet::GetFrameCount(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iAnimation::iSpriteSheet::GetFrameCount(VI::iEntity::GetEntity(_entityName, _sceneName));
}

/*!*****************************************************************************
\brief
Movement animation
*******************************************************************************/
void InternalCalls::iMovementAI::Start(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->StartAnimation(e);
}
void InternalCalls::iMovementAI::ForceStop(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->ForceStopAnimation(e);
}
bool InternalCalls::iMovementAI::SetNext(const int _eId, int _i) { // return true if successful (withing 0 to the vector MAX)
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return false;
	}
	return movementAIManager->SetNextStep(e, _i);
}
void InternalCalls::iMovementAI::GoToNext(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->SetNextStep(e, e.GetComponent<MovementAI>().nextStep);
}
void InternalCalls::iMovementAI::Stop(const int _eId, bool _next) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->StopAfterThisAnimation(e, _next);
}
void InternalCalls::iMovementAI::StopAfterEndLoop(const int _eId, bool _loop) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->StopAfterEndofAnimationLoop(e, _loop);
}
void InternalCalls::iMovementAI::ReverseOrder(const int _eId, bool _reverse) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->ReverseOrderAfterNextAnimation(e, _reverse);
}
void InternalCalls::iMovementAI::SetLoopCycle(const int _eId, bool _cycle) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->SetAnimationLoopToCycle(e, _cycle);
}
int InternalCalls::iMovementAI::GetCurrentIndex(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return -1;
	}
	return e.GetComponent<MovementAI>().step;
}
void InternalCalls::iMovementAI::Remove(const int _eId, int _index) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->RemoveTransformAt(e, _index);
	e.GetComponent<MovementAI>().currtime = e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step];
}
void InternalCalls::iMovementAI::EditTiming(const int _eId, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step] = _time;
}
void InternalCalls::iMovementAI::EditCurrentTiming(const int _eId, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	e.GetComponent<MovementAI>().currtime = _time;
}
void InternalCalls::iMovementAI::EditCurrent(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ {_scaleX, _scaleY}, _rot, {_posX, _posY} };
	e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step] = trans;
	e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step] = _time;
}
void InternalCalls::iMovementAI::CurrentScaleX(const int _eId, float _scaleX) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].scale.x = _scaleX;
}
void InternalCalls::iMovementAI::CurrentScaleY(const int _eId, float _scaleY) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].scale.y = _scaleY;
}
void InternalCalls::iMovementAI::CurrentRotate(const int _eId, float _rot) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].rotation = _rot;
}
void InternalCalls::iMovementAI::CurrentPosX(const int _eId, float _posX) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].translation.x = _posX;
}
void InternalCalls::iMovementAI::CurrentPosY(const int _eId, float _posY) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].translation.y = _posY;
}
float InternalCalls::iMovementAI::GetTiming(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return -1.f;
	}
	return e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step];
}
float InternalCalls::iMovementAI::GetCurrentTiming(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return -1.f;
	}
	return e.GetComponent<MovementAI>().currtime;
}
float InternalCalls::iMovementAI::GetCurrentScaleX(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return -1.f;
	}
	return e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].scale.x;
}
float InternalCalls::iMovementAI::GetCurrentScaleY(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return -1.f;
	}
	return e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].scale.y;
}
float InternalCalls::iMovementAI::GetCurrentRotate(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return -1.f;
	}
	return e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].rotation;
}
float InternalCalls::iMovementAI::GetCurrentPosX(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return -1.f;
	}
	return e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].translation.x;
}
float InternalCalls::iMovementAI::GetCurrentPosY(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return -1.f;
	}
	return e.GetComponent<MovementAI>().targetTransforms[e.GetComponent<MovementAI>().step].translation.y;
}
void InternalCalls::iMovementAI::AddTransform(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ {_scaleX, _scaleY}, _rot, {_posX, _posY} };
	movementAIManager->AddTransform(e, trans, _time);
	e.GetComponent<MovementAI>().currtime = e.GetComponent<MovementAI>().time[e.GetComponent<MovementAI>().step];
}
void InternalCalls::iMovementAI::AddTransformAt(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time, int _index) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ {_scaleX, _scaleY}, _rot, {_posX, _posY} };
	movementAIManager->AddTransformAt(e, trans, _time, _index);
}
void InternalCalls::iMovementAI::TransformScaleAt(const int _eId, float _scaleX, float _scaleY, float _time, int _index) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ {_scaleX, _scaleY}, e.GetComponent<Transform>().rotation, e.GetComponent<Transform>().translation };
	movementAIManager->AddTransformAt(e, trans, _time, _index);
}
void InternalCalls::iMovementAI::TransformRotateAt(const int _eId,  float _rot, float _time, int _index) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ e.GetComponent<Transform>().scale, _rot, e.GetComponent<Transform>().translation };
	movementAIManager->AddTransformAt(e, trans, _time, _index);
}
void InternalCalls::iMovementAI::TransformPosAt(const int _eId, float _posX, float _posY, float _time, int _index) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ e.GetComponent<Transform>().scale, e.GetComponent<Transform>().rotation, { _posX, _posY} };
	movementAIManager->AddTransformAt(e, trans, _time, _index);
}
void InternalCalls::iMovementAI::AddTransformDifference(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ {_scaleX, _scaleY}, _rot, {_posX, _posY} };
	movementAIManager->AddTransformDifference(e, trans, _time);
}
void InternalCalls::iMovementAI::SetCalculatedTimeFromPosition(const int _eId, float _posX, float _posY, int _step) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->SetCalculatedTimeFromPosition(e, { _posX, _posY }, _step);
}
void InternalCalls::iMovementAI::SetCalculatedTimeFromRotation(const int _eId, float _rot, int _step) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->SetCalculatedTimeFromRotation(e, _rot, _step);
}
void InternalCalls::iMovementAI::SetCalculatedTimeFromScale(const int _eId, float _scaleX, float _scaleY, int _step) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	movementAIManager->SetCalculatedTimeFromScale(e, { _scaleX, _scaleY }, _step);
}

void InternalCalls::iMovementAI::AddTransformAtCurrent(const int _eId, float _scaleX, float _scaleY, float _rot, float _posX, float _posY, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ {_scaleX, _scaleY}, _rot, {_posX, _posY} };
	movementAIManager->AddTransformAt(e, trans, _time, e.GetComponent<MovementAI>().step);
}
void InternalCalls::iMovementAI::TransformScaleAtCurrent(const int _eId, float _scaleX, float _scaleY, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ {_scaleX, _scaleY}, e.GetComponent<Transform>().rotation, e.GetComponent<Transform>().translation };
	movementAIManager->AddTransformAt(e, trans, _time, e.GetComponent<MovementAI>().step);
}
void InternalCalls::iMovementAI::TransformRotateAtCurrent(const int _eId, float _rot, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ e.GetComponent<Transform>().scale, _rot, e.GetComponent<Transform>().translation };
	movementAIManager->AddTransformAt(e, trans, _time, e.GetComponent<MovementAI>().step);
}
void InternalCalls::iMovementAI::TransformPosAtCurrent(const int _eId, float _posX, float _posY, float _time) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<MovementAI>()) {
		MissingComponent(e.GetComponent<General>().name, "MovementAI");
		return;
	}
	Transform trans{ e.GetComponent<Transform>().scale, e.GetComponent<Transform>().rotation, { _posX, _posY} };
	movementAIManager->AddTransformAt(e, trans, _time, e.GetComponent<MovementAI>().step);
}

/*!*****************************************************************************
\brief
Color AI
*******************************************************************************/
void InternalCalls::iColorAI::StartAnimation(const Entity& _e) {
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return;
	}
	colorAIManager->StartAnimation(_e);
}
void InternalCalls::iColorAI::AddColor(const Entity& _e, int _r, int _g, int _b, int _a, float _time) {
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return;
	}
	Color c = { (GLubyte)_r,(GLubyte)_g,(GLubyte)_b, (GLubyte)_a };
	colorAIManager->AddColor(_e, c, _time);
}
void InternalCalls::iColorAI::AddColorAt(const Entity& _e, int _r, int _g, int _b, int _a, float _time, int _index) {
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return;
	}
	Color c = { (GLubyte)_r,(GLubyte)_g,(GLubyte)_b, (GLubyte)_a };
	colorAIManager->AddColorAt(_e, c, _time, _index);
}
void InternalCalls::iColorAI::RemoveColorAt(const Entity& _e, int _index) {
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return;
	}
	colorAIManager->RemoveColorAt(_e, _index);
}
bool InternalCalls::iColorAI::SetNextStep(const Entity& _e, int _i) { // return ture if sucessful (withing 0 to the vector MAX)
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return false;
	}
	return colorAIManager->SetNextStep(_e, _i);
}
void InternalCalls::iColorAI::StopAfterThisAnimation(const Entity& _e, bool _next) {
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return;
	}
	colorAIManager->StopAfterThisAnimation(_e, _next);
}
void InternalCalls::iColorAI::StopAfterEndofAnimationLoop(const Entity& _e, bool _loop) {
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return;
	}
	colorAIManager->StopAfterEndofAnimationLoop(_e, _loop);
}
void InternalCalls::iColorAI::ReverseOrderAfterNextAnimation(const Entity& _e, bool _reverse) {
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return;
	}
	colorAIManager->ReverseOrderAfterNextAnimation(_e, _reverse);
}
void InternalCalls::iColorAI::SetAnimationLoopToCycle(const Entity& _e, bool _cycle) {
	if (!_e.HasComponent<ColorAI>()) {
		MissingComponent(_e.GetComponent<General>().name, "ColorAI");
		return;
	}
	colorAIManager->SetAnimationLoopToCycle(_e, _cycle);
}

void InternalCalls::iColorAI::StartAnimation(const int _eId) {
	VI::iColorAI::StartAnimation(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iColorAI::AddColor(const int _eId, int _r, int _g, int _b, int _a, float _time) {
	VI::iColorAI::AddColor(VI::iEntity::GetEntity(_eId), _r, _g, _b, _a, _time);
}
void InternalCalls::iColorAI::AddColorAt(const int _eId, int _r, int _g, int _b, int _a, float _time, int _index) {
	VI::iColorAI::AddColorAt(VI::iEntity::GetEntity(_eId), _r, _g, _b, _a, _time, _index);
}
void InternalCalls::iColorAI::RemoveColorAt(const int _eId, int _index) {
	VI::iColorAI::RemoveColorAt(VI::iEntity::GetEntity(_eId), _index);
}
bool InternalCalls::iColorAI::SetNextStep(const int _eId, int _i) { // return ture if sucessful (withing 0 to the vector MAX)
	return VI::iColorAI::SetNextStep(VI::iEntity::GetEntity(_eId), _i);
}
void InternalCalls::iColorAI::StopAfterThisAnimation(const int _eId, bool _next) {
	VI::iColorAI::StopAfterThisAnimation(VI::iEntity::GetEntity(_eId), _next);
}
void InternalCalls::iColorAI::StopAfterEndofAnimationLoop(const int _eId, bool _loop) {
	VI::iColorAI::StopAfterEndofAnimationLoop(VI::iEntity::GetEntity(_eId), _loop);
}
void InternalCalls::iColorAI::ReverseOrderAfterNextAnimation(const int _eId, bool _reverse) {
	VI::iColorAI::ReverseOrderAfterNextAnimation(VI::iEntity::GetEntity(_eId), _reverse);
}
void InternalCalls::iColorAI::SetAnimationLoopToCycle(const int _eId, bool _cycle) {
	VI::iColorAI::SetAnimationLoopToCycle(VI::iEntity::GetEntity(_eId), _cycle);
}

void InternalCalls::iColorAI::StartAnimation(std::string const& _entityName, std::string const& _sceneName) {
	VI::iColorAI::StartAnimation(VI::iEntity::GetEntity(_entityName, _sceneName));
}
void InternalCalls::iColorAI::AddColor(std::string const& _entityName, std::string const& _sceneName, int _r, int _g, int _b, int _a, float _time) {
	VI::iColorAI::AddColor(VI::iEntity::GetEntity(_entityName, _sceneName), _r, _g, _b, _a, _time);
}
void InternalCalls::iColorAI::AddColorAt(std::string const& _entityName, std::string const& _sceneName, int _r, int _g, int _b, int _a, float _time, int _index) {
	VI::iColorAI::AddColorAt(VI::iEntity::GetEntity(_entityName, _sceneName), _r, _g, _b, _a, _time, _index);
}
void InternalCalls::iColorAI::RemoveColorAt(std::string const& _entityName, std::string const& _sceneName, int _index) {
	VI::iColorAI::RemoveColorAt(VI::iEntity::GetEntity(_entityName, _sceneName), _index);
}
bool InternalCalls::iColorAI::SetNextStep(std::string const& _entityName, std::string const& _sceneName, int _i) { // return ture if sucessful (withing 0 to the vector MAX)
	return VI::iColorAI::SetNextStep(VI::iEntity::GetEntity(_entityName, _sceneName), _i);
}
void InternalCalls::iColorAI::StopAfterThisAnimation(std::string const& _entityName, std::string const& _sceneName, bool _next) {
	VI::iColorAI::StopAfterThisAnimation(VI::iEntity::GetEntity(_entityName, _sceneName), _next);
}
void InternalCalls::iColorAI::StopAfterEndofAnimationLoop(std::string const& _entityName, std::string const& _sceneName, bool _loop) {
	VI::iColorAI::StopAfterEndofAnimationLoop(VI::iEntity::GetEntity(_entityName, _sceneName), _loop);
}
void InternalCalls::iColorAI::ReverseOrderAfterNextAnimation(std::string const& _entityName, std::string const& _sceneName, bool _reverse) {
	VI::iColorAI::ReverseOrderAfterNextAnimation(VI::iEntity::GetEntity(_entityName, _sceneName), _reverse);
}
void InternalCalls::iColorAI::SetAnimationLoopToCycle(std::string const& _entityName, std::string const& _sceneName, bool _cycle) {
	VI::iColorAI::SetAnimationLoopToCycle(VI::iEntity::GetEntity(_entityName, _sceneName), _cycle);
}

/*!*****************************************************************************
\brief
Particle system
*******************************************************************************/
int InternalCalls::iParticleSystem::GetDensity(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1;
	}
	return e.GetComponent<ParticleSystem>().mDensity;
}
void InternalCalls::iParticleSystem::SetDensity(const int _eId, int _density) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mDensity = _density;
}

float InternalCalls::iParticleSystem::GetPosX(int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return  -1.f;
	}
	return e.GetComponent<ParticleSystem>().mCenter.x;
}
void InternalCalls::iParticleSystem::SetPosX(const int _eId, float _posX) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mCenter.x = _posX;
}
float InternalCalls::iParticleSystem::GetPosY(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mCenter.y;
}
void InternalCalls::iParticleSystem::SetPosY(const int _eId, float _posY) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mCenter.y = _posY;
}

float InternalCalls::iParticleSystem::GetWidth(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mAreaWidth;
}
void InternalCalls::iParticleSystem::SetWidth(const int _eId, float _width) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mAreaWidth = _width;
}

float InternalCalls::iParticleSystem::GetDir(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mDirection;
}
void InternalCalls::iParticleSystem::SetDir(const int _eId, float _dir) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mDirection = _dir;
}

float InternalCalls::iParticleSystem::GetSpread(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mSpread;
}
void InternalCalls::iParticleSystem::SetSpread(const int _eId, float _spread) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mSpread = _spread;
}

float InternalCalls::iParticleSystem::GetDuration(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mDuration;
}
void InternalCalls::iParticleSystem::SetDuration(const int _eId, float _dur) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mDuration = _dur;
}

bool InternalCalls::iParticleSystem::IsActive(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return false;
	}
	return e.GetComponent<ParticleSystem>().mIsActive;
}
void InternalCalls::iParticleSystem::SetIsActive(const int _eId, bool _active) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mIsActive = _active;
}

float InternalCalls::iParticleSystem::GetSlow(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mSlow;
}
void InternalCalls::iParticleSystem::SetSlow(const int _eId, float _slow) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mSlow = _slow;
}

// Generate one frame of particles
void InternalCalls::iParticleSystem::GenerateOnce(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	particleManager->GenerateOnce(e);
}
// Generate loop of time frame
void InternalCalls::iParticleSystem::GenerateLoop(const int _eId, float _duration) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	particleManager->GenerateLoop(e, _duration);
}

/*!*****************************************************************************
\brief
Individual particle
*******************************************************************************/
float InternalCalls::iParticleSystem::iParticleInfo::GetScale(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mParticleInfo.mScale;
}
float InternalCalls::iParticleSystem::iParticleInfo::GetDirection(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mParticleInfo.mFacing;
}
float InternalCalls::iParticleSystem::iParticleInfo::GetLifeSpan(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mParticleInfo.mLifespan;
}
float InternalCalls::iParticleSystem::iParticleInfo::GetRotation(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mParticleInfo.mRotation;
}
float InternalCalls::iParticleSystem::iParticleInfo::GetSpeed(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1.f;
	}
	return e.GetComponent<ParticleSystem>().mParticleInfo.mSpeed;
}
bool InternalCalls::iParticleSystem::iParticleInfo::GetFading(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return false;
	}
	return e.GetComponent<ParticleSystem>().mParticleInfo.mFading;
}
int InternalCalls::iParticleSystem::iParticleInfo::GetSpriteType(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1;
	}
	return (int)e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.sprite;
}
int InternalCalls::iParticleSystem::iParticleInfo::GetLayer(const int _eId) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return -1;
	}
	return (int)e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.layer;
}

void InternalCalls::iParticleSystem::iParticleInfo::SetScale(const int _eId, float _scale) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mScale = _scale;
}
void InternalCalls::iParticleSystem::iParticleInfo::SetDirection(const int _eId, float _dir) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mFacing = _dir;
}
void InternalCalls::iParticleSystem::iParticleInfo::SetLifeSpan(const int _eId, float _lifespan) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mLifespan = _lifespan;
}
void InternalCalls::iParticleSystem::iParticleInfo::SetRotation(const int _eId, float _rot) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mRotation = _rot;
}
void InternalCalls::iParticleSystem::iParticleInfo::SetSpeed(const int _eId, float _speed) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mSpeed = _speed;
}
void InternalCalls::iParticleSystem::iParticleInfo::SetFading(const int _eId, bool _fade) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mFading = _fade;
}
void InternalCalls::iParticleSystem::iParticleInfo::SetColour(const int _eId, int _r, int _g, int _b, int _a) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	Color clr{ (GLubyte)_r, (GLubyte)_g, (GLubyte)_b, (GLubyte)_a };
	e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.color = clr;
}
void InternalCalls::iParticleSystem::iParticleInfo::SetSpriteType(const int _eId, int _type) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.sprite = (SPRITE)_type;
}
void InternalCalls::iParticleSystem::iParticleInfo::SetTexture(const int _eId, std::string _texture) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.texture = GET_TEXTURE_ID(_texture);
}
void InternalCalls::iParticleSystem::iParticleInfo::SetLayer(const int _eId, int _layer) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<ParticleSystem>()) {
		MissingComponent(e.GetComponent<General>().name, "ParticleSystem");
		return;
	}
	e.GetComponent<ParticleSystem>().mParticleInfo.mSprite.layer = _layer;
}

/*!*****************************************************************************
\brief
Edit/Get the sprite details of an entity.
*******************************************************************************/
void InternalCalls::iSprite::SetTexture(const Entity& _e, const std::string& _path) {
	if (!_e.HasComponent<Sprite>()) {
		MissingComponent(_e.GetComponent<General>().name, "Sprite");
		return;
	}
	spriteManager->SetTexture(_e, _path);
}
std::string InternalCalls::iSprite::GetTexture(const Entity& _e) {
	if (!_e.HasComponent<Sprite>()) {
		MissingComponent(_e.GetComponent<General>().name, "Sprite");
		return "";
	}
	return spriteManager->GetTexturePath(spriteManager->GetTexture(_e));
}
void InternalCalls::iSprite::SetTexture(const int _eId, const std::string& _path) {
	VI::iSprite::SetTexture(VI::iEntity::GetEntity(_eId), _path);
}
std::string InternalCalls::iSprite::GetTexture(const int _eId) {
	return VI::iSprite::GetTexture(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iSprite::SetTexture(std::string const& _entityName, std::string const& _sceneName, const std::string& _path) {
	VI::iSprite::SetTexture(VI::iEntity::GetEntity(_entityName, _sceneName), _path);
}
std::string InternalCalls::iSprite::GetTexture(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iSprite::GetTexture(VI::iEntity::GetEntity(_entityName, _sceneName));
}
int InternalCalls::iSprite::GetType(const Entity& _e) {
	return (int)_e.GetComponent<Sprite>().sprite;
}
int InternalCalls::iSprite::GetType(const int _eId) {
	return VI::iSprite::GetType(VI::iEntity::GetEntity(_eId));
}
int InternalCalls::iSprite::GetType(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iSprite::GetType(VI::iEntity::GetEntity(_entityName, _sceneName));
}
void InternalCalls::iSprite::SetType(const Entity& _e, int _type) {
	if (!_e.HasComponent<Sprite>()) {
		MissingComponent(_e.GetComponent<General>().name, "Sprite");
		return;
	}
	_e.GetComponent<Sprite>().sprite = (SPRITE)_type;
}
void InternalCalls::iSprite::SetType(const int _eId, int _type) {
	VI::iSprite::SetType(VI::iEntity::GetEntity(_eId), _type);
}
void InternalCalls::iSprite::SetType(std::string const& _entityName, std::string const& _sceneName, int _type) {
	VI::iSprite::SetType(VI::iEntity::GetEntity(_entityName, _sceneName), _type);
}
int InternalCalls::iSprite::GetLayer(const Entity& _e) {
	if (!_e.HasComponent<Sprite>()) {
		MissingComponent(_e.GetComponent<General>().name, "Sprite");
		return -1;
	}
	return _e.GetComponent<Sprite>().layer;
}
void InternalCalls::iSprite::SetLayer(const Entity& _e, int _layer) {
	_e.GetComponent<Sprite>().layer = _layer;
}
int InternalCalls::iSprite::GetLayer(const int _eId) {
	return VI::iSprite::GetLayer(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iSprite::SetLayer(const int _eId, int _layer) {
	VI::iSprite::SetLayer(VI::iEntity::GetEntity(_eId), _layer);
}
int InternalCalls::iSprite::GetLayer(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iSprite::GetLayer(VI::iEntity::GetEntity(_entityName, _sceneName));
}
void InternalCalls::iSprite::SetLayer(std::string const& _entityName, std::string const& _sceneName, int layer) {
	VI::iSprite::SetLayer(VI::iEntity::GetEntity(_entityName, _sceneName), layer);
}
void InternalCalls::iSprite::SetAlpha(const Entity& _e, int _a) {
	if (!_e.HasComponent<Sprite>()) {
		MissingComponent(_e.GetComponent<General>().name, "Sprite");
		return;
	}
	_e.GetComponent<Sprite>().color.a = (GLubyte)_a;
}
void InternalCalls::iSprite::SetAlpha(const int _eId, int _a) {
	VI::iSprite::SetAlpha(VI::iEntity::GetEntity(_eId), _a);
}
void InternalCalls::iSprite::SetAlpha(std::string const& _entityName, std::string const& _sceneName, int _a) {
	VI::iSprite::SetAlpha(VI::iEntity::GetEntity(_entityName, _sceneName), _a);
}
void InternalCalls::iSprite::SetColor(const Entity& _e, int _r, int _g, int _b, int _a) {
	if (!_e.HasComponent<Sprite>()) {
		MissingComponent(_e.GetComponent<General>().name, "Sprite");
		return;
	}
	Color clr{ (GLubyte)_r, (GLubyte)_g, (GLubyte)_b, (GLubyte)_a };
	_e.GetComponent<Sprite>().color = clr;
}
void InternalCalls::iSprite::SetColor(const int _eId, int _r, int _g, int _b, int _a) {
	VI::iSprite::SetColor(VI::iEntity::GetEntity(_eId), _r, _g, _b, _a);
}
void InternalCalls::iSprite::SetColor(std::string const& _entityName, std::string const& _sceneName, int _r, int _g, int _b, int _a) {
	VI::iSprite::SetColor(VI::iEntity::GetEntity(_entityName, _sceneName), _r, _g, _b, _a);
}
int InternalCalls::iSprite::GetColor(const Entity& _e, int _rgba) {
	if (!_e.HasComponent<Sprite>()) {
		MissingComponent(_e.GetComponent<General>().name, "Sprite");
		return -1;
	}
	switch (_rgba) {
	case 0:
		return _e.GetComponent<Sprite>().color.r;
		break;
	case 1:
		return _e.GetComponent<Sprite>().color.g;
		break;
	case 2:
		return _e.GetComponent<Sprite>().color.b;
		break;
	case 3:
		return _e.GetComponent<Sprite>().color.a;
		break;
	default:
		return -1;
	}
}
int InternalCalls::iSprite::GetColor(const int _eId, int _rgba) {
	return VI::iSprite::GetColor(VI::iEntity::GetEntity(_eId), _rgba);
}
int InternalCalls::iSprite::GetColor(std::string const& _entityName, std::string const& _sceneName, int _rgba) {
	return VI::iSprite::GetColor(VI::iEntity::GetEntity(_entityName, _sceneName), _rgba);
}

/*!*****************************************************************************
\brief
	Plays sound
*******************************************************************************/
void InternalCalls::iAudio::Play(const Entity& _e) {
	if (!_e.HasComponent<Audio>()) {
		MissingComponent(_e.GetComponent<General>().name, "Audio");
		return;
	}
	_e.GetComponent<Audio>().sound.toPlay = true;
}
void InternalCalls::iAudio::Play(const int _eId) {
	VI::iAudio::Play(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iAudio::Play(std::string const& _entityName, std::string const& _sceneName) {
	VI::iAudio::Play(VI::iEntity::GetEntity(_entityName, _sceneName));
}

//playnow
void InternalCalls::iAudio::PlayNow(const Entity& _e) {
	if (!_e.HasComponent<Audio>()) {
		MissingComponent(_e.GetComponent<General>().name, "Audio");
		return;
	}
	audioManager->PlaySound(_e);
}
void InternalCalls::iAudio::PlayNow(const int _eId) {
	VI::iAudio::PlayNow(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iAudio::PlayNow(std::string const& _entityName, std::string const& _sceneName) {
	VI::iAudio::PlayNow(VI::iEntity::GetEntity(_entityName, _sceneName));
}

/*!*****************************************************************************
\brief
	Plays a sound on loop
*******************************************************************************/
void InternalCalls::iAudio::PlayOnLoop(const Entity& _e) {
	if (!_e.HasComponent<Audio>()) {
		MissingComponent(_e.GetComponent<General>().name, "Audio");
		return;
	}
	_e.GetComponent<Audio>().sound.toPlay = true;
	_e.GetComponent<Audio>().sound.isLoop = true;
}
void InternalCalls::iAudio::PlayOnLoop(const int _eId) {
	VI::iAudio::PlayOnLoop(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iAudio::PlayOnLoop(std::string const& _entityName, std::string const& _sceneName) {
	VI::iAudio::PlayOnLoop(VI::iEntity::GetEntity(_entityName, _sceneName));
}

/*!*****************************************************************************
\brief
   Loop functions
*******************************************************************************/
void InternalCalls::iAudio::SetLoop(const Entity& _e, bool _loop) {
	if (!_e.HasComponent<Audio>()) {
		MissingComponent(_e.GetComponent<General>().name, "Audio");
		return;
	}
	_e.GetComponent<Audio>().sound.isLoop = _loop;
}
void InternalCalls::iAudio::SetLoop(const int _eId, bool _loop) {
	VI::iAudio::SetLoop(VI::iEntity::GetEntity(_eId), _loop);
}
void InternalCalls::iAudio::SetLoop(std::string const& _entityName, std::string const& _sceneName, bool _loop) {
	VI::iAudio::SetLoop(VI::iEntity::GetEntity(_entityName, _sceneName), _loop);
}

/*!*****************************************************************************
\brief
	Stop a sound
*******************************************************************************/
void InternalCalls::iAudio::Stop(const Entity& _e) {
	if (!_e.HasComponent<Audio>()) {
		MissingComponent(_e.GetComponent<General>().name, "Audio");
		return;
	}
	audioManager->StopSound(_e);
	_e.GetComponent<Audio>().sound.toPlay = false;
}
void InternalCalls::iAudio::Stop(const int _eId) {
	VI::iAudio::Stop(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iAudio::Stop(std::string const& _entityName, std::string const& _sceneName) {
	VI::iAudio::Stop(VI::iEntity::GetEntity(_entityName, _sceneName));
}

void InternalCalls::iAudio::StopAll() {
	audioManager->StopAllSound();
}
void InternalCalls::iAudio::StopBGM() {
	audioManager->StopBGMSound();
}
void InternalCalls::iAudio::StopSFX() {
	audioManager->StopSFXSound();
}

/*!*****************************************************************************
\brief
	BGM & force play
*******************************************************************************/
void InternalCalls::iAudio::PlayBGM(std::string const& _soundName) {
	audioManager->PlayBGSound(_soundName, (int)E_AUDIO_CHANNEL::MAINBACKGROUND);
}
bool InternalCalls::iAudio::IsBgmPlaying() {
	return audioManager->isPlaying((int)E_AUDIO_CHANNEL::MAINBACKGROUND);
}
//void InternalCalls::iAudio::ForcePlay(std::string const& _soundName) {
//	audioManager->PlayAnySound(_soundName, (int)E_AUDIO_CHANNEL::FORCEPLAY);
//}

/*!*****************************************************************************
\brief
	Set and get volume
*******************************************************************************/
void InternalCalls::iAudio::SetAllVolume(float _vol) {
	audioManager->SetALLVolume(_vol);
}
void InternalCalls::iAudio::SetBGMVolume(float _vol) {
	audioManager->SetBGMVolume(_vol);
}
void InternalCalls::iAudio::SetSFXVolume(float _vol) {
	audioManager->SetSFXVolume(_vol);
}
float InternalCalls::iAudio::GetBgmVol() {
	return audioManager->GetBgmVol();
}
float InternalCalls::iAudio::GetSfxVol() {
	return audioManager->GetSfxVol();
}

/*!*****************************************************************************
\brief
Get transform of an entity.
*******************************************************************************/
Transform& InternalCalls::iTransform::GetTransform(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		Transform tran;
		return tran;
	}
	return _e.GetComponent<Transform>();
}
float InternalCalls::iTransform::GetRotation(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return -1.f;
	}
	return _e.GetComponent<Transform>().rotation;
}
Math::Vec2 InternalCalls::iTransform::GetScale(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return { -1.f, -1.f };
	}
	return _e.GetComponent<Transform>().scale;
}
Math::Vec2 InternalCalls::iTransform::GetTranslate(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return { -1.f, -1.f };
	}
	return _e.GetComponent<Transform>().translation;
}

/*!*****************************************************************************
\brief
Get/Set pos of an entity.
*******************************************************************************/
float InternalCalls::iTransform::GetPosX(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return -1.f;
	}
	return _e.GetComponent<Transform>().translation.x;
}
float InternalCalls::iTransform::GetPosY(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return -1.f;
	}
	return _e.GetComponent<Transform>().translation.y;
}
void InternalCalls::iTransform::SetPosX(const Entity& _e, float _posX) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return;
	}
	_e.GetComponent<Transform>().translation.x = _posX;
}
void InternalCalls::iTransform::SetPosY(const Entity& _e, float _posY) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return;
	}
	_e.GetComponent<Transform>().translation.y = _posY;
}

float InternalCalls::iTransform::GetPosX(const int _eId) {
	return VI::iTransform::GetPosX(VI::iEntity::GetEntity(_eId));
}
float InternalCalls::iTransform::GetPosY(const int _eId) {
	return VI::iTransform::GetPosY(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iTransform::SetPosX(const int _eId, float _posX) {
	VI::iTransform::SetPosX(VI::iEntity::GetEntity(_eId), _posX);
}
void InternalCalls::iTransform::SetPosY(const int _eId, float _posY) {
	VI::iTransform::SetPosY(VI::iEntity::GetEntity(_eId), _posY);
}

float InternalCalls::iTransform::GetPosX(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iTransform::GetPosX(VI::iEntity::GetEntity(_entityName, _sceneName));
}
float InternalCalls::iTransform::GetPosY(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iTransform::GetPosY(VI::iEntity::GetEntity(_entityName, _sceneName));
}
void InternalCalls::iTransform::SetPosX(std::string const& _entityName, std::string const& _sceneName, float _posX) {
	VI::iTransform::SetPosX(VI::iEntity::GetEntity(_entityName, _sceneName), _posX);
}
void InternalCalls::iTransform::SetPosY(std::string const& _entityName, std::string const& _sceneName, float _posY) {
	VI::iTransform::SetPosY(VI::iEntity::GetEntity(_entityName, _sceneName), _posY);
}

/*!*****************************************************************************
\brief
Set/Get scale of an entity.
*******************************************************************************/
float InternalCalls::iTransform::GetScaleY(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return -1.f;
	}
	return _e.GetComponent<Transform>().scale.y;
}
float InternalCalls::iTransform::GetScaleX(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return -1.f;
	}
	return _e.GetComponent<Transform>().scale.x;
}
void InternalCalls::iTransform::SetScaleY(const Entity& _e, float _scaleY) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return;
	}
	_e.GetComponent<Transform>().scale.y = _scaleY;
}
void InternalCalls::iTransform::SetScaleX(const Entity& _e, float _scaleX) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return;
	}
	_e.GetComponent<Transform>().scale.x = _scaleX;
}

float InternalCalls::iTransform::GetScaleY(const int _eId) {
	return VI::iTransform::GetScaleY(VI::iEntity::GetEntity(_eId));
}
float InternalCalls::iTransform::GetScaleX(const int _eId) {
	return VI::iTransform::GetScaleX(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iTransform::SetScaleY(const int _eId, float _scaleY) {
	VI::iTransform::SetScaleY(VI::iEntity::GetEntity(_eId), _scaleY);
}
void InternalCalls::iTransform::SetScaleX(const int _eId, float _scaleX) {
	VI::iTransform::SetScaleX(VI::iEntity::GetEntity(_eId), _scaleX);
}

float InternalCalls::iTransform::GetScaleY(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iTransform::GetScaleY(VI::iEntity::GetEntity(_entityName, _sceneName));
}
float InternalCalls::iTransform::GetScaleX(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iTransform::GetScaleX(VI::iEntity::GetEntity(_entityName, _sceneName));
}
void InternalCalls::iTransform::SetScaleY(std::string const& _entityName, std::string const& _sceneName, float _scaleY) {
	VI::iTransform::SetScaleY(VI::iEntity::GetEntity(_entityName, _sceneName), _scaleY);
}
void InternalCalls::iTransform::SetScaleX(std::string const& _entityName, std::string const& _sceneName, float _scaleX) {
	VI::iTransform::SetScaleX(VI::iEntity::GetEntity(_entityName, _sceneName), _scaleX);
}

/*!*****************************************************************************
\brief
Set/Get rotation of an entity.
*******************************************************************************/
float InternalCalls::iTransform::GetRotate(const Entity& _e) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return -1.f;
	}
	return _e.GetComponent<Transform>().rotation;
}
void InternalCalls::iTransform::SetRotate(const Entity& _e, float _rotate) {
	if (!_e.HasComponent<Transform>()) {
		MissingComponent(_e.GetComponent<General>().name, "Transform");
		return;
	}
	_e.GetComponent<Transform>().rotation = _rotate;
}

float InternalCalls::iTransform::GetRotate(const int _eId) {
	return VI::iTransform::GetRotate(VI::iEntity::GetEntity(_eId));
}
void InternalCalls::iTransform::SetRotate(const int _eId, float _rotate) {
	VI::iTransform::SetRotate(VI::iEntity::GetEntity(_eId), _rotate);
}

float InternalCalls::iTransform::GetRotate(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iTransform::GetRotate(VI::iEntity::GetEntity(_entityName, _sceneName));
}
void InternalCalls::iTransform::SetRotate(std::string const& _entityName, std::string const& _sceneName, float _rotate) {
	VI::iTransform::SetRotate(VI::iEntity::GetEntity(_entityName, _sceneName), _rotate);
}

/*!*****************************************************************************
\brief
Changing text component text.
*******************************************************************************/
void InternalCalls::iText::UpdateText(const Entity& _e, std::string const& _text) {
	if (!_e.HasComponent<Text>()) {
		MissingComponent(_e.GetComponent<General>().name, "Text");
		return;
	}
	_e.GetComponent<Text>().text = _text;
}
void InternalCalls::iText::UpdateText(const int _eId, std::string const& _text) {
	VI::iText::UpdateText(VI::iEntity::GetEntity(_eId), _text);
}
void InternalCalls::iText::UpdateText(std::string const& _entityName, std::string const& _sceneName, std::string const& _text) {
	VI::iText::UpdateText(VI::iEntity::GetEntity(_entityName, _sceneName), _text);
}

void InternalCalls::iText::SetOffset(const Entity& _e, float _xoffset, float _yoffset) {
	if (!_e.HasComponent<Text>()) {
		MissingComponent(_e.GetComponent<General>().name, "Text");
		return;
	}
	_e.GetComponent<Text>().offset = Math::Vec2(_xoffset, _yoffset);
}
void InternalCalls::iText::SetOffset(const int _eId, float _xoffset, float _yoffset) {
	VI::iText::SetOffset(VI::iEntity::GetEntity(_eId), _xoffset, _yoffset);
}
void InternalCalls::iText::SetOffset(std::string const& _entityName, std::string const& _sceneName, float _xoffset, float _yoffset) {
	VI::iText::SetOffset(VI::iEntity::GetEntity(_entityName, _sceneName), _xoffset, _yoffset);
}

float InternalCalls::iText::GetOffsetX(const Entity& _e) {
	if (!_e.HasComponent<Text>()) {
		MissingComponent(_e.GetComponent<General>().name, "Text");
		return -1.f;
	}
	return _e.GetComponent<Text>().offset.x;
}
float InternalCalls::iText::GetOffsetX(const int _eId) {
	return VI::iText::GetOffsetX(VI::iEntity::GetEntity(_eId));
}
float InternalCalls::iText::GetOffsetX(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iText::GetOffsetX(VI::iEntity::GetEntity(_entityName, _sceneName));
}

float InternalCalls::iText::GetOffsetY(const Entity& _e) {
	if (!_e.HasComponent<Text>()) {
		MissingComponent(_e.GetComponent<General>().name, "Text");
		return -1.f;
	}
	return _e.GetComponent<Text>().offset.y;
}
float InternalCalls::iText::GetOffsetY(const int _eId) {
	return VI::iText::GetOffsetX(VI::iEntity::GetEntity(_eId));
}
float InternalCalls::iText::GetOffsetY(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iText::GetOffsetX(VI::iEntity::GetEntity(_entityName, _sceneName));
}

int InternalCalls::iText::GetLineCount(const Entity& _e) {
	if (!_e.HasComponent<Text>()) {
		MissingComponent(_e.GetComponent<General>().name, "Text");
		return -1;
	}
	return renderManager->GetTextLines(_e);
}
int InternalCalls::iText::GetLineCount(const int _eId) {
	return VI::iText::GetLineCount(VI::iEntity::GetEntity(_eId));
}
int InternalCalls::iText::GetLineCount(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iText::GetLineCount(VI::iEntity::GetEntity(_entityName, _sceneName));
}

float InternalCalls::iText::GetScale(const Entity& _e) {
	if (!_e.HasComponent<Text>()) {
		MissingComponent(_e.GetComponent<General>().name, "Text");
		return -1.f;
	}
	return _e.GetComponent<Text>().scale;
}
float InternalCalls::iText::GetScale(const int _eId) {
	return VI::iText::GetScale(VI::iEntity::GetEntity(_eId));
}
float InternalCalls::iText::GetScale(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iText::GetScale(VI::iEntity::GetEntity(_entityName, _sceneName));
}

void InternalCalls::iText::SetScale(const Entity& _e, float _scale) {
	if (!_e.HasComponent<Text>()) {
		MissingComponent(_e.GetComponent<General>().name, "Text");
		return;
	}
	_e.GetComponent<Text>().scale = _scale;
}
void InternalCalls::iText::SetScale(const int _eId, float _scale) {
	VI::iText::SetScale(VI::iEntity::GetEntity(_eId), _scale);
}
void InternalCalls::iText::SetScale(std::string const& _entityName, std::string const& _sceneName, float _scale) {
	VI::iText::SetScale(VI::iEntity::GetEntity(_entityName, _sceneName), _scale);
}

void InternalCalls::iText::SetColour(const int _eId, int _r, int _g, int _b, int _a) {
	Entity e = VI::iEntity::GetEntity(_eId);
	if (!e.HasComponent<Text>()) {
		MissingComponent(e.GetComponent<General>().name, "Text");
		return;
	}
	Color clr{ (GLubyte)_r, (GLubyte)_g, (GLubyte)_b, (GLubyte)_a };
	e.GetComponent<Text>().color = clr;
}

/*!*****************************************************************************
\brief
Weather functions.
*******************************************************************************/
void InternalCalls::iWeather::WeatherInit(float _width, float _height) {
	aiManager->weatherAIinit(_width, _height);
}
int InternalCalls::iWeather::GetCurrentWeather(int _index, float _posX, float _posY) {
	return aiManager->GetCurrentWeather(_index, _posX, _posY);
}

/*!*****************************************************************************
\brief
Changing lightsource component.
*******************************************************************************/
float InternalCalls::iLightSource::GetRadius(const Entity& _e) {
	if (!_e.HasComponent<LightSource>()) {
		MissingComponent(_e.GetComponent<General>().name, "LightSource");
		return -1.f;
	}
	return _e.GetComponent<LightSource>().radius;
}
float InternalCalls::iLightSource::GetRadius(const int _eId) {
	return VI::iLightSource::GetRadius(VI::iEntity::GetEntity(_eId));
}
float InternalCalls::iLightSource::GetRadius(std::string const& _entityName, std::string const& _sceneName) {
	return VI::iLightSource::GetRadius(VI::iEntity::GetEntity(_entityName, _sceneName));
}

void InternalCalls::iLightSource::SetRadius(const Entity& _e, float _radius) {
	if (!_e.HasComponent<LightSource>()) {
		MissingComponent(_e.GetComponent<General>().name, "LightSource");
		return;
	}
	_e.GetComponent<LightSource>().radius = _radius;
}
void InternalCalls::iLightSource::SetRadius(const int _eId, float _radius) {
	VI::iLightSource::SetRadius(VI::iEntity::GetEntity(_eId), _radius);
}
void InternalCalls::iLightSource::SetRadius(std::string const& _entityName, std::string const& _sceneName, float _radius) {
	VI::iLightSource::SetRadius(VI::iEntity::GetEntity(_entityName, _sceneName), _radius);
}
