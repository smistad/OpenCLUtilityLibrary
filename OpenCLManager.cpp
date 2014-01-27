#include "OpenCLManager.hpp"
#include <iostream>

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

bool OpenCLManager::deviceHasOpenGLInteropCapability(cl::Device device) {
    // TODO
}

OpenCLManager::OpenCLManager()
{
	debugMode = false;
	cl::Platform::get(&platforms);
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

	if(debugMode)
	    std::cout << "Found " << platforms.size() << " OpenCL platforms." << std::endl;

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
    // TODO: Can only use one platform for a context. Need to select the best platform if multiple platforms are selected
	if(debugMode)
	    std::cout << validPlatforms.size() << " platforms selected for inspection." << std::endl;

	// Create a vector of devices for each platform
	std::vector<cl::Device> * platformDevices = new std::vector<cl::Device>[validPlatforms.size()];
	bool * devicePlatformVendorMismatch = new bool[validPlatforms.size()];
    for(int i = 0; i < validPlatforms.size(); i++) {
        if(debugMode)
            std::cout << "Platform " << i << ": " << validPlatforms[i].getInfo<CL_PLATFORM_VENDOR>() << std::endl;
        // Next, get all devices of correct type for each of those platforms
        std::vector<cl::Device> devices;
        cl_device_type deviceType;
        if(deviceCriteria.getTypeCriteria() == DEVICE_TYPE_ANY) {
            deviceType = CL_DEVICE_TYPE_ALL;
            if(debugMode)
                std::cout << "Looking for all types of devices." << std::endl;
        } else if(deviceCriteria.getTypeCriteria() == DEVICE_TYPE_GPU) {
            deviceType = CL_DEVICE_TYPE_GPU;
            if(debugMode)
                std::cout << "Looking for GPU devices only." << std::endl;
        } else if(deviceCriteria.getTypeCriteria() == DEVICE_TYPE_CPU) {
            deviceType = CL_DEVICE_TYPE_CPU;
            if(debugMode)
                std::cout << "Looking for CPU devices only." << std::endl;
        }
        platforms[i].getDevices(deviceType, &devices);
        if(debugMode)
            std::cout << devices.size() << " devices found for this platform." << std::endl;

        // Go through each device and see if they have the correct capabilities (if any)
        for(int j = 0; j < devices.size(); j++) {
            if(debugMode)
                std::cout << "Inspecting device " << j << " with the name " << devices[j].getInfo<CL_DEVICE_NAME>() << std::endl;
            std::vector<DeviceCapability> capabilityCriteria = deviceCriteria.getCapabilityCriteria();
            for(int k = 0; k < capabilityCriteria.size(); k++) {
                // TODO: implement some capability criteria
                if(capabilityCriteria[k] == DEVICE_CAPABILITY_OPENGL_INTEROP) {
                    if(!deviceHasOpenGLInteropCapability(devices[j]))
                        continue;
                } else if(capabilityCriteria[k] == DEVICE_CAPABILITY_NOT_CONNECTED_TO_SCREEN) {
                    if(deviceHasOpenGLInteropCapability(devices[j]))
                        continue;
                }
            }
            if(debugMode)
                std::cout << "The device was accepted." << std::endl;
            platformDevices[i].push_back(devices[j]);

            // TODO: Check for a device-platform mismatch.
            // This happens for instance if we try to use the AMD platform on a Intel CPU
            // In this case, the Intel platform would be preferred.
            devicePlatformVendorMismatch[i] = false;

            // Watch the device count
            if(deviceCriteria.getDeviceCount() != DEVICE_COUNT_INFINITE && deviceCriteria.getDeviceCount() == platformDevices[i].size())
                break;
        }
    }

    // Now, finally, select the best platform and its devices by inspecting the platformDevices list
    int bestPlatform = -1;
    for(int i = 0; i < validPlatforms.size(); i++) {
        if(platformDevices[i].size() > 0) { // Make sure the platform has some devices that has all criteria
            if(bestPlatform == -1) {
                bestPlatform = i;
            } else {
                // Check devicePlatformVendorMismatch
                if(devicePlatformVendorMismatch[bestPlatform] == true &&
                        devicePlatformVendorMismatch[i] == false) {
                    bestPlatform = i;
                } else if(platformDevices[i].size() > platformDevices[bestPlatform].size()) {
                    // If more than one platform has not devicePlatformVendorMismatch. Select the one with most devices
                    bestPlatform = i;
                }
            }
        }
    }
    if(bestPlatform == -1) {
        // TODO: throw an exception
    } else if(debugMode) {
        std::cout << "The platform " << validPlatforms[bestPlatform].getInfo<CL_PLATFORM_NAME>() << " was selected as the best platform." << std::endl;
        std::cout << "A total of " << platformDevices[bestPlatform].size() << " devices were selected for the context from this platform." << std::endl;
    }
    validDevices = platformDevices[bestPlatform];

    // Cleanup
    delete[] platformDevices;
    delete[] devicePlatformVendorMismatch;

    return Context(validDevices);
}

void OpenCLManager::setDebugMode(bool mode) {
    this->debugMode = mode;
}

OpenCLManager* OpenCLManager::instance = NULL;
OpenCLManager* opencl() { return OpenCLManager::getInstance(); }

}//namespace oul
