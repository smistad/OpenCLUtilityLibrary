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

bool TestFixture::canBuildAndRunATestKernel()
{
	return false;
}

} /* namespace oul */
