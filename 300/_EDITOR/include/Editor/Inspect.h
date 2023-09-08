/*!*************************************************************************
****
\file Inspect.h
\author Han ChengWen, Charissa Yip, Lwin Li Yue Sandi
\par DP email: c.han@digipen.edu, charissa.yip\@digipen.edu, l.liyuesandi\@digipen.edu
\date 22-9-2022
\brief
This header file contains the inspector window and relative functions to run
the system logic.

The functions

- init
Initialize related data needed for Inspector window to draw

- update
Run Inspector logic that checks for component and render related data which
provides user to edit in run time

- tags
Tagging system logic allows User to edit and add tags

- TagPosition
Return tag position (tag id)

- GetTagName
Return tag name (tag string)

- Add_component
Add component button and logic implementation that allows user to add
components

- Transform::Inspect
Inspect display for Transform component

-Collider::Inspect
Inspect display for Collider component

- BoxCollider::Inspect
Inspect display for BoxCollider component

- CircleCollider::Inspect
Inspect display for CircleCollider component

- Texture::Inspect
Inspect display for Texture components

- Camera::Inspect
Inspect display for Camera components

-RigidBody::Inspect
Inspect display for RigidBody components
****************************************************************************
***/
#pragma once
#include "EditorWindow.h"
#include <string>

/***************************************************************************/
/*!
\brief
 Pass position (put in public)  EntityManager will call this function
 (#include leftpanel (or smth else)

\param	tag
the bitset of tag to return tag name

\return int
position of tag name
*/
/***************************************************************************/


int GetTagName();
/***************************************************************************/
/*!
\brief
 Returns Tagposition through reading tag NAME

\param	tagname
tag string name

\param	EntityID
Entity ID for the object to get tagposition
*/
/***************************************************************************/
void TagPosition();


/*******************************/
/*!
	Inspect Class
*/
/*******************************/
class Inspect : public EditorWindow 
{
public:
	enum {
		inspector_layer,
		tags_layer
	};

//	float windowWidth;

	static int inspectmode;
/***************************************************************************/
/*!
\brief
Tagging system logic allows User to edit and add tags

*/
/**************************************************************************/
	void tags();

/***************************************************************************/
/*!
\brief
Inspect display for Transform component

*/
/**************************************************************************/
	void Add_component();

/***************************************************************************/
/*!
\brief
Initialize related data needed for Inspector window to draw

*/
/**************************************************************************/
	void init();
/***************************************************************************/
/*!
\brief
Run Inspector logic that checks for component and render related data which
provides user to edit in run time

*/
/***************************************************************************/
	void update();
	
	static int scaleCounter;
	static bool isEditName;
	// true - Left Panel (Add Entity)
	// false - Inspect update
	//For future implementation of undo/redo
	//static bool entityFlag;

private:
	//Transform transform;
	//Texture texture;
};
