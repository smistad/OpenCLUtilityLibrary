#include "OpenCLManager.hpp"

namespace oul
{

OpenCLManager* OpenCLManager::getInstance()
{
    if(instance == NULL) {
        instance = new OpenCLManager();
    }
    return instance;
}

void OpenCLManager::shutdown()
{
	//TODO make sure all opencl threads  are finished before returning

  delete instance;
  instance = NULL;
}

OpenCLManager::OpenCLManager()
{
	debugMode = false;
}

Context OpenCLManager::createContext(std::vector<cl::Device> devices) {
    return Context(devices);
}

/**
 * This method parses program arguments into device criteria and returns a context
 */
Context OpenCLManager::createContext(int argc, char** argv) {

	//TODO
}

/**
 * This method finds a set of devices which satisfies the supplied device criteria and creates a context
 */
Context OpenCLManager::createContext(DeviceCriteria deviceCriteria) {

    std::vector<cl::Device> validDevices;

    // First, get all the platforms that fit the platform criteria
    std::vector<cl::Platform> validPlatforms;
    if(deviceCriteria.getPlatformCriteria() == DEVICE_PLATFORM_ANY) {
        validPlatforms = this->platforms;
    } else {
        // Find the correct platform and add to validPlatforms
        std::string find;
        switch(deviceCriteria.getPlatformCriteria()) {
            case DEVICE_PLATFORM_NVIDIA:
                find = "NVIDIA";
            break;
            case DEVICE_PLATFORM_AMD:
                find = "Advanced Micro Devices";
            break;
            case DEVICE_PLATFORM_INTEL:
                find = "Intel";
            break;
            case DEVICE_PLATFORM_APPLE:
                find = "Apple";
            break;
            case DEVICE_PLATFORM_ANY:
            break;
        }
        for(int i = 0; i < platforms.size(); i++) {
            if(platforms[i].getInfo<CL_PLATFORM_VENDOR>().find(find) != std::string::npos) {
                validPlatforms.push_back(platforms[i]);
                break;
            }
        }
    }

    for(int i = 0; i < validPlatforms.size(); i++) {
        // Next, get all devices of correct type for each of those platforms
        std::vector<cl::Device> platformDevices;
        cl_device_type deviceType;
        if(deviceCriteria.getTypeCriteria() == DEVICE_TYPE_ANY) {
            deviceType = CL_DEVICE_TYPE_ALL;
        } else if(deviceCriteria.getTypeCriteria() == DEVICE_TYPE_GPU) {
            deviceType = CL_DEVICE_TYPE_GPU;
        } else if(deviceCriteria.getTypeCriteria() == DEVICE_TYPE_CPU) {
            deviceType = CL_DEVICE_TYPE_CPU;
        }
        platforms[i].getDevices(deviceType, &platformDevices);

        // Go through each device and see if they have the correct capabilities (if any)
        for(int j = 0; j < platformDevices.size(); j++) {
            if(deviceCriteria.getCapabilityCriteria().size() > 0) {
                // TODO: implement some capability criteria
            }
            validDevices.push_back(platformDevices[j]);

            // Watch the device count
            if(deviceCriteria.getDeviceCount() != DEVICE_COUNT_INFINITE && deviceCriteria.getDeviceCount() == validDevices.size())
                break;
        }

        // Watch the device count
        if(deviceCriteria.getDeviceCount() != DEVICE_COUNT_INFINITE && deviceCriteria.getDeviceCount() == validDevices.size())
            break;
    }

    return Context(validDevices);
}

void OpenCLManager::setDebugMode(bool mode) {
    this->debugMode = mode;
}

OpenCLManager* OpenCLManager::instance = NULL;
OpenCLManager* opencl() { return OpenCLManager::getInstance(); }

}//namespace oul
