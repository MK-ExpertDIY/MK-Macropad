#pragma once
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>

class BleKeyboard {
public:
    BleKeyboard(const char* deviceName,
                const char* deviceManufacturer,
                uint8_t batteryLevel);

    void begin();
    bool isConnected();
    void writeKey(uint8_t hidKey);

    // Envoi d’un rapport HID brut (ex: CTRL+lettre)
    void sendRawReport(uint8_t* report, size_t len);

private:
    // Infos d’identification BLE
    const char* _deviceName;
    const char* _deviceManufacturer;
    uint8_t _batteryLevel;

    // Objet HID principal
    NimBLEHIDDevice* hid = nullptr;

    // Report HID (clavier)
    NimBLECharacteristic* inputReport = nullptr;
};
