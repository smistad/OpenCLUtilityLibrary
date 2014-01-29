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
    cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

    programs.push_back(buildSources(source, buildOptions));
}

/**
 * Compile several source files together
 */
void Context::createProgramFromSource(
        std::vector<std::string> filenames,
        std::string buildOptions) {
    cl::Program::Sources sources;
    for(int i = 0; i < filenames.size(); i++) {
        sources.push_back(std::make_pair(filenames[i].c_str(), filenames[i].length()));
    }

    programs.push_back(buildSources(sources, buildOptions));
}

void Context::createProgramFromString(
        std::string code,
        std::string buildOptions) {
    // Read source file
    cl::Program::Sources source(1, std::make_pair(code.c_str(), code.length()));

    programs.push_back(buildSources(source, buildOptions));
}


cl::Program Context::buildSources(
        cl::Program::Sources source, 
        std::string buildOptions) {

    // Make program of the source code in the context
    cl::Program program = cl::Program(context, source);

    // Build program for the context devices
    try{
        program.build(devices, buildOptions.c_str());
        programs.push_back(program);
    } catch(cl::Error &error) {
        if(error.err() == CL_BUILD_PROGRAM_FAILURE) {
            // TODO: do this for all devices
            std::cout << "Build log:" << std::endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;
        }
        throw error;
    }
    return program;
}


void Context::createProgramFromBinary(
        std::string filename,
        std::string buildOptions) {
}

} //namespace oul
