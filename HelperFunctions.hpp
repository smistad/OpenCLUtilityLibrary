#ifndef HELPER_FUNCTIONS_HPP
#define HELPER_FUNCTIONS_HPP

#include "OpenCLUtilityLibrary_Export.h"

#include <string>
#include "CL/OpenCL.hpp"

/*
 * A set of useful functions that are not dependent on any of the other objects
 */
namespace oul{

cl::size_t<3> OPENCLUTILITYLIBRARY_EXPORT createRegion(unsigned int x, unsigned int y, unsigned int z);
cl::size_t<3> OPENCLUTILITYLIBRARY_EXPORT createOrigoRegion();

std::string OPENCLUTILITYLIBRARY_EXPORT getCLErrorString(cl_int err);

std::string OPENCLUTILITYLIBRARY_EXPORT readFile(std::string filename);

cl_context_properties OPENCLUTILITYLIBRARY_EXPORT *createInteropContextProperties(
        const cl::Platform &platform,
        cl_context_properties OpenGLContext,
        cl_context_properties display);


}; // End namespace oul

#endif
