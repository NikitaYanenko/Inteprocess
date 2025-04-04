#include "sharedMemory.hpp"
#include <thread>
#include <chrono>

using namespace sharedMemory;

template<Allocatable T>
void clientEmulation(const int clientId, sharedMemoryManager<T>& manager, const std::string& separation) {
	std::cout << separation << std::endl;
	T pushValue = manager.getVectorSize() == 0 ? T(1) : manager.getLastValue() + T(1);
	manager.pushValue(pushValue);
	manager.displayVector("Client " + std::to_string(clientId) + " updated vector:");
}

int main() {
	const std::string separation = "------------------------------------------------------";
	const int CLIENTS_COUNT = 20;

	auto* manager = sharedSegmentInt::getInstance();
	std::vector<std::thread> clients;

	for (size_t i = 0; i < CLIENTS_COUNT; ++i) {
		clients.emplace_back(clientEmulation<int>, i+1, std::ref(*manager), std::ref(separation));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	for (auto& client : clients) {
		client.join();
	}
	std::cout << separation << std::endl;

	std::cout << "Shared vector size: " << manager->getVectorSize() << std::endl;

	std::cout << std::endl;

	manager->destroySegment();

	return 0;
}