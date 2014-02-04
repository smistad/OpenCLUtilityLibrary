#ifndef TESTFIXTURE_HPP_
#define TESTFIXTURE_HPP_

#include "CL/OpenCL.hpp"
#include "DeviceCriteria.hpp"
#include "Context.hpp"

namespace oul
{
/**
 * \brief A collection of functionality for running tests on OpenCL
 *
 * \date Jan 24, 2014
 * \author Janne Beate Bakeng, SINTEF
 */

class TestFixture
{
public:
	TestFixture();
	~TestFixture();

	//will not throw exceptions
	std::string getTestCode();

	std::vector<cl::Device> getDevices(oul::DeviceType type);
	std::vector<cl::Device> getCPUDevices();
	std::vector<cl::Device> getGPUDevices();
	std::vector<cl::Device> getAllDevices();

	bool isCPUDeviceAvailable();
	bool isGPUDeviceAvailable();
	bool isAnyDeviceAvailable();

	//will throw exceptions if they fail
	void canReadTestKernelFile();
	void canRunCodeFromString(oul::Context context, std::string source, std::string kernel_name);
	void canRunCodeFromFile(oul::Context context, std::string kernel_name);
	void canRunProgramOnQueue(cl::Program program, cl::CommandQueue queue, std::string kernel_name);

private:
	std::string test_kernels;

};

} /* namespace oul */

#endif /* TESTFIXTURE_HPP_ */
