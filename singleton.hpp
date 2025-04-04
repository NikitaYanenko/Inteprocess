#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>

namespace sharedMemory {

	template<typename T>
	class Singleton {
	public:
		static T* getInstance() {
			if (instance == nullptr) {
				instance = std::make_unique<T>();
			}

			return instance.get();
		}

		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;

		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) = delete;
	protected:
		Singleton() = default;
		~Singleton() = default;
	private:
		static inline std::unique_ptr<T> instance{};
	};
}

#endif // !SINGLETON_HPP