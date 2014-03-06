#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include "CL/OpenCL.hpp"
#include <vector>
#include <map>
#include "Exceptions.hpp"
#include "GarbageCollector.hpp"
#include "Reporter.hpp"

namespace oul {

/**
 * This class holds an OpenCL context, with all of its queues and devices.
 * Its main purpose is to be a class that can't be sent between different 
 * functions and objects that does OpenCL processing.
 */
class Context {

public:
    Context() {garbageCollector = NULL;};
    Context(std::vector<cl::Device> devices, unsigned long * OpenGLContext);

    int createProgramFromSource(std::string filename, std::string buildOptions = "");
    int createProgramFromSource(std::vector<std::string> filenames, std::string buildOptions = "");
    int createProgramFromString(std::string code, std::string buildOptions = "");
    int createProgramFromBinary(std::string filename, std::string buildOptions = "");
    int createProgramFromSourceWithName(std::string programName, std::string filename, std::string buildOptions = "");
    int createProgramFromSourceWithName(std::string programName, std::vector<std::string> filenames, std::string buildOptions = "");
    int createProgramFromStringWithName(std::string programName, std::string code, std::string buildOptions = "");
    int createProgramFromBinaryWithName(std::string programName, std::string filename, std::string buildOptions = "");
    cl::Program getProgram(unsigned int i);
    cl::Program getProgram(std::string name);
    bool hasProgram(std::string name);

    cl::Kernel createKernel(cl::Program program, std::string kernel_name);
    void executeKernel(cl::CommandQueue queue, cl::Kernel kernel, size_t global_work_size, size_t local_work_size);

    cl::Buffer createBuffer(cl::Context context, cl_mem_flags flags, size_t size, void * host_data, std::string bufferName);
    void readBuffer(cl::CommandQueue queue, cl::Buffer outputBuffer, size_t outputVolumeSize, void *outputData);

    cl::CommandQueue getQueue(unsigned int i);
    cl::Device getDevice(unsigned int i);
    cl::Context getContext();
    cl::Platform getPlatform();
    GarbageCollector * getGarbageCollector();

private:
    cl::Program buildSources(cl::Program::Sources source, std::string buildOptions);

    Reporter reporter;
    cl::Context context;
    std::vector<cl::CommandQueue> queues;
    std::map<std::string, int> programNames;
    std::vector<cl::Program> programs;
    std::vector<cl::Device> devices;
    cl::Platform platform;
    GarbageCollector * garbageCollector;
};

};

#endif /* CONTEXT_HPP_ */
