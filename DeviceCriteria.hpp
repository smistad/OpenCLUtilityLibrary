#ifndef DEVICECRITERIA_HPP_
#define DEVICECRITERIA_HPP_

#include <vector>

namespace oul {


enum DeviceType {DEVICE_TYPE_ANY, DEVICE_TYPE_GPU, DEVICE_TYPE_CPU};

enum DevicePlatform {DEVICE_PLATFORM_ANY, DEVICE_PLATFORM_AMD, DEVICE_PLATFORM_NVIDIA, DEVICE_PLATFORM_INTEL, DEVICE_PLATFORM_APPLE};

enum DeviceCapability {DEVICE_CAPABILITY_OPENGL_INTEROP, DEVICE_CAPABILITY_NOT_CONNECTED_TO_SCREEN};

/**
 * Class used to set up a set of criteria for choosing devices
 */
class DeviceCriteria {
    public:
        DeviceCriteria();
        void setPlatformCriteria(DevicePlatform platform);
        void setCapabilityCriteria(DeviceCapability capability);
        void setTypeCriteria(DeviceType typeCriteria);
        void setDeviceCountCriteria(unsigned int min, unsigned int max);
        void setDeviceCountCriteria(unsigned int count);
        const std::vector<DeviceCapability>& getCapabilityCriteria() const;
        DevicePlatform getPlatformCriteria() const;
        DeviceType getTypeCriteria() const;
        unsigned int getDeviceCountMinCriteria() const;
        unsigned int getDeviceCountMaxCriteria() const;
    private:
        DevicePlatform platformCriteria;
        DeviceType typeCriteria; // Can only be one
        std::vector<DeviceCapability> capabilityCriteria; // If multiple capabilities are selected, all of them have to be true
        unsigned int deviceCountMin;
        unsigned int deviceCountMax;

};

};

#endif /* DEVICECRITERIA_HPP_ */
