#ifndef OPENCLMANAGER_HPP_
#define OPENCLMANAGER_HPP_

#include "CL/cl.hpp"
#include <vector>
#include "Context.hpp"
#include "DeviceCriteria.hpp"

namespace oul {

/**
 * Singleton class which is used mainly for creating OpenCL contexts in an easy way
 */
class OpenCLManager
{
public:
	static OpenCLManager * getInstance();
	static void shutdown();

	Context createContext(std::vector<cl::Device> devices);
	Context createContext(int argc, char ** argv);
	Context createContext(DeviceCriteria criteria);
	void setDebugMode(bool mode);

private:
	OpenCLManager();
	std::vector<cl::Platform> platforms;
	bool debugMode;

	static OpenCLManager * instance;
};


}; // namespace oul

#endif /* OPENCLMANAGER_HPP_ */
