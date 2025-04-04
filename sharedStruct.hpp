#ifndef SHARED_STRUCT_HPP
#define SHARED_STRUCT_HPP

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <concepts>
#include <memory>

namespace sharedMemory {

	namespace bip = boost::interprocess;

	// concept 
	template<typename T>
	concept Allocatable = requires {
		typename std::allocator_traits<std::allocator<T>>::value_type;
	};

	template<Allocatable T>
	struct sharedData {

		// for ease of use
		using segment = bip::managed_shared_memory;
		using allocator = bip::allocator<T, segment::segment_manager>;
		using shadedVector = bip::vector<T, allocator>;

		shadedVector vector;
		bip::interprocess_mutex mtx;

		explicit sharedData(const allocator& alloc);
	};

	template<Allocatable T>
	sharedData<T>::sharedData(const allocator& alloc) : vector(alloc) {
	}
}

#endif // !SHARED_STRUCT