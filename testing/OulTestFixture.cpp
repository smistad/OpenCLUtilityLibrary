#include "OulTestFixture.hpp"

#include "OpenCLManager.hpp"

namespace oul
{

OulTestFixture::OulTestFixture()
{
	opencl();
}

OulTestFixture::~OulTestFixture()
{
	opencl()->shutdown();
}

bool OulTestFixture::canBuildAndRunATestKernel()
{
	return false;
}

} /* namespace oul */
