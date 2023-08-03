#pragma once
#include "ISingleton.h"

template <typename T>
class Singleton : public ISingleton {
private:
	static T* instance;
	void Destroy() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

protected:
	Singleton() = default;

public:
	~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator= (Singleton&&) = delete;
	static T* GetInstance() {
		if (!instance) instance = new T;
		return instance;
	}

};

template<typename T>
T* Singleton<T>::instance = nullptr;
