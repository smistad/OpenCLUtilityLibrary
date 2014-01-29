#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include "OpenCLManager.hpp"

namespace oul {

cl::size_t<3> createRegion(unsigned int x, unsigned int y, unsigned int z);
cl::size_t<3> createOrigoRegion();

}; // End namespace oul


#endif
