#ifndef TESTFIXTURE_HPP_
#define TESTFIXTURE_HPP_

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

	bool canBuildAndRunATestKernel();
};

} /* namespace oul */

#endif /* TESTFIXTURE_HPP_ */
