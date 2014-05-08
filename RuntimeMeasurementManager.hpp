#ifndef RUNTIMEMEASUREMENTMANAGER_HPP_
#define RUNTIMEMEASUREMENTMANAGER_HPP_

#include <string>
#include <map>
#include "CL/OpenCL.hpp"
#include "RuntimeMeasurement.hpp"

namespace oul {

class RuntimeMeasurementsManager {

public:
	RuntimeMeasurementsManager();

	void enable();
	void disable();
	bool isEnabled();

	void startCLTimer(std::string name, cl::CommandQueue queue);
	void stopCLTimer(std::string name, cl::CommandQueue queue);

	void startRegularTimer(std::string name);
	void stopRegularTimer(std::string name);

	void startNumberedCLTimer(std::string name, cl::CommandQueue queue);
	void stopNumberedCLTimer(std::string name, cl::CommandQueue queue);

	void startNumberedRegularTimer(std::string name);
	void stopNumberedRegularTimer(std::string name);

	RuntimeMeasurement getTiming(std::string name);

	void print(std::string name);
	void printAll();

private:
	cl::Event enqueueNewMarker(cl::CommandQueue queue) ;
	void verifyThatEventExists(std::string name);
	void verifyQueueProfilingIsEnabled(cl::CommandQueue queue);
	void addSampleToRuntimeMeasurement(std::string name, double runtime);

	bool enabled;
	std::map<std::string, RuntimeMeasurementPtr> timings;
	std::map<std::string, unsigned int> numberings;
	std::map<std::string, cl::Event> startEvents;
};

typedef boost::shared_ptr<class RuntimeMeasurementsManager> RuntimeMeasurementsManagerPtr;

} //namespace oul

#endif /* RUNTIMEMEASUREMENTMANAGER_HPP_ */
