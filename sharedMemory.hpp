#ifndef SHARED_MEMORY_HPP
#define SHARED_MEMORY_HPP

#include <string>
#include <iostream>

#include "singleton.hpp"
#include "sharedStruct.hpp"

namespace sharedMemory {

	// const variables
	const std::string SHARED_MEMORY_NAME = "SharedMemory";
	const std::string VECTOR_NAME = "SharedVector";
	const size_t SIZE = 65536;

	template<Allocatable T>
	class sharedMemoryManager {
	private:
		using segment = typename sharedData<T>::segment;
		using allocator = typename sharedData<T>::allocator;

		segment segment_;
		sharedMemory::sharedData<T>* shData{};
	public:
		sharedMemoryManager();
		~sharedMemoryManager() = default;
		[[nodiscard]] std::vector<T> getCopy() const;

		void pushValue(const T& value);
		void displayVector(const std::string& titel = "") const;

		[[nodiscard]] size_t getVectorSize() const;
		[[nodiscard]] T getLastValue() const;
		static void destroySegment();
	};

	template <Allocatable T>
	sharedMemoryManager<T>::sharedMemoryManager()
		: segment_(bip::open_or_create, SHARED_MEMORY_NAME.c_str(), SIZE) {
		shData = segment_.find_or_construct<sharedData<T>>(VECTOR_NAME.c_str())(segment_.get_allocator<T>());
	}

	template <Allocatable T>
	std::vector<T> sharedMemoryManager<T>::getCopy() const {
		bip::scoped_lock<bip::interprocess_mutex> lock(shData->mtx);
		return std::vector<T>(shData->vector.begin(), shData->vector.end());
	}

	template<Allocatable T>
	void sharedMemoryManager<T>::pushValue(const T& value) {
		bip::scoped_lock<bip::interprocess_mutex> lock(shData->mtx);
		shData->vector.push_back(value);
	}

	template<Allocatable T>
	void sharedMemoryManager<T>::displayVector(const std::string& title) const {
		bip::scoped_lock<bip::interprocess_mutex> lock(shData->mtx);

		if (!title.empty()) {
			std::cout << title << std::endl;
		}

		std::cout << "[";

		for (const T& val : shData->vector) {
			std::cout << val << " ";
		}

		std::cout << "]" << std::endl;
	}

	template <Allocatable T>
	size_t sharedMemoryManager<T>::getVectorSize() const {
		bip::scoped_lock<bip::interprocess_mutex> lock(shData->mtx);
		return shData->vector.size();
	}

	template <Allocatable T>
	T sharedMemoryManager<T>::getLastValue() const {
		bip::scoped_lock<bip::interprocess_mutex> lock(shData->mtx);
		return shData->vector.empty() ? T() : shData->vector.back();
	}

	template <Allocatable T>
	void sharedMemoryManager<T>::destroySegment() {
		if (bip::shared_memory_object::remove(SHARED_MEMORY_NAME.c_str())) {
			std::cout << "Shared memory destroyed" << std::endl;
		}
		else {
			std::cerr << "Warning: No shared memory found to destroy" << std::endl;
		}
	}

	// use type <int>
	using sharedSegmentInt = sharedMemory::Singleton<sharedMemoryManager<int>>;
}
#endif // SHARED_MEMORY_HPP