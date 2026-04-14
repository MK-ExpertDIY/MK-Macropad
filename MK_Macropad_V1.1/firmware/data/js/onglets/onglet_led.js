// Onglet LEDs : rendu + logique
import { sendLedConfig, getLedState } from "../core/api.js";

export function renderLedsTab() {
    const flashyColors = ["#FF0000","#FF7F00","#FFFF00","#00FF00","#00FFFF","#0000FF","#8B00FF","#FF00FF"];

    return `
        <div class="led-section" style="position:relative;">

            <!-- 🔌 Switch ON/OFF -->
            <div style="position:absolute; top:10px; right:10px;">
                <label class="led-switch">
                    <input type="checkbox" id="ledPowerSwitch">
                    <span class="slider"></span>
                </label>
            </div>

            <style>
                .led-switch {
                    position: relative;
                    display: inline-block;
                    width: 48px;
                    height: 24px;
                }
                .led-switch input {
                    opacity: 0;
                    width: 0;
                    height: 0;
                }
                .slider {
                    position: absolute;
                    cursor: pointer;
                    top: 0;
                    left: 0;
                    right: 0;
                    bottom: 0;
                    background-color: #e74c3c;
                    transition: .3s;
                    border-radius: 24px;
                }
                .slider:before {
                    position: absolute;
                    content: "";
                    height: 18px;
                    width: 18px;
                    left: 3px;
                    bottom: 3px;
                    background-color: white;
                    transition: .3s;
                    border-radius: 50%;
                }
                input:checked + .slider {
                    background-color: #2ecc71;
                }
                input:checked + .slider:before {
                    transform: translateX(24px);
                }
            </style>

            <!-- 🎨 Couleur -->
            <h3 data-i18n="led_color" style="margin-top:50px;">Couleur</h3>
            <div id="colorWheel" style="width:220px;height:220px;margin:0 auto;"></div>

            <!-- 💡 Luminosité -->
            <h3 data-i18n="led_brightness">Luminosité</h3>
            <div class="brightness-container" style="margin-bottom:10px;">
                <input 
                    type="range" 
                    id="ledBrightness" 
                    min="0" 
                    max="255" 
                    value="255"
                    class="brightness-slider"
                >
            </div>

            <!-- ⚡ Vitesse -->
            <h3 data-i18n="led_speed">Vitesse</h3>
            <div class="speed-container" style="margin-bottom:15px;">
                <input 
                    type="range" 
                    id="ledSpeed" 
                    min="1" 
                    max="255" 
                    value="128"
                    class="brightness-slider"
                >
            </div>

            <!-- 🎯 Presets -->
            <div class="preset-list" style="display:flex;gap:8px;justify-content:center;margin:15px 0 20px 0;">
                ${flashyColors.map(c => 
                    `<div class="preset" data-color="${c}" 
                        style="background:${c};width:30px;height:30px;border-radius:50%;cursor:pointer;border:2px solid #ccc;">
                    </div>`
                ).join("")}
            </div>

            <!-- 🎞️ Animations -->
            <h3 data-i18n="led_animation">Animation</h3>
            <div class="animation-list">

                <div class="anim-item" data-anim="none">
                    <span class="anim-icon">✨</span>
                    <span class="anim-label" data-i18n="anim_none">Aucune</span>
                </div>

                <div class="anim-item" data-anim="breathing">
                    <span class="anim-icon">🌬️</span>
                    <span class="anim-label" data-i18n="anim_breathing">Respiration</span>
                </div>

                <div class="anim-item" data-anim="rainbow">
                    <span class="anim-icon">🌈</span>
                    <span class="anim-label" data-i18n="anim_rainbow">Arc-en-ciel</span>
                </div>

                <div class="anim-item" data-anim="wave">
                    <span class="anim-icon">🌊</span>
                    <span class="anim-label" data-i18n="anim_wave">Vague</span>
                </div>

                <div class="anim-item" data-anim="idle">
                    <span class="anim-icon">💤</span>
                    <span class="anim-label" data-i18n="anim_idle">Repos</span>
                </div>

                <div class="anim-item" data-anim="aurora">
                    <span class="anim-icon">🌌</span>
                    <span class="anim-label" data-i18n="anim_aurora">Aurora</span>
                </div>

                <div class="anim-item" data-anim="gradient_flow">
                    <span class="anim-icon">➡️🌈</span>
                    <span class="anim-label" data-i18n="anim_gradient_flow">Gradient Flow</span>
                </div>

                <div class="anim-item" data-anim="plasma">
                    <span class="anim-icon">🔥</span>
                    <span class="anim-label" data-i18n="anim_plasma">Plasma</span>
                </div>

                <div class="anim-item" data-anim="seve">
                    <span class="anim-icon">🌱</span>
                    <span class="anim-label" data-i18n="anim_seve">Sève</span>
                </div>

                <div class="anim-item" data-anim="pastel_flow">
                    <span class="anim-icon">🌸</span>
                    <span class="anim-label" data-i18n="anim_pastel_flow">Pastel Flow</span>
                </div>

                <div class="anim-item" data-anim="warm_glow">
                    <span class="anim-icon">🔥</span>
                    <span class="anim-label" data-i18n="anim_warm_glow">Warm Glow</span>
                </div>

                <div class="anim-item" data-anim="sunset">
                    <span class="anim-icon">🌅</span>
                    <span class="anim-label" data-i18n="anim_sunset">Sunset</span>
                </div>

                <div class="anim-item" data-anim="pastel_palette">
                    <span class="anim-icon">🎀</span>
                    <span class="anim-label" data-i18n="anim_pastel_palette">Pastel Palette</span>
                </div>

                <div class="anim-item" data-anim="cotton_candy">
                    <span class="anim-icon">🍭</span>
                    <span class="anim-label" data-i18n="anim_cotton_candy">Cotton Candy</span>
                </div>

                <div class="anim-item" data-anim="soft_aurora">
                    <span class="anim-icon">🌌</span>
                    <span class="anim-label" data-i18n="anim_soft_aurora">Soft Aurora</span>
                </div>

                <div class="anim-item" data-anim="dreamwave">
                    <span class="anim-icon">🌙</span>
                    <span class="anim-label" data-i18n="anim_dreamwave">Dreamwave</span>
                </div>

            </div>
        </div>
    `;
}

