#include "OpenCLManager.hpp"
#include <iostream>

#if defined(__APPLE__) || defined(__MACOSX)
   #include <OpenCL/cl_gl.h>
   #include <OpenGL/OpenGL.h>
#else
#if _WIN32
#else
   #include <GL/glx.h>
   #include <CL/cl_gl.h>
#endif
#endif

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
    // Get the cl_device_id of the device
    cl_device_id deviceID = device();
    // Get the platform of device
    cl::Platform platform = device.getInfo<CL_DEVICE_PLATFORM>();
    // Get all devices that are capable of OpenGL interop with this platform
    // Create properties for CL-GL context
    #if defined(__APPLE__) || defined(__MACOSX)
    // Apple (untested)
    // TODO: create GL context for Apple
    cl_context_properties cps[] = {
       CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
       (cl_context_properties)CGLGetShareGroup(CGLGetCurrentContext()),
       0};

    return false;
    #else
    #ifdef _WIN32
    // Windows
    // TODO: create GL context for Windows
      cl_context_properties cps[] = {
          CL_GL_CONTEXT_KHR,
          (cl_context_properties)wglGetCurrentContext(),
          CL_WGL_HDC_KHR,
          (cl_context_properties)wglGetCurrentDC(),
          CL_CONTEXT_PLATFORM,
          (cl_context_properties)(platforms[j])(),
          0
      };
    return false;
    #else
    // Linux
    // Create a GL context using glX
    int  sngBuf[] = {
                GLX_RGBA,
                GLX_RED_SIZE, 1,
                GLX_GREEN_SIZE, 1,
                GLX_BLUE_SIZE, 1,
                GLX_DEPTH_SIZE, 12,
                None };

    // TODO: should probably free this stuff
    Display * display = XOpenDisplay(0);
    XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), sngBuf);
    GLXContext gl2Context = glXCreateContext(display, vi, 0, GL_TRUE);

    if(gl2Context == NULL) {
        throw Exception("Could not create a GL 2.1 context, please check your graphics drivers");
    }

    cl_context_properties cps[] = {
          CL_GL_CONTEXT_KHR,
          (cl_context_properties)gl2Context,
          CL_GLX_DISPLAY_KHR,
          (cl_context_properties)display,
          CL_CONTEXT_PLATFORM,
          (cl_context_properties)(platform)(),
          0
    };
    if(debugMode)
        std::cout << "Current glX context is: " << cps[1] << std::endl;

    // check if any of these devices have the same cl_device_id as deviceID
    // Query which devices are associated with GL context
    cl_device_id cl_gl_device_ids[32];
    size_t returnSize = 0;
    clGetGLContextInfoKHR_fn glGetGLContextInfo_func = (clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddress("clGetGLContextInfoKHR");
    glGetGLContextInfo_func(
            cps,
            CL_DEVICES_FOR_GL_CONTEXT_KHR,
            32*sizeof(cl_device_id),
            &cl_gl_device_ids,
            &returnSize
    );

    if(debugMode)
        std::cout << "There are " << returnSize / sizeof(cl_device_id) << " devices that can be associated with the GL context" << std::endl;

    bool found = false;
    for(int i = 0; i < returnSize/sizeof(cl_device_id); i++) {
        cl::Device device2(cl_gl_device_ids[i]);
        if(deviceID == device2()) {
            found = true;
            break;
        }
    }
    return found;
    #endif
    #endif
}

