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

	this->verifyQueueProfilingIsEnabled(queue);

	cl::Event startEvent = this->enqueueNewMarker(queue);
	startEvents.insert(std::make_pair(name, startEvent));
}

void RuntimeMeasurementsManager::stopCLTimer(std::string name, cl::CommandQueue queue) {
	if (!enabled)
		return;

	this->verifyQueueProfilingIsEnabled(queue);
	this->verifyThatEventExists(name);

	cl::Event startEvent = startEvents.at(name);
	cl_ulong start = startEvent.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    startEvents.erase(name);

	cl::Event endEvent = this->enqueueNewMarker(queue);
	cl_ulong end = endEvent.getProfilingInfo<CL_PROFILING_COMMAND_START>();

	double runtime_ms = (end - start) * 1.0e-6; //converting from nano- to milliseconds
	this->addSampleToRuntimeMeasurement(name, runtime_ms);
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
	return *timings.at(name).get();
}

void RuntimeMeasurementsManager::print(std::string name) {
	if (!enabled)
		return;

	timings.at(name)->print();
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
    return -1;
}

bool RuntimeMeasurementsManager::isEnabled() {
	return enabled;
}

cl::Event RuntimeMeasurementsManager::enqueueNewMarker(cl::CommandQueue queue) {
    cl::Event event;
#if !defined(CL_VERSION_1_2) || defined(CL_USE_DEPRECATED_OPENCL_1_1_APIS)
    // Use deprecated API
    queue.enqueueMarker(&event);
#else
    queue.enqueueMarkerWithWaitList(NULL, &event)
#endif
    queue.finish();

    return event;
}

void RuntimeMeasurementsManager::verifyThatEventExists(std::string name) {
    if (startEvents.count(name) == 0) {
        throw oul::Exception("Unknown CL timer");
    }
}

void RuntimeMeasurementsManager::verifyQueueProfilingIsEnabled(cl::CommandQueue queue) {
    if (queue.getInfo<CL_QUEUE_PROPERTIES>() != CL_QUEUE_PROFILING_ENABLE) {
        throw oul::Exception(
                "Failed to get profiling info. Make sure that RuntimeMeasurementManager::enable() is called before the OpenCL context is created.",
                __LINE__, __FILE__);
    }
}

void RuntimeMeasurementsManager::addSampleToRuntimeMeasurement(std::string name, double runtime) {
    if (timings.count(name) == 0) {
        // No timings with this name exists, create a new one
        RuntimeMeasurementPtr runtimeMeasurement(new RuntimeMeasurement(name));
        runtimeMeasurement->addSample(runtime);
        timings[name] =  runtimeMeasurement;
    } else {
        timings.at(name)->addSample(runtime);
    }
}

} //namespace oul
