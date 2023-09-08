/*!*************************************************************************
****
\file Inspect.cpp
\author Han ChengWen, Charissa Yip, Lwin Li Yue Sandi
\par DP email: c.han@\digipen.edu, charissa.yip\@digipen.edu, l.liyuesandi\@digipen.edu
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
Included ImGui custom dragfloat3 function for data required in command system.

-Collider::Inspect
Inspect display for Collider component
Included ImGui custom dragfloat3 function for data required in command system.

- BoxCollider::Inspect
Inspect display for BoxCollider component
Included ImGui custom dragfloat3 function for data required in command system.

- CircleCollider::Inspect
Inspect display for CircleCollider component
Included ImGui custom dragfloat3 function for data required in command system.

- Texture::Inspect
Inspect display for Texture components
Error pop up will appear when user attempts to include the wrong asset type.

- Camera::Inspect
Inspect display for Camera components
Included ImGui custom dragfloat3 function for data required in command system.

-RigidBody::Inspect
Inspect display for RigidBody components
****************************************************************************
***/

#include "Inspect.h"
#include "Editor.h"


int Inspect::inspectmode{ inspector_layer };
int Inspect::scaleCounter = 0;
bool prev{}, current{};
bool Inspect::isEditName = false;
//For future implementation of undo/redo
//bool Inspect::entityFlag = false;

void Inspect::init() {
//empty
}

void Inspect::update() 
{
	static bool addtag{ false };
	addtag = false;

	float windowWidth = ImGui::GetWindowSize().x;
	std::string typeUR = "Name";

}


