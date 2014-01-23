#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <CL/cl.hpp>
#include <vector>

namespace oul {

class Context {
    public:
        Context(std::vector<cl::Device> devices);
    private:
        cl::Context context;
        std::vector<cl::CommandQueue> queues;
        std::vector<cl::Program> programs;
        cl::Platform platform;
};

};

#endif /* CONTEXT_HPP_ */
