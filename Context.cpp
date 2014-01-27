#include "Context.hpp"
#include <iostream>
#include <fstream>

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

void Context::createProgramFromSource(
        std::string filename,
        std::string buildOptions) {
    // Read source file
    std::ifstream sourceFile(filename.c_str());
    if(sourceFile.fail())
        throw Exception("Failed to open OpenCL source file.");
    std::string sourceCode(
        std::istreambuf_iterator<char>(sourceFile),
        (std::istreambuf_iterator<char>()));
    cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

    // Make program of the source code in the context
    cl::Program program = cl::Program(context, source);

    // Build program for the context devices
    try{
        program.build(devices, buildOptions.c_str());
    } catch(cl::Error &error) {
        if(error.err() == CL_BUILD_PROGRAM_FAILURE) {
            // TODO: do this for all devices
            std::cout << "Build log:" << std::endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;
        }
        throw error;
    }
}

void Context::createProgramFromBinary(
        std::string filename,
        std::string buildOptions) {
}

} //namespace oul
