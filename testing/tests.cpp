#include "catch/catch.hpp"

#include "TestFixture.hpp"
#include "OpenCLManager.hpp"

namespace test
{
TEST_CASE("Can create instance of the manager.","[oul][OpenCLManager]"){
    CHECK(oul::opencl());
}

TEST_CASE("Can set debug mode.","[oul][OpenCLManager]"){
    CHECK(oul::opencl()->getDebugMode() == false);
    oul::opencl()->setDebugMode(true);
    CHECK(oul::opencl()->getDebugMode() == true);
}

TEST_CASE("Can create a oul::Context with defaul oul::DeviceCriteria.","[oul][OpenCLManager]"){
    oul::DeviceCriteria criteria;

    CHECK_NOTHROW(oul::opencl()->createContext(criteria));
}

TEST_CASE("OpenCL platform(s) installed.","[oul][OpenCLManager]"){
    oul::DeviceCriteria criteria;

    CHECK(oul::opencl()->getPlatforms(oul::DEVICE_PLATFORM_ANY).size() != 0);
}

TEST_CASE("OpenCL device(s) available.","[oul][OpenCLManager]"){
    oul::TestFixture fixture;
    CHECK(fixture.getAllDevices().size() != 0);
}

TEST_CASE("OpenCL CPU device(s) available.","[oul][OpenCLManager]"){
    oul::TestFixture fixture;
    CHECK(fixture.getCPUDevices().size() != 0);
}

TEST_CASE("OpenCL GPU device(s) available.","[oul][OpenCLManager]"){
    oul::TestFixture fixture;
    CHECK(fixture.getGPUDevices().size() != 0);
}

//This test fails on Apple
TEST_CASE("At least one OpenCL device has OpenGL interop capability.","[oul][OpenCLManage][OpenGL]"){
    oul::TestFixture fixture;
    bool foundOpenGLInteropCapableDevice  = false;

    std::vector<cl::Device> devices = fixture.getAllDevices();
    for(unsigned int i=0; i< devices.size(); i++){
        foundOpenGLInteropCapableDevice = foundOpenGLInteropCapableDevice or oul::OpenCLManager::deviceHasOpenGLInteropCapability(devices[i]);
    }

    CHECK(foundOpenGLInteropCapableDevice);
}

TEST_CASE("Default construction gives expected values.","[oul][DeviceCriteria]"){
    oul::DeviceCriteria criteria;

    CHECK(criteria.getTypeCriteria() == oul::DEVICE_TYPE_ANY);
    CHECK(criteria.getDeviceCountMinCriteria() == 0);
    CHECK(criteria.getDeviceCountMaxCriteria() == 100);
    CHECK(criteria.getPlatformCriteria() == oul::DEVICE_PLATFORM_ANY);
    CHECK(criteria.getDevicePreference() == oul::DEVICE_PREFERENCE_NONE);
}

TEST_CASE("Check for device platform mismatch","[oul][DeviceCriteria][OpenCLManager]"){
    oul::DeviceCriteria criteria;
    oul::Context context = oul::opencl()->createContext(criteria);

    CHECK_FALSE(oul::OpenCLManager::devicePlatformMismatch(context.getDevice(0), context.getPlatform()));
}

TEST_CASE("Can run simple kernel.","[oul][Context]"){
    oul::DeviceCriteria criteria;
    oul::Context context = oul::opencl()->createContext(criteria);
    context.createProgramFromString("__kernel void test(void){size_t id = get_global_id(0);}");

    cl::Kernel testKernel(context.getProgram(0), "test");
    CHECK_NOTHROW(context.getQueue(0).enqueueTask(testKernel));
    CHECK_NOTHROW(context.getQueue(0).enqueueNDRangeKernel(testKernel, cl::NullRange, cl::NDRange(4,1,1)));

    CHECK_NOTHROW(context.getQueue(0).finish());
}


}//namespace test
