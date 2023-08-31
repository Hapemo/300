#pragma once
#include "ECS.h"
#include "ECS_Components.h"
#include "Input.h"
#include "../../_SCRIPTING/ScriptingSystem.h"
#include <iostream>
#include <fstream>
#include <string>

/*
struct Transform
{
	glm::vec3 scale;
	glm::vec3 rotate;
	glm::vec3 translate;
};

struct TestComponent1
{
	float x;
};

struct TestComponent2
{
	float y;
};

struct TestComponent3
{
	float z;
};
*/
void InputExample() {
	// There are 4 states of key, press, release, hold and not press
	// There are even more keys you can press, for mouse and keyboard, refer to E_KEY for more details

	if (Input::CheckKey(E_STATE::PRESS, E_KEY::SPACE)) {
		// Do something if spacebar is pressed

		glm::vec2 mousePosition = Input::CursorPos(); // Get the mouse position
		//double scrollState = Input::GetScroll(); Scroll is not working, let jazz know if you need it
	}
}


void Example()
{
	//-----------------------------------------------------------
	/*--------------------ADDING COMPONENTS--------------------*/
	//-----------------------------------------------------------
	// create new entities 
	Entity one = ECS::GetInstance()->NewEntity();
	Entity two = ECS::GetInstance()->NewEntity();
	Entity three = ECS::GetInstance()->NewEntity();
	Entity four = ECS::GetInstance()->NewEntity();
	Entity five = ECS::GetInstance()->NewEntity();

	// adds a component to an entity using default constructor of that component
	one.AddComponent<Transform>();

	//-----------------------------------------------------------
	/*--------------------!!!!IMPORTANT!!!!--------------------*/
	//-----------------------------------------------------------
	two.AddComponent<Transform>();
	// adds a component to an entity with user specifying values
	two.AddComponent<TestComponent1>(TestComponent1{ 1.f });	// two.GetComponent<TestComponent1>().x == 1.f
	// since a similar component already exists in the entity, component will be REPLACED with DEFAULT values
	two.AddComponent<TestComponent1>();							// two.GetComponent<TestComponent1>().x == float()
	// since a similar component already exists in the entity, component will be REPLACED with NEW values as specified
	two.AddComponent<TestComponent1>(TestComponent1{ 2.f });	// two.GetComponent<TestComponent1>().x == 2.f

	// adds a component to an entity using default constructor of that component
	three.AddComponent<Transform>();
	three.AddComponent<TestComponent1>();
	three.AddComponent<TestComponent2>();

	// adds a component to an entity using default constructor of that component
	four.AddComponent<Transform>();
	four.AddComponent<TestComponent1>();
	four.AddComponent<TestComponent2>();
	four.AddComponent<TestComponent3>();

	// adds a component to an entity using default constructor of that component
	five.AddComponent<Transform>();
	five.AddComponent<TestComponent1>();
	five.AddComponent<TestComponent2>();
	five.AddComponent<TestComponent3>();

	//--------------------------------------------------------------------------
	/*--------------------GETTING AND MODIFYING COMPONENTS--------------------*/
	//--------------------------------------------------------------------------
	// returns a reference to the component
	two.GetComponent<TestComponent1>().x += 1.f;	
	float tempfloat = two.GetComponent<TestComponent1>().x;						// tempfloat == 3.f

	//-----------------------------------------------------------
	/*--------------------!!!!IMPORTANT!!!!--------------------*/
	//-----------------------------------------------------------
	// entity two does not have TestComponent2
	bool tempbool1 = two.HasComponent<TestComponent2>();						// tempbool1 == false
	// trying to get a component from an entity that does not contain that component 
	// ADDS that component with DEFAULT values to the entity!!!
	two.GetComponent<TestComponent2>();
	bool tempbool2 = two.HasComponent<TestComponent2>();						// tempbool2 == true

	// checks if entity has ALL of the components listed
	bool tempbool3 = one.HasAllOfComponents<TestComponent1, TestComponent2>();	// tempbool3 == false

	// checks if entity has ANY of the components listed
	bool tempbool4 = two.HasAnyOfComponents<TestComponent2, TestComponent3>();	// tempbool4 == true

	// iterate through entities with ONE of the specified component
	auto tempView1 = ECS::GetInstance()->GetEntitiesWith<Transform>();
	for (Entity e : tempView1)													// iterates through all 5 entities as all of them has transform
	{
		
		e.GetComponent<Transform>().scale.x += 1.f;								// do stuff
	}
	tempfloat = one.GetComponent<Transform>().scale.x;							// 1.f
	tempfloat = two.GetComponent<Transform>().scale.x;							// 1.f
	tempfloat = three.GetComponent<Transform>().scale.x;						// 1.f
	tempfloat = four.GetComponent<Transform>().scale.x;							// 1.f
	tempfloat = five.GetComponent<Transform>().scale.x;							// 1.f

	// iterate through entities with ONE of the specified component
	auto tempView2 = ECS::GetInstance()->GetEntitiesWith<TestComponent3>();		// only entities four and five contain TestComponent3
	for (Entity e : tempView2)													// iterates through entities four and five ONLY
	{
		e.GetComponent<TestComponent3>().z += 1.f;								// do stuff
	}
	tempfloat = four.GetComponent<TestComponent3>().z;							// 1.f
	tempfloat = five.GetComponent<TestComponent3>().z;							// 1.f

	// iterate through entities with ALL of the specified components
	auto tempGroup1 = ECS::GetInstance()->GetEntitiesWith<TestComponent1, TestComponent3>();
	for (Entity e : tempGroup1)													// iterates through entities four and five ONLY
	{
		e.GetComponent<TestComponent1>().x += 1.f;								// do stuff
		e.GetComponent<TestComponent3>().z += 1.f;								// do stuff

		//more efficient and graceful way to do stuff
		auto [testComp1, testComp3] = Entity(e).GetComponent<TestComponent1, TestComponent3>();
		testComp1.x += 1.f;														// do stuff
		testComp3.z += 1.f;														// do stuff

		//even more efficient 
		auto [betterComp1, betterComp3] = tempGroup1.get<TestComponent1, TestComponent3>(e.id);
		betterComp1.x += 1.f;														// do stuff
		betterComp3.z += 1.f;														// do stuff
	}

	tempfloat = four.GetComponent<TestComponent1>().x;							// 2.f
	tempfloat = five.GetComponent<TestComponent1>().x;							// 2.f

	tempfloat = four.GetComponent<TestComponent3>().z;							// 3.f
	tempfloat = five.GetComponent<TestComponent3>().z;							// 3.f

	// adding and initializing component
	two.AddComponent<TestComponent3>() = { 50.f };
	// iterate through entities with ALL of the specified components
	auto tempGroup2 = ECS::GetInstance()->GetEntitiesWith<TestComponent2, TestComponent3>();
	for (Entity e : tempGroup2)													// iterates through entities two, four and five ONLY
	{
		e.GetComponent<TestComponent2>().y += 1.f;								// do stuff
		e.GetComponent<TestComponent3>().z += 1.f;								// do stuff

		//more efficient and graceful way to do stuff
		auto [testComp2, testComp3] = Entity(e).GetComponent<TestComponent2, TestComponent3>();
		testComp2.y += 1.f;														// do stuff
		testComp3.z += 1.f;														// do stuff
	}	
	tempfloat = two.GetComponent<TestComponent2>().y;							// 2.f
	tempfloat = four.GetComponent<TestComponent2>().y;							// 2.f
	tempfloat = five.GetComponent<TestComponent2>().y;							// 2.f

	tempfloat = two.GetComponent<TestComponent3>().z;							// 52.f
	tempfloat = four.GetComponent<TestComponent3>().z;							// 5.f
	tempfloat = five.GetComponent<TestComponent3>().z;							// 5.f

	// test serialization here
}

