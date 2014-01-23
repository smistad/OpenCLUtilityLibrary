#include "OpenCLManager.hpp"

using namespace cl;

oul::OpenCLManager* oul::OpenCLManager::getInstance() {
    if(this->instance == NULL) {
        this->instance = new OpenCLManager();
    }
    return this->instance;
}

oul::Context oul::OpenCLManager::createContext(
        std::vector<cl::Device> devices) {
    return oul::Context(devices);
}

/**
 * This method parses program arguments into device criteria and returns a context
 */
oul::Context oul::OpenCLManager::createContext(int argc, char** argv) {
}

/**
 * This method finds a set of devices which satisfies the supplied device criteria and creates a context
 */
oul::Context oul::OpenCLManager::createContext(DeviceCriteria deviceCriteria) {

}

void oul::OpenCLManager::setDebugMode(bool mode) {
    this->debugMode = mode;
}
