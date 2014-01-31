#include "catch/catch.hpp"

#include "TestFixture.hpp"
#include "OpenCLManager.hpp"

// TODO Make tests for:
// OpenCL 1.1 device available

namespace test
{


//Helper functions
std::vector<cl::Device> getDevices(oul::DeviceType type){
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(type);

    return oul::opencl()->getDevices(criteria);
}

std::vector<cl::Device> getCPUDevices(){
    return getDevices(oul::DEVICE_TYPE_CPU);
}

std::vector<cl::Device> getGPUDevices(){
    return getDevices(oul::DEVICE_TYPE_GPU);
}

std::vector<cl::Device> getAllDevices(){
    return getDevices(oul::DEVICE_TYPE_ANY);
}
//------------------------------------------------------------------------------------------------
//Tests
TEST_CASE("Can create instance of the manager.","[oul][OpenCLManager]"){
    CHECK(oul::opencl() != NULL);
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
    CHECK(getAllDevices().size() != 0);
}

TEST_CASE("OpenCL CPU device(s) available.","[oul][OpenCLManager]"){
    CHECK(getCPUDevices().size() != 0);
}

TEST_CASE("OpenCL GPU device(s) available.","[oul][OpenCLManager]"){
    CHECK(getGPUDevices().size() != 0);
}

//This test fails on Apple
TEST_CASE("At least one OpenCL device has OpenGL interop capability.","[oul][OpenCLManage][OpenGL]"){
    bool foundOpenGLInteropCapableDevice  = false;

    std::vector<cl::Device> devices = getAllDevices();
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

}//namespace test
