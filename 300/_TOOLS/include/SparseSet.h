/*!*****************************************************************************
\file SparseSet.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Pepe Production
\date 18-10-2023
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
#include <array>
#include <assert.h>

template<typename T, uint32_t size>
class SparseSet {
private:
	using DataType = T; // Container's data
	using IndexType = long long int; // Index unit for pairing
	// Data's ID must be convertable to int.
	
public:
	SparseSet(SparseSet const&) = delete;

	/*!*****************************************************************************
	\brief
	Parametrized constructor for constructing SparseSet

	\param int
	- Size of SparseSet
	*******************************************************************************/
	SparseSet();

	/*!*****************************************************************************
	\brief
	Operator overload to access data like array

	\param IndexType const&
	- Index of data

	\return DataType
	- Refernece to a value in the sparse set
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
	std::array<int, size> mShallow; // Interface container
	std::array<DataType, size> mDense; // Storage container
	int mCapacity;
	const int emptyID;
};

template<typename T, uint32_t size>
SparseSet<T, size>::SparseSet() : emptyID(INT_MAX), mCapacity(0) {
	std::fill(mShallow.begin(), mShallow.end(), emptyID);
}

template<typename T, uint32_t size>
T& SparseSet<T, size>::operator[](IndexType const& _index) {
	int deepID{ mShallow[static_cast<int>(_index)] };
	//BREAKPOINT(deepID == emptyID); // If ECS breaks here, means the ecs is breaking here.
	assert(deepID == emptyID && std::string("SparseSet cannot find ID: " + std::to_string(deepID)).c_str());
	return mDense[deepID];
}

template<typename T, uint32_t size>
void SparseSet<T, size>::RemoveData(IndexType const& _index) {
	int& deepID{ mShallow[static_cast<int>(_index)] };
	assert(deepID == emptyID && std::string("SparseSet cannot find ID: " + std::to_string(deepID)).c_str());

	--mCapacity;
	mDense[deepID] = mDense[mCapacity];
	for (int i = 0; i < size; ++i) {
		if (mShallow[i] == mCapacity) {
			mShallow[i] = deepID;
			break;
		}
	}
	deepID = emptyID;
}

template<typename T, uint32_t size>
void SparseSet<T, size>::AddData(DataType const& _data, IndexType const& _index) {
	int& deepID{ mShallow[static_cast<int>(_index)] };
	//std::string errorMessage{ "ID: " + std::to_string(deepID) + " is already added, you can try operator[] to adjust the data instead" };
	//ASSERT(deepID == emptyID, errorMessage);
	if (deepID != emptyID) {
		std::cout << "Error in SparseSet AddData\n";
	}
	deepID = mCapacity;
	mDense[mCapacity] = _data;
	++mCapacity;
}

template<typename T, uint32_t size>
bool SparseSet<T, size>::CheckData(IndexType const& _index) {
	return mShallow[static_cast<int>(_index)] != emptyID;
}

template<typename T, uint32_t size>
void SparseSet<T, size>::PrintSet() {
	std::cout << "Size: " << size << "\nCapacity: " << mCapacity;
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
