#include "DeviceCriteria.hpp"
using namespace oul;

const std::vector<DeviceCapability>& DeviceCriteria::getCapabilityCriteria() const {
    return capabilityCriteria;
}

const std::vector<DevicePlatform>& DeviceCriteria::getPlatformCriteria() const {
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
}

void oul::DeviceCriteria::setPlatformCriteria(DevicePlatform platform) {
    platformCriteria.push_back(platform);
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
