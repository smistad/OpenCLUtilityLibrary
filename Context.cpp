#include "Context.hpp"

namespace oul
{
Context::Context(std::vector<cl::Device> devices) {
    this->devices = devices;
}
} //namespace oul
