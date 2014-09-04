#include "RuntimeMeasurementManager.hpp"
#include "Exceptions.hpp"

namespace oul {

void RuntimeMeasurementsManager::enable() {
	enabled = true;
}

void RuntimeMeasurementsManager::disable() {
	enabled = false;
}

void RuntimeMeasurementsManager::startCLTimer(std::string name, cl::CommandQueue queue) {
	if (!enabled)
		return;

	if (queue.getInfo<CL_QUEUE_PROPERTIES>() != CL_QUEUE_PROFILING_ENABLE) {
		throw oul::Exception(
				"Failed to get profiling info. Make sure that RuntimeMeasurementManager::enable() is called before the OpenCL context is created.",
				__LINE__, __FILE__);
	}
	cl::Event startEvent;
#if !defined(CL_VERSION_1_2) || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
	// Use deprecated API
	queue.enqueueMarker(&startEvent);
#else
	queue.enqueueMarkerWithWaitList(NULL, &startEvent)
#endif
	queue.finish();
	startEvents.insert(std::make_pair(name, startEvent));
}

void RuntimeMeasurementsManager::stopCLTimer(std::string name, cl::CommandQueue queue) {
	if (!enabled)
		return;

	if (queue.getInfo<CL_QUEUE_PROPERTIES>() != CL_QUEUE_PROFILING_ENABLE) {
		throw oul::Exception(
				"Failed to get profiling info. Make sure that RuntimeMeasurementManager::enable() is called before the OpenCL context is created.",
				__LINE__, __FILE__);
	}

	// check that the startEvent actually exist
	if (startEvents.count(name) == 0) {
		throw oul::Exception("Unknown CL timer");
	}
	cl_ulong start, end;
	cl::Event endEvent;
#if !defined(CL_VERSION_1_2) || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
	// Use deprecated API
	queue.enqueueMarker(&endEvent);
#else
	queue.enqueueMarkerWithWaitList(NULL, &endEvent);
#endif
	queue.finish();
	cl::Event startEvent = startEvents[name];
	startEvent.getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_START, &start);
	endEvent.getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_START, &end);
	if (timings.count(name) == 0) {
		// No timings with this name exists, create a new one
		RuntimeMeasurementPtr runtime(new RuntimeMeasurement(name));
		runtime->addSample((end - start) * 1.0e-6);
		timings[name] =  runtime;
	} else {
		timings[name]->addSample((end - start) * 1.0e-6);
	}

	// Remove the start event
	startEvents.erase(name);
}

void RuntimeMeasurementsManager::startRegularTimer(std::string name) {
	if (!enabled)
		return;
}

void RuntimeMeasurementsManager::stopRegularTimer(std::string name) {
	if (!enabled)
		return;
}

void RuntimeMeasurementsManager::startNumberedCLTimer(std::string name, cl::CommandQueue queue) {
	if (!enabled)
		return;
}

void RuntimeMeasurementsManager::stopNumberedCLTimer(std::string name, cl::CommandQueue queue) {
	if (!enabled)
		return;
}

void RuntimeMeasurementsManager::startNumberedRegularTimer(std::string name) {
	if (!enabled)
		return;
}

void RuntimeMeasurementsManager::stopNumberedRegularTimer(std::string name) {
	if (!enabled)
		return;
}

RuntimeMeasurement RuntimeMeasurementsManager::getTiming(std::string name) {
//	return *timings[name].get(); //This causes segfaults in some situations
	return *timings.at(name).get();
}

void RuntimeMeasurementsManager::print(std::string name) {
	if (!enabled)
		return;

	timings[name]->print();
}

void RuntimeMeasurementsManager::printAll() {
	if (!enabled)
		return;

	std::map<std::string, RuntimeMeasurementPtr>::iterator it;
	for (it = timings.begin(); it != timings.end(); it++) {
		it->second->print();
	}
}

RuntimeMeasurementsManager::RuntimeMeasurementsManager() {
}

double RuntimeMeasurement::getSum() const {
	return sum;
}

double RuntimeMeasurement::getAverage() const {
	return sum / samples;
}

double RuntimeMeasurement::getStdDeviation() const {
	// TODO: implement
}

bool RuntimeMeasurementsManager::isEnabled() {
	return enabled;
}

} //namespace oul
