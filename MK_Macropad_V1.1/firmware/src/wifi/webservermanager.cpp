/*
 * MK Macropad - Firmware / User Interface
 * Copyright (C) 2026 MK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */




// src/wifi/webservermanager.cpp
#include "wifi/webservermanager.h"
#include "../globals.h"
#include <WiFi.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <DNSServer.h>

#include "../macros/macros.h"
#include "../led/led_engine.h"

#include <Preferences.h>
Preferences prefs;

WebServer server(80);

// DNS Captive Portal
DNSServer dnsServer;
const byte DNS_PORT = 53;

extern void runMacro(const String &key);

// ------------------------------------------------------------
//  VALIDATION GPIO
// ------------------------------------------------------------
bool isForbiddenGPIO(int gpio) {
    int forbidden[] = {0, 2, 12, 15};
    for (int f : forbidden) if (gpio == f) return true;
    return false;
}

bool isGPIOUsed(int gpio, int ignoreIndex = -1) {
    for (int i = 0; i < 10; i++) {
        if (i == ignoreIndex) continue;
        if (gpioMap[i] == gpio) return true;
    }
    return false;
}

void resetPin(int gpio) {
    if (gpio != -1) pinMode(gpio, INPUT);
}

// ------------------------------------------------------------
//  LED STATE
// ------------------------------------------------------------
static String lastColor      = "#FF0000";
static String lastAnimation  = "none";
static int    lastBrightness = 255;
static int    lastSpeed      = 128;

// ------------------------------------------------------------
//  FILE HANDLER
// ------------------------------------------------------------
void handle_file() {
    String path = server.uri();
    if (path == "/") path = "/index.html";

    if (!LittleFS.exists(path)) {
        server.send(404, "text/plain", "File Not Found");
        return;
    }

    File f = LittleFS.open(path, "r");

    String type = "text/html";
    if (path.endsWith(".css")) type = "text/css";
    else if (path.endsWith(".js")) type = "application/javascript";
    else if (path.endsWith(".json")) type = "application/json";

    server.streamFile(f, type);
    f.close();
}

// ------------------------------------------------------------
//  GPIO GET
// ------------------------------------------------------------
void handle_gpio_get() {
    StaticJsonDocument<256> doc;

    for (int i = 1; i <= 9; i++) doc[String(i)] = gpioMap[i];
    doc["led"] = gpioMap[0];

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

// ------------------------------------------------------------
//  GPIO POST
// ------------------------------------------------------------
void handle_gpio_post() {

    if (!server.hasArg("plain")) {
        server.send(400, "text/plain", "Missing body");
        return;
    }

    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, server.arg("plain"))) {
        server.send(400, "text/plain", "JSON invalide");
        return;
    }

    // Validation
    for (JsonPair kv : doc.as<JsonObject>()) {
        String key = kv.key().c_str();
        int gpio = kv.value().as<int>();

        if (gpio == -1) continue;
        if (isForbiddenGPIO(gpio)) {
            server.send(400, "text/plain", "GPIO interdit");
            return;
        }

        int index = (key == "led") ? 0 : key.toInt();
        if (isGPIOUsed(gpio, index)) {
            server.send(400, "text/plain", "GPIO déjà utilisé");
            return;
        }
    }

    // Touches
    for (int i = 1; i <= 9; i++) {
        String key = String(i);
        if (doc.containsKey(key)) {
            int newGpio = doc[key] | -1;
            int oldGpio = gpioMap[i];

            if (oldGpio != newGpio) {
                resetPin(oldGpio);
                gpioMap[i] = newGpio;
                prefs.putInt(key.c_str(), newGpio);
            }
        }
    }

    // LED
    if (doc.containsKey("led")) {
        int newLed = doc["led"] | -1;
        int oldLed = gpioMap[0];

        if (oldLed != newLed) {
            resetPin(oldLed);
            gpioMap[0] = newLed;
            prefs.putInt("led", newLed);
        }
    }

    applyDynamicGPIO();
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// ------------------------------------------------------------
//  KEYCONFIG GET
// ------------------------------------------------------------
void handle_keyconfig_get() {
    if (!LittleFS.exists(KEYCONFIG_FILE)) {
        server.send(200, "application/json", "{}");
        return;
    }

    File file = LittleFS.open(KEYCONFIG_FILE, "r");
    if (!file) {
        server.send(500, "text/plain", "File error");
        return;
    }

    String content = file.readString();
    file.close();
    server.send(200, "application/json", content);
}

