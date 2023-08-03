#pragma once
class ISingleton {
public:
	ISingleton() {}
	~ISingleton() {}

	ISingleton(const ISingleton&) = delete;
	ISingleton& operator= (const ISingleton&) = delete;
	ISingleton(ISingleton&&) = delete;
	ISingleton& operator= (ISingleton&&) = delete;

	// Deallocate heap memory
	virtual void Destroy() = 0;
};