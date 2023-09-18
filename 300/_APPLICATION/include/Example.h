#pragma once
//#include "ECS.h"
//#include "ECS_Components.h"
#include "Input.h"
#include "Multithread.h"
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

void MultithreadExample() {
	// A blocking function that runs MTFoo1, MTFoo2, MTFoo3 concurrently, exits once all functions finish running
	// Only for function pointer of type void(*)()
	std::cout << "RunFunctions\n";
	Multithread::GetInstance()->RunFunctions(MTFoo1, MTFoo2, MTFoo3);

	// Creating some instances for testing
	MTCoo1 a1, a2, a3;
	MTCoo2 b1, b2, b3;
	MTCoo3 c1, c2, c3;

	// Blocking function that runs non-static member functions
	// Requires the class instance and pointer to the member function
	// It can be run with any combination of functions and class instances, for example below

	// Different member functions of the same instance (BECAREFUL!!! MAKE SURE YOUR FUNCTIONS ARE ALL THREAD SAFE)
	std::cout << "Different member functions of the same instance" << '\n';
	Multithread::GetInstance()->RunMemberFunctions(std::pair(&a1, &MTCoo1::Boo1),
																								 std::pair(&a1, &MTCoo1::Boo2),
																								 std::pair(&a1, &MTCoo1::Boo3));

	// Different member function of different instances
	std::cout << "Different member function of different instances" << '\n';
	Multithread::GetInstance()->RunMemberFunctions(std::pair(&a1, &MTCoo1::Boo1),
																								 std::pair(&a2, &MTCoo1::Boo2),
																								 std::pair(&a3, &MTCoo1::Boo3));

	// Different member function of different classes
	std::cout << "Different member function of different classes" << '\n';
	Multithread::GetInstance()->RunMemberFunctions(std::pair(&a1, &MTCoo1::Boo1),
																								 std::pair(&b1, &MTCoo2::Boo2),
																								 std::pair(&c1, &MTCoo3::Boo3));

	// Singleton
	std::cout << "Different member function of different classes" << '\n';
	Multithread::GetInstance()->RunMemberFunctions(std::pair(systemManager->ecs, &ECS::DeleteAllEntities));

}
//
//void Example()
//{
//	//-----------------------------------------------------------
//	/*--------------------ADDING COMPONENTS--------------------*/
//	//-----------------------------------------------------------
//	// create new entities 
//	Entity one = ECS::GetInstance()->NewEntity();
//	Entity two = ECS::GetInstance()->NewEntity();
//	Entity three = ECS::GetInstance()->NewEntity();
//	Entity four = ECS::GetInstance()->NewEntity();
//	Entity five = ECS::GetInstance()->NewEntity();
//
//	// adds a component to an entity using default constructor of that component
//	one.AddComponent<Transform>();
//
//	//-----------------------------------------------------------
//	/*--------------------!!!!IMPORTANT!!!!--------------------*/
//	//-----------------------------------------------------------
//	two.AddComponent<Transform>();
//	// adds a component to an entity with user specifying values
//	two.AddComponent<TestComponent1>(TestComponent1{ 1.f });	// two.GetComponent<TestComponent1>().x == 1.f
//	// since a similar component already exists in the entity, component will be REPLACED with DEFAULT values
//	two.AddComponent<TestComponent1>();							// two.GetComponent<TestComponent1>().x == float()
//	// since a similar component already exists in the entity, component will be REPLACED with NEW values as specified
//	two.AddComponent<TestComponent1>(TestComponent1{ 2.f });	// two.GetComponent<TestComponent1>().x == 2.f
//
//	// adds a component to an entity using default constructor of that component
//	three.AddComponent<Transform>();
//	three.AddComponent<TestComponent1>();
//	three.AddComponent<TestComponent2>();
//
//	// adds a component to an entity using default constructor of that component
//	four.AddComponent<Transform>();
//	four.AddComponent<TestComponent1>();
//	four.AddComponent<TestComponent2>();
//	four.AddComponent<TestComponent3>();
//
//	// adds a component to an entity using default constructor of that component
//	five.AddComponent<Transform>();
//	five.AddComponent<TestComponent1>();
//	five.AddComponent<TestComponent2>();
//	five.AddComponent<TestComponent3>();
//
//	//--------------------------------------------------------------------------
//	/*--------------------GETTING AND MODIFYING COMPONENTS--------------------*/
//	//--------------------------------------------------------------------------
//	// returns a reference to the component
//	two.GetComponent<TestComponent1>().x += 1.f;	
//	float tempfloat = two.GetComponent<TestComponent1>().x;						// tempfloat == 3.f
//
//	//-----------------------------------------------------------
//	/*--------------------!!!!IMPORTANT!!!!--------------------*/
//	//-----------------------------------------------------------
//	// entity two does not have TestComponent2
//	bool tempbool1 = two.HasComponent<TestComponent2>();						// tempbool1 == false
//	// trying to get a component from an entity that does not contain that component 
//	// ADDS that component with DEFAULT values to the entity!!!
//	two.GetComponent<TestComponent2>();
//	bool tempbool2 = two.HasComponent<TestComponent2>();						// tempbool2 == true
//
//	// checks if entity has ALL of the components listed
//	bool tempbool3 = one.HasAllOfComponents<TestComponent1, TestComponent2>();	// tempbool3 == false
//
//	// checks if entity has ANY of the components listed
//	bool tempbool4 = two.HasAnyOfComponents<TestComponent2, TestComponent3>();	// tempbool4 == true
//
//	// iterate through entities with ONE of the specified component
//	auto tempView1 = ECS::GetInstance()->GetEntitiesWith<Transform>();
//	for (Entity e : tempView1)													// iterates through all 5 entities as all of them has transform
//	{
//		
//		e.GetComponent<Transform>().scale.x += 1.f;								// do stuff
//	}
//	tempfloat = one.GetComponent<Transform>().scale.x;							// 1.f
//	tempfloat = two.GetComponent<Transform>().scale.x;							// 1.f
//	tempfloat = three.GetComponent<Transform>().scale.x;						// 1.f
//	tempfloat = four.GetComponent<Transform>().scale.x;							// 1.f
//	tempfloat = five.GetComponent<Transform>().scale.x;							// 1.f
//
//	// iterate through entities with ONE of the specified component
//	auto tempView2 = ECS::GetInstance()->GetEntitiesWith<TestComponent3>();		// only entities four and five contain TestComponent3
//	for (Entity e : tempView2)													// iterates through entities four and five ONLY
//	{
//		e.GetComponent<TestComponent3>().z += 1.f;								// do stuff
//	}
//	tempfloat = four.GetComponent<TestComponent3>().z;							// 1.f
//	tempfloat = five.GetComponent<TestComponent3>().z;							// 1.f
//
//	// iterate through entities with ALL of the specified components
//	auto tempGroup1 = ECS::GetInstance()->GetEntitiesWith<TestComponent1, TestComponent3>();
//	for (Entity e : tempGroup1)													// iterates through entities four and five ONLY
//	{
//		e.GetComponent<TestComponent1>().x += 1.f;								// do stuff
//		e.GetComponent<TestComponent3>().z += 1.f;								// do stuff
//
//		//more efficient and graceful way to do stuff
//		auto [testComp1, testComp3] = Entity(e).GetComponent<TestComponent1, TestComponent3>();
//		testComp1.x += 1.f;														// do stuff
//		testComp3.z += 1.f;														// do stuff
//
//		//even more efficient 
//		auto [betterComp1, betterComp3] = tempGroup1.get<TestComponent1, TestComponent3>(e.id);
//		betterComp1.x += 1.f;														// do stuff
//		betterComp3.z += 1.f;														// do stuff
//	}
//
//	tempfloat = four.GetComponent<TestComponent1>().x;							// 2.f
//	tempfloat = five.GetComponent<TestComponent1>().x;							// 2.f
//
//	tempfloat = four.GetComponent<TestComponent3>().z;							// 3.f
//	tempfloat = five.GetComponent<TestComponent3>().z;							// 3.f
//
//	// adding and initializing component
//	two.AddComponent<TestComponent3>() = { 50.f };
//	// iterate through entities with ALL of the specified components
//	auto tempGroup2 = ECS::GetInstance()->GetEntitiesWith<TestComponent2, TestComponent3>();
//	for (Entity e : tempGroup2)													// iterates through entities two, four and five ONLY
//	{
//		e.GetComponent<TestComponent2>().y += 1.f;								// do stuff
//		e.GetComponent<TestComponent3>().z += 1.f;								// do stuff
//
//		//more efficient and graceful way to do stuff
//		auto [testComp2, testComp3] = Entity(e).GetComponent<TestComponent2, TestComponent3>();
//		testComp2.y += 1.f;														// do stuff
//		testComp3.z += 1.f;														// do stuff
//	}	
//	tempfloat = two.GetComponent<TestComponent2>().y;							// 2.f
//	tempfloat = four.GetComponent<TestComponent2>().y;							// 2.f
//	tempfloat = five.GetComponent<TestComponent2>().y;							// 2.f
//
//	tempfloat = two.GetComponent<TestComponent3>().z;							// 52.f
//	tempfloat = four.GetComponent<TestComponent3>().z;							// 5.f
//	tempfloat = five.GetComponent<TestComponent3>().z;							// 5.f
//}







