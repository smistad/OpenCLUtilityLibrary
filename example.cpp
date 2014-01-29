#include "OpenCLManager.hpp"
#include "HelperFunctions.hpp"
#include <iostream>

using namespace cl;


void notify(cl_mem *, void * user_data) {
    std::cout << "Memory object was deleted." << std::endl;
}

int main() {
    oul::OpenCLManager * manager = oul::OpenCLManager::getInstance();
    manager->setDebugMode(true);
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    //criteria.setPlatformCriteria(oul::DEVICE_PLATFORM_INTEL);
    //criteria.setCapabilityCriteria(oul::DEVICE_CAPABILITY_OPENGL_INTEROP);
    //criteria.setDeviceCountCriteria(1);

    oul::Context context = manager->createContext(criteria);

    // Test garbage collector
    Buffer * a = new Buffer(context.getContext(), CL_MEM_READ_WRITE, sizeof(float)*100);
    oul::GarbageCollector * GC = context.getGarbageCollector();
    GC->addMemoryObject(a);
    a->setDestructorCallback((void (*)(_cl_mem *, void *))notify, NULL);
    float * data = new float[100]();
    context.getQueue(0).enqueueWriteBuffer(*a,CL_TRUE,0,sizeof(float)*100,data);
    GC->deleteMemoryObject(a);

    // Test run a kernel
    context.createProgramFromString(
            "__kernel void test() {\n"
            "    printf(\"Hello world! %d\\n\", 4);\n"
            "}\n"
    );

    Kernel testKernel(context.getProgram(0), "test");
    context.getQueue(0).enqueueTask(testKernel);
    context.getQueue(0).enqueueNDRangeKernel(testKernel, NullRange, NDRange(4,1,1));

    context.getQueue(0).finish();
}
