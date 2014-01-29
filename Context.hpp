#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include "CL/OpenCL.hpp"
#include <vector>
#include "Exceptions.hpp"
#include "GarbageCollector.hpp"

namespace oul {

class Context {
    public:
        Context(std::vector<cl::Device> devices, bool OpenGLInterop, bool profilingEnabled);
        void createProgramFromSource(std::string filename, std::string buildOptions = "");
        void createProgramFromSource(std::vector<std::string> filenames, std::string buildOptions = "");
        void createProgramFromString(std::string code, std::string buildOptions = "");
        void createProgramFromBinary(std::string filename, std::string buildOptions = "");
        cl::Program getProgram(unsigned int i);
        cl::CommandQueue getQueue(unsigned int i);
        cl::Device getDevice(unsigned int i);
        cl::Context getContext();
        cl::Platform getPlatform();
        GarbageCollector * getGarbageCollector();
        ~Context() { garbageCollector->deleteAllMemoryObjects(); };
    private:
        cl::Context context;
        std::vector<cl::CommandQueue> queues;
        std::vector<cl::Program> programs;
        std::vector<cl::Device> devices;
        cl::Platform platform;
        cl::Program buildSources(cl::Program::Sources source, std::string buildOptions);
        GarbageCollector * garbageCollector;
};

};

#endif /* CONTEXT_HPP_ */
