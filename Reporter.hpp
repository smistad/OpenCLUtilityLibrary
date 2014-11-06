#ifndef REPORTER_H_
#define REPORTER_H_

#include "OpenCLUtilityLibrary_Export.h"

#include <string>
#include <sstream>

namespace oul
{

enum OPENCLUTILITYLIBRARY_EXPORT ReporterLevel{
    rlDEBUG,
    rlINFO,
    rlWARNING,
    rlERROR
};
template <class T>
std::string number(T number){
	std::ostringstream s;
	s << number;
	return s.str();
}

/**
 * \brief Handles reporting messages from the system.
 *
 *  \date: Feb 28, 2014
 *  \author: Janne Beate Bakeng, SINTEF
 */
class OPENCLUTILITYLIBRARY_EXPORT Reporter
{
public:
	Reporter();
	virtual ~Reporter();

	void report(std::string message, ReporterLevel level);
};

} /* namespace oul */
#endif /* REPORTER_H_ */
