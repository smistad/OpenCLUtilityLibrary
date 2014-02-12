#include "RuntimeMeasurement.hpp"
#include <iostream>
#include "HelperFunctions.hpp"

using namespace oul;
RuntimeMeasurementsManager* RuntimeMeasurementsManager::instance = NULL;
bool RuntimeMeasurementsManager::enabled = false;

RuntimeMeasurement::RuntimeMeasurement(std::string name) {
    sum = 0.0f;
    samples = 0;
    this->name = name;
}

void RuntimeMeasurement::addSample(double runtime) {
    samples++;
    sum += runtime;
}

void RuntimeMeasurement::print() const {
    std::cout << "Runtime of " << name << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
    if(samples == 0) {
        std::cout << "None recorded." << std::endl;
    } else if(samples == 1) {
        std::cout << sum << " ms" << std::endl;
    } else {
        std::cout << "Total: " << sum << " ms" << std::endl;
        std::cout << "Average: " << sum/samples << " ms" << std::endl;
        std::cout << "Standard deviation: " << "TODO" << " ms" << std::endl; //TODO
        std::cout << "Number of samples: " << samples << std::endl;
    }
    std::cout << "----------------------------------------------------" << std::endl;
}

void RuntimeMeasurementsManager::enable() {
    enabled = true;
}

void RuntimeMeasurementsManager::disable() {
    enabled = false;
}

RuntimeMeasurementsManager* RuntimeMeasurementsManager::getInstance() {
    if(instance == NULL) {
        instance = new RuntimeMeasurementsManager();
    }
    return instance;
}

void RuntimeMeasurementsManager::startCLTimer(
        std::string name,
        cl::CommandQueue queue) {
    if(!enabled)
        return;

    if(queue.getInfo<CL_QUEUE_PROPERTIES>() != CL_QUEUE_PROFILING_ENABLE) {
        throw oul::Exception("Failed to get profiling info. Make sure that RuntimeMeasurementManager::enable() is called before the OpenCL context is created.", __LINE__, __FILE__);
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

void RuntimeMeasurementsManager::stopCLTimer(
        std::string name,
        cl::CommandQueue queue) {
    if(!enabled)
        return;

    if(queue.getInfo<CL_QUEUE_PROPERTIES>() != CL_QUEUE_PROFILING_ENABLE) {
        throw oul::Exception("Failed to get profiling info. Make sure that RuntimeMeasurementManager::enable() is called before the OpenCL context is created.", __LINE__, __FILE__);
    }

    // check that the startEvent actually exist
    if(startEvents.count(name) == 0) {
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
    if(timings.count(name) == 0) {
        // No timings with this name exists, create a new one
        RuntimeMeasurement runtime(name);
        runtime.addSample((end-start)*1.0e-6);
        timings.insert(std::make_pair(name, runtime));
    } else {
        RuntimeMeasurement &runtime = timings[name];
        runtime.addSample((end-start)*1.0e-6);
    }

    // Remove the start event
    startEvents.erase(name);
}

void RuntimeMeasurementsManager::startRegularTimer(std::string name) {
    if(!enabled)
        return;
}

void RuntimeMeasurementsManager::stopRegularTimer(std::string name) {
    if(!enabled)
        return;
}

void RuntimeMeasurementsManager::startNumberedCLTimer(
        std::string name,
        cl::CommandQueue queue) {
    if(!enabled)
        return;
}

void RuntimeMeasurementsManager::stopNumberedCLTimer(
        std::string name,
        cl::CommandQueue queue) {
    if(!enabled)
        return;
}

void RuntimeMeasurementsManager::startNumberedRegularTimer(std::string name) {
    if(!enabled)
        return;
}

void RuntimeMeasurementsManager::stopNumberedRegularTimer(std::string name) {
    if(!enabled)
        return;
}

RuntimeMeasurement RuntimeMeasurementsManager::getTiming(std::string name) {
    return timings[name];
}

void RuntimeMeasurementsManager::print(std::string name) {
    if(!enabled)
        return;

    RuntimeMeasurement runtime = timings[name];
    runtime.print();
}

void RuntimeMeasurementsManager::printAll() {
    if(!enabled)
        return;

    std::map<std::string, RuntimeMeasurement>::iterator it;
    for(it = timings.begin(); it != timings.end(); it++) {
        RuntimeMeasurement runtime = it->second;
        runtime.print();
    }
}

RuntimeMeasurementsManager::RuntimeMeasurementsManager() {
}

double RuntimeMeasurement::getSum() const {
    return sum;
}

double RuntimeMeasurement::getAverage() const {
    return sum/samples;
}

double RuntimeMeasurement::getStdDeviation() const {
    // TODO: implement
}

bool RuntimeMeasurementsManager::isEnabled() {
    return enabled;
}