export async function initLedControls(root) {
    const wheelEl = root.querySelector("#colorWheel");
    const brightnessSlider = root.querySelector("#ledBrightness");
    const speedSlider = root.querySelector("#ledSpeed");
    const animItems = root.querySelectorAll(".anim-item");
    const powerSwitch = root.querySelector("#ledPowerSwitch");

    let pureColor = { r: 255, g: 0, b: 0 };
    let brightness = 255;
    let currentSpeed = 128;
    let currentAnimation = "none";
    let userPower = true;

    let initializing = true;
    let debounceTimer = null;

    const colorWheel = new iro.ColorPicker(wheelEl, {
        width: 220,
        color: "#FF0000",
        borderWidth: 2,
        borderColor: "#444",
        layout: [{ component: iro.ui.Wheel }]
    });

    function updateBrightnessGradient(rgb) {
        const hex = rgbToHex(rgb.r, rgb.g, rgb.b);
        brightnessSlider.style.setProperty(
            "--slider-gradient",
            `linear-gradient(to right, #000000, ${hex})`
        );
    }

    // Chargement état LED
    try {
        const state = await getLedState();

        if (state.color) {
            pureColor = hexToRgb(state.color);
            colorWheel.color.set(state.color);
        }

        if (typeof state.brightness === "number") {
            brightness = state.brightness;
            brightnessSlider.value = brightness;
        }

        if (typeof state.speed === "number") {
            currentSpeed = state.speed;
            speedSlider.value = currentSpeed;
        }

        if (state.animation) {
            currentAnimation = state.animation;
            animItems.forEach(item =>
                item.classList.toggle("active", item.dataset.anim === currentAnimation)
            );
        }

        if (typeof state.userPower === "boolean") {
            userPower = state.userPower;
            powerSwitch.checked = userPower;
        }

        updateBrightnessGradient(pureColor);

    } catch (e) {
        updateBrightnessGradient(pureColor);
    } finally {
        initializing = false;
    }

    function sendLiveUpdate(extra = {}) {
        clearTimeout(debounceTimer);
        debounceTimer = setTimeout(() => {

            const hex = rgbToHex(pureColor.r, pureColor.g, pureColor.b);

            sendLedConfig({
                color: hex,
                animation: currentAnimation,
                brightness,
                speed: currentSpeed,
                ...extra
            });

        }, 40);
    }

    // ON/OFF
    powerSwitch.addEventListener("change", async () => {
        userPower = powerSwitch.checked;

        await fetch("/api/led-power", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ power: "toggle" })
        });
    });

    // Couleur
    colorWheel.on("color:change", (c) => {
        if (initializing) return;
        pureColor = c.rgb;
        updateBrightnessGradient(pureColor);
        sendLiveUpdate();
    });

    // Luminosité
    brightnessSlider.addEventListener("input", (e) => {
        if (initializing) return;
        brightness = Number(e.target.value);
        sendLiveUpdate();
    });

    // Vitesse
    speedSlider.addEventListener("input", (e) => {
        if (initializing) return;
        currentSpeed = Number(e.target.value);
        sendLiveUpdate();
    });

    // Presets
    root.querySelectorAll(".preset").forEach(p => {
        p.addEventListener("click", () => {
            if (initializing) return;
            colorWheel.color.set(p.dataset.color);
            updateBrightnessGradient(colorWheel.color.rgb);
            sendLiveUpdate();
        });
    });

    // Animations
    animItems.forEach(item => {
        item.addEventListener("click", () => {
            if (initializing) return;

            animItems.forEach(i => i.classList.remove("active"));
            item.classList.add("active");

            currentAnimation = item.dataset.anim;
            sendLiveUpdate();
        });
    });
}

function rgbToHex(r, g, b) {
    return "#" + [r, g, b].map(x => x.toString(16).padStart(2, "0")).join("").toUpperCase();
}

function hexToRgb(hex) {
    const m = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    if (!m) return { r: 255, g: 0, b: 0 };
    return {
        r: parseInt(m[1], 16),
        g: parseInt(m[2], 16),
        b: parseInt(m[3], 16)
    };
}
