#include "DeviceCriteria.hpp"
using namespace oul;

const std::vector<DeviceCapability>& DeviceCriteria::getCapabilityCriteria() const {
    return capabilityCriteria;
}

DevicePlatform DeviceCriteria::getPlatformCriteria() const {
    return platformCriteria;
}

void oul::DeviceCriteria::setDeviceCount(DeviceCount count) {
    this->deviceCount = count;
}

DeviceType DeviceCriteria::getTypeCriteria() const {
    return typeCriteria;
}

oul::DeviceCriteria::DeviceCriteria() {
    typeCriteria = DEVICE_TYPE_ANY;
    deviceCount = DEVICE_COUNT_INFINITE;
    platformCriteria = DEVICE_PLATFORM_ANY;
}

void oul::DeviceCriteria::setPlatformCriteria(DevicePlatform platform) {
    platformCriteria = platform;
}

void oul::DeviceCriteria::setCapabilityCriteria(DeviceCapability capability) {
    capabilityCriteria.push_back(capability);
}

void oul::DeviceCriteria::setTypeCriteria(DeviceType typeCriteria) {
    this->typeCriteria = typeCriteria;
}

DeviceCount oul::DeviceCriteria::getDeviceCount() const {
    return deviceCount;
}
