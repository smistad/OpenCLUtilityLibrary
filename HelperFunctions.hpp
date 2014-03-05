#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include <string>
#include "CL/OpenCL.hpp"

namespace oul{

cl::size_t<3> createRegion(unsigned int x, unsigned int y, unsigned int z);
cl::size_t<3> createOrigoRegion();

std::string getCLErrorString(cl_int err);

std::string readFile(std::string filename);

cl_context_properties * createInteropContextProperties(
        const cl::Platform &platform,
        cl_context_properties OpenGLContext,
        cl_context_properties display);


}; // End namespace oul

#endif
