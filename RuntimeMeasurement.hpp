#ifndef TIMING_HPP_
#define TIMING_HPP_
#include <string>
#include <map>
#include "CL/OpenCL.hpp"

/**
 * A class for a runtime measurement
 */
class RuntimeMeasurement {
    public:
        RuntimeMeasurement() {};
        RuntimeMeasurement(std::string name);
        void addSample(double runtime);
        double getSum() const;
        double getAverage() const;
        double getStdDeviation() const;
        void print() const;
    private:
        double sum;
        unsigned int samples;
        std::string name;
};

/**
 * Singleton class to keep track of different runtime measurements
 */
class RuntimeMeasurementsManager {
    public:
        static void enable();
        static void disable();
        static RuntimeMeasurementsManager * getInstance();
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
        RuntimeMeasurementsManager();
        static RuntimeMeasurementsManager * instance;
        static bool enabled;
        std::map<std::string, RuntimeMeasurement> timings;
        std::map<std::string, unsigned int> numberings;
        std::map<std::string, cl::Event> startEvents;
};



#endif /* TIMING_HPP_ */
