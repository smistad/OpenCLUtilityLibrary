#include "Context.hpp"

#include <iostream>
#include "HelperFunctions.hpp"
#include "RuntimeMeasurement.hpp"

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

Context::Context(std::vector<cl::Device> devices, bool OpenGLInterop) {
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
        cps = createInteropContextProperties(
                this->platform,
                (cl_context_properties)CGLGetShareGroup(CGLGetCurrentContext()),
                NULL
        );
#else
#ifdef _WIN32
        cps = createInteropContextProperties(
                this->platform,
                (cl_context_properties)wglGetCurrentContext(),
                (cl_context_properties)wglGetCurrentDC()
        );
#else
        cps = createInteropContextProperties(
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
        if(oul::RuntimeMeasurementsManager::isEnabled()) {
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
    // Do this in a weird way, because the the logical way does not work.
    std::string sourceCode = readFile(filenames[0]);
    cl::Program::Sources sources(filenames.size(), std::make_pair(sourceCode.c_str(), sourceCode.length()));
    for(int i = 1; i < filenames.size(); i++) {
        std::string sourceCode2 = readFile(filenames[i]);
        sources[i] = std::make_pair(sourceCode2.c_str(), sourceCode2.length());
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

int Context::createProgramFromSourceWithName(
        std::string programName,
        std::string filename,
        std::string buildOptions) {
    programNames[programName] = createProgramFromSource(filename,buildOptions);
    return programNames[programName];
}

int Context::createProgramFromSourceWithName(
        std::string programName,
        std::vector<std::string> filenames,
        std::string buildOptions) {
    programNames[programName] = createProgramFromSource(filenames,buildOptions);
    return programNames[programName];
}

int Context::createProgramFromStringWithName(
        std::string programName,
        std::string code,
        std::string buildOptions) {
    programNames[programName] = createProgramFromString(code,buildOptions);
    return programNames[programName];
}

int Context::createProgramFromBinaryWithName(
        std::string programName,
        std::string filename,
        std::string buildOptions) {
    programNames[programName] = createProgramFromBinary(filename,buildOptions);
    return programNames[programName];
}

cl::Program Context::getProgram(std::string name) {
    if(programNames.count(name) == 0) {
        std::string msg ="Could not find OpenCL program with the name" + name;
        throw Exception(msg.c_str(), __LINE__, __FILE__);
    }

    return programs[programNames[name]];
}

bool Context::hasProgram(std::string name) {
    return programNames.count(name) > 0;
}

} //namespace oul

