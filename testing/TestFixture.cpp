#include "TestFixture.hpp"

#include <iostream>
#include "CL/OpenCL.hpp"
#include "OpenCLManager.hpp"
#include "OulConfig.hpp"
#include "HelperFunctions.hpp"

namespace oul
{

TestFixture::TestFixture(){
	opencl();
	test_kernels = std::string(TEST_DIR)+"/TestKernels.cl";
}

TestFixture::~TestFixture(){
	opencl()->shutdown();
}

std::string TestFixture::getTestCode(){
    return "__kernel void test(void){size_t id = get_global_id(0);}";
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

bool TestFixture::isCPUDeviceAvailable(){
    return (this->getCPUDevices().size() != 0);
}

bool TestFixture::isGPUDeviceAvailable(){
    return (this->getGPUDevices().size() != 0);
}

bool TestFixture::isAnyDeviceAvailable(){
    return (this->getAllDevices().size() != 0);
}

void TestFixture::canReadTestKernelFile(){
    oul::readFile(test_kernels);
}

void TestFixture::canRunCodeFromString(oul::Context context, std::string source, std::string kernel_name){
    context.createProgramFromString(source);
    this->canRunProgramOnQueue(context.getProgram(0), context.getQueue(0), kernel_name);
}

void TestFixture::canRunCodeFromFile(oul::Context context, std::string kernel_name){
    context.createProgramFromSource(test_kernels);
    this->canRunProgramOnQueue(context.getProgram(0), context.getQueue(0), kernel_name);
}

void TestFixture::canRunProgramOnQueue(cl::Program program, cl::CommandQueue queue, std::string kernel_name){
    cl::Kernel kernel(program, kernel_name.c_str());
    queue.enqueueTask(kernel);
    //TODO remove magic numbers
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(4,1,1));
    queue.finish();
}

} /* namespace oul */
