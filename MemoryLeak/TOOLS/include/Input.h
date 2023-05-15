/*!*****************************************************************************
\file Input.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Input detects keyboard and mouse input states and returns that to caller
*******************************************************************************/

#pragma once
#include "pch.h"

// Available keys
enum E_KEY {
  UNKNOWN = -1,
  SPACE = 32,
  APOSTROPHE = 39,/* ' */
  COMMA = 44,/* , */
  MINUS = 45,/* - */
  PERIOD = 46,/* . */
  SLASH = 47,/* / */
  _0 = 48,
  _1 = 49,
  _2 = 50,
  _3 = 51,
  _4 = 52,
  _5 = 53,
  _6 = 54,
  _7 = 55,
  _8 = 56,
  _9 = 57,
  SEMICOLON = 59,/* ; */
  EQUAL = 61,/* = */
  A = 65,
  B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  LEFT_BRACKET, /* [ */
  BACKSLASH, /* \ */
  RIGHT_BRACKET, /* ] */
  GRAVE_ACCENT = 96, /* ` */
  WORLD_1 = 161, /* non-US #1 */
  WORLD_2, /* non-US #2 */
  ESCAPE = 256,
  ENTER,
  TAB,
  BACKSPACE,
  INSERT,
  DELETE,
  RIGHT,
  LEFT,
  DOWN,
  UP,
  PAGE_UP,
  PAGE_DOWN,
  HOME,
  END,
  CAPS_LOCK = 280,
  SCROLL_LOCK,
  NUM_LOCK,
  PRINT_SCREEN,
  PAUSE,
  F1 = 290,
  F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14,
  F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
  PAD_0 = 320,
  PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9,
  PAD_DECIMAL,
  PAD_DIVIDE,
  PAD_MULTIPLY,
  PAD_SUBTRACT,
  PAD_ADD,
  PAD_ENTER,
  PAD_EQUAL,
  LEFT_SHIFT = 340,
  LEFT_CONTROL,
  LEFT_ALT,
  LEFT_SUPER,
  RIGHT_SHIFT,
  RIGHT_CONTROL,
  RIGHT_ALT,
  RIGHT_SUPER,
  MENU,
  M_BUTTON_L,
  M_BUTTON_R,
  M_BUTTON_M,
  M_BUTTON_4,
  M_BUTTON_5,
  M_BUTTON_6,
  M_BUTTON_7,
  M_BUTTON_8
};

// State of key pressed
enum E_STATE {
  NOTPRESS,
  RELEASE,
  PRESS,
  HOLD
};

class Input {
private:
  static std::array<bool, 324> mPrevKeyStates;
  static int mStartingIndex;
  static int mTotalMouseKey;
  static int mMaxKeyboardIndex;
  static GLFWwindow* mWindow;
  static GLFWcursor* mCursor;
  static double mScrollTotal;
  static double mScrollOffset;
  
public:
  /*!*****************************************************************************
  \brief
  Initialises the Input system
  *******************************************************************************/
  static void Init(GLFWwindow*);

  //-------------------------------------------------------------
  // Keyboard controls
  //-------------------------------------------------------------
  
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
  static bool CheckKey(E_STATE, E_KEY);
  
  /*!*****************************************************************************
  \brief
  Update the previous state of all keys.
  Must be called once after all key inquiry
  *******************************************************************************/
  static void UpdatePrevKeyStates();

  /*!*****************************************************************************
  \brief
  Returns the mouse cursor position

  \return Math::Vec2
  - Position of mouse cursor
  *******************************************************************************/
  static Math::Vec2 CursorPos();

  static void scroll_callback(GLFWwindow* _window, double _xoffset, double _yoffset);
  static double GetScroll();
};
