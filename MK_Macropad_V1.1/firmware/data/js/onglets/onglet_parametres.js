import { getLayout, saveLayout } from "../core/api.js";

export function renderSystemTab() {
    return `
        <div class="section">
            <h3 data-i18n="system_layout_title">Layout clavier</h3>
            <select id="keyboardLayout">
                <option value="FR">AZERTY (FR)</option>
                <option value="BE">AZERTY (BE)</option>
                <option value="US">QWERTY (US)</option>
            </select>
        </div>

        <div class="section">
            <h3 data-i18n="system_gpio_title">Assignation GPIO</h3>

            <div id="gpioMapping">
                ${[1,2,3,4,5,6,7,8,9].map(i => `
                    <div class="gpio-row" style="display:flex;justify-content:space-between;align-items:center;margin-bottom:8px;">
                        <label>
                            <span data-i18n="system_gpio_key">Touche</span> ${i}
                        </label>
                        <select class="gpio-select" data-key="${i}" style="width:120px;">
                            <option value="">—</option>
                            ${[4,5,13,14,16,17,18,19,21,22,23,25,26,27,32,33]
                                .map(g => `<option value="${g}">GPIO ${g}</option>`).join("")}
                        </select>
                    </div>
                `).join("")}

                <div class="gpio-row" style="display:flex;justify-content:space-between;align-items:center;margin-top:16px;">
                    <label>
                        <span data-i18n="system_gpio_led">Ruban LED</span>
                    </label>
                    <select class="gpio-select" data-key="led" style="width:120px;">
                        <option value="">—</option>
                        ${[4,5,13,14,16,17,18,19,21,22,23,25,26,27,32,33]
                            .map(g => `<option value="${g}">GPIO ${g}</option>`).join("")}
                    </select>
                </div>
            </div>
        </div>
    `;
}

export async function loadLayoutOptions() {
    try {
        const data = await getLayout();
        const select = document.getElementById("keyboardLayout");

        if (select && data.layout) {
            select.value = data.layout;
            if (window.updateLayoutDisplay) {
                window.updateLayoutDisplay(data.layout);
            }
        }

        select.addEventListener("change", async () => {
            const newLayout = select.value;
            await saveLayout(newLayout);

            if (window.updateLayoutDisplay) {
                window.updateLayoutDisplay(newLayout);
            }
        });

    } catch {
        console.error("Erreur lors du chargement du layout");
    }
}

async function loadGpioMapping() {
    try {
        const res = await fetch("/api/gpio-map");
        const map = await res.json();

        document.querySelectorAll(".gpio-select").forEach(sel => {
            const key = sel.dataset.key;

            if (key === "led") {
                sel.value = map["led"] !== undefined && map["led"] !== -1 ? map["led"] : "";
            } else {
                sel.value = map[key] !== undefined && map[key] !== -1 ? map[key] : "";
            }
        });

    } catch (e) {
        console.error("Erreur chargement GPIO", e);
    }
}

async function saveSingleGpio(key, value) {
    const payload = {};
    payload[key] = value ? Number(value) : -1;

    await fetch("/api/gpio-map", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload)
    });
}

function refreshGpioOptions(root) {
    const selects = [...root.querySelectorAll(".gpio-select")];

    const used = selects
        .map(s => s.value)
        .filter(v => v !== "");

    selects.forEach(select => {
        const current = select.value;

        [...select.options].forEach(opt => {
            if (opt.value === "") return;

            if (used.includes(opt.value) && opt.value !== current) {
                opt.disabled = true;
                opt.style.color = "red";
                opt.style.fontWeight = "bold";
            } else {
                opt.disabled = false;
                opt.style.color = "";
                opt.style.fontWeight = "";
            }
        });
    });
}

export function initSystemTab(root) {
    loadLayoutOptions();

    loadGpioMapping().then(() => {
        refreshGpioOptions(root);
    });

    document.querySelectorAll(".gpio-select").forEach(sel => {
        sel.addEventListener("change", () => {
            saveSingleGpio(sel.dataset.key, sel.value);
            refreshGpioOptions(root);
        });
    });
}
