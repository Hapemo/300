/*!*****************************************************************************
\file Input.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Input detects keyboard and mouse input states and returns that to caller
*******************************************************************************/
#include "Input.h"
#include "pch.h"

std::array<bool, 324> Input::mPrevKeyStates;
int Input::mStartingIndex{ 32 };
int Input::mTotalMouseKey{ 8 };
int Input::mMaxKeyboardIndex{ 348 };
double Input::mScrollTotal{ 0 };
double Input::mScrollOffset{ 0 };
GLFWwindow* Input::mWindow;
GLFWcursor* Input::mCursor;

void Input::Init(GLFWwindow* _window) {
  mWindow = _window;

  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetScrollCallback(mWindow, scroll_callback);
 }

bool Input::CheckKey(E_STATE state, E_KEY key) {
  int curr_state{};
  if ((int)key > mMaxKeyboardIndex)
    curr_state = glfwGetMouseButton(mWindow, (int)key - mMaxKeyboardIndex - 1);
  else 
    curr_state = glfwGetKey(mWindow, (int)key);

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
      return state == PRESS;
      break;
    case 1: // Prev pressed
      return state == HOLD;
      break;
    }

   default:
#ifdef NDEBUG
    std::cout << "Invalid current keyId: " + std::to_string((int)key) + " | with current state: " + std::to_string(glfwGetKey(mWindow, (int)key)) << '\n';
#endif
    assert(0 && "Invalid current key pressed");

  }
  return 0;
}

void Input::UpdatePrevKeyStates() {
  for (int i = 0; i < static_cast<int>(sizeof(mPrevKeyStates)) - mTotalMouseKey; ++i)
    mPrevKeyStates[i] = (bool)glfwGetKey(mWindow, mStartingIndex + i);
  for (int i = static_cast<int>(sizeof(mPrevKeyStates)) - mTotalMouseKey + 1, j = 0; i < static_cast<int>(sizeof(mPrevKeyStates)); ++i, ++j) {
    mPrevKeyStates[i] = (bool)glfwGetMouseButton(mWindow, j);
  }
  mScrollOffset = 0.0;
}

Math::Vec2 Input::CursorPos() {
  double xpos, ypos;
  glfwGetCursorPos(mWindow, &xpos, &ypos);
  return Math::Vec2{ static_cast<float>(xpos), static_cast<float>(ypos) };
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

