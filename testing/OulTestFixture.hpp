#ifndef OULTESTFIXTURE_HPP_
#define OULTESTFIXTURE_HPP_

namespace oul
{
/**
 * \brief A collection of functionality for running tests on OpenCL
 *
 * \date Jan 24, 2014
 * \author Janne Beate Bakeng, SINTEF
 */

class OulTestFixture
{
public:
	OulTestFixture();
	~OulTestFixture();

	bool canBuildAndRunATestKernel();
};

} /* namespace oul */

#endif /* OULTESTFIXTURE_HPP_ */
