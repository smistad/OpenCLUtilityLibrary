#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include <CL/cl.hpp>
#include <vector>
#include "Context.hpp"
#include "DeviceCriteria.hpp"

namespace oul {

/**
 * Singleton class which is used mainly for creating OpenCL contexts in an easy way
 */
class OpenCLManager {
    public:
        static OpenCLManager * getInstance();
        oul::Context createContext(std::vector<cl::Device> devices);
        oul::Context createContext(int argc, char ** argv);
        oul::Context createContext(DeviceCriteria criteria);
        void setDebugMode(bool mode);
    private:
        static OpenCLManager * instance;
        OpenCLManager() {debugMode = false;};
        std::vector<cl::Platform> platforms;
        bool debugMode;
};



}; // namespace oul

#endif /* MANAGER_HPP_ */
