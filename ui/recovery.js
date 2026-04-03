(function () {
  const logEl = document.getElementById("log");
  const ipEl = document.getElementById("ip");
  const recoveryVerEl = document.getElementById("recoveryVer");
  const packVerEl = document.getElementById("packVer");

  const filePack = document.getElementById("filePack");
  const barPack = document.getElementById("barPack");
  const statsPack = document.getElementById("statsPack");
  const btnPack = document.getElementById("btnPack");
  const btnReset = document.getElementById("btnReset");
  const btnBoot = document.getElementById("btnBoot");
  const btnGithub = document.getElementById("btnGithub");
  const btnFwUpdate = document.getElementById("btnFwUpdate");
  const fwMenuModal = document.getElementById("fwUpdateMenuModal");
  const btnFwOnline = document.getElementById("btnFwUpdateOnline");
  const btnFwManual = document.getElementById("btnFwUpdateManual");
  const btnFwCancel = document.getElementById("btnFwUpdateCancel");
  const fwManualPicker = document.getElementById("fwUpdatePicker");

  const runningSlotEl = document.getElementById("runningSlot");
  const bootSlotEl = document.getElementById("bootSlot");
  const bootPill = document.getElementById("bootPill");
  const otaAInfo = document.getElementById("otaAInfo");
  const otaAPill = document.getElementById("otaAPill");

  let lastStatus = null;
  const kMirrorBase = "https://serjio193.github.io/legacy-bridge/";
  const kMirrorIndex = `${kMirrorBase}releases/index.json`;

  function ts() {
    const d = new Date();
    const p2 = (n) => String(n).padStart(2, "0");
    return `${d.getFullYear()}-${p2(d.getMonth() + 1)}-${p2(d.getDate())} ${p2(d.getHours())}:${p2(d.getMinutes())}:${p2(d.getSeconds())}`;
  }

  function append(line) {
    if (!logEl) return;
    const cur = (logEl.textContent || "").replace(/\s+$/, "");
    logEl.textContent = `${cur}\n[ui] ${ts()} ${line}`;
    logEl.scrollTop = logEl.scrollHeight;
  }

  function fmtBytes(n) {
    const v = Math.max(0, Number(n || 0));
    if (v >= 1024 * 1024) return `${(v / (1024 * 1024)).toFixed(2)} MiB`;
    if (v >= 1024) return `${(v / 1024).toFixed(1)} KiB`;
    return `${Math.round(v)} B`;
  }

  function fmtRate(bps) {
    const v = Math.max(0, Number(bps || 0));
    if (v >= 1024 * 1024) return `${(v / (1024 * 1024)).toFixed(2)} MiB/s`;
    if (v >= 1024) return `${(v / 1024).toFixed(1)} KiB/s`;
    return `${Math.round(v)} B/s`;
  }

  function pillSet(el, text, kind) {
    if (!el) return;
    el.textContent = String(text || "");
    el.className = "pill";
    if (kind === "good") el.className = "pill good";
    else if (kind === "warn") el.className = "pill warn";
    else if (kind === "cyan") el.className = "pill cyan";
  }

  async function apiJson(path, opts) {
    const init = Object.assign({}, opts || {});
    init.headers = Object.assign({}, init.headers || {});
    if (init.body != null && !init.headers["Content-Type"]) init.headers["Content-Type"] = "application/json";
    try {
      const r = await fetch(path, init);
      const t = await r.text();
      try { return JSON.parse(t || "{}"); } catch (_) { return { ok: false, error: t || "bad json", status: r.status }; }
    } catch (e) {
      return { ok: false, error: String((e && e.message) ? e.message : e) };
    }
  }

  function setBar(bar, pct) {
    if (!bar) return;
    const p = Math.max(0, Math.min(100, pct || 0));
    bar.style.width = `${p.toFixed(1)}%`;
  }

  function setBusy(on, what) {
    const v = !!on;
    if (btnPack) btnPack.disabled = v;
    if (btnFwUpdate) btnFwUpdate.disabled = v;
    if (btnReset) btnReset.disabled = v;
    if (btnBoot) btnBoot.disabled = v;
    if (v && what) append(what);
  }
  function parseAutoPackUrl() {
    try {
      const u = new URL(String(window.location.href || ""));
      return String(u.searchParams.get("autopack") || "").trim();
    } catch (_) {
      return "";
    }
  }
  function packFilenameFromUrl(url) {
    const s = String(url || "").trim();
    if (!s) return "update.lbpack";
    try {
      const u = new URL(s);
      const p = String(u.pathname || "").split("/");
      const name = String(p[p.length - 1] || "").trim();
      return name || "update.lbpack";
    } catch (_) {
      return "update.lbpack";
    }
  }
  async function downloadPackFile(url) {
    const r = await fetch(url, { method: "GET", cache: "no-store" });
    if (!r || !r.ok) throw new Error(`download failed: ${r ? r.status : "fetch"}`);
    const blob = await r.blob();
    if (!blob || !blob.size) throw new Error("downloaded package is empty");
    const name = packFilenameFromUrl(url);
    try {
      return new File([blob], name, { type: "application/octet-stream" });
    } catch (_) {
      blob.name = name;
      return blob;
    }
  }
  async function maybeRunAutoPackUpdate() {
    const packUrl = parseAutoPackUrl();
    if (!packUrl) return;
    append(`auto update: package url detected`);
    try {
      setBusy(true, "AUTO UPDATE: downloading package...");
      setBar(barPack, 0);
      if (statsPack) statsPack.textContent = "downloading package...";
      const f = await downloadPackFile(packUrl);
      append(`auto update: downloaded ${f.name || "update.lbpack"} (${Math.round((f.size || 0) / 1024)} KiB)`);
      if (filePack) setInputFile(filePack, f);
      if (statsPack) statsPack.textContent = `${fmtBytes(0)} / ${fmtBytes(f.size || 0)} | avg ${fmtRate(0)} | now ${fmtRate(0)} | 0.0s`;
      await flashPack(f, true);
    } catch (e) {
      append(`auto update FAIL: ${String((e && e.message) ? e.message : e)}`);
      setBusy(false);
    }
  }
  async function fetchLatestPackUrl() {
    const r = await fetch(kMirrorIndex, { cache: "no-store" });
    if (!r || !r.ok) throw new Error(`latest index failed: ${r ? r.status : "fetch"}`);
    const arr = await r.json();
    const src = Array.isArray(arr) ? arr : [];
    for (const item of src) {
      const assets = (item && item.assets && typeof item.assets === "object") ? item.assets : null;
      const rel = String(assets && assets["update.lbpack"] ? assets["update.lbpack"] : "").trim();
      if (!rel) continue;
      return new URL(rel, kMirrorBase).href;
    }
    throw new Error("no update.lbpack in mirror index");
  }
  function hostLooksIpv4(host) {
    return /^\d{1,3}(?:\.\d{1,3}){3}$/.test(String(host || "").trim());
  }
  function pushUnique(list, value) {
    const v = String(value || "").trim();
    if (!v) return;
    if (list.indexOf(v) >= 0) return;
    list.push(v);
  }
  function mdnsFromApHint(apHint) {
    const s = String(apHint || "").trim().toUpperCase();
    const m = /^LB-SETUP-([0-9A-F]{5})$/.exec(s);
    if (!m) return "";
    return `http://lb-bridge-${m[1].toLowerCase()}.local/`;
  }
  function buildMainBootTargets(status, bootRsp) {
    const out = [];
    const proto = String(location.protocol || "http:");
    const host = String(location.hostname || "").trim();
    const ipFromStatus = String(status && status.ip ? status.ip : "").trim();
    const ipFromBootRsp = String(bootRsp && bootRsp.next_url ? bootRsp.next_url : "").trim();

    if (ipFromStatus && hostLooksIpv4(ipFromStatus) && ipFromStatus !== "192.168.4.1") {
      pushUnique(out, `${proto}//${ipFromStatus}/`);
    }
    if (hostLooksIpv4(host) && host !== "192.168.4.1") {
      pushUnique(out, `${proto}//${host}/`);
    }
    if (ipFromBootRsp) pushUnique(out, ipFromBootRsp);

    const mdns = mdnsFromApHint(status && status.main_ap_ssid_hint);
    if (mdns) pushUnique(out, mdns);

    pushUnique(out, "http://192.168.4.1/");
    return out;
  }
  function redirectToMainWithFallback(bootRsp, initialDelayMs) {
    const targets = buildMainBootTargets(lastStatus, bootRsp);
    const nextUrl = targets[0] || "http://192.168.4.1/";
    const mdns = targets.find((u) => u.indexOf(".local/") > 0) || "";
    const apHint = String((lastStatus && lastStatus.main_ap_ssid_hint) ? lastStatus.main_ap_ssid_hint : "LB-SETUP-xxxxx");
    if (mdns) append(`boot main: reconnect to ${apHint}, open ${nextUrl} (mDNS fallback: ${mdns})`);
    else append(`boot main: reconnect to ${apHint}, then open ${nextUrl}`);

    const delay0 = Math.max(500, Number(initialDelayMs || 3000));
    setTimeout(() => { try { window.location.href = nextUrl; } catch (_) {} }, delay0);
    if (targets.length > 1) {
      setTimeout(() => { try { window.location.href = targets[1]; } catch (_) {} }, delay0 + 6000);
    }
    setTimeout(() => { try { window.location.href = "http://192.168.4.1/"; } catch (_) {} }, delay0 + 12000);
  }
  function closeFwMenu() {
    if (fwMenuModal) fwMenuModal.classList.add("hidden");
  }
  function openFwMenu() {
    if (!fwMenuModal) return;
    fwMenuModal.classList.remove("hidden");
    if (btnFwOnline) setTimeout(() => { try { btnFwOnline.focus(); } catch (_) {} }, 0);
  }

  function setInputFile(input, file) {
    if (!input || !file) return false;
    try {
      const dt = new DataTransfer();
      dt.items.add(file);
      input.files = dt.files;
      input.dispatchEvent(new Event("change", { bubbles: true }));
      return true;
    } catch (_) {
      return false;
    }
  }

  function uploadPack(endpoint, file, bar, stats, slotName) {
    return new Promise((resolve) => {
      if (!file) return resolve({ ok: false, error: "no package selected" });
      const xhr = new XMLHttpRequest();
      const form = new FormData();
      form.append("pack", file, file.name || "update.lbpack");

      const t0 = performance.now();
      let lastT = t0;
      let lastLoaded = 0;

      function updateStats(loaded, total) {
        const now = performance.now();
        const dt = Math.max(0.001, (now - t0) / 1000);
        const avg = loaded / dt;
        const instDt = Math.max(0.001, (now - lastT) / 1000);
        const inst = (loaded - lastLoaded) / instDt;
        lastT = now;
        lastLoaded = loaded;
        if (stats) {
          const tot = total > 0 ? total : file.size;
          stats.textContent = `${fmtBytes(loaded)} / ${fmtBytes(tot)} | avg ${fmtRate(avg)} | now ${fmtRate(inst)} | ${dt.toFixed(1)}s`;
        }
      }

      xhr.upload.addEventListener("progress", (ev) => {
        if (!ev.lengthComputable) return;
        setBar(bar, (ev.loaded / ev.total) * 100);
        updateStats(ev.loaded, ev.total);
      });
      let settled = false;
      function done(result) {
        if (settled) return;
        settled = true;
        setBusy(false);
        resolve(result);
      }

      xhr.addEventListener("load", () => {
        setBar(bar, 100);
        updateStats(file.size, file.size);
        try {
          const j = JSON.parse(xhr.responseText || "{}");
          done(j);
        } catch (_) {
          done({ ok: false, error: xhr.responseText || "bad json" });
        }
      });
      xhr.addEventListener("error", () => done({ ok: false, error: "network error" }));
      xhr.addEventListener("abort", () => done({ ok: false, error: "aborted" }));

      try {
        setBusy(true, `FLASHING ${slotName} ...`);
        setBar(bar, 0);
        if (stats) stats.textContent = `${fmtBytes(0)} / ${fmtBytes(file.size)} | avg ${fmtRate(0)} | now ${fmtRate(0)} | 0.0s`;
        xhr.open("POST", endpoint, true);
        xhr.send(form);
      } catch (e) {
        done({ ok: false, error: String((e && e.message) ? e.message : e) });
      }
    });
  }

  async function flashPack(file, fromMenu) {
    const f = file || (filePack && filePack.files ? filePack.files[0] : null);
    if (!f) {
      append("flash PACKAGE FAIL: no package selected");
      return { ok: false, error: "no package selected" };
    }
    if (filePack && file) setInputFile(filePack, file);
    append(`flash PACKAGE started${fromMenu ? " (menu)" : ""}`);
    const r = await uploadPack("/api/recovery/flash/allpack", f, barPack, statsPack, "PACKAGE");
    if (r && r.ok) {
      append(`flash PACKAGE OK: ${fmtBytes(f ? f.size : 0)} (REBOOTING...)`);
      setBusy(true, "REBOOTING... device will switch to System");
    } else {
      append(`flash PACKAGE FAIL: ${(r && r.error) ? r.error : "unknown"}`);
    }
    return r || { ok: false, error: "unknown" };
  }

  async function refreshStatus() {
    const r = await apiJson("/api/status");
    if (!r || !r.ok) return;
    lastStatus = r;

    if (ipEl && r.ip) ipEl.textContent = String(r.ip);
    if (recoveryVerEl) recoveryVerEl.textContent = String(r.recovery_version || "unknown");
    if (packVerEl) packVerEl.textContent = (typeof r.pack_version === "number") ? `v${r.pack_version}` : "unknown";

    if (runningSlotEl) runningSlotEl.textContent = String(r.running_slot || "recovery");
    if (bootSlotEl) bootSlotEl.textContent = String(r.boot_slot || "unknown");
    pillSet(bootPill, "BOOT", "cyan");

    const aValid = !!r.ota_a_valid;
    const bValid = !!r.ota_b_valid;
    const systemValid = aValid || bValid;
    pillSet(otaAPill, systemValid ? "VALID" : "EMPTY", systemValid ? "good" : "warn");
    if (otaAInfo) otaAInfo.textContent = systemValid ? "contains valid image" : "no valid image header";
  }

  function bindFileInfo(input, stats) {
    if (!input || !stats) return;
    input.addEventListener("change", () => {
      const f = input.files && input.files[0] ? input.files[0] : null;
      if (!f) { stats.textContent = "0 / 0"; return; }
      stats.textContent = `${fmtBytes(0)} / ${fmtBytes(f.size)} | avg ${fmtRate(0)} | now ${fmtRate(0)} | 0.0s`;
    });
  }
  bindFileInfo(filePack, statsPack);

  if (btnFwUpdate) btnFwUpdate.addEventListener("click", openFwMenu);
  if (btnFwCancel) btnFwCancel.addEventListener("click", closeFwMenu);
  if (fwMenuModal) {
    fwMenuModal.addEventListener("click", (ev) => {
      if (ev.target === fwMenuModal) closeFwMenu();
    });
    document.addEventListener("keydown", (ev) => {
      if (!fwMenuModal.classList.contains("hidden") && (ev.key === "Escape" || ev.key === "Esc")) closeFwMenu();
    });
  }
  if (btnFwOnline) {
    btnFwOnline.disabled = false;
    btnFwOnline.title = "Download latest update.lbpack and flash";
    btnFwOnline.addEventListener("click", async () => {
      closeFwMenu();
      try {
        setBusy(true, "ONLINE UPDATE: finding latest package...");
        const packUrl = await fetchLatestPackUrl();
        append(`online update: latest package ${packUrl}`);
        if (statsPack) statsPack.textContent = "downloading package...";
        setBar(barPack, 0);
        const f = await downloadPackFile(packUrl);
        append(`online update: downloaded ${f.name || "update.lbpack"} (${Math.round((f.size || 0) / 1024)} KiB)`);
        if (filePack) setInputFile(filePack, f);
        await flashPack(f, true);
      } catch (e) {
        append(`online update FAIL: ${String((e && e.message) ? e.message : e)}`);
        setBusy(false);
      }
    });
  }
  if (btnFwManual) btnFwManual.addEventListener("click", () => {
    closeFwMenu();
    append("manual update: choose update.lbpack");
    if (!fwManualPicker) {
      if (filePack) filePack.click();
      return;
    }
    try { fwManualPicker.value = ""; } catch (_) {}
    fwManualPicker.click();
  });
  if (fwManualPicker) fwManualPicker.addEventListener("change", async () => {
    const files = Array.from((fwManualPicker.files || [])).filter(Boolean);
    if (!files.length) return;
    const packFile = files.find((f) => /\.lbpack$/i.test(String(f.name || ""))) || null;
    if (!packFile) {
      append("manual update FAIL: choose update.lbpack");
      return;
    }
    append(`package selected: ${packFile.name} (${Math.round((packFile.size || 0) / 1024)} KiB)`);
    if (filePack) setInputFile(filePack, packFile);
    await flashPack(packFile, true);
  });

  if (btnPack) btnPack.addEventListener("click", async () => {
    await flashPack(null, false);
  });

  if (btnReset) btnReset.addEventListener("click", async () => {
    if (!confirm("Reset main settings?")) return;
    append("reset main settings");
    const r = await apiJson("/api/recovery/reset_settings", { method: "POST", body: "{}" });
    if (r && r.ok) append("reset OK");
    else append(`reset FAIL: ${(r && r.error) ? r.error : "unknown"}`);
  });

  if (btnBoot) btnBoot.addEventListener("click", async () => {
    append("boot main requested");
    setBusy(true, "REBOOTING to Main firmware...");
    const r = await apiJson("/api/recovery/boot_main", { method: "POST", body: "{}" });
    if (!r || !r.ok) {
      const err = String((r && r.error) ? r.error : "unknown");
      append(`boot main response: ${err}`);
      if (err.toLowerCase().indexOf("failed to fetch") >= 0 || err.toLowerCase().indexOf("network") >= 0) {
        append("boot main: request likely interrupted by reboot, trying main URLs...");
        redirectToMainWithFallback(null, 2500);
        return;
      }
      append(`boot main FAIL: ${err}`);
      setBusy(false);
      return;
    }
    redirectToMainWithFallback(r, 3500);
  });

  if (btnGithub) btnGithub.addEventListener("click", () => {
    const url = "https://github.com/Serjio193/legacy-bridge/";
    append(`open github: ${url}`);
    try {
      window.open(url, "_blank", "noopener,noreferrer");
    } catch (_) {
      try { window.location.href = url; } catch (_) {}
    }
  });

  refreshStatus();
  setInterval(refreshStatus, 3000);
  maybeRunAutoPackUpdate().catch(() => {});
})();
