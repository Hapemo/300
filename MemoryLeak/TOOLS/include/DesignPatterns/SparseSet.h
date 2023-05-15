/*!*****************************************************************************
\file SparseSet.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
SparseSet is a design pattern that contains 2 arrays, one shallow one dense.
Shallow array contains key pointing to addresses in dense array
Dense array contains template type that contains data.
SparseSet acts like a std::map<int, templateType>. it's faster but it only maps
integer to templateType data.
It's faster than array too since it's data are densely occupied. Dense array
contains existing data that are arranged contiguously.
*******************************************************************************/

#pragma once
#include "pch.h"
#include "logger.h"

template<typename T>
class SparseSet {
private:
	using DataType = T; // Container's data
	using IndexType = long long int; // Index unit for pairing
	// Data's ID must be convertable to int.
	
public:
	SparseSet() = delete;
	SparseSet(SparseSet const&) = delete;

	/*!*****************************************************************************
	\brief
	Parametrized constructor for constructing SparseSet

	\param int
	- Size of SparseSet
	*******************************************************************************/
	SparseSet(int);

	/*!*****************************************************************************
	\brief
	Operator overload to access data like array

	\param IndexType const&
	- Index of data
	*******************************************************************************/
	DataType& operator[](IndexType const&);

	/*!*****************************************************************************
	\brief
	Add data into SparseSet

	\param DataType const&
	Data

	\param IndexType const&
	- Index of data
	*******************************************************************************/
	void AddData(DataType const&, IndexType const&);

	/*!*****************************************************************************
	\brief
	Remove data from SparseSet

	\param IndexType const&
	- Index of data
	*******************************************************************************/
	void RemoveData(IndexType const&);

	/*!*****************************************************************************
	\brief
	Check if data is available

	\param IndexType const&
	- Index of data

	\return bool
	- True if data exists, otherwise false
	*******************************************************************************/
	bool CheckData(IndexType const&);
	
	/*!*****************************************************************************
	\brief
	Print SparseSet
	*******************************************************************************/
	void PrintSet();

	DataType* DenseBegin() { return mDense.begin(); }
	DataType* DenseEnd() { return mDense.begin() + mCapacity; }

private:
	std::vector<int> mShallow; // Interface container
	std::vector<DataType> mDense; // Storage container
	int mCapacity;
	const int mSize;
	const int emptyID;
};

template<typename T>
SparseSet<T>::SparseSet(int _size) : mSize(_size), emptyID(INT_MAX), mCapacity(0) {
	mShallow.resize(mSize);
	mDense.resize(mSize);
	std::fill(mShallow.begin(), mShallow.end(), emptyID);
}

template<typename T>
SparseSet<T>::DataType& SparseSet<T>::operator[](IndexType const& _index) {
	int deepID{ mShallow[static_cast<int>(_index)] };
	BREAKPOINT(deepID == emptyID); // If ECS breaks here, means the ecs is breaking here.
	ASSERT(deepID == emptyID, "SparseSet cannot find ID: " + std::to_string(deepID));
	return mDense[deepID];
}

template<typename T>
void SparseSet<T>::RemoveData(IndexType const& _index) {
	int& deepID{ mShallow[static_cast<int>(_index)] };
	BREAKPOINT(deepID == emptyID);
	ASSERT(deepID == emptyID, "SparseSet cannot find ID: " + std::to_string(deepID));

	--mCapacity;
	mDense[deepID] = mDense[mCapacity];
	for (int i = 0; i < mSize; ++i) {
		if (mShallow[i] == mCapacity) {
			mShallow[i] = deepID;
			break;
		}
	}
	deepID = emptyID;
}

template<typename T>
void SparseSet<T>::AddData(DataType const& _data, IndexType const& _index) {
	int& deepID{ mShallow[static_cast<int>(_index)] };
	std::string errorMessage{ "ID: " + std::to_string(deepID) + " is already added, you can try operator[] to adjust the data instead" };
	//ASSERT(deepID == emptyID, errorMessage);
	if (deepID != emptyID) {
		std::cout << "Error in AddData\n";
	}
	deepID = mCapacity;
	mDense[mCapacity] = _data;
	++mCapacity;
}

template<typename T>
bool SparseSet<T>::CheckData(IndexType const& _index) {
	return mShallow[static_cast<int>(_index)] != emptyID;
}

template<typename T>
void SparseSet<T>::PrintSet() {
	std::cout << "Size: " << mSize << "\nCapacity: " << mCapacity;
	std::cout << "\nShallow:\n";
	int shallowCount{};
	for (int const& pairingID : mShallow) {
		std::cout << "Index " << shallowCount++ << " | PairingID " << pairingID << "\n";
	}

	int denseCount{};
	std::cout << "Dense:\n";
	for (auto const& data : mDense) {
		std::cout << "PairingID " << denseCount++ << " | Data " << data << "\n";
	}
	std::cout << "\n\n";
}
