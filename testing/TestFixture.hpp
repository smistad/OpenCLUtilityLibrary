#ifndef TESTFIXTURE_HPP_
#define TESTFIXTURE_HPP_

#include "CL/OpenCL.hpp"
#include "DeviceCriteria.hpp"

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

	std::vector<cl::Device> getDevices(oul::DeviceType type);
	std::vector<cl::Device> getCPUDevices();
	std::vector<cl::Device> getGPUDevices();
	std::vector<cl::Device> getAllDevices();
};

} /* namespace oul */

#endif /* TESTFIXTURE_HPP_ */
