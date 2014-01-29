#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include "OpenCLManager.hpp"
#include <string>

namespace oul {

cl::size_t<3> createRegion(unsigned int x, unsigned int y, unsigned int z);
cl::size_t<3> createOrigoRegion();

std::string getCLErrorString(cl_int err) {

}
}; // End namespace oul


#endif
