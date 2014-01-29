#include "HelperFunctions.hpp"

using namespace oul;

cl::size_t<3> oul::createRegion(
        unsigned int x,
        unsigned int y,
        unsigned int z) {
    cl::size_t<3> region;
    region[0] = x;
    region[1] = y;
    region[2] = z;
    return region;
}

cl::size_t<3> oul::createOrigoRegion() {
    cl::size_t<3> region;
    region[0] = 0;
    region[1] = 0;
    region[2] = 0;
    return region;
}
