#include "Context.hpp"

oul::Context::Context(std::vector<cl::Device> devices) {
    this->devices = devices;
}