void ScriptTestInit()
{
	Entity player = ECS::GetInstance()->NewEntity();
	std::cout << "Player (EntityID: " << std::to_string(unsigned int(player.id)) << ")added." << std::endl;
	Entity enemy1 = ECS::GetInstance()->NewEntity();
	std::cout << "Enemy1 (EntityID: " << std::to_string(unsigned int(enemy1.id)) << ")added." << std::endl;
	Entity enemy2 = ECS::GetInstance()->NewEntity();
	std::cout << "Enemy2 (EntityID: " << std::to_string(unsigned int(enemy2.id)) << ")added." << std::endl;

	player.AddComponent<Transform>();
	player.AddComponent<Scripts>();

	enemy1.AddComponent<Transform>();
	enemy1.AddComponent<Scripts>();

	enemy2.AddComponent<Scripts>();


	
}

void ScriptTestUpdate()
{
	// Button press number 1 to add new script
	if (Input::CheckKey(E_STATE::PRESS, E_KEY::_1))
	{
		std::string scriptName, line;
		// get prefered name for script
		std::cout << "Enter new script name: ";
		std::getline(std::cin, scriptName);
		std::stringstream ss;
		std::ifstream defScript{ "../../Resources/Scripts/DefaultTemplate.lua" };
		std::ofstream output;
		ss << "../../Resources/Scripts/" << scriptName << ".lua";
		// create the script
		std::string path = ss.str();
		output.open(path.c_str(), std::ios_base::out);
		// copy the default template into the newly created script
		while (getline(defScript, line))
		{
			output << line << std::endl;
		}
		defScript.close();
		std::cout << "Reached" << std::endl;
		std::ifstream stream;
		stream.exceptions(std::ios::failbit | std::ios::badbit);
		stream.open(ss.str().c_str());
	}

	// Button press number 2 to add script to entity
	if (Input::CheckKey(E_STATE::PRESS, E_KEY::_2))
	{
		std::string entityID, scriptName;
		std::cout << "Indicate entity (by their id) to add script to: ";
		std::cin >> entityID;
		std::cout << "Enter script name to add to entity(Reference from Resources/Scripts: ";
		std::cin >> scriptName;
		auto scriptEntities = ECS::GetInstance()->GetEntitiesWith<Scripts>();
		for (Entity entity : scriptEntities)
		{
			if (entity.id == static_cast<entt::entity>(std::stoul(entityID)))
			{
				// if entity does not contain any script, just add
				if (scriptEntities.get<Scripts>(entity.id).scriptsContainer.size() == 0)
				{
					Script script;
					script.scriptFile = scriptName;
					script.env = { ScriptingSystem::luaState, sol::create, ScriptingSystem::luaState.globals() };
					scriptEntities.get<Scripts>(entity.id).scriptsContainer.push_back(script);
					std::cout << "Script " << scriptName << ".lua added to entity " << entityID << std::endl;
				}
				// if entity already has scripts attached, check if duplicate
				else
				{
					bool hasScript{ };

					for (auto& elem : scriptEntities.get<Scripts>(entity.id).scriptsContainer)
					{
						if (elem.scriptFile == scriptName)
						{
							hasScript = true;
							std::cout << "Script is already attached! " << std::endl;
							break;
						}
					}

					if (!hasScript)
					{
						Script script;
						script.scriptFile = scriptName;
						script.env = { ScriptingSystem::luaState, sol::create, ScriptingSystem::luaState.globals() };
						scriptEntities.get<Scripts>(entity.id).scriptsContainer.push_back(script);
						std::cout << "Script " << scriptName << ".lua added to entity " << entityID << std::endl;
					}
				}
			}
		}
	}
}
