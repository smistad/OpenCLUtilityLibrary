#ifndef DEVICECRITERIA_HPP_
#define DEVICECRITERIA_HPP_

namespace oul {

enum DeviceType {DEVICE_TYPE_ANY, DEVICE_TYPE_GPU, DEVICE_TYPE_CPU};

enum DevicePlatform {DEVICE_PLATFORM_AMD, DEVICE_PLATFORM_NVIDIA, DEVICE_PLATFORM_INTEL};

enum DeviceCapability {DEVICE_CAPABILITY_OPENGL_INTEROP};

enum DeviceCount {DEVICE_COUNT_INFINITE};

/**
 * Class used to set up a set of criteria for choosing devices
 */
class DeviceCriteria {
    public:
        DeviceCriteria();
        void setPlatformCriteria(DevicePlatform platform);
        void setCapabilityCriteria(DeviceCapability capability);
        void setTypeCriteria(DeviceType typeCriteria);
        void setDeviceCount(DeviceCount count);
        const std::vector<DeviceCapability>& getCapabilityCriteria() const;
        const std::vector<DevicePlatform>& getPlatformCriteria() const;
        DeviceType getTypeCriteria() const;
        DeviceCount getDeviceCount() const;
    private:
        std::vector<DevicePlatform> platformCriteria; // If multiple platforms are selected, any will be ok
        DeviceType typeCriteria; // Can only be one
        std::vector<DeviceCapability> capabilityCriteria; // If multiple capabilities are selected, all of them have to be true
        DeviceCount deviceCount;

};

};

#endif /* DEVICECRITERIA_HPP_ */
