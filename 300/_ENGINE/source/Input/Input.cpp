/*!*****************************************************************************
\file Input.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Input detects keyboard and mouse input states and returns that to caller
*******************************************************************************/
#include "Input/Input.h"
#include "pch.h"
#include "ECS/ECS_Systems.h"
#include <Windows.h>
//#include <Graphics/GraphicsSystem.h>

//#include <WinUser.h>

std::array<bool, 324> Input::mPrevKeyStates;
int Input::mStartingIndex{ 32 };
int Input::mTotalMouseKey{ 8 };
int Input::mMaxKeyboardIndex{ 348 };
double Input::mScrollTotal{ 0 };
double Input::mScrollOffset{ 0 };
GLFWcursor* Input::mCursor;
bool Input::mIsEditor{ false };
bool Input::m_EditorSceneHovered{ false };
glm::vec2 Input::mosposEditor {};
glm::vec2 Input::prev_cursorpos{};

 glm::vec2 Input::m_EditorWindowPos;
 glm::vec2 Input::m_EditorCursorPos;
 glm::vec2 Input::m_GameWindowPos;
 glm::vec2 Input::m_EditorWindowPosUnadjusted;


void Input::Init(bool isEditor) {
    mIsEditor = isEditor;
  glfwSetScrollCallback(systemManager->GetWindow()->GetHandle(), scroll_callback);
 }

bool Input::CheckKey(E_STATE state, E_KEY key) {
  if (key == E_KEY::ERROR_EKEY) return false;
  int curr_state{};
  if ((int)key > mMaxKeyboardIndex)
    curr_state = systemManager->GetWindow()->IsMousePressed((int)key - mMaxKeyboardIndex - 1);
  else 
    curr_state = systemManager->GetWindow()->IsKeyPressed((int)key);

  switch (curr_state) {
  case 0: // Curr not pressed
    
    switch (mPrevKeyStates[(int)key - mStartingIndex]) {
    case 0: // Prev not pressed
      return state == NOTPRESS;
      break;
    case 1: // Prev pressed
      return state == RELEASE;
      break;
    }
    break;
  
  case 1: // Curr pressed
  
    switch (mPrevKeyStates[(int)key - mStartingIndex]) {
    case 0: // Prev not pressed
      return (state == PRESS) || (state == HOLD);
      break;
    case 1: // Prev pressed
      return state == HOLD;
      break;
    }

   default:
#ifdef NDEBUG
    std::cout << "Invalid current keyId: " + std::to_string((int)key) + " | with current state: " + std::to_string(glfwGetKey(systemManager->GetWindow()->GetHandle(), (int)key)) << '\n';
#endif
    assert(0 && "Invalid current key pressed");

  }
  return 0;
}

void Input::UpdatePrevKeyStates() {
  for (int i = 0; i < static_cast<int>(sizeof(mPrevKeyStates)) - mTotalMouseKey; ++i)
    mPrevKeyStates[i] = systemManager->GetWindow()->IsKeyPressed(mStartingIndex + i);
  for (int i = static_cast<int>(sizeof(mPrevKeyStates)) - mTotalMouseKey + 1, j = 0; i < static_cast<int>(sizeof(mPrevKeyStates)); ++i, ++j) {
    mPrevKeyStates[i] = systemManager->GetWindow()->IsMousePressed(j);
  }
  mScrollOffset = 0.0;
}

glm::vec2 Input::CursorPos() {
  double xpos, ypos;

//  if (m_right == true) {
      glfwGetCursorPos(systemManager->GetWindow()->GetHandle(), &xpos, &ypos);
      return glm::vec2{ static_cast<float>(xpos), static_cast<float>(ypos) };
  //}
  //else {
  //    return mosposEditor;
  //}
}

void Input::SetCursorCenter()
{
    ////if (0) {
    //if (systemManager->IsEditor()) {
    //    SetCursorPos( int(m_EditorWindowPos.x ), int(m_EditorWindowPos.y));
    //    //glfwSetCursorPos(systemManager->GetWindow()->GetHandle(), m_EditorWindowPos.x, m_EditorWindowPos.y);
    //}

    //else {
    //	int x{}, y{};
    //	int winlength{}, winheight{};

    //	glfwGetWindowPos(systemManager->GetWindow()->GetHandle(), &x, &y);
    //	glfwGetWindowSize(systemManager->GetWindow()->GetHandle(), &winlength, &winheight);

    //	//SetCursorPos(x + winlength / 2, y + winheight / 2);
    //    glfwSetCursorPos(systemManager->GetWindow()->GetHandle(), x + winlength / 2, y + winheight / 2);
    //}

    prev_cursorpos = CursorPos();
}

glm::vec2 Input::GetCursorCenter()
{
    ////if (0) {
    //if (systemManager->IsEditor()) {
    //    return m_EditorWindowPosUnadjusted;
    //}

    //else {
    //	int x{}, y{};
    //	int winlength{}, winheight{};

    //	glfwGetWindowPos(systemManager->GetWindow()->GetHandle(), &x, &y);
    //	glfwGetWindowSize(systemManager->GetWindow()->GetHandle(), &winlength, &winheight);

    //	return vec2(x + winlength / 2, y + winheight / 2);
    //}
    return prev_cursorpos;
}


void Input::scroll_callback(GLFWwindow* _window, double _xoffset, double _yoffset)
{
    (void)_window;
    (void)_xoffset;
    mScrollTotal += _yoffset;
    mScrollOffset = _yoffset;
}

double Input::GetScroll()
{
    return mScrollOffset;
}

