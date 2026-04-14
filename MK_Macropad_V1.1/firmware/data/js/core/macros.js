// core/macros.js
// --------------------------------------------------
// Exécution de macros et séquences côté firmware
// --------------------------------------------------

const JSON_HEADERS = { "Content-Type": "application/json" };

// -------------------------
// Exécuter une macro par ID
// (ex: "open_youtube", "copy", "wled_on", etc.)
// -------------------------
export async function runMacro(id) {
    const res = await fetch("/api/macro", {
        method: "POST",
        headers: JSON_HEADERS,
        body: JSON.stringify({ id })
    });
    if (!res.ok) throw new Error("Impossible d'exécuter la macro");
    return await res.json().catch(() => ({}));
}

// -------------------------
// Exécuter une séquence HID brute
// (utilisé par PATH si tu veux tester directement)
// -------------------------
export async function runSequence(sequence) {
    const res = await fetch("/api/hid", {
        method: "POST",
        headers: JSON_HEADERS,
        body: JSON.stringify({ sequence })
    });
    if (!res.ok) throw new Error("Impossible d'exécuter la séquence HID");
    return await res.json().catch(() => ({}));
}

// -------------------------
// Tester une macro (alias explicite)
// -------------------------
export async function testMacro(id) {
    return runMacro(id);
}
