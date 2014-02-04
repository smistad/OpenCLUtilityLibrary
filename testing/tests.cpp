#include "catch/catch.hpp"

#include "TestFixture.hpp"
#include "OpenCLManager.hpp"

namespace test
{
TEST_CASE("Can create instance of the manager.","[oul][OpenCL]"){
    CHECK(oul::opencl());
}

TEST_CASE("Can set debug mode.","[oul][OpenCL]"){
    CHECK(oul::opencl()->getDebugMode() == false);
    oul::opencl()->setDebugMode(true);
    CHECK(oul::opencl()->getDebugMode() == true);
}

TEST_CASE("Can create a oul::Context with defaul oul::DeviceCriteria.","[oul][OpenCL]"){
    oul::DeviceCriteria criteria;

    CHECK_NOTHROW(oul::opencl()->createContext(criteria));
}

TEST_CASE("OpenCL platform(s) installed.","[oul][OpenCL]"){
    oul::DeviceCriteria criteria;

    CHECK(oul::opencl()->getPlatforms(oul::DEVICE_PLATFORM_ANY).size() != 0);
}

TEST_CASE("OpenCL device(s) available.","[oul][OpenCL]"){
    oul::TestFixture fixture;
    CHECK(fixture.isAnyDeviceAvailable());
}

TEST_CASE("OpenCL CPU device(s) available.","[oul][OpenCL]"){
    oul::TestFixture fixture;
    CHECK(fixture.isCPUDeviceAvailable());
}

TEST_CASE("OpenCL GPU device(s) available.","[oul][OpenCL]"){
    oul::TestFixture fixture;
    CHECK(fixture.isGPUDeviceAvailable());
}

//TODO This test fails on Apple
//Apple functionality not implemented yet
TEST_CASE("At least one OpenCL device has OpenGL interop capability.","[oul][OpenCL][OpenGL]"){
    oul::TestFixture fixture;
    bool foundOpenGLInteropCapableDevice  = false;

    std::vector<cl::Device> devices = fixture.getAllDevices();
    for(unsigned int i=0; i< devices.size(); i++){
        foundOpenGLInteropCapableDevice = foundOpenGLInteropCapableDevice or oul::OpenCLManager::deviceHasOpenGLInteropCapability(devices[i]);
    }

    CHECK(foundOpenGLInteropCapableDevice);
}

TEST_CASE("Default construction gives expected values.","[oul][OpenCL]"){
    oul::TestFixture fixture;
    oul::DeviceCriteria criteria;

    CHECK(criteria.getTypeCriteria() == oul::DEVICE_TYPE_ANY);
    CHECK(criteria.getDeviceCountMinCriteria() == 0);
    CHECK(criteria.getDeviceCountMaxCriteria() == 100);
    CHECK(criteria.getPlatformCriteria() == oul::DEVICE_PLATFORM_ANY);
    CHECK(criteria.getDevicePreference() == oul::DEVICE_PREFERENCE_NONE);
}

//TODO make a better test for the devicePlatformMismatch function...
TEST_CASE("Check for device and platform mismatch","[oul][OpenCL]"){
    oul::TestFixture fixture;
    oul::Context context = oul::opencl()->createContext(oul::DeviceCriteria());

    bool mismatch = oul::OpenCLManager::devicePlatformMismatch(context.getDevice(0), context.getPlatform());

#if defined(__APPLE__) || defined(__MACOSX)
    CHECK(mismatch);
#else
    CHECK_FALSE(mismatch);
#endif
}

TEST_CASE("Can run simple kernel from string","[oul][OpenCL]"){
    oul::TestFixture fixture;
    oul::Context context = oul::opencl()->createContext(oul::DeviceCriteria());
    CHECK_NOTHROW(fixture.canRunCodeFromString(context, fixture.getTestCode(), "test"));
}

TEST_CASE("Can read TestKernels.cl", "[oul]"){
    oul::TestFixture fixture;
    CHECK_NOTHROW(fixture.canReadTestKernelFile());
}

TEST_CASE("Can create a program from file","[oul][OpenCL]"){
    oul::TestFixture fixture;
    oul::Context context = oul::opencl()->createContext(oul::DeviceCriteria());
    CHECK_NOTHROW(fixture.canRunCodeFromFile(context, "test_one"));
}


}//namespace test
