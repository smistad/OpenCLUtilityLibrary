#ifndef GARBAGECOLLECTOR_HPP_
#define GARBAGECOLLECTOR_HPP_

#include "CL/OpenCL.hpp"
#include <set>

namespace oul {
class GarbageCollector {
    public:
        void addMemoryObject(cl::Memory *);
        void deleteMemoryObject(cl::Memory *);
        void deleteAllMemoryObjects();
    private:
        std::set<cl::Memory *> memoryObjects;

};
}

#endif /* GARBAGECOLLECTOR_HPP_ */
