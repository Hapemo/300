/*!
file:	SingletonManager.cpp
author:	Jazz Teoh Yu Jue
date:	16/05/2022

email:	j.teoh@digipen.edu

brief:	Singleton Manager keeps a list of all Singletons and free their memory together

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "SingletonManager.h"
#include "ISingleton.h"

std::vector<ISingleton*> SingletonManager::singletons;

void SingletonManager::addSingleton(ISingleton* singleton) { singletons.push_back(singleton); }

void SingletonManager::destroyAllSingletons() {
	for (ISingleton* singleton : singletons)
		singleton->Destroy();
}
