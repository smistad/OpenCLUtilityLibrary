#include "TestFixture.hpp"

#include "OpenCLManager.hpp"

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

bool TestFixture::canBuildAndRunATestKernel()
{
	return false;
}

} /* namespace oul */
