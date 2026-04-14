#include "ble/BleKeyboard.h"

class BleKeyboardServerCallbacks : public NimBLEServerCallbacks {
public:
    void onConnect(NimBLEServer* pServer) {
        Serial.println("======================================");
        Serial.println(">>> [BLE] CALLBACK CONNECT TRIGGERED <<<");
        Serial.println("======================================");

        extern void onBleConnected();
        onBleConnected();
    }

    void onDisconnect(NimBLEServer* pServer) {
        Serial.println("========================================");
        Serial.println(">>> [BLE] CALLBACK DISCONNECT TRIGGERED <<<");
        Serial.println("========================================");

        extern void onBleDisconnected();
        onBleDisconnected();

        Serial.println("[BLE] Restart advertising in 500ms...");
        delay(500);
        NimBLEDevice::startAdvertising();
    }
};

BleKeyboard::BleKeyboard(const char* deviceName,
                         const char* deviceManufacturer,
                         uint8_t batteryLevel)
    : _deviceName(deviceName),
      _deviceManufacturer(deviceManufacturer),
      _batteryLevel(batteryLevel) {}

void BleKeyboard::begin() {
    Serial.println("[BLE] Initialisation NimBLE...");
    NimBLEDevice::init(_deviceName);

    // Sécurité minimale mais stable pour Windows
    NimBLEDevice::setSecurityAuth(true, false, false);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);

    NimBLEServer* server = NimBLEDevice::createServer();
    server->setCallbacks(new BleKeyboardServerCallbacks());

    NimBLEAdvertising* advertising = NimBLEDevice::getAdvertising();
    advertising->setAppearance(HID_KEYBOARD);
    advertising->setName(_deviceName);

    // ⚠️ Pas de advertising ici (instabilité Windows)
    // advertising->start();

    // ----------------------------------------------------
    // HID
    // ----------------------------------------------------
    hid = new NimBLEHIDDevice(server);

    Serial.println("[BLE] Configuration HID...");
    hid->setManufacturer(_deviceManufacturer);
    hid->setHidInfo(0x00, 0x01);
    hid->setBatteryLevel(_batteryLevel);

    inputReport = hid->getInputReport(1);

    static uint8_t reportMap[] = {
        0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01,
        0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7,
        0x15, 0x00, 0x25, 0x01,
        0x75, 0x01, 0x95, 0x08, 0x81, 0x02,
        0x75, 0x08, 0x95, 0x01, 0x81, 0x01,
        0x75, 0x01, 0x95, 0x05,
        0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02,
        0x75, 0x03, 0x95, 0x01, 0x91, 0x01,
        0x75, 0x08, 0x95, 0x06,
        0x15, 0x00, 0x25, 0x65,
        0x05, 0x07, 0x19, 0x00, 0x29, 0x65, 0x81, 0x00,
        0xC0
    };

    Serial.println("[BLE] Chargement du report map HID...");
    hid->setReportMap(reportMap, sizeof(reportMap));

    Serial.println("[BLE] Démarrage des services HID...");
    hid->startServices();

    // ----------------------------------------------------
    // Advertising final (stable)
    // ----------------------------------------------------
    advertising->addServiceUUID(hid->getHidService()->getUUID());
    advertising->start();
    Serial.println("[BLE] Advertising lancé (post-HID)");

    Serial.println("[BLE] Clavier BLE prêt. En attente connexion Windows...");
}

bool BleKeyboard::isConnected() {
    NimBLEServer* server = NimBLEDevice::getServer();
    if (!server) return false;
    return server->getConnectedCount() > 0;
}

void BleKeyboard::writeKey(uint8_t hidKey) {
    if (!isConnected()) return;

    uint8_t report[8] = {0};
    report[2] = hidKey;

    inputReport->setValue(report, sizeof(report));
    inputReport->notify();

    delay(5);

    uint8_t empty[8] = {0};
    inputReport->setValue(empty, sizeof(empty));
    inputReport->notify();
}

void BleKeyboard::sendRawReport(uint8_t* report, size_t len) {
    if (!isConnected()) return;
    if (!inputReport) return;

    inputReport->setValue(report, len);
    inputReport->notify();
}
