// core/api.js
// --------------------------------------------------
// Abstraction des appels HTTP vers le firmware ESP32
// --------------------------------------------------

const JSON_HEADERS = { "Content-Type": "application/json" };

//
// -------------------------
// KEYCONFIG
// -------------------------
export async function getKeyConfig() {
    const res = await fetch("/api/keyconfig");
    if (!res.ok) throw new Error("Impossible de récupérer keyconfig");
    return await res.json();
}

export async function saveKeyConfig(partialConfig) {
    const res = await fetch("/api/keyconfig", {
        method: "POST",
        headers: JSON_HEADERS,
        body: JSON.stringify(partialConfig)
    });
    if (!res.ok) throw new Error("Impossible d'enregistrer keyconfig");
    return await res.json().catch(() => ({}));
}

//
// -------------------------
// LAYOUT
// -------------------------
export async function getLayout() {
    const res = await fetch("/api/layout");
    if (!res.ok) throw new Error("Impossible de récupérer le layout");
    return await res.json();
}

export async function saveLayout(layout) {
    const res = await fetch("/api/layout", {
        method: "POST",
        headers: JSON_HEADERS,
        body: JSON.stringify({ layout })
    });
    if (!res.ok) throw new Error("Impossible d'enregistrer le layout");
    return await res.json().catch(() => ({}));
}

//
// -------------------------
// ESP32 SCAN
// -------------------------
export async function scanEsp32() {
    const res = await fetch("/api/esp32scan");
    if (!res.ok) throw new Error("Impossible de scanner les ESP32");
    return await res.json();
}

//
// -------------------------
// LED
// -------------------------
export async function getLedState() {
    const res = await fetch("/api/led-state");
    if (!res.ok) throw new Error("Impossible de récupérer l'état LED");
    return await res.json();
}

export async function sendLedConfig({ color, animation, brightness, speed }) {
    const payload = {};
    if (color !== undefined) payload.color = color;
    if (animation !== undefined) payload.animation = animation;
    if (brightness !== undefined) payload.brightness = brightness;
    if (speed !== undefined) payload.speed = speed;

    const res = await fetch("/api/led", {
        method: "POST",
        headers: JSON_HEADERS,
        body: JSON.stringify(payload)
    });
    if (!res.ok) throw new Error("Impossible d'envoyer la config LED");
    return await res.json().catch(() => ({}));
}

//
// -------------------------
// STATUT / SYSTEME
// -------------------------
export async function getStatus() {
    const res = await fetch("/status");
    if (!res.ok) throw new Error("Impossible de récupérer le statut");
    return await res.json();
}

//
// -------------------------
// NOTIFICATION DE TOUCHE
// -------------------------
export async function notifyKeySelected(id) {
    try {
        await fetch(`/key?code=${encodeURIComponent(id)}`);
    } catch {
        // silencieux
    }
}
