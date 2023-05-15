/*!*****************************************************************************
\file PassengerScript.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Course: CSD2400
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
Allow the passenger to be picked up and dropped off by the player
*******************************************************************************/
#include "PassengerScript.h"


REGISTER_SCRIPT(ScriptComponent, PassengerScript);

namespace {
	static bool speedCheat{ false };
	static float passengerOriginalSizeX{};
}

void PassengerScript::Alive(Entity const& _e) {
	(void)_e;
}

void PassengerScript::Init(const Entity& _e) {
	(void)_e;
	boat = (VI::iEntity::GetEntity("Boat", "Level1"));
	passenger = (VI::iEntity::GetEntity("Passenger_1", "Level1"));
	destination = (VI::iEntity::GetEntity("destination", "Level1"));
	destinationHouse = VI::iEntity::GetEntity("Single Story House", "Level1");
	passengerTransform = &_e.GetComponent<Transform>();
	boatTransform = &boat.GetComponent<Transform>();
	pickedUp = false;
	readyToPickUp = true;
}

void PassengerScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void PassengerScript::Update(const Entity& _e) {
	// If ready to pick up and it collided, make it pick up next frame.
	if (readyToPickUp) {
		//if (collision2DManager->IsCollided(_e, boat)) {
		if(passenger.GetComponent<Transform>().rotation == 0.5f){
			passenger.GetComponent<Transform>().rotation = 0.0f;
			passengerOriginalSizeX = passengerTransform->scale.x;
			pickingUp = true;
			//pickedUp = true;
			_e.GetComponent<RectCollider>().isTrigger = true;
			readyToPickUp = false;
		}
		return;
	}
	if (pickingUp)
	{//animation of going to boat
		diff = boatTransform->translation - passengerTransform->translation;
		if (std::fabs(diff.x) > boatTransform->scale.x/3 || std::fabs(diff.y) > boatTransform->scale.y/3)
		{
			passengerTransform->translation += diff.Normalized()*(float)FPSManager::dt*200;
			passengerTransform->scale /= 1 + 8 *(float)FPSManager::dt;
			passenger.GetComponent<Sprite>().color.a -= 10;
		}
		else
		{
			pickedUp = true;
			pickingUp = false;
		}

	}
	// If picked up, follow boat
	if (pickedUp) {
		if (passenger.GetComponent<Sprite>().color.a < 255)
		{
			passenger.GetComponent<Sprite>().color.a += 10;
		}
		if (passengerTransform->scale.x < passengerOriginalSizeX)
		{
			passengerTransform->scale *= 1 + 8 *(float)FPSManager::dt;
		}
		//passengerTransform->translation = boatTransform->translation;
		int direction = boat.GetComponent<Animation>().currentImageIndex % 8;
		switch (direction) 
		{
		case 0://front
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
			passengerTransform->translation = boatTransform->translation;
			break;
		case 1:
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
			passengerTransform->translation.x = boatTransform->translation.x - boatTransform->scale.x / 10;
			passengerTransform->translation.y = boatTransform->translation.y;// -boatTransform->scale.y / 40;
			break;
		case 2://left
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
			passengerTransform->translation.x = boatTransform->translation.x - boatTransform->scale.x/4;
			passengerTransform->translation.y = boatTransform->translation.y + boatTransform->scale.y / 16;
			break;
		case 3:
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
			passengerTransform->translation.x = boatTransform->translation.x - boatTransform->scale.x / 6;
			passengerTransform->translation.y = boatTransform->translation.y + boatTransform->scale.y / 8;
			break;
		case 4://back
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + -1;
			passengerTransform->translation = boatTransform->translation;
			break;
		case 5:
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
			passengerTransform->translation.x = boatTransform->translation.x + boatTransform->scale.x / 6;
			passengerTransform->translation.y = boatTransform->translation.y + boatTransform->scale.y / 8;
			break;
		case 6://right
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
			passengerTransform->translation.x = boatTransform->translation.x + boatTransform->scale.x / 4;
			passengerTransform->translation.y = boatTransform->translation.y + boatTransform->scale.y / 16;
			break;
		case 7:
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
			passengerTransform->translation.x = boatTransform->translation.x + boatTransform->scale.x / 10;
			passengerTransform->translation.y = boatTransform->translation.y;// -boatTransform->scale.y / 40;
			break;
		/*default:
			passenger.GetComponent<Sprite>().layer = boat.GetComponent<Sprite>().layer + 1;
			passengerTransform->translation = boatTransform->translation;*/
			
		}


		// If reached destination, get off boat (pickedUp false) and go onto island.
		if (collision2DManager->IsCollided(boat, destination)) {
			pickedUp = false;
			passengerTransform->translation = destinationHouse.GetComponent<Transform>().translation;
		}
	}
}

void PassengerScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

void PassengerScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

void PassengerScript::Exit(const Entity& _e) {
	(void)_e;
}

void PassengerScript::Dead(Entity const& _e) {
	(void)_e;
}
