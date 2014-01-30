#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include "OpenCLManager.hpp"

// TODO Make tests for:
// OpenCL 1.1 device available

namespace test
{

TEST_CASE("OpenCLManager: Can create instance of the manager.","[OpenCLManager]"){
    CHECK(oul::opencl() != NULL);
}

TEST_CASE("OpenCLManager: Can set debug mode.","[OpenCLManager]"){
    CHECK(oul::opencl()->getDebugMode() == false);
    oul::opencl()->setDebugMode(true);
    CHECK(oul::opencl()->getDebugMode() == true);
}

TEST_CASE("OpenCLManager: Can create a oul::Context with defaul oul::DeviceCriteria.","[OpenCLManager]"){
    oul::DeviceCriteria criteria;

    CHECK_NOTHROW(oul::opencl()->createContext(criteria));
}

TEST_CASE("OpenCLManager: OpenCL platforms installed.","[OpenCLManager]"){
    oul::DeviceCriteria criteria;

    CHECK(oul::opencl()->getPlatforms(oul::DEVICE_PLATFORM_ANY).size() != 0);
}

TEST_CASE("OpenCLManager: OpenCL device(s) available.","[OpenCLManager]"){
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_ANY);

    CHECK(oul::opencl()->getDevices(criteria).size() != 0);
}

TEST_CASE("OpenCLManager: OpenCL CPU device(s) available.","[OpenCLManager]"){
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_CPU);

    CHECK(oul::opencl()->getDevices(criteria).size() != 0);
}

TEST_CASE("OpenCLManager: OpenCL GPU device(s) available.","[OpenCLManager]"){
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(oul::DEVICE_TYPE_CPU);

    CHECK(oul::opencl()->getDevices(criteria).size() != 0);
}

TEST_CASE("DeviceCriteria: Default construction gives expected values.","[DeviceCriteria]"){
    oul::DeviceCriteria criteria;

    CHECK(criteria.getTypeCriteria() == oul::DEVICE_TYPE_ANY);
    CHECK(criteria.getDeviceCountMinCriteria() == 0);
    CHECK(criteria.getDeviceCountMaxCriteria() == 100);
    CHECK(criteria.getPlatformCriteria() == oul::DEVICE_PLATFORM_ANY);
    CHECK(criteria.getDevicePreference() == oul::DEVICE_PREFERENCE_NONE);
}

}//namespace test
