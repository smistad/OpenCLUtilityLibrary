#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include "CL/OpenCL.hpp"
#include <vector>
#include <map>
#include "Exceptions.hpp"
#include "GarbageCollector.hpp"

namespace oul {

class Context {
    public:
        Context() {garbageCollector = NULL;};
        Context(std::vector<cl::Device> devices, bool OpenGLInterop);
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
        cl::CommandQueue getQueue(unsigned int i);
        cl::Device getDevice(unsigned int i);
        cl::Context getContext();
        cl::Platform getPlatform();
        GarbageCollector * getGarbageCollector();
        ~Context() { if(garbageCollector != NULL) garbageCollector->deleteAllMemoryObjects(); };
    private:
        cl::Context context;
        std::vector<cl::CommandQueue> queues;
        std::map<std::string, int> programNames;
        std::vector<cl::Program> programs;
        std::vector<cl::Device> devices;
        cl::Platform platform;
        cl::Program buildSources(cl::Program::Sources source, std::string buildOptions);
        GarbageCollector * garbageCollector;
};

};

#endif /* CONTEXT_HPP_ */
