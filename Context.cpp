#include "Context.hpp"

#include <iostream>
#include "HelperFunctions.hpp"
#include "OpenCLManager.hpp"

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

Context::Context(std::vector<cl::Device> devices, bool OpenGLInterop, bool profilingEnabled) {
    this->garbageCollector = new GarbageCollector;
    this->devices = devices;
    // TODO: make sure that all devices have the same platform
    this->platform = devices[0].getInfo<CL_DEVICE_PLATFORM>();

    // TODO: OpenGL interop properties
    // TODO: must check that a OpenGL context and display is available
    // TODO: Use current context and display, or take this as input??
    cl_context_properties * cps;
    if(OpenGLInterop) {
#if defined(__APPLE__) || defined(__MACOSX)
        cps = OpenCLManager::createInteropContextProperties(
                this->platform,
                (cl_context_properties)CGLGetShareGroup(CGLGetCurrentContext()),
                NULL
        );
#else
#ifdef _WIN32
        cps = OpenCLManager::createInteropContextProperties(
                this->platform,
                (cl_context_properties)wglGetCurrentContext(),
                (cl_context_properties)wglGetCurrentDC()
        );
#else
        cps = OpenCLManager::createInteropContextProperties(
                this->platform,
                (cl_context_properties)glXGetCurrentContext(),
                (cl_context_properties)glXGetCurrentDisplay()
        );
#endif
#endif
    } else {
        cps = new cl_context_properties[3];
        cps[0] = CL_CONTEXT_PLATFORM;
        cps[1] = (cl_context_properties)(platform)();
        cps[2] = 0;
    }
    this->context = cl::Context(devices,cps);
    delete[] cps;

    // Create a command queue for each device
    for(int i = 0; i < devices.size(); i++) {
        if(profilingEnabled) {
            this->queues.push_back(cl::CommandQueue(context, devices[i], CL_QUEUE_PROFILING_ENABLE));
        } else {
            this->queues.push_back(cl::CommandQueue(context, devices[i]));
        }
    }
}

int Context::createProgramFromSource(std::string filename, std::string buildOptions) {
    std::string sourceCode = readFile(filename);

    cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));
    cl::Program program = buildSources(source, buildOptions);
    programs.push_back(program);
    return programs.size()-1;
}

/**
 * Compile several source files together
 */
int Context::createProgramFromSource(std::vector<std::string> filenames, std::string buildOptions) {
    cl::Program::Sources sources;
    for(int i = 0; i < filenames.size(); i++) {
        sources.push_back(std::make_pair(filenames[i].c_str(), filenames[i].length()));
    }

    cl::Program program = buildSources(sources, buildOptions);
    programs.push_back(program);
    return programs.size()-1;
}

int Context::createProgramFromString(std::string code, std::string buildOptions) {
    cl::Program::Sources source(1, std::make_pair(code.c_str(), code.length()));

    cl::Program program = buildSources(source, buildOptions);
    programs.push_back(program);
    return programs.size()-1;
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


int Context::createProgramFromBinary(std::string filename, std::string buildOptions) {
    //TODO todo
}

} //namespace oul
