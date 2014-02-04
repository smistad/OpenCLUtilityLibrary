#include "Context.hpp"

#include <iostream>
#include "HelperFunctions.hpp"
#include "OpenCLManager.hpp"

namespace oul
{

Context::Context(std::vector<cl::Device> devices, bool OpenGLInterop, bool profilingEnabled) {
    this->garbageCollector = new GarbageCollector;
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

void Context::createProgramFromSource(std::string filename, std::string buildOptions) {
    std::string sourceCode = readFile(filename);

    cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));
    cl::Program program = buildSources(source, buildOptions);
    programs.push_back(program);
}

/**
 * Compile several source files together
 */
void Context::createProgramFromSource(std::vector<std::string> filenames, std::string buildOptions) {
    cl::Program::Sources sources;
    for(int i = 0; i < filenames.size(); i++) {
        sources.push_back(std::make_pair(filenames[i].c_str(), filenames[i].length()));
    }

    cl::Program program = buildSources(sources, buildOptions);
    programs.push_back(program);
}

void Context::createProgramFromString(std::string code, std::string buildOptions) {
    cl::Program::Sources source(1, std::make_pair(code.c_str(), code.length()));

    cl::Program program = buildSources(source, buildOptions);
    programs.push_back(program);
}

cl::Program Context::getProgram(unsigned int i) {
    return programs[i];
}

cl::CommandQueue Context::getQueue(unsigned int i) {
    return queues[i];
}

cl::Device Context::getDevice(unsigned int i) {
    return devices[i];
}

cl::Context Context::getContext() {
    return context;
}

cl::Platform Context::getPlatform() {
    return platform;
}

GarbageCollector * Context::getGarbageCollector() {
    return garbageCollector;
}

cl::Program Context::buildSources(cl::Program::Sources source, std::string buildOptions) {

    // Make program of the source code in the context
    cl::Program program = cl::Program(context, source);

    // Build program for the context devices
    try{
        program.build(devices, buildOptions.c_str());
        programs.push_back(program);
    } catch(cl::Error &error) {
        if(error.err() == CL_BUILD_PROGRAM_FAILURE) {
            for(unsigned int i=0; i<devices.size(); i++){
                std::cout << "Build log, device " << i <<":\n" << std::endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[i]) << std::endl;
            }
        }
        std::cout << "[ERROR] " << getCLErrorString(error.err()) << std::endl;

        throw error;
    }
    return program;
}


void Context::createProgramFromBinary(std::string filename, std::string buildOptions) {
    //TODO todo
}

} //namespace oul
