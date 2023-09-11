#pragma once
#include "pch.h"
#include "Singleton.h"

#define MTTesting 1

#if MTTesting
void MTFoo1();
void MTFoo2();
void MTFoo3();

class MTCoo1 {
public:
	MTCoo1() { id = ++counter; }
	void Boo1() { std::cout << "MTCoo1 index (" << id << ") - Boo1\n"; }
	void Boo2() { std::cout << "MTCoo1 index (" << id << ") - Boo2\n"; }
	void Boo3() { std::cout << "MTCoo1 index (" << id << ") - Boo3\n"; }

private:
	static int counter;
	int id;
};

class MTCoo2 {
public:
	MTCoo2() { id = ++counter; }
	void Boo1() { std::cout << "MTCoo2 index (" << id << ") - Boo1\n"; }
	void Boo2() { std::cout << "MTCoo2 index (" << id << ") - Boo2\n"; }
	void Boo3() { std::cout << "MTCoo2 index (" << id << ") - Boo3\n"; }

private:
	static int counter;
	int id;
};


class MTCoo3 {
public:
	MTCoo3() { id = ++counter; }
	void Boo1() { std::cout << "MTCoo3 index (" << id << ") - Boo1\n"; }
	void Boo2() { std::cout << "MTCoo3 index (" << id << ") - Boo2\n"; }
	void Boo3() { std::cout << "MTCoo3 index (" << id << ") - Boo3\n"; }

private:
	static int counter;
	int id;
};
#endif

class Multithread : public Singleton<Multithread> {
public:
	void Init();

	template<typename ...VTypes>
	void RunFunctions(void(*first)(), VTypes... systems);

	template<typename ...VTypes>
	void RecurRunFunctions(std::vector<std::thread>& threads, void(*first)(), VTypes... systems);
	
	template<typename T, typename ...VTypes>
	void RunMemberFunctions(std::pair<T*, void(T::*)()> first, VTypes... systems) {
		std::vector<std::thread> threads;

		RecurRunMemberFunctions(threads, first, systems...);

		//threads.push_back(std::thread(first.second, first.first));

		for (auto& thread : threads)
			thread.join();
		threads.clear();
	}

	template<typename T, typename ...VTypes>
	void RecurRunMemberFunctions(std::vector<std::thread>& threads, std::pair<T*, void(T::*)()> first, VTypes... systems) {

		threads.push_back(std::thread(std::thread(first.second, first.first)));

		if constexpr (sizeof...(systems) != 0)
			RecurRunMemberFunctions(threads, systems...);
	}


private:
	std::vector<std::thread> mThreads;

};

//------------------------------
// Template Function Definitions
//------------------------------

template<typename ...VTypes>
void Multithread::RunFunctions(void(*first)(), VTypes... systems) {
	std::vector<std::thread> threads;

	RecurRunFunctions(threads, first, systems...);

	for (auto& thread : threads)
		thread.join();
	threads.clear();
}

template<typename ...VTypes>
void Multithread::RecurRunFunctions(std::vector<std::thread>& threads, void(*first)(), VTypes... systems) {
	threads.push_back(std::thread(first));

	if constexpr (sizeof...(systems) != 0)
		RecurRunFunctions(threads, systems...);
}
//
//template<typename T*, typename ...VTypes>
//void Multithread::RunMemberFunctions(std::pair<T, void(T::*)()> first, VTypes... systems)
//
//template<typename T, typename ...VTypes>
//void Multithread::RecurRunMemberFunctions(std::vector<std::thread>&threads, std::pair<T*, void(T::*)()> first, VTypes... systems)



