#include "OpenCLManager.hpp"
#include "HelperFunctions.hpp"

using namespace cl;

int main() {
    oul::OpenCLManager * manager = oul::OpenCLManager::getInstance();
    manager->setDebugMode(true);
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_GPU);
    //criteria.setPlatformCriteria(oul::DEVICE_PLATFORM_INTEL);
    //criteria.setCapabilityCriteria(oul::DEVICE_CAPABILITY_OPENGL_INTEROP);
    //criteria.setDeviceCountCriteria(1);

    oul::Context context = manager->createContext(criteria);
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
