#ifndef DEVICECRITERIA_HPP_
#define DEVICECRITERIA_HPP_

#include "OpenCLUtilityLibrary_Export.h"

#include <vector>

namespace oul {


enum OPENCLUTILITYLIBRARY_EXPORT DeviceType {DEVICE_TYPE_ANY, DEVICE_TYPE_GPU, DEVICE_TYPE_CPU};

enum OPENCLUTILITYLIBRARY_EXPORT DevicePlatform {DEVICE_PLATFORM_ANY, DEVICE_PLATFORM_AMD, DEVICE_PLATFORM_NVIDIA, DEVICE_PLATFORM_INTEL, DEVICE_PLATFORM_APPLE};

enum OPENCLUTILITYLIBRARY_EXPORT DeviceCapability {DEVICE_CAPABILITY_OPENGL_INTEROP};

enum OPENCLUTILITYLIBRARY_EXPORT DevicePreference {DEVICE_PREFERENCE_NONE, DEVICE_PREFERENCE_NOT_CONNECTED_TO_SCREEN, DEVICE_PREFERENCE_COMPUTE_UNITS, DEVICE_PREFERENCE_GLOBAL_MEMORY};

/**
 * Class used to set up a set of criteria for choosing devices
 */
class OPENCLUTILITYLIBRARY_EXPORT DeviceCriteria {
    public:
        DeviceCriteria();
        void setPlatformCriteria(DevicePlatform platform);
        void setCapabilityCriteria(DeviceCapability capability);
        void setTypeCriteria(DeviceType typeCriteria);
        void setDevicePreference(DevicePreference preference);
        void setDeviceCountCriteria(unsigned int min, unsigned int max);
        void setDeviceCountCriteria(unsigned int count);
        const std::vector<DeviceCapability>& getCapabilityCriteria() const;
        DevicePlatform getPlatformCriteria() const;
        DeviceType getTypeCriteria() const;
        DevicePreference getDevicePreference() const;
        unsigned int getDeviceCountMinCriteria() const;
        unsigned int getDeviceCountMaxCriteria() const;
        bool hasCapabilityCriteria(DeviceCapability capability) const;
    private:
        DevicePlatform platformCriteria;
        DeviceType typeCriteria; // Can only be one
        DevicePreference devicePreference; // Currently only support one preference
        std::vector<DeviceCapability> capabilityCriteria; // If multiple capabilities are selected, all of them have to be true
        unsigned int deviceCountMin;
        unsigned int deviceCountMax;

};

};

#endif /* DEVICECRITERIA_HPP_ */
