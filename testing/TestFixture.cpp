#include "TestFixture.hpp"

// TODO: a non elegant hack here to make this work:
#include "../OpenCLManager.hpp"

namespace oul
{

TestFixture::TestFixture()
{
	opencl();
}

TestFixture::~TestFixture()
{
	opencl()->shutdown();
}

std::vector<cl::Device> TestFixture::getDevices(oul::DeviceType type){
    oul::DeviceCriteria criteria;
    criteria.setTypeCriteria(type);

    return oul::opencl()->getDevices(criteria);
}

std::vector<cl::Device> TestFixture::getCPUDevices(){
    return getDevices(oul::DEVICE_TYPE_CPU);
}

std::vector<cl::Device> TestFixture::getGPUDevices(){
    return getDevices(oul::DEVICE_TYPE_GPU);
}

std::vector<cl::Device> TestFixture::getAllDevices(){
    return getDevices(oul::DEVICE_TYPE_ANY);
}

} /* namespace oul */