// ------------------------------------------------------------
//  KEYCONFIG POST
// ------------------------------------------------------------
void handle_keyconfig_post() {

    if (!server.hasArg("plain")) {
        server.send(400, "text/plain", "Missing body");
        return;
    }

    File file = LittleFS.open(KEYCONFIG_FILE, "w");
    if (!file) {
        server.send(500, "text/plain", "Cannot open file");
        return;
    }

    file.print(server.arg("plain"));
    file.close();

    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// ------------------------------------------------------------
//  LED STATE GET
// ------------------------------------------------------------
void handle_led_state_get() {
    StaticJsonDocument<256> doc;

    doc["color"]      = lastColor;
    doc["animation"]  = lastAnimation;
    doc["brightness"] = lastBrightness;
    doc["speed"]      = lastSpeed;
    doc["userPower"]  = LedEngine::getUserPower();

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

// ------------------------------------------------------------
//  LED POST
// ------------------------------------------------------------
void handle_led_post() {
    if (!server.hasArg("plain")) {
        server.send(400, "text/plain", "Missing body");
        return;
    }

    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, server.arg("plain"))) {
        server.send(400, "text/plain", "JSON invalide");
        return;
    }

    if (doc.containsKey("color")) {
        const char* c = doc["color"];
        if (c && strlen(c) == 7 && c[0] == '#') {
            lastColor = String(c);
            LedEngine::setColorHex(lastColor);
        }
    }

    if (doc.containsKey("animation")) {
        const char* a = doc["animation"];
        if (a) {
            lastAnimation = String(a);
            LedEngine::setModeByName(lastAnimation);
        }
    }

    if (doc.containsKey("brightness")) {
        int b = doc["brightness"].as<int>();
        b = constrain(b, 0, 255);
        lastBrightness = b;
        LedEngine::setBrightness((uint8_t)b);
    }

    if (doc.containsKey("speed")) {
        int s = doc["speed"].as<int>();
        s = constrain(s, 1, 255);
        lastSpeed = s;
        LedEngine::setAnimationSpeed((uint8_t)s);
    }

    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// ------------------------------------------------------------
//  LED POWER POST
// ------------------------------------------------------------
void handle_led_power_post() {
    if (!server.hasArg("plain")) {
        server.send(400, "text/plain", "Missing body");
        return;
    }

    StaticJsonDocument<128> doc;
    if (deserializeJson(doc, server.arg("plain"))) {
        server.send(400, "text/plain", "JSON invalide");
        return;
    }

    bool current = LedEngine::getUserPower();

    if (doc.containsKey("power")) {
        String p = doc["power"].as<const char*>();
        p.toLowerCase();

        if (p == "toggle") LedEngine::setUserPower(!current);
        else if (p == "on") LedEngine::setUserPower(true);
        else if (p == "off") LedEngine::setUserPower(false);
    }

    StaticJsonDocument<64> resp;
    resp["userPower"] = LedEngine::getUserPower();
    String json;
    serializeJson(resp, json);
    server.send(200, "application/json", json);
}

// ------------------------------------------------------------
//  LAYOUT GET
// ------------------------------------------------------------
void handle_layout_get() {
    StaticJsonDocument<64> doc;

    switch (currentLayout) {
        case LAYOUT_BE: doc["layout"] = "BE"; break;
        case LAYOUT_FR: doc["layout"] = "FR"; break;
        case LAYOUT_US: doc["layout"] = "US"; break;
    }

    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
}

// ------------------------------------------------------------
//  LAYOUT POST
// ------------------------------------------------------------
void handle_layout_post() {

    if (!server.hasArg("plain")) {
        server.send(400, "text/plain", "Missing body");
        return;
    }

    StaticJsonDocument<64> doc;
    if (deserializeJson(doc, server.arg("plain"))) {
        server.send(400, "text/plain", "JSON invalide");
        return;
    }

    if (!doc.containsKey("layout")) {
        server.send(400, "text/plain", "Champ 'layout' manquant");
        return;
    }

    String l = doc["layout"].as<String>();
    l.toUpperCase();

    if (l == "BE") setLayout(LAYOUT_BE);
    else if (l == "FR") setLayout(LAYOUT_FR);
    else if (l == "US") setLayout(LAYOUT_US);
    else {
        server.send(400, "text/plain", "Layout inconnu");
        return;
    }

    Preferences p;
    p.begin("layout", false);
    p.putString("layout", l);
    p.end();

    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// ------------------------------------------------------------
//  INIT
// ------------------------------------------------------------
void WebServerManager::begin() {
    Serial.println("Starting AP...");
    WiFi.softAP("MK Macropad", "mkmacropad");
    delay(300);

    IPAddress ip = WiFi.softAPIP();
    Serial.print("AP started: MK Macropad, IP: ");
    Serial.println(ip);

    dnsServer.start(DNS_PORT, "*", ip);
    Serial.println("DNS server started");

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    prefs.begin("gpio", false);

    for (int i = 1; i <= 9; i++)
        gpioMap[i] = prefs.getInt(String(i).c_str(), -1);

    gpioMap[0] = prefs.getInt("led", -1);

    applyDynamicGPIO();

    // Fichiers
    server.on("/", handle_file);
    server.on("/index.html", handle_file);

    // Captive portal OS detection (version simple / propre)

    // Windows
    server.on("/connecttest.txt", []() {
        server.send(200, "text/plain", "Microsoft Connect Test");
    });
    server.on("/fwlink", []() {
        server.sendHeader("Location", "/index.html", true);
        server.send(302, "text/plain", "");
    });

    // Android
    server.on("/generate_204", []() {
        server.send(204, "text/plain", "");
    });
    server.on("/gen_204", []() {
        server.send(204, "text/plain", "");
    });

    // Apple
    server.on("/hotspot-detect.html", []() {
        server.send(200, "text/html", "<html><body>Success</body></html>");
    });

    // Redirection générique : si ce n'est pas un fichier connu → UI
    server.onNotFound([]() {
        String path = server.uri();

        if (LittleFS.exists(path)) {
            handle_file();
            return;
        }

        server.sendHeader("Location", "/index.html", true);
        server.send(302, "text/plain", "");
    });

    // API
    server.on("/api/gpio-map", HTTP_GET,  handle_gpio_get);
    server.on("/api/gpio-map", HTTP_POST, handle_gpio_post);

    server.on("/api/keyconfig", HTTP_GET,  handle_keyconfig_get);
    server.on("/api/keyconfig", HTTP_POST, handle_keyconfig_post);

    server.on("/api/led-state", HTTP_GET,  handle_led_state_get);
    server.on("/api/led",       HTTP_POST, handle_led_post);
    server.on("/api/led-power", HTTP_POST, handle_led_power_post);

    server.on("/api/layout", HTTP_GET,  handle_layout_get);
    server.on("/api/layout", HTTP_POST, handle_layout_post);

    server.begin();
    Serial.println("Web server started");
}

// ------------------------------------------------------------
//  LOOP
// ------------------------------------------------------------
void WebServerManager::handle() {
    dnsServer.processNextRequest();
    server.handleClient();
}

// ------------------------------------------------------------
//  GET KEY CONFIG
// ------------------------------------------------------------
KeyConfig WebServerManager::getKeyConfig(uint8_t id) {
    KeyConfig cfg;

    if (!LittleFS.exists(KEYCONFIG_FILE)) return cfg;

    File file = LittleFS.open(KEYCONFIG_FILE, "r");
    if (!file) return cfg;

    String content = file.readString();
    file.close();

    StaticJsonDocument<2048> doc;
    if (deserializeJson(doc, content)) return cfg;

    if (doc.containsKey(String(id))) {
        JsonObject obj = doc[String(id)].as<JsonObject>();

        if (obj.containsKey("sequence"))
            for (JsonVariant v : obj["sequence"].as<JsonArray>())
                cfg.sequence.push_back(String(v.as<const char*>()));

        if (obj.containsKey("text")) cfg.text = obj["text"].as<const char*>();
        if (obj.containsKey("esp"))  cfg.esp  = obj["esp"].as<const char*>();
        if (obj.containsKey("type")) cfg.type = obj["type"].as<const char*>();

        if (obj.containsKey("appPath")) cfg.appPath = obj["appPath"].as<const char*>();
        else if (obj.containsKey("path")) cfg.appPath = obj["path"].as<const char*>();

        if (obj.containsKey("appName")) cfg.appName = obj["appName"].as<const char*>();
    }

    return cfg;
}

// ------------------------------------------------------------
//  SAVE KEY CONFIG
// ------------------------------------------------------------
void WebServerManager::saveKeyConfig(uint8_t id, const KeyConfig &cfg) {
    StaticJsonDocument<2048> doc;

    if (LittleFS.exists(KEYCONFIG_FILE)) {
        File f = LittleFS.open(KEYCONFIG_FILE, "r");
        if (f) {
            deserializeJson(doc, f.readString());
            f.close();
        }
    }

    JsonObject obj = doc[String(id)].to<JsonObject>();

    obj["type"] = cfg.type;
    obj["text"] = cfg.text;
    obj["esp"]  = cfg.esp;

    JsonArray arr = obj["sequence"].to<JsonArray>();
    arr.clear();
    for (auto &s : cfg.sequence) arr.add(s);

    obj["appPath"] = cfg.appPath;
    obj["appName"] = cfg.appName;

    File f = LittleFS.open(KEYCONFIG_FILE, "w");
    if (f) {
        serializeJson(doc, f);
        f.close();
    }
}
