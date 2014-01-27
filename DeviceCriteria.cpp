#include "DeviceCriteria.hpp"
using namespace oul;

const std::vector<DeviceCapability>& DeviceCriteria::getCapabilityCriteria() const {
    return capabilityCriteria;
}

DevicePlatform DeviceCriteria::getPlatformCriteria() const {
    return platformCriteria;
}

void oul::DeviceCriteria::setDeviceCountCriteria(unsigned int count) {
    this->deviceCountMin = count;
    this->deviceCountMax = count;
}

void oul::DeviceCriteria::setDeviceCountCriteria(unsigned int min, unsigned int max) {
    this->deviceCountMin = min;
    this->deviceCountMax = max;
}

DeviceType DeviceCriteria::getTypeCriteria() const {
    return typeCriteria;
}

oul::DeviceCriteria::DeviceCriteria() {
    typeCriteria = DEVICE_TYPE_ANY;
    deviceCountMin = 0;
    deviceCountMax = -1;
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

unsigned int oul::DeviceCriteria::getDeviceCountMinCriteria() const {
    return deviceCountMin;
}
unsigned int oul::DeviceCriteria::getDeviceCountMaxCriteria() const {
    return deviceCountMax;
}
