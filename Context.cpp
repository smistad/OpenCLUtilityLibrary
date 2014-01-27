#include "Context.hpp"
#include <iostream>

namespace oul
{
Context::Context(std::vector<cl::Device> devices, bool OpenGLInterop, bool profilingEnabled) {
    this->devices = devices;
    // TODO: make sure that all devices have the same platform
    this->platform = devices[0].getInfo<CL_DEVICE_PLATFORM>();

    // TODO: OpenGL interop properties
    cl_context_properties properties[] = {
          CL_CONTEXT_PLATFORM,
          (cl_context_properties)(platform)(),
          0
    };
    this->context = cl::Context(devices,properties);

    // Create a command queue for each device
    for(int i = 0; i < devices.size(); i++) {
        if(profilingEnabled) {
            this->queues.push_back(cl::CommandQueue(context, devices[i], CL_QUEUE_PROFILING_ENABLE));
        } else {
            this->queues.push_back(cl::CommandQueue(context, devices[i]));
        }
    }
}
} //namespace oul