bool OpenCLManager::devicePlatformMismatch(
        cl::Device device,
        cl::Platform platform) {
    // First, find the type/vendor the of platform
    DevicePlatform platformVendor;
    std::string platformVendorStr = platform.getInfo<CL_PLATFORM_VENDOR>();
    if(platformVendorStr.find("Advanced Micro Devices, Inc.") != std::string::npos) {
        platformVendor = DEVICE_PLATFORM_AMD;
    } else if(platformVendorStr.find("Apple") != std::string::npos) {
        platformVendor = DEVICE_PLATFORM_APPLE;
    } else if(platformVendorStr.find("Intel") != std::string::npos) {
        platformVendor = DEVICE_PLATFORM_INTEL;
    } else if(platformVendorStr.find("NVIDIA") != std::string::npos) {
        platformVendor = DEVICE_PLATFORM_NVIDIA;
    }

    // Find the device vendor
    DevicePlatform deviceVendor;
    std::string deviceVendorStr = device.getInfo<CL_DEVICE_VENDOR>();
    if(deviceVendorStr.find("Intel") != std::string::npos) {
        deviceVendor = DEVICE_PLATFORM_INTEL;
    } else if(deviceVendorStr.find("Advanced Micro Devices, Inc.") != std::string::npos) {
        deviceVendor = DEVICE_PLATFORM_AMD;
    } else if(deviceVendorStr.find("NVIDIA") != std::string::npos) {
        deviceVendor = DEVICE_PLATFORM_NVIDIA;
    }

    return platformVendor != deviceVendor;
}

OpenCLManager::OpenCLManager()
{
	debugMode = false;
	cl::Platform::get(&platforms);
}

Context OpenCLManager::createContext(std::vector<cl::Device> devices, bool OpenGLInterop, bool profilingEnabled) {
    return Context(devices, OpenGLInterop, profilingEnabled);
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

    if(platforms.size() == 0)
        throw NoPlatformsInstalledException();

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
    bool OpenGLInterop = false;
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
            bool accepted = true;
            for(int k = 0; k < capabilityCriteria.size(); k++) {
                if(capabilityCriteria[k] == DEVICE_CAPABILITY_OPENGL_INTEROP) {
                    OpenGLInterop = true;
                    if(!deviceHasOpenGLInteropCapability(devices[j]))
                        accepted = false;
                } else if(capabilityCriteria[k] == DEVICE_CAPABILITY_NOT_CONNECTED_TO_SCREEN) {
                    if(deviceHasOpenGLInteropCapability(devices[j]))
                        accepted = false;
                }
            }
            if(accepted) {
                if(debugMode)
                    std::cout << "The device was accepted." << std::endl;
                platformDevices[i].push_back(devices[j]);
            }

            // Check for a device-platform mismatch.
            // This happens for instance if we try to use the AMD platform on a Intel CPU
            // In this case, the Intel platform would be preferred.
            devicePlatformVendorMismatch[i] = devicePlatformMismatch(devices[j], validPlatforms[i]);
            if(debugMode && devicePlatformVendorMismatch[i])
                std::cout << "A device-platform mismatch was detected." << std::endl;

            // Watch the device count
            if(deviceCriteria.getDeviceCountMaxCriteria() == platformDevices[i].size())
                break;
        }
    }

    // Now, finally, select the best platform and its devices by inspecting the platformDevices list
    int bestPlatform = -1;
    for(int i = 0; i < validPlatforms.size(); i++) {
        if(platformDevices[i].size() > 0) { // Make sure the platform has some devices that has all criteria
            if(bestPlatform == -1) {
                bestPlatform = i;
            } else if(platformDevices[i].size() >= deviceCriteria.getDeviceCountMinCriteria()) { // was enough devices found?
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
        throw NoValidPlatformsException();
    } else if(debugMode) {
        std::cout << "The platform " << validPlatforms[bestPlatform].getInfo<CL_PLATFORM_NAME>() << " was selected as the best platform." << std::endl;
        std::cout << "A total of " << platformDevices[bestPlatform].size() << " devices were selected for the context from this platform." << std::endl;
    }
    validDevices = platformDevices[bestPlatform];

    // Cleanup
    delete[] platformDevices;
    delete[] devicePlatformVendorMismatch;

    return oul::Context(validDevices, OpenGLInterop, false);
}

void OpenCLManager::setDebugMode(bool mode) {
    this->debugMode = mode;
}

OpenCLManager* OpenCLManager::instance = NULL;
bool OpenCLManager::debugMode = false;
OpenCLManager* opencl() { return OpenCLManager::getInstance(); }

}//namespace oul
