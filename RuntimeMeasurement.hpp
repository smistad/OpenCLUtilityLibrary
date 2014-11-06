#ifndef TIMING_HPP_
#define TIMING_HPP_

#include "openclutilitylibrary_export.h"

#include <string>
#include <boost/shared_ptr.hpp>

namespace oul
{
/**
 * A class for a runtime measurement
 */
class OPENCLUTILITYLIBRARY_EXPORT RuntimeMeasurement {

public:
	RuntimeMeasurement(std::string name);
	void addSample(double runtime);
	double getSum() const;
	double getAverage() const;
	double getStdDeviation() const;
	void print() const;

private:
	RuntimeMeasurement();

	double sum;
	unsigned int samples;
	std::string name;
};

typedef boost::shared_ptr<class RuntimeMeasurement> RuntimeMeasurementPtr;

}
;
// end namespace

#endif /* TIMING_HPP_ */
