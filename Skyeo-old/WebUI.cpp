#include "WebUI.h"

// ============================================
// SKYEO - Embedded Web UI Implementation
// Mit allen Features: Scrollbarer WLAN-Scan,
// Antennen-Konfiguration, Logs-Tab
// ============================================

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Skyeo - Somfy Controller</title>
    <style>
* { box-sizing: border-box; margin: 0; padding: 0; }

body {
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    background: #0f0f23;
    color: #fff;
    min-height: 100vh;
    overflow-x: hidden;
}

/* Animated Background */
.bg-animation {
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    z-index: -1;
    overflow: hidden;
}

.bg-animation::before {
    content: '';
    position: absolute;
    width: 200%;
    height: 200%;
    top: -50%;
    left: -50%;
    background: radial-gradient(circle at 30% 30%, rgba(0, 212, 255, 0.15) 0%, transparent 40%),
                radial-gradient(circle at 70% 70%, rgba(0, 255, 136, 0.1) 0%, transparent 40%),
                radial-gradient(circle at 50% 50%, rgba(138, 43, 226, 0.08) 0%, transparent 50%);
    animation: bgMove 20s ease-in-out infinite;
}

@keyframes bgMove {
    0%, 100% { transform: translate(0, 0) rotate(0deg); }
    25% { transform: translate(2%, 2%) rotate(2deg); }
    50% { transform: translate(0, 4%) rotate(0deg); }
    75% { transform: translate(-2%, 2%) rotate(-2deg); }
}

.bg-animation .orb {
    position: absolute;
    border-radius: 50%;
    filter: blur(60px);
    animation: float 15s infinite ease-in-out;
}

.bg-animation .orb:nth-child(1) {
    width: 400px;
    height: 400px;
    background: rgba(0, 212, 255, 0.2);
    top: 10%;
    left: 10%;
    animation-delay: 0s;
}

.bg-animation .orb:nth-child(2) {
    width: 300px;
    height: 300px;
    background: rgba(0, 255, 136, 0.15);
    bottom: 20%;
    right: 15%;
    animation-delay: -5s;
}

.bg-animation .orb:nth-child(3) {
    width: 250px;
    height: 250px;
    background: rgba(138, 43, 226, 0.15);
    top: 50%;
    left: 60%;
    animation-delay: -10s;
}

@keyframes float {
    0%, 100% { transform: translate(0, 0) scale(1); }
    33% { transform: translate(30px, -30px) scale(1.1); }
    66% { transform: translate(-20px, 20px) scale(0.9); }
}

/* Header */
.header {
    text-align: center;
    padding: 2rem 1rem 1rem;
    position: relative;
}

.header h1 {
    font-size: 2.8rem;
    font-weight: 300;
    letter-spacing: 0.15em;
    background: linear-gradient(135deg, #00d4ff, #fff, #00ff88);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    background-clip: text;
    margin-bottom: 0.3rem;
    text-shadow: 0 0 30px rgba(0, 212, 255, 0.5);
}

.header .version {
    font-size: 0.9rem;
    color: #888;
}

/* Container */
.container {
    max-width: 900px;
    margin: 0 auto;
    padding: 0 1rem 100px;
}

/* Card */
.card {
    background: rgba(255, 255, 255, 0.05);
    backdrop-filter: blur(10px);
    border: 1px solid rgba(255, 255, 255, 0.1);
    border-radius: 16px;
    padding: 1.5rem;
    margin-bottom: 1.2rem;
    box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
}

.card h2 {
    font-size: 1.2rem;
    margin-bottom: 1rem;
    color: #00d4ff;
    font-weight: 500;
    border-bottom: 1px solid rgba(0, 212, 255, 0.3);
    padding-bottom: 0.6rem;
}

/* Tabs */
.tabs {
    display: flex;
    gap: 0.5rem;
    margin-bottom: 1.2rem;
    flex-wrap: wrap;
    justify-content: center;
}

.tab-btn {
    padding: 0.8rem 1.5rem;
    background: rgba(255, 255, 255, 0.05);
    border: 1px solid rgba(255, 255, 255, 0.1);
    border-radius: 25px;
    cursor: pointer;
    font-size: 0.95rem;
    color: #aaa;
    transition: all 0.3s;
}

.tab-btn:hover {
    background: rgba(0, 212, 255, 0.1);
    color: #00d4ff;
    transform: translateY(-2px);
}

.tab-btn.active {
    background: linear-gradient(135deg, #00d4ff, #0099cc);
    color: #fff;
    border-color: transparent;
    box-shadow: 0 4px 15px rgba(0, 212, 255, 0.4);
}

.tab-content { display: none; }
.tab-content.active { display: block; }

/* Subtabs */
.subtabs {
    display: flex;
    gap: 0.4rem;
    margin-bottom: 1rem;
    flex-wrap: wrap;
    justify-content: center;
}

.subtab-btn {
    padding: 0.5rem 1rem;
    background: rgba(255, 255, 255, 0.03);
    border: 1px solid rgba(255, 255, 255, 0.08);
    border-radius: 20px;
    cursor: pointer;
    font-size: 0.85rem;
    color: #777;
    transition: all 0.2s;
}

.subtab-btn:hover {
    background: rgba(0, 212, 255, 0.1);
    color: #00d4ff;
}

.subtab-btn.active {
    background: rgba(0, 212, 255, 0.2);
    color: #00d4ff;
    border-color: rgba(0, 212, 255, 0.3);
}

/* Buttons */
.btn {
    display: inline-block;
    padding: 0.8rem 1.5rem;
    border: none;
    border-radius: 12px;
    cursor: pointer;
    font-size: 1rem;
    font-weight: 500;
    transition: all 0.3s;
    margin: 0.3rem;
    background: rgba(255, 255, 255, 0.1);
    color: #fff;
    border: 1px solid rgba(255, 255, 255, 0.1);
}

.btn:hover {
    transform: translateY(-2px);
    box-shadow: 0 5px 20px rgba(0, 0, 0, 0.3);
}

.btn-primary {
    background: linear-gradient(135deg, #00d4ff, #0099cc);
    border: none;
}

.btn-success {
    background: linear-gradient(135deg, #00ff88, #00cc66);
    color: #000;
}

.btn-danger {
    background: linear-gradient(135deg, #ff3366, #cc0044);
}

.btn-warning {
    background: linear-gradient(135deg, #ffaa00, #cc8800);
    color: #000;
}

.btn-sm {
    padding: 0.5rem 1rem;
    font-size: 0.85rem;
}

/* Forms */
.form-group {
    margin-bottom: 1rem;
}

.form-group label {
    display: block;
    margin-bottom: 0.5rem;
    font-weight: 500;
    font-size: 0.95rem;
    color: #ccc;
}

.form-group input,
.form-group select {
    width: 100%;
    padding: 0.8rem 1rem;
    background: rgba(0, 0, 0, 0.3);
    border: 1px solid rgba(255, 255, 255, 0.1);
    border-radius: 10px;
    color: #fff;
    font-size: 1rem;
    transition: all 0.3s;
}

.form-group input:focus,
.form-group select:focus {
    outline: none;
    border-color: #00d4ff;
    box-shadow: 0 0 15px rgba(0, 212, 255, 0.3);
}

/* Grid */
.grid-2 {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 1rem;
}

.grid-3 {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
    gap: 1rem;
}

/* Status */
.status-indicator {
    display: inline-block;
    width: 10px;
    height: 10px;
    border-radius: 50%;
    margin-right: 0.5rem;
    vertical-align: middle;
}

.status-online { background: #00ff88; box-shadow: 0 0 10px #00ff88; }
.status-offline { background: #ff3366; }
.status-connecting { background: #ffaa00; animation: blink 1s infinite; }

@keyframes blink {
    0%, 100% { opacity: 1; }
    50% { opacity: 0.3; }
}

/* Wifi */
.wifi-scan-container {
    max-height: 280px;
    overflow-y: auto;
    border-radius: 12px;
    padding: 0.5rem;
    background: rgba(0, 0, 0, 0.2);
}

.wifi-network {
    padding: 0.8rem;
    border-radius: 10px;
    margin-bottom: 0.5rem;
    cursor: pointer;
    background: rgba(255, 255, 255, 0.05);
    display: flex;
    justify-content: space-between;
    align-items: center;
    transition: all 0.2s;
    border: 1px solid transparent;
}

.wifi-network:hover {
    background: rgba(0, 212, 255, 0.1);
    border-color: rgba(0, 212, 255, 0.3);
    transform: translateX(5px);
}

.wifi-signal { font-weight: 500; }
.wifi-signal.strong { color: #00ff88; }
.wifi-signal.medium { color: #ffaa00; }
.wifi-signal.weak { color: #ff3366; }

/* Logs */
.logs-container {
    background: rgba(0, 0, 0, 0.4);
    color: #bbb;
    padding: 1rem;
    border-radius: 12px;
    font-family: 'Courier New', monospace;
    font-size: 0.85rem;
    height: 350px;
    overflow-y: auto;
    border: 1px solid rgba(255, 255, 255, 0.05);
}

.log-entry {
    padding: 0.4rem 0;
    border-bottom: 1px solid rgba(255, 255, 255, 0.05);
    display: flex;
    gap: 0.8rem;
    align-items: center;
    font-size: 0.8rem;
}

.log-time { color: #666; min-width: 55px; }
.log-address { color: #00d4ff; }
.log-cmd { color: #ffaa66; }
.log-rc { color: #88ff88; }

.log-direction {
    padding: 0.15rem 0.4rem;
    border-radius: 6px;
    font-size: 0.7rem;
    margin-left: auto;
}

.log-direction.tx { background: rgba(0, 212, 255, 0.3); }
.log-direction.rx { background: rgba(0, 255, 136, 0.3); }

/* Pairing */
.pairing-container {
    background: rgba(0, 0, 0, 0.2);
    padding: 1.2rem;
    border-radius: 16px;
    margin-top: 1rem;
}

.pairing-step {
    display: flex;
    align-items: flex-start;
    gap: 1rem;
    padding: 1rem;
    margin-bottom: 0.6rem;
    background: rgba(255, 255, 255, 0.03);
    border-radius: 12px;
    border-left: 3px solid rgba(255, 255, 255, 0.1);
    transition: all 0.3s;
}

.pairing-step.active {
    border-left-color: #00d4ff;
    background: rgba(0, 212, 255, 0.1);
}

.pairing-step.completed {
    border-left-color: #00ff88;
    opacity: 0.6;
}

.step-number {
    width: 30px;
    height: 30px;
    border-radius: 50%;
    background: rgba(255, 255, 255, 0.1);
    color: #888;
    display: flex;
    align-items: center;
    justify-content: center;
    font-weight: bold;
    flex-shrink: 0;
}

.pairing-step.active .step-number {
    background: #00d4ff;
    color: #000;
}

.pairing-step.completed .step-number {
    background: #00ff88;
    color: #000;
}

.step-content { flex: 1; }
.step-title { font-weight: 500; margin-bottom: 0.3rem; }
.step-desc { font-size: 0.85rem; color: #777; }

/* Modal */
.modal {
    display: none;
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background: rgba(0, 0, 0, 0.8);
    z-index: 1000;
    justify-content: center;
    align-items: center;
}

.modal.active { display: flex; }

.modal-content {
    background: #1a1a2e;
    padding: 2rem;
    border-radius: 20px;
    max-width: 450px;
    width: 90%;
    max-height: 85vh;
    overflow-y: auto;
    border: 1px solid rgba(0, 212, 255, 0.2);
    box-shadow: 0 20px 60px rgba(0, 0, 0, 0.5);
}

.modal-header h3 {
    margin: 0 0 1rem;
    color: #00d4ff;
    font-size: 1.3rem;
}

.modal-footer {
    display: flex;
    justify-content: flex-end;
    gap: 0.5rem;
    margin-top: 1.5rem;
}

/* Toast */
.toast {
    position: fixed;
    bottom: 1.5rem;
    right: 1.5rem;
    background: #1a1a2e;
    color: #fff;
    padding: 1rem 1.5rem;
    border-radius: 12px;
    box-shadow: 0 8px 32px rgba(0, 0, 0, 0.4);
    transform: translateX(150%);
    transition: transform 0.3s;
    z-index: 2000;
    max-width: 300px;
    font-size: 0.95rem;
    border: 1px solid rgba(255, 255, 255, 0.1);
}

.toast.show { transform: translateX(0); }
.toast.success { border-color: #00ff88; color: #00ff88; }
.toast.error { border-color: #ff3366; color: #ff3366; }

/* Shade Item */
.shade-item {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 1rem;
    border-radius: 12px;
    margin-bottom: 0.6rem;
    background: rgba(255, 255, 255, 0.05);
    border: 1px solid rgba(255, 255, 255, 0.08);
    transition: all 0.2s;
}

.shade-item:hover {
    background: rgba(0, 212, 255, 0.08);
    border-color: rgba(0, 212, 255, 0.2);
    transform: translateX(5px);
}

.shade-controls {
    display: flex;
    gap: 0.4rem;
    align-items: center;
    flex-wrap: wrap;
}

.position-slider {
    width: 80px;
    margin: 0 0.4rem;
}

/* Range Slider */
input[type="range"] {
    -webkit-appearance: none;
    width: 100%;
    height: 6px;
    background: rgba(255, 255, 255, 0.1);
    border-radius: 3px;
}

input[type="range"]::-webkit-slider-thumb {
    -webkit-appearance: none;
    width: 18px;
    height: 18px;
    border-radius: 50%;
    background: linear-gradient(135deg, #00d4ff, #0099cc);
    cursor: pointer;
    box-shadow: 0 0 10px rgba(0, 212, 255, 0.5);
}

/* Select */
select {
    appearance: none;
    background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='12' fill='%2300d4ff'%3E%3Cpath d='M6 9L1 4h10z'/%3E%3C/svg%3E");
    background-repeat: no-repeat;
    background-position: right 1rem center;
    padding-right: 2.5rem;
}

.empty-state {
    text-align: center;
    padding: 2.5rem;
    color: #666;
    font-style: italic;
    font-size: 1rem;
}

.frequency-badge {
    display: inline-block;
    background: linear-gradient(135deg, #00d4ff, #0099cc);
    color: #fff;
    padding: 0.3rem 0.8rem;
    border-radius: 20px;
    font-size: 0.75rem;
    font-weight: 600;
    margin-left: 0.6rem;
    vertical-align: middle;
}

/* Scrollbar */
::-webkit-scrollbar { width: 8px; }
::-webkit-scrollbar-track { background: transparent; }
::-webkit-scrollbar-thumb { background: rgba(255, 255, 255, 0.1); border-radius: 4px; }
::-webkit-scrollbar-thumb:hover { background: rgba(255, 255, 255, 0.2); }
    </style>
</head>

<body>
    <div class="bg-animation">
        <div class="orb"></div>
        <div class="orb"></div>
        <div class="orb"></div>
    </div>
    <div class="header">
        <h1>Skyeo <span class="frequency-badge">433 MHz</span></h1>
        <div class="version">v1.0.0 | <span id="connectionStatus"><span class="status-indicator status-connecting"></span>Verbinden...</span></div>
    </div>
    
    <!-- Antenna Status Warning Banner -->
.wifi-network:hover{transform:translateY(-2px);box-shadow:0.2rem 0.2rem 0.4rem var(--shadow),-0.15rem -0.15rem 0.3rem rgba(0,212,255,0.15);color:#00d4ff}
.wifi-signal{font-size:0.8rem}
.wifi-signal.strong{color:#00ff88}
.wifi-signal.medium{color:#ffaa00}
.wifi-signal.weak{color:#ff3366}

.logs-container{background:#0d0d1a;color:#a0a0a0;padding:0.8rem;border-radius:1rem;font-family:monospace;font-size:0.75rem;height:300px;overflow-y:auto;box-shadow:inset 0.3rem 0.3rem 0.6rem rgba(0,0,0,0.5),inset -0.2rem -0.2rem 0.4rem rgba(255,255,255,0.02)}
.log-entry{padding:0.3rem 0;border-bottom:1px solid rgba(255,255,255,0.03);display:flex;gap:0.5rem;font-size:0.7rem;align-items:center}
.log-time{color:#555;min-width:50px}
.log-address{color:#00d4ff}
.log-cmd{color:#ffaa66}
.log-direction{padding:0.1rem 0.3rem;border-radius:0.3rem;font-size:0.65rem}
.log-direction.tx{background:rgba(0,212,255,0.3)}
.log-direction.rx{background:rgba(0,255,136,0.3)}

.pairing-container{background:#151520;padding:1rem;border-radius:1.5rem;margin-top:1rem;box-shadow:inset 0.2rem 0.2rem 0.4rem rgba(0,0,0,0.4),inset -0.15rem -0.15rem 0.3rem rgba(255,255,255,0.02)}
.pairing-step{display:flex;align-items:flex-start;gap:0.8rem;padding:0.8rem;margin-bottom:0.5rem;background:#1a1a2e;border-radius:1rem;box-shadow:0.2rem 0.2rem 0.4rem var(--shadow),-0.1rem -0.1rem 0.2rem rgba(255,255,255,0.02);transition:all 0.3s}
.pairing-step.active{border-left:3px solid #00d4ff;box-shadow:0.3rem 0.3rem 0.6rem var(--shadow),-0.15rem -0.15rem 0.3rem rgba(0,212,255,0.1)}
.pairing-step.completed{border-left:3px solid #00ff88;opacity:0.6}
.step-number{width:28px;height:28px;border-radius:50%;background:#252535;color:#888;display:flex;align-items:center;justify-content:center;font-weight:bold;flex-shrink:0;font-size:0.8rem}
.pairing-step.active .step-number{background:#00d4ff;color:#000}
.pairing-step.completed .step-number{background:#00ff88;color:#000}
.step-content{flex:1}
.step-title{font-size:0.85rem;margin-bottom:0.2rem}
.step-desc{font-size:0.7rem;color:#666}

.modal{display:none;position:fixed;top:0;left:0;width:100%;height:100%;background:rgba(0,0,0,0.8);z-index:1000;justify-content:center;align-items:center}
.modal.active{display:flex}
.modal-content{background:#1a1a2e;padding:1.5rem;border-radius:1.5rem;max-width:400px;width:90%;max-height:80vh;overflow-y:auto;box-shadow:1rem 1rem 2rem var(--shadow),-0.5rem -0.5rem 1rem rgba(255,255,255,0.03)}
.modal-header{margin-bottom:1rem}
.modal-header h3{margin:0;color:#00d4ff;font-size:1.2rem;letter-spacing:0.1em}
.modal-footer{display:flex;justify-content:flex-end;gap:0.5rem;margin-top:1rem}

.toast{position:fixed;bottom:1.5rem;right:1.5rem;background:#1a1a2e;color:#e0e0e0;padding:0.8rem 1.2rem;border-radius:1rem;box-shadow:0.5rem 0.5rem 1rem var(--shadow),-0.3rem -0.3rem 0.6rem rgba(255,255,255,0.03);transform:translateX(150%);transition:transform 0.3s;z-index:2000;max-width:280px;font-size:0.85rem;border:1px solid rgba(255,255,255,0.05)}
.toast.show{transform:translateX(0)}
.toast.success{border-color:#00ff88;color:#00ff88}
.toast.error{border-color:#ff3366;color:#ff3366}

.shade-item{display:flex;align-items:center;justify-content:space-between;padding:0.8rem;border-radius:1rem;margin-bottom:0.5rem;background:#1a1a2e;box-shadow:0.2rem 0.2rem 0.4rem var(--shadow),-0.1rem -0.1rem 0.2rem rgba(255,255,255,0.02);transition:all 0.2s}
.shade-item:hover{transform:translateX(3px);box-shadow:0.3rem 0.3rem 0.5rem var(--shadow),-0.15rem -0.15rem 0.3rem rgba(0,212,255,0.1)}
.shade-controls{display:flex;gap:0.3rem;align-items:center;flex-wrap:wrap}
.position-slider{width:70px;height:6px;-webkit-appearance:none;background:#252535;border-radius:3px;box-shadow:inset 0.1rem 0.1rem 0.2rem rgba(0,0,0,0.4)}
.position-slider::-webkit-slider-thumb{-webkit-appearance:none;width:16px;height:16px;border-radius:50%;background:#00d4ff;cursor:pointer;box-shadow:0 0 8px rgba(0,212,255,0.5)}
.empty-state{text-align:center;padding:2rem;color:#555;font-style:italic;font-size:0.9rem}
.frequency-badge{display:inline-block;background:#00d4ff;color:#000;padding:0.2rem 0.6rem;border-radius:0.5rem;font-size:0.65rem;font-weight:bold;margin-left:0.5rem;vertical-align:middle}

input[type="range"]{-webkit-appearance:none;width:100%;height:6px;background:#252535;border-radius:3px;box-shadow:inset 0.1rem 0.1rem 0.2rem rgba(0,0,0,0.4)}
input[type="range"]::-webkit-slider-thumb{-webkit-appearance:none;width:18px;height:18px;border-radius:50%;background:#00d4ff;cursor:pointer;box-shadow:0 0 10px rgba(0,212,255,0.5)}

select{appearance:none;background-image:url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='12' fill='%2300d4ff'%3E%3Cpath d='M6 9L1 4h10z'/%3E%3C/svg%3E");background-repeat:no-repeat;background-position:right 0.8rem center;padding-right:2rem}
    </style>
</head>

<body>
    <div class="bg-animation">
        <div class="orb"></div>
        <div class="orb"></div>
        <div class="orb"></div>
    </div>
    <div class="header">
        <h1>Skyeo <span class="frequency-badge">433 MHz</span></h1>
        <div class="version">v1.0.0 | <span id="connectionStatus"><span class="status-indicator status-connecting"></span>Verbinden...</span></div>
    </div>
    
    <!-- Antenna Status Warning Banner -->

.btn{display:inline-block;padding:0.7rem 1.4rem;border:none;border-radius:25px;cursor:pointer;font-size:0.9rem;font-weight:500;transition:all 0.3s cubic-bezier(0.4,0,0.2,1);margin:0.3rem;position:relative;overflow:hidden}
.btn::before{content:'';position:absolute;top:0;left:-100%;width:100%;height:100%;background:linear-gradient(90deg,transparent,rgba(255,255,255,0.3),transparent);transition:left 0.5s}
.btn:hover::before{left:100%}
.btn-primary{background:linear-gradient(135deg,var(--primary),#00a0cc);color:#fff;box-shadow:0 4px 15px var(--primary-glow)}
.btn-primary:hover{transform:translateY(-2px);box-shadow:0 6px 25px var(--primary-glow)}
.btn-success{background:linear-gradient(135deg,var(--success),#00cc66);color:#000;box-shadow:0 4px 15px rgba(0,255,136,0.3)}
.btn-danger{background:linear-gradient(135deg,var(--danger),#cc0044);color:#fff;box-shadow:0 4px 15px rgba(255,51,102,0.3)}
.btn-warning{background:linear-gradient(135deg,var(--warning),#cc8800);color:#000;box-shadow:0 4px 15px rgba(255,170,0,0.3)}
.btn:hover{transform:translateY(-2px)}
.btn-sm{padding:0.5rem 1rem;font-size:0.8rem}

.form-group{margin-bottom:1rem}
.form-group label{display:block;margin-bottom:0.5rem;font-weight:400;font-size:0.9rem;color:var(--text-dim)}
.form-group input,.form-group select{width:100%;padding:0.8rem 1rem;background:rgba(0,0,0,0.3);border:1px solid rgba(255,255,255,0.1);border-radius:15px;color:var(--text);font-size:0.95rem;transition:all 0.3s}
.form-group input:focus,.form-group select:focus{outline:none;border-color:var(--primary);box-shadow:0 0 20px var(--primary-glow)}

.grid-3{display:grid;grid-template-columns:repeat(auto-fit,minmax(150px,1fr));gap:1rem}
.grid-2{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:1rem}
@media(max-width:600px){.grid-2,.grid-3{grid-template-columns:1fr}}

.status-indicator{display:inline-block;width:12px;height:12px;border-radius:50%;margin-right:0.5rem;position:relative}
.status-indicator::after{content:'';position:absolute;top:-2px;left:-2px;width:16px;height:16px;border-radius:50%;background:inherit;opacity:0.5;animation:pulse 2s infinite}
.status-online{background:var(--success);box-shadow:0 0 10px var(--success)}
.status-offline{background:var(--danger)}
.status-connecting{background:var(--warning);animation:blink 1s infinite}
@keyframes blink{0%,100%{opacity:1}50%{opacity:0.3}}
@keyframes pulse{0%,100%{transform:scale(1);opacity:0.5}50%{transform:scale(1.5);opacity:0}}

.wifi-scan-container{max-height:300px;overflow-y:auto;border-radius:15px;padding:0.5rem;background:rgba(0,0,0,0.2)}
.wifi-scan-container::-webkit-scrollbar{width:6px}
.wifi-scan-container::-webkit-scrollbar-track{background:transparent}
.wifi-scan-container::-webkit-scrollbar-thumb{background:var(--glass-border);border-radius:3px}
.wifi-network{padding:0.8rem;border:1px solid rgba(255,255,255,0.05);border-radius:12px;margin-bottom:0.5rem;cursor:pointer;background:rgba(255,255,255,0.03);transition:all 0.3s;display:flex;justify-content:space-between;align-items:center}
.wifi-network:hover{background:rgba(0,212,255,0.1);border-color:var(--primary-glow);transform:translateX(5px)}
.wifi-signal{font-weight:500;font-size:0.85rem}
.wifi-signal.strong{color:var(--success)}
.wifi-signal.medium{color:var(--warning)}
.wifi-signal.weak{color:var(--danger)}

.logs-container{background:rgba(0,0,0,0.5);color:#c0c0c0;padding:1rem;border-radius:15px;font-family:'SF Mono',monospace;font-size:0.8rem;height:400px;overflow-y:auto;border:1px solid rgba(255,255,255,0.05)}
.logs-container::-webkit-scrollbar{width:8px}
.logs-container::-webkit-scrollbar-track{background:transparent}
.logs-container::-webkit-scrollbar-thumb{background:var(--glass-border);border-radius:4px}
.log-entry{padding:0.4rem 0;border-bottom:1px solid rgba(255,255,255,0.05);display:flex;gap:0.8rem;align-items:center;font-size:0.8rem}
.log-entry:last-child{border-bottom:none}
.log-time{color:#666;font-size:0.75rem}
.log-address{color:#00d4ff}
.log-cmd{color:#ffaa66}
.log-rc{color:#88ff88}
.log-rssi{color:#ff88ff}
.log-direction{font-size:0.7rem;padding:0.15rem 0.4rem;border-radius:8px}
.log-direction.tx{background:linear-gradient(135deg,var(--primary),#00a0cc)}
.log-direction.rx{background:linear-gradient(135deg,var(--success),#00cc66)}

.pairing-container{background:rgba(255,255,255,0.03);padding:1.5rem;border-radius:20px;margin-top:1rem;border:1px solid rgba(255,255,255,0.05)}
.pairing-step{display:flex;align-items:flex-start;gap:1rem;padding:1rem;margin-bottom:0.8rem;background:rgba(255,255,255,0.03);border-radius:15px;border-left:3px solid rgba(255,255,255,0.1);transition:all 0.3s}
.pairing-step.active{border-left-color:var(--primary);background:rgba(0,212,255,0.1)}
.pairing-step.completed{border-left-color:var(--success);opacity:0.6}
.pairing-step.error{border-left-color:var(--danger);background:rgba(255,51,102,0.1)}
.step-number{width:32px;height:32px;border-radius:50%;background:rgba(255,255,255,0.1);color:var(--text-dim);display:flex;align-items:center;justify-content:center;font-weight:600;flex-shrink:0}
.pairing-step.active .step-number{background:linear-gradient(135deg,var(--primary),#00a0cc);color:#fff}
.pairing-step.completed .step-number{background:var(--success);color:#000}
.pairing-step.error .step-number{background:var(--danger)}
.step-content{flex:1}
.step-title{font-weight:500;margin-bottom:0.3rem}
.step-desc{font-size:0.85rem;color:var(--text-dim)}

.modal{display:none;position:fixed;top:0;left:0;width:100%;height:100%;background:rgba(0,0,0,0.7);backdrop-filter:blur(10px);z-index:1000;justify-content:center;align-items:center;animation:fadeIn 0.3s}
.modal.active{display:flex}
.modal-content{background:linear-gradient(145deg,rgba(30,30,60,0.95),rgba(20,20,40,0.98));padding:2rem;border-radius:24px;max-width:480px;width:90%;max-height:85vh;overflow-y:auto;border:1px solid var(--glass-border);box-shadow:0 20px 60px rgba(0,0,0,0.5);animation:modalIn 0.4s cubic-bezier(0.4,0,0.2,1)}
@keyframes modalIn{from{opacity:0;transform:scale(0.9) translateY(20px)}to{opacity:1;transform:scale(1) translateY(0)}}
.modal-header{margin-bottom:1.5rem}
.modal-header h3{margin:0;color:var(--primary);font-weight:300;font-size:1.4rem}
.modal-footer{display:flex;justify-content:flex-end;gap:0.5rem;margin-top:1.5rem}

.toast{position:fixed;bottom:2rem;right:2rem;background:linear-gradient(145deg,rgba(40,40,80,0.95),rgba(20,20,40,0.98));color:#fff;padding:1rem 1.5rem;border-radius:15px;box-shadow:0 8px 32px rgba(0,0,0,0.4);transform:translateX(150%);transition:transform 0.4s cubic-bezier(0.4,0,0.2,1);z-index:2000;max-width:350px;font-size:0.9rem;border:1px solid var(--glass-border)}
.toast.show{transform:translateX(0)}
.toast.success{border-color:var(--success);box-shadow:0 8px 32px rgba(0,255,136,0.3)}
.toast.error{border-color:var(--danger);box-shadow:0 8px 32px rgba(255,51,102,0.3)}

.shade-item{display:flex;align-items:center;justify-content:space-between;padding:1rem;border:1px solid rgba(255,255,255,0.08);border-radius:16px;margin-bottom:0.8rem;background:rgba(255,255,255,0.03);transition:all 0.3s}
.shade-item:hover{background:rgba(0,212,255,0.05);border-color:var(--primary-glow);transform:translateX(5px)}
.shade-controls{display:flex;gap:0.5rem;align-items:center;flex-wrap:wrap}
.position-slider{width:100px;margin:0 0.5rem;-webkit-appearance:none;height:6px;border-radius:3px;background:rgba(255,255,255,0.1)}
.position-slider::-webkit-slider-thumb{-webkit-appearance:none;width:18px;height:18px;border-radius:50%;background:linear-gradient(135deg,var(--primary),#00a0cc);cursor:pointer;box-shadow:0 2px 10px var(--primary-glow)}
.empty-state{text-align:center;padding:3rem;color:var(--text-dim);font-style:italic;font-size:1rem}
.frequency-badge{display:inline-block;background:linear-gradient(135deg,var(--primary),#00a0cc);color:#fff;padding:0.3rem 0.8rem;border-radius:20px;font-size:0.75rem;font-weight:500;margin-left:0.8rem;box-shadow:0 4px 15px var(--primary-glow)}

input[type="range"]{-webkit-appearance:none;width:100%;height:6px;border-radius:3px;background:rgba(255,255,255,0.1)}
input[type="range"]::-webkit-slider-thumb{-webkit-appearance:none;width:20px;height:20px;border-radius:50%;background:linear-gradient(135deg,var(--primary),#00a0cc);cursor:pointer;box-shadow:0 2px 15px var(--primary-glow)}

select{appearance:none;background-image:url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='12' fill='%2300d4ff'%3E%3Cpath d='M6 9L1 4h10z'/%3E%3C/svg%3E");background-repeat:no-repeat;background-position:right 1rem center;padding-right:2.5rem}

<body>
    <div class="bg-animation">
        <div class="orb"></div>
        <div class="orb"></div>
        <div class="orb"></div>
    </div>
    <div class="header">
        <h1>Skyeo <span class="frequency-badge">433 MHz</span></h1>
        <div class="version">v1.0.0 | <span id="connectionStatus"><span class="status-indicator status-connecting"></span>Verbinden...</span></div>
    </div>
    
    <!-- Antenna Status Warning Banner -->
    <div id="antennaWarning" style="display:none;background:var(--danger);color:#fff;padding:1rem;text-align:center;font-weight:600">
        <span style="font-size:1.2rem">⚠️</span> CC1101 Antenne NICHT initialisiert! <br>
        <span style="font-size:0.85rem;font-weight:normal">Gehe zu Einstellungen → Antenne um die Pins zu konfigurieren</span>
    </div>

    <div class="container">
        <div class="tabs">
            <button class="tab-btn active" onclick="showTab('shades')">🏠 Rolläden</button>
            <button class="tab-btn" onclick="showTab('schedules')">⏰ Zeitpläne</button>
            <button class="tab-btn" onclick="showTab('settings')">⚙️ Einstellungen</button>
        </div>

        <!-- Rolläden Tab -->
        <div id="tab-shades" class="tab-content active">
            <div class="card">
                <h2>Meine Rolläden</h2>
                <div id="shadesList"></div>
                <div style="text-align:center;margin-top:1rem">
                    <button class="btn btn-primary" onclick="showAddShadeModal()">+ Rolladen hinzufügen</button>
                </div>
            </div>
        </div>

        <!-- Zeitpläne Tab -->
        <div id="tab-schedules" class="tab-content">
            <div class="card">
                <h2>Zeitgesteuerte Aktionen</h2>
                <div id="schedulesList"></div>
                <div style="text-align:center;margin-top:1rem">
                    <button class="btn btn-primary" onclick="showAddScheduleModal()">+ Zeitplan erstellen</button>
                </div>
            </div>
        </div>

        <!-- Einstellungen Tab -->
        <div id="tab-settings" class="tab-content">
            <div class="subtabs">
                <button class="subtab-btn active" onclick="showSubTab('settings-wifi')">📶 WLAN</button>
                <button class="subtab-btn" onclick="showSubTab('settings-antenna')">📡 Antenne</button>
                <button class="subtab-btn" onclick="showSubTab('settings-logs')">📋 Logs</button>
                <button class="subtab-btn" onclick="showSubTab('settings-pairing')">🔗 Pairing</button>
                <button class="subtab-btn" onclick="showSubTab('settings-led')">💡 LED</button>
                <button class="subtab-btn" onclick="showSubTab('settings-system')">⚙️ System</button>
            </div>
            
            <!-- WLAN Sub-Tab -->
            <div id="subtab-settings-wifi" class="subtab-content active">
            <div class="card">
                <h2>WLAN Konfiguration</h2>
                <div style="margin-bottom:1rem">
                    <button class="btn btn-primary" onclick="scanWifiNetworks()">
                        <span id="scanBtnText">📶 Nach Netzwerken suchen</span>
                    </button>
                </div>
                
                <!-- Scrollbarer Container für WLAN-Ergebnisse -->
                <div id="wifiScanResults" class="wifi-scan-container" style="display:none;margin-bottom:1rem">
                    <!-- WLAN-Netzwerke werden hier eingefügt -->
                </div>
                
                <form id="wifiForm" onsubmit="saveWifi(event)">
                    <div class="form-group">
                        <label>SSID (Netzwerkname)</label>
                        <input type="text" id="wifiSsid" placeholder="MeinWLAN" required>
                    </div>
                    <div class="form-group">
                        <label>Passwort (optional für offene Netzwerke)</label>
                        <input type="password" id="wifiPass" placeholder="••••••••">
                    </div>
                    <button type="submit" class="btn btn-primary">💾 Speichern & Neustarten</button>
                </form>
            </div>
            </div>
            
            <!-- Antenne Sub-Tab -->
            <div id="subtab-settings-antenna" class="subtab-content" style="display:none">
            <div class="card">
                <h2>CC1101 Antennen-Konfiguration</h2>
                <div id="antennaStatusInfo" style="margin-bottom:1rem;padding:0.8rem;border-radius:4px;font-size:0.9rem">
                    Lade Status...
                </div>
                
                <p style="margin-bottom:1rem;color:#666;font-size:0.9rem">Konfiguriere die GPIO-Pins für den CC1101 Transceiver.</p>
                
                <div class="grid-3">
                    <div class="form-group">
                        <label>SCLK (Clock)</label>
                        <input type="number" id="pinSclk" value="18" min="0" max="39">
                    </div>
                    <div class="form-group">
                        <label>CSN (Chip Select)</label>
                        <input type="number" id="pinCsn" value="5" min="0" max="39">
                    </div>
                    <div class="form-group">
                        <label>MOSI (Data Out)</label>
                        <input type="number" id="pinMosi" value="23" min="0" max="39">
                    </div>
                    <div class="form-group">
                        <label>MISO (Data In)</label>
                        <input type="number" id="pinMiso" value="19" min="0" max="39">
                    </div>
                    <div class="form-group">
                        <label>GDO0 (TX)</label>
                        <input type="number" id="pinTx" value="22" min="0" max="39">
                    </div>
                    <div class="form-group">
                        <label>GDO2 (RX)</label>
                        <input type="number" id="pinRx" value="21" min="0" max="39">
                    </div>
                </div>
                
                <div style="display:flex;gap:0.5rem;margin-top:1rem;flex-wrap:wrap">
                    <button class="btn btn-primary" onclick="saveAntennaConfig()">💾 Konfiguration speichern</button>
                    <button class="btn" onclick="loadDefaultPins()">🔄 Standardwerte</button>
                    <button class="btn btn-success" onclick="testAntennaConnection()">🔄 Verbindung testen</button>
                </div>
                
                <div style="margin-top:1rem;padding:0.8rem;background:#fff3cd;border-radius:4px;border-left:4px solid #ffc107;font-size:0.85rem">
                    <strong>⚠️ Hinweis:</strong> Nach dem Speichern startet das Gerät neu. Stelle sicher, dass die Pins korrekt mit dem CC1101 Modul verbunden sind.
                </div>
                
                <div style="margin-top:1rem;padding:0.8rem;background:#e3f2fd;border-radius:4px;font-size:0.85rem">
                    <strong>📡 Frequenz:</strong> 433.42 MHz (für Somfy RTS)<br>
                    <strong>🔧 Modulation:</strong> OOK (On-Off Keying)<br>
                    <strong>📶 Datenrate:</strong> 40 kbps
                </div>
            </div>
            </div>
            
            <!-- Logs Sub-Tab -->
            <div id="subtab-settings-logs" class="subtab-content" style="display:none">
            <div class="card">
                <h2>Signalempfangs-Logs <small style="font-weight:normal;color:#666">(433 MHz)</small></h2>
                <div style="display:flex;gap:0.5rem;margin-bottom:0.5rem;flex-wrap:wrap">
                    <button class="btn btn-sm" onclick="clearLogs()">🗑️ Löschen</button>
                    <button class="btn btn-primary btn-sm" onclick="exportLogs()">💾 Export</button>
                </div>
                <div style="padding:0.5rem;background:#e8f5e9;border-radius:4px;margin-bottom:0.5rem;font-size:0.85rem">
                    <span class="status-indicator status-online" style="animation:pulse 1s infinite"></span> 
                    <strong>Empfangsmodus immer aktiv</strong> - Die Antenne empfängt ständig Signale
                </div>
                <div class="logs-container" id="antennaLogs">
                    <div class="log-entry" style="color:#858585;font-style:italic">Warte auf empfangene Signale... Drücke eine Taste an deiner Fernbedienung.</div>
                </div>
                <div style="margin-top:0.5rem;font-size:0.8rem;color:#666;display:flex;gap:1rem;flex-wrap:wrap">
                    <span>📊 <strong id="rxCount">0</strong> Signale empfangen</span>
                    <span>📡 RSSI: <strong id="lastRssi">-</strong> dBm</span>
                    <span>⏱️ Letztes: <strong id="lastRxTime">-</strong></span>
                </div>
            </div>
            </div>
            
            <!-- Pairing Sub-Tab -->
            <div id="subtab-settings-pairing" class="subtab-content" style="display:none">
            <div class="card">
                <h2>Rolladen Pairing-Assistent</h2>
                <div id="pairingIntro">
                    <p style="margin-bottom:1rem;color:#666">Verbinde deinen Rolladen mit Skyeo. Folge den Anweisungen Schritt für Schritt.</p>
                    <div class="form-group">
                        <label>Rolladen auswählen</label>
                        <select id="pairingShadeSelect" style="width:100%"></select>
                    </div>
                    <button class="btn btn-primary btn-lg" onclick="startPairing()" style="width:100%;margin-top:0.5rem">🚀 Pairing starten</button>
                </div>
                
                <div id="pairingProgress" class="pairing-container" style="display:none">
                    <div id="step1" class="pairing-step">
                        <div class="step-number">1</div>
                        <div class="step-content">
                            <div class="step-title">PROG-Taste drücken</div>
                            <div class="step-desc">Halte die PROG-Taste an deiner Original-Fernbedienung <strong>3 Sekunden</strong> gedrückt, bis der Rolladen kurz juckelt.</div>
                        </div>
                    </div>
                    <div id="step2" class="pairing-step">
                        <div class="step-number">2</div>
                        <div class="step-content">
                            <div class="step-title">Signal senden</div>
                            <div class="step-desc">Skyeo sendet automatisch das PROG-Signal. Warte auf die Bestätigung...</div>
                        </div>
                    </div>
                    <div id="step3" class="pairing-step">
                        <div class="step-number">3</div>
                        <div class="step-content">
                            <div class="step-title">Bestätigen</div>
                            <div class="step-desc">Drücke nochmals kurz die PROG-Taste an der Fernbedienung um das Pairing zu bestätigen.</div>
                        </div>
                    </div>
                    <div style="text-align:center;margin-top:1rem">
                        <button class="btn btn-danger" onclick="stopPairing()">❌ Abbrechen</button>
                    </div>
                </div>
                
                <div id="pairingStatus" style="margin-top:1rem;padding:0.8rem;background:#f5f5f5;border-radius:4px;font-size:0.9rem">
                    Bereit für Pairing
                </div>
            </div>
            </div>
            
            <!-- LED Sub-Tab -->
            <div id="subtab-settings-led" class="subtab-content" style="display:none">
            <div class="card">
                <h2>💡 LED Gimmick Steuerung</h2>
                <div id="ledStatusInfo" style="margin-bottom:1rem;padding:0.8rem;border-radius:4px;font-size:0.9rem;background:#e3f2fd;border-left:4px solid #2196F3">
                    Lade LED Status...
                </div>
                
                <div class="form-group">
                    <label>LED aktivieren</label>
                    <div style="display:flex;align-items:center;gap:1rem">
                        <label style="display:flex;align-items:center;gap:0.5rem;font-weight:normal">
                            <input type="checkbox" id="ledEnabled" onchange="saveLEDConfig()"> LED aktiviert
                        </label>
                    </div>
                </div>
                
                <div class="form-group">
                    <label>Helligkeit (0-255)</label>
                    <div style="display:flex;align-items:center;gap:1rem">
                        <input type="range" id="ledBrightness" min="0" max="255" value="128" style="flex:1" onchange="ledSetBrightness(this.value)">
                        <span id="ledBrightnessValue" style="min-width:50px">128</span>
                    </div>
                </div>
                
                <div class="form-group">
                    <label>Modus</label>
                    <select id="ledModeSelect" onchange="saveLEDConfig()" style="width:100%">
                        <option value="0">Aus</option>
                        <option value="1">Dauerhaft an</option>
                        <option value="2">Blinken (benutzerdefiniert)</option>
                        <option value="3">Dimmen</option>
                        <option value="4">Bei Zeitplan blinken</option>
                    </select>
                </div>
                
                <div id="blinkSettings" style="display:none">
                    <div class="grid-2">
                        <div class="form-group">
                            <label>An-Dauer (ms)</label>
                            <input type="number" id="ledBlinkOn" value="500" min="50" max="5000" onchange="saveLEDConfig()">
                        </div>
                        <div class="form-group">
                            <label>Aus-Dauer (ms)</label>
                            <input type="number" id="ledBlinkOff" value="500" min="50" max="5000" onchange="saveLEDConfig()">
                        </div>
                    </div>
                </div>
                
                <div id="dimSettings" style="display:none">
                    <div class="form-group">
                        <label>Dimmer-Stufe (%)</label>
                        <div style="display:flex;align-items:center;gap:1rem">
                            <input type="range" id="ledDimLevel" min="0" max="100" value="50" style="flex:1" onchange="saveLEDConfig()">
                            <span id="ledDimLevelValue" style="min-width:50px">50%</span>
                        </div>
                    </div>
                </div>
                
                <div id="scheduleTriggerSettings" class="form-group" style="display:none">
                    <label style="display:flex;align-items:center;gap:0.5rem;font-weight:normal">
                        <input type="checkbox" id="ledScheduleTrigger" onchange="saveLEDConfig()"> Bei Zeitplan-Auslösung blinken
                    </label>
                </div>
                
                <div style="margin-top:1rem;display:flex;gap:0.5rem;flex-wrap:wrap">
                    <button class="btn btn-success" onclick="ledOn()">💡 LED An</button>
                    <button class="btn btn-danger" onclick="ledOff()">🌑 LED Aus</button>
                    <button class="btn btn-primary" onclick="saveLEDConfig()">💾 Speichern</button>
                </div>
            </div>
            </div>
            
            <!-- System Sub-Tab -->
            <div id="subtab-settings-system" class="subtab-content" style="display:none">
            <div class="card">
                <h2>⚙️ System-Einstellungen</h2>
                
                <div class="card" style="background:#f8f9fa">
                    <h3 style="font-size:1rem;margin-bottom:0.75rem">🌍 Zeitzone</h3>
                    <div class="form-group">
                        <label>Zeitversatz (UTC)</label>
                        <select id="timezoneOffset" onchange="saveTimezone()" style="width:100%">
                            <option value="-12">UTC-12</option>
                            <option value="-11">UTC-11</option>
                            <option value="-10">UTC-10</option>
                            <option value="-9">UTC-9</option>
                            <option value="-8">UTC-8</option>
                            <option value="-7">UTC-7</option>
                            <option value="-6">UTC-6</option>
                            <option value="-5">UTC-5</option>
                            <option value="-4">UTC-4</option>
                            <option value="-3">UTC-3</option>
                            <option value="-2">UTC-2</option>
                            <option value="-1">UTC-1</option>
                            <option value="0">UTC+0</option>
                            <option value="1" selected>UTC+1 (Deutschland)</option>
                            <option value="2">UTC+2</option>
                            <option value="3">UTC+3</option>
                            <option value="4">UTC+4</option>
                            <option value="5">UTC+5</option>
                            <option value="6">UTC+6</option>
                            <option value="7">UTC+7</option>
                            <option value="8">UTC+8</option>
                            <option value="9">UTC+9</option>
                            <option value="10">UTC+10</option>
                            <option value="11">UTC+11</option>
                            <option value="12">UTC+12</option>
                        </select>
                    </div>
                    <div style="font-size:0.85rem;color:#666;margin-top:0.5rem">
                        <span id="currentTimeDisplay">Aktuelle Zeit: --:--:--</span>
                    </div>
                </div>
                
                <div class="card" style="background:#f8f9fa;margin-top:1rem">
                    <h3 style="font-size:1rem;margin-bottom:0.75rem">📱 System-Informationen</h3>
                    <div id="deviceInfo" style="font-size:0.85rem;color:#666;line-height:1.8">
                        Lade...
                    </div>
                </div>
                
                <div style="display:flex;gap:0.5rem;margin-top:1rem;flex-wrap:wrap">
                    <button class="btn btn-danger" onclick="rebootDevice()">🔄 System neustarten</button>
                    <button class="btn btn-warning" onclick="factoryReset()">⚠️ Werksreset</button>
                </div>
            </div>
        </div>
    </div>

    <!-- Add Shade Modal -->
    <div id="addShadeModal" class="modal">
        <div class="modal-content">
            <div class="modal-header"><h3>Rolladen hinzufügen</h3></div>
            
            <!-- Schritt 1: Basis-Informationen -->
            <div id="addShadeStep1">
                <form id="addShadeForm" onsubmit="showAddShadeStep2(event)">
                    <div class="form-group">
                        <label>Name</label>
                        <input type="text" id="newShadeName" placeholder="z.B. Wohnzimmer" required>
                    </div>
                    <div class="form-group">
                        <label>Remote Adresse (Hex, z.B. A1B2C3) - Oder via Pairing ermitteln</label>
                        <input type="text" id="newShadeAddr" placeholder="A1B2C3" pattern="[0-9A-Fa-f]{6}">
                    </div>
                    <div class="grid-2">
                        <div class="form-group">
                            <label>Fahrzeit hoch (ms)</label>
                            <input type="number" id="newShadeUpTime" value="20000" min="1000" max="120000">
                        </div>
                        <div class="form-group">
                            <label>Fahrzeit runter (ms)</label>
                            <input type="number" id="newShadeDownTime" value="20000" min="1000" max="120000">
                        </div>
                    </div>
                    <div style="margin:1rem 0;padding:0.8rem;background:#e3f2fd;border-radius:4px;font-size:0.9rem">
                        <strong>💡 Tipp:</strong> Du kannst die Remote-Adresse entweder manuell eingeben oder durch Pairing automatisch ermitteln lassen.
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn" onclick="closeModal('addShadeModal')">Abbrechen</button>
                        <button type="submit" class="btn btn-success" id="btnAddWithPairing">🔗 Mit Pairing fortfahren</button>
                        <button type="button" class="btn btn-primary" onclick="addShadeDirect()">✓ Direkt hinzufügen</button>
                    </div>
                </form>
            </div>
            
            <!-- Schritt 2: Pairing-Assistent -->
            <div id="addShadeStep2" style="display:none">
                <div class="form-group">
                    <label>Name (wird übernommen)</label>
                    <input type="text" id="pairingShadeName" readonly style="background:#f5f5f5">
                </div>
                
                <div id="addShadePairingProgress" class="pairing-container">
                    <div id="addStep1" class="pairing-step active">
                        <div class="step-number">1</div>
                        <div class="step-content">
                            <div class="step-title">PROG-Taste drücken</div>
                            <div class="step-desc">Halte die PROG-Taste an deiner Original-Fernbedienung <strong>3 Sekunden</strong> gedrückt, bis der Rolladen kurz juckelt.</div>
                        </div>
                    </div>
                    <div id="addStep2" class="pairing-step">
                        <div class="step-number">2</div>
                        <div class="step-content">
                            <div class="step-title">Signal erkannt</div>
                            <div class="step-desc">Die Adresse wurde erkannt! Drücke jetzt in Skyeo auf "Bestätigen".</div>
                        </div>
                    </div>
                    <div id="addStep3" class="pairing-step">
                        <div class="step-number">3</div>
                        <div class="step-content">
                            <div class="step-title">Bestätigen</div>
                            <div class="step-desc">Drücke nochmals kurz die PROG-Taste an der Fernbedienung um das Pairing zu bestätigen.</div>
                        </div>
                    </div>
                </div>
                
                <div id="addShadePairingStatus" style="margin:1rem 0;padding:0.8rem;background:#f5f5f5;border-radius:4px;font-size:0.9rem">
                    Bereit für Pairing...
                </div>
                
                <div id="addShadeAddressDisplay" style="display:none;margin:1rem 0;padding:0.8rem;background:#e8f5e9;border-radius:4px;font-size:0.9rem">
                    <strong>✓ Erkannte Adresse:</strong> <span id="detectedAddress">-</span>
                </div>
                
                <div class="modal-footer">
                    <button type="button" class="btn" onclick="cancelAddShadePairing()">❌ Abbrechen</button>
                    <button type="button" class="btn btn-primary" id="btnConfirmAddress" onclick="confirmDetectedAddress()" style="display:none">✓ Adresse übernehmen</button>
                </div>
            </div>
        </div>
    </div>

    <!-- Add Schedule Modal -->
    <div id="addScheduleModal" class="modal">
        <div class="modal-content">
            <div class="modal-header"><h3>Zeitplan erstellen</h3></div>
            <form onsubmit="addSchedule(event)">
                <div class="form-group">
                    <label>Rolladen</label>
                    <select id="scheduleShade" required></select>
                </div>
                <div class="grid-2">
                    <div class="form-group">
                        <label>Uhrzeit</label>
                        <input type="time" id="scheduleTime" required>
                    </div>
                    <div class="form-group">
                        <label>Aktion</label>
                        <select id="scheduleAction" required>
                            <option value="0">⬆️ Hoch</option>
                            <option value="1">⬇️ Runter</option>
                            <option value="2">⏹️ MY (Stop)</option>
                            <option value="3">🎯 Position</option>
                        </select>
                    </div>
                </div>
                <div class="form-group" id="scheduleTargetGroup" style="display:none">
                    <label>Zielposition (0-100%)</label>
                    <input type="number" id="scheduleTarget" min="0" max="100" value="50">
                </div>
                <div class="form-group">
                    <label>Wochentage</label>
                    <div style="display:flex;gap:0.5rem;flex-wrap:wrap">
                        <label style="display:flex;align-items:center;gap:0.3rem;font-weight:normal;font-size:0.9rem"><input type="checkbox" class="scheduleDay" value="0"> So</label>
                        <label style="display:flex;align-items:center;gap:0.3rem;font-weight:normal;font-size:0.9rem"><input type="checkbox" class="scheduleDay" value="1"> Mo</label>
                        <label style="display:flex;align-items:center;gap:0.3rem;font-weight:normal;font-size:0.9rem"><input type="checkbox" class="scheduleDay" value="2"> Di</label>
                        <label style="display:flex;align-items:center;gap:0.3rem;font-weight:normal;font-size:0.9rem"><input type="checkbox" class="scheduleDay" value="3"> Mi</label>
                        <label style="display:flex;align-items:center;gap:0.3rem;font-weight:normal;font-size:0.9rem"><input type="checkbox" class="scheduleDay" value="4"> Do</label>
                        <label style="display:flex;align-items:center;gap:0.3rem;font-weight:normal;font-size:0.9rem"><input type="checkbox" class="scheduleDay" value="5"> Fr</label>
                        <label style="display:flex;align-items:center;gap:0.3rem;font-weight:normal;font-size:0.9rem"><input type="checkbox" class="scheduleDay" value="6"> Sa</label>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn" onclick="closeModal('addScheduleModal')">Abbrechen</button>
                    <button type="submit" class="btn btn-primary">Erstellen</button>
                </div>
            </form>
        </div>
    </div>

    <div id="toast" class="toast"></div>

    <script>
// Globale Variablen
let ws=null,shades=[],schedules=[],pairingInterval=null,logInterval=null,logs=[];
const WS_URL='ws://'+window.location.hostname+':8080';
const cmdNames={0:'UNKNOWN',1:'MY',2:'UP',3:'MY_UP',4:'DOWN',5:'MY_DOWN',6:'UP_DOWN',8:'PROG',9:'SUNFLAG',10:'FLAG'};

// Initialisierung
function init(){connectWebSocket();loadShades();loadSchedules();loadDeviceInfo();loadAntennaStatus();setupEventListeners();}

// Antenna Status laden und Warnung anzeigen
async function loadAntennaStatus(){
    try{
        const r=await fetch('/api/antenna/status');
        const status=await r.json();
        
        // Warnung anzeigen wenn nicht initialisiert
        const warningEl=document.getElementById('antennaWarning');
        if(warningEl){
            warningEl.style.display=status.initialized?'none':'block';
        }
        
        // Status Info in den Settings anzeigen
        const statusEl=document.getElementById('antennaStatusInfo');
        if(statusEl){
            if(status.initialized){
                statusEl.style.background='#e8f5e9';
                statusEl.style.borderLeft='4px solid #4CAF50';
                statusEl.innerHTML='<strong>✓ CC1101 verbunden</strong><br>Version: 0x'+status.versionHex+' | RSSI: '+status.rssi+' dBm | Frames: '+status.framesReceived+' empfangen / '+status.framesSent+' gesendet';
            }else{
                statusEl.style.background='#ffebee';
                statusEl.style.borderLeft='4px solid #f44336';
                statusEl.innerHTML='<strong>✗ CC1101 nicht verbunden</strong><br>Fehler: '+status.lastError+'<br>Prüfe die Verkabelung und ob die Pins korrekt konfiguriert sind.';
            }
        }
        
        // Aktuelle Pin-Werte laden
        loadAntennaConfig();
    }catch(e){
        console.error('Fehler beim Laden des Antennen-Status:',e);
    }
}

// Antenna Config laden
async function loadAntennaConfig(){
    try{
        const r=await fetch('/api/antenna/config');
        const config=await r.json();
        document.getElementById('pinSclk').value=config.sclk;
        document.getElementById('pinCsn').value=config.csn;
        document.getElementById('pinMosi').value=config.mosi;
        document.getElementById('pinMiso').value=config.miso;
        document.getElementById('pinTx').value=config.tx;
        document.getElementById('pinRx').value=config.rx;
    }catch(e){
        console.error('Fehler beim Laden der Antennen-Konfiguration:',e);
    }
}

// Verbindung testen
async function testAntennaConnection(){
    try{
        const r=await fetch('/api/antenna/status');
        const status=await r.json();
        if(status.initialized){
            showToast('✓ CC1101 verbunden! Version: 0x'+status.versionHex,'success');
        }else{
            showToast('✗ CC1101 nicht erkannt: '+status.lastError,'error');
        }
        loadAntennaStatus();
    }catch(e){
        showToast('Fehler beim Testen','error');
    }
}

// WebSocket
function connectWebSocket(){
    try{
        ws=new WebSocket(WS_URL);
        ws.onopen=()=>{updateConnectionStatus(true);showToast('Verbunden','success');};
        ws.onmessage=(e)=>{handleMessage(JSON.parse(e.data));};
        ws.onclose=()=>{updateConnectionStatus(false);setTimeout(connectWebSocket,3000);};
        ws.onerror=(e)=>{updateConnectionStatus(false);};
    }catch(e){updateConnectionStatus(false);}
}

function updateConnectionStatus(connected){
    const el=document.getElementById('connectionStatus');
    if(connected){el.innerHTML='<span class="status-indicator status-online"></span>Online';}
    else{el.innerHTML='<span class="status-indicator status-offline"></span>Offline';}
}

function handleMessage(msg){
    if(msg.type==='shadeUpdate'){updateShadeInList(msg.data);}
    else if(msg.type==='scheduleUpdate'){loadSchedules();}
    else if(msg.type==='logUpdate'){addLogEntry(msg.data);}
}

// Tabs
function showTab(tab){
    document.querySelectorAll('.tab-btn').forEach(t=>t.classList.remove('active'));
    document.querySelectorAll('.tab-content').forEach(t=>t.classList.remove('active'));
    event.target.classList.add('active');
    document.getElementById('tab-'+tab).classList.add('active');
}

function showSubTab(tab){
    document.querySelectorAll('.subtab-btn').forEach(t=>t.classList.remove('active'));
    document.querySelectorAll('.subtab-content').forEach(t=>t.style.display='none');
    event.target.classList.add('active');
    document.getElementById('subtab-'+tab).style.display='block';
    
    // Automatisch Logs laden wenn Logs-Tab geöffnet wird
    if(tab === 'settings-logs'){
        startLogRefresh();
    } else {
        stopLogRefresh();
    }
    
    // LED Status laden wenn LED-Tab geöffnet wird
    if(tab === 'settings-led'){
        loadLEDStatus();
    }
}

// WLAN Scan mit scrollbarem Container
async function scanWifiNetworks(){
    const btn=document.getElementById('scanBtnText');
    const results=document.getElementById('wifiScanResults');
    btn.textContent='🔍 Suche...';
    results.style.display='none';
    results.innerHTML='<div style="padding:1rem;text-align:center;color:#666">Suche nach Netzwerken...</div>';
    results.style.display='block';
    
    try{
        const r=await fetch('/api/wifi/scan');
        const networks=await r.json();
        
        if(networks.length===0){
            results.innerHTML='<div style="padding:1rem;text-align:center;color:#666">Keine Netzwerke gefunden</div>';
            return;
        }
        
        // Sortiere nach Signalstärke
        networks.sort((a,b)=>b.rssi-a.rssi);
        
        let html='';
        networks.forEach(n=>{
            let signalClass='weak';
            let signalIcon='📶';
            if(n.rssi>=-50){signalClass='strong';signalIcon='📶';}
            else if(n.rssi>=-70){signalClass='medium';signalIcon='📶';}
            
            html+=`<div class="wifi-network" onclick="selectNetwork('${n.ssid.replace(/'/g,"\\'")}')">`;
            html+=`<div style="display:flex;align-items:center;gap:0.5rem">`;
            html+=`<span style="font-size:1.2rem">${signalIcon}</span>`;
            html+=`<div><div style="font-weight:600">${n.ssid}</div>`;
            html+=`<div style="font-size:0.75rem;color:#666">${n.encrypted?'🔒 Gesichert':'🔓 Offen'}</div></div>`;
            html+=`</div>`;
            html+=`<span class="wifi-signal ${signalClass}">${n.rssi} dBm</span>`;
            html+=`</div>`;
        });
        
        results.innerHTML=html;
    }catch(e){
        results.innerHTML='<div style="padding:1rem;text-align:center;color:#f44336">❌ Fehler beim Scannen</div>';
    }
    btn.textContent='📶 Nach Netzwerken suchen';
}

function selectNetwork(ssid){
    document.getElementById('wifiSsid').value=ssid;
    document.getElementById('wifiPass').focus();
    showToast('SSID ausgewählt: '+ssid,'success');
}

// Antennen-Config
function loadDefaultPins(){
    document.getElementById('pinSclk').value='18';
    document.getElementById('pinCsn').value='5';
    document.getElementById('pinMosi').value='23';
    document.getElementById('pinMiso').value='19';
    document.getElementById('pinTx').value='22';
    document.getElementById('pinRx').value='21';
    showToast('Standardwerte geladen','success');
}

async function saveAntennaConfig(){
    const config={
        sclk:parseInt(document.getElementById('pinSclk').value),
        csn:parseInt(document.getElementById('pinCsn').value),
        mosi:parseInt(document.getElementById('pinMosi').value),
        miso:parseInt(document.getElementById('pinMiso').value),
        tx:parseInt(document.getElementById('pinTx').value),
        rx:parseInt(document.getElementById('pinRx').value)
    };
    
    try{
        await fetch('/api/antenna/config',{
            method:'POST',
            headers:{'Content-Type':'application/json'},
            body:JSON.stringify(config)
        });
        showToast('Antennen-Konfiguration gespeichert. Gerät startet neu...','success');
    }catch(e){
        showToast('Fehler beim Speichern','error');
    }
}

// Logs - Antenne ist immer aktiv, wir zeigen nur die empfangenen Frames an
let logRefreshInterval = null;

function startLogRefresh() {
    if(logRefreshInterval) clearInterval(logRefreshInterval);
    logRefreshInterval = setInterval(loadAntennaLogs, 500);
    loadAntennaLogs(); // Sofort laden
}

function stopLogRefresh() {
    if(logRefreshInterval) {
        clearInterval(logRefreshInterval);
        logRefreshInterval = null;
    }
}

async function loadAntennaLogs(){
    try{
        const r=await fetch('/api/antenna/logs');
        if(!r.ok){
            console.error('Fehler beim Laden der Logs:', r.status);
            return;
        }
        
        const newLogs=await r.json();
        console.log('Empfangene Logs:', newLogs.length, newLogs);
        
        if(newLogs.length===0)return;
        
        const container=document.getElementById('antennaLogs');
        const wasAtBottom=container.scrollHeight-container.scrollTop<=container.clientHeight+50;
        
        let newCount = 0;
        newLogs.forEach(log=>{
            // Prüfe ob Log bereits existiert (basierend auf timestamp + rollingCode + address)
            const exists = logs.find(l=>l.timestamp===log.timestamp && 
                                       l.rollingCode===log.rollingCode &&
                                       l.remoteAddress===log.address);
            if(!exists){
                // Korrigiere Feldnamen falls nötig
                const normalizedLog = {
                    timestamp: log.timestamp,
                    remoteAddress: log.address || log.remoteAddress,
                    rollingCode: log.rollingCode,
                    command: log.command,
                    rssi: log.rssi,
                    direction: log.direction || 'RX',
                    valid: log.valid !== undefined ? log.valid : true
                };
                logs.push(normalizedLog);
                addLogEntry(normalizedLog);
                newCount++;
            }
        });
        
        // Max 100 Logs
        if(logs.length>100){
            logs=logs.slice(-100);
            container.innerHTML='';
            logs.forEach(l=>addLogEntry(l,false));
        }
        
        if(wasAtBottom && newCount > 0){
            container.scrollTop=container.scrollHeight;
        }
        
        document.getElementById('rxCount').textContent=logs.length;
        if(logs.length>0){
            const last=logs[logs.length-1];
            document.getElementById('lastRssi').textContent=last.rssi+' dBm';
            document.getElementById('lastRxTime').textContent=new Date(last.timestamp).toLocaleTimeString();
        }
    }catch(e){
        console.error('Fehler in loadAntennaLogs:', e);
    }
}

function addLogEntry(log,animate=true){
    const container=document.getElementById('antennaLogs');
    const cmdName=cmdNames[log.command]||'CMD'+log.command;
    const dir=log.direction||'RX';
    
    const entry=document.createElement('div');
    entry.className='log-entry';
    if(animate)entry.style.animation='fadeIn 0.3s';
    
    entry.innerHTML=`
        <span class="log-time">${new Date(log.timestamp).toLocaleTimeString('de-DE',{hour12:false,hour:'2-digit',minute:'2-digit',second:'2-digit'})}</span>
        <span class="log-direction ${dir.toLowerCase()}">${dir}</span>
        <span class="log-address">0x${log.remoteAddress.toString(16).toUpperCase().padStart(6,'0')}</span>
        <span class="log-cmd">${cmdName}</span>
        <span class="log-rc">RC:${log.rollingCode}</span>
        <span class="log-rssi">${log.rssi}dBm</span>
    `;
    
    container.appendChild(entry);
    if(container.children.length>100){
        container.removeChild(container.firstChild);
    }
}

function clearLogs(){
    logs=[];
    document.getElementById('antennaLogs').innerHTML='<div class="log-entry" style="color:#858585;font-style:italic">Logs gelöscht...</div>';
    document.getElementById('rxCount').textContent='0';
    document.getElementById('lastRssi').textContent='-';
    showToast('Logs gelöscht','success');
}

function exportLogs(){
    if(logs.length===0){
        showToast('Keine Logs zum Exportieren','error');
        return;
    }
    
    let csv='Time,Direction,Address,Command,RollingCode,RSSI\n';
    logs.forEach(l=>{
        csv+=`${new Date(l.timestamp).toISOString()},${l.direction||'RX'},0x${l.remoteAddress.toString(16)},${cmdNames[l.command]||l.command},${l.rollingCode},${l.rssi}\n`;
    });
    
    const blob=new Blob([csv],{type:'text/csv'});
    const url=URL.createObjectURL(blob);
    const a=document.createElement('a');
    a.href=url;
    a.download='skyeo-logs-'+new Date().toISOString().slice(0,10)+'.csv';
    a.click();
    showToast('Logs exportiert','success');
}

// Pairing
function updatePairingShadeSelect(){
    const sel=document.getElementById('pairingShadeSelect');
    if(shades.length===0){
        sel.innerHTML='<option value="">Keine Rolläden vorhanden</option>';
        return;
    }
    sel.innerHTML=shades.map(s=>`<option value="${s.id}">${s.name} (0x${s.remoteAddress.toString(16).toUpperCase()})</option>`).join('');
}

async function startPairing(){
    const shadeId=document.getElementById('pairingShadeSelect').value;
    if(!shadeId){showToast('Bitte einen Rolladen auswählen','error');return;}
    
    try{
        await fetch('/api/pairing/start',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({shadeId:parseInt(shadeId)})});
        document.getElementById('pairingIntro').style.display='none';
        document.getElementById('pairingProgress').style.display='block';
        document.getElementById('step1').classList.add('active');
        showToast('Pairing gestartet','success');
        
        if(pairingInterval)clearInterval(pairingInterval);
        pairingInterval=setInterval(updatePairingStatus,1000);
        
        // Antenne ist immer aktiv - kein Start nötig
        showToast('Pairing gestartet - drücke PROG an der Fernbedienung','success');
    }catch(e){showToast('Fehler beim Starten','error');}
}

async function stopPairing(){
    try{
        await fetch('/api/pairing/stop',{method:'POST'});
        document.getElementById('pairingIntro').style.display='block';
        document.getElementById('pairingProgress').style.display='none';
        document.querySelectorAll('.pairing-step').forEach(el=>{el.classList.remove('active','completed','error');});
        if(pairingInterval)clearInterval(pairingInterval);
        showToast('Pairing gestoppt','success');
    }catch(e){showToast('Fehler','error');}
}

async function updatePairingStatus(){
    try{
        const r=await fetch('/api/pairing/status');
        const status=await r.json();
        
        document.getElementById('pairingStatus').innerHTML='<strong>Status:</strong> '+status.instructions;
        
        const steps=['step1','step2','step3'];
        steps.forEach((id,idx)=>{
            const el=document.getElementById(id);
            el.classList.remove('active','completed');
            if(idx+1<status.step)el.classList.add('completed');
            else if(idx+1===status.step)el.classList.add('active');
        });
        
        if(status.step===4){
            setTimeout(()=>{stopPairing();showToast('✓ Pairing erfolgreich!','success');},2000);
        }else if(status.step===5){
            document.querySelectorAll('.pairing-step').forEach(el=>el.classList.add('error'));
        }
    }catch(e){}
}

// Bestehende Funktionen (vereinfacht)
async function loadShades(){try{const r=await fetch('/api/shades');shades=await r.json();renderShades();updateScheduleShadeSelect();updatePairingShadeSelect();}catch(e){}}
function renderShades(){const list=document.getElementById('shadesList');if(shades.length===0){list.innerHTML='<div class="empty-state">Keine Rolläden konfiguriert</div>';return;}list.innerHTML=shades.map(s=>`<div class="shade-item"><div><strong>${s.name}</strong><br><small style="color:#666">Pos: ${s.position}%${s.moving?' (fährt)':''} | Addr: 0x${s.remoteAddress.toString(16).toUpperCase()}</small></div><div class="shade-controls"><button class="btn btn-sm btn-success" onclick="sendCommand(${s.id},'up')">⬆️</button><button class="btn btn-sm" onclick="sendCommand(${s.id},'my')" title="MY/Stop">⏹️</button><button class="btn btn-sm btn-danger" onclick="sendCommand(${s.id},'down')">⬇️</button><input type="range" class="position-slider" min="0" max="100" value="${s.position}" onchange="setPosition(${s.id},this.value)" title="Position"><button class="btn btn-sm" onclick="deleteShade(${s.id})" title="Löschen">🗑️</button></div></div>`).join('');}
async function sendCommand(id,cmd){try{await fetch('/api/shades/command',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({id,cmd})});showToast('Befehl gesendet','success');}catch(e){showToast('Fehler','error');}}
async function setPosition(id,pos){try{await fetch('/api/shades/command',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({id,cmd:'target',target:parseInt(pos)})});showToast('Position: '+pos+'%','success');}catch(e){showToast('Fehler','error');}}
// Add Shade mit Pairing Integration
let addShadeTempData = null;
let addShadePairingInterval = null;

function showAddShadeStep2(e) {
    e.preventDefault();
    const name = document.getElementById('newShadeName').value;
    const addr = document.getElementById('newShadeAddr').value;
    const upTime = parseInt(document.getElementById('newShadeUpTime').value);
    const downTime = parseInt(document.getElementById('newShadeDownTime').value);
    
    // Speichere temporär
    addShadeTempData = { name, addr, upTime, downTime };
    
    // Zeige Schritt 2
    document.getElementById('addShadeStep1').style.display = 'none';
    document.getElementById('addShadeStep2').style.display = 'block';
    document.getElementById('pairingShadeName').value = name;
    
    // Starte automatisch Empfang
    startAddShadePairing();
}

async function addShadeDirect() {
    const name = document.getElementById('newShadeName').value;
    const addr = document.getElementById('newShadeAddr').value;
    
    if (!name || !addr) {
        showToast('Bitte Name und Adresse eingeben', 'error');
        return;
    }
    
    const upTime = parseInt(document.getElementById('newShadeUpTime').value);
    const downTime = parseInt(document.getElementById('newShadeDownTime').value);
    
    try {
        await fetch('/api/shades', {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({name, remoteAddress: parseInt(addr, 16), upTime, downTime})
        });
        closeModal('addShadeModal');
        loadShades();
        showToast('Rolladen hinzugefügt', 'success');
        resetAddShadeForm();
    } catch(e) {
        showToast('Fehler beim Hinzufügen', 'error');
    }
}

async function startAddShadePairing() {
    document.getElementById('addShadePairingStatus').textContent = 'Warte auf PROG-Signal... Die Antenne ist aktiv.';
    
    // Antenne ist immer aktiv - kein explizites Aktivieren nötig
    
    // Starte Intervall zum Prüfen auf empfangene Frames
    if (addShadePairingInterval) clearInterval(addShadePairingInterval);
    addShadePairingInterval = setInterval(checkForPairingSignal, 500);
}

async function checkForPairingSignal() {
    try {
        const r = await fetch('/api/antenna/logs');
        const logs = await r.json();
        
        if (logs.length > 0) {
            // Nimm das neueste Log
            const lastLog = logs[logs.length - 1];
            
            if (lastLog.command === 0x8) { // PROG Befehl
                // Adresse gefunden!
                const detectedAddr = '0x' + lastLog.remoteAddress.toString(16).toUpperCase().padStart(6, '0');
                document.getElementById('detectedAddress').textContent = detectedAddr;
                document.getElementById('addShadeAddressDisplay').style.display = 'block';
                document.getElementById('btnConfirmAddress').style.display = 'inline-block';
                
                // Speichere die erkannte Adresse
                addShadeTempData.detectedAddr = lastLog.remoteAddress;
                
                // Aktualisiere Status
                document.getElementById('addShadePairingStatus').innerHTML = '<strong>✓ Adresse erkannt!</strong> Klicke auf "Adresse übernehmen" um fortzufahren.';
                
                // Markiere Schritt 2 als abgeschlossen
                document.getElementById('addStep1').classList.remove('active');
                document.getElementById('addStep1').classList.add('completed');
                document.getElementById('addStep2').classList.add('active');
                
                // Stoppe Intervall
                if (addShadePairingInterval) {
                    clearInterval(addShadePairingInterval);
                    addShadePairingInterval = null;
                }
            }
        }
    } catch(e) {
        console.error('Fehler beim Prüfen auf Pairing-Signal:', e);
    }
}

async function confirmDetectedAddress() {
    if (!addShadeTempData || !addShadeTempData.detectedAddr) {
        showToast('Keine Adresse erkannt!', 'error');
        return;
    }
    
    try {
        await fetch('/api/shades', {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({
                name: addShadeTempData.name,
                remoteAddress: addShadeTempData.detectedAddr,
                upTime: addShadeTempData.upTime,
                downTime: addShadeTempData.downTime
            })
        });
        
        // Antenne bleibt aktiv
        
        closeModal('addShadeModal');
        loadShades();
        showToast('✓ Rolladen erfolgreich gepairt!', 'success');
        resetAddShadeForm();
    } catch(e) {
        showToast('Fehler beim Hinzufügen', 'error');
    }
}

function cancelAddShadePairing() {
    // Stoppe Intervall
    if (addShadePairingInterval) {
        clearInterval(addShadePairingInterval);
        addShadePairingInterval = null;
    }
    
    // Antenne bleibt aktiv - kein Deaktivieren nötig
    
    // Zurück zu Schritt 1
    document.getElementById('addShadeStep2').style.display = 'none';
    document.getElementById('addShadeStep1').style.display = 'block';
    
    // Reset Status
    document.getElementById('addShadeAddressDisplay').style.display = 'none';
    document.getElementById('btnConfirmAddress').style.display = 'none';
    document.getElementById('addShadePairingStatus').textContent = 'Bereit für Pairing...';
    
    // Reset Steps
    document.getElementById('addStep1').classList.add('active');
    document.getElementById('addStep1').classList.remove('completed');
    document.getElementById('addStep2').classList.remove('active', 'completed');
    document.getElementById('addStep3').classList.remove('active', 'completed');
}

function resetAddShadeForm() {
    document.getElementById('addShadeForm').reset();
    document.getElementById('addShadeStep2').style.display = 'none';
    document.getElementById('addShadeStep1').style.display = 'block';
    document.getElementById('addShadeAddressDisplay').style.display = 'none';
    document.getElementById('btnConfirmAddress').style.display = 'none';
    document.getElementById('addShadePairingStatus').textContent = 'Bereit für Pairing...';
    
    // Reset Steps
    document.getElementById('addStep1').classList.add('active');
    document.getElementById('addStep1').classList.remove('completed');
    document.getElementById('addStep2').classList.remove('active', 'completed');
    document.getElementById('addStep3').classList.remove('active', 'completed');
    
    addShadeTempData = null;
}

// Alte Funktion für Kompatibilität
async function addShade(e) {
    e.preventDefault();
    await addShadeDirect();
}
async function deleteShade(id){if(!confirm('Rolladen wirklich löschen?'))return;try{const res=await fetch('/api/shades/'+id,{method:'DELETE'});const data=await res.json();if(data.success){loadShades();showToast('Rolladen gelöscht','success');}else{showToast('Fehler: '+data.error,'error');}}catch(e){showToast('Fehler beim Löschen','error');}}
async function loadSchedules(){try{const r=await fetch('/api/schedules');schedules=await r.json();renderSchedules();}catch(e){}}
function renderSchedules(){const list=document.getElementById('schedulesList');if(schedules.length===0){list.innerHTML='<div class="empty-state">Keine Zeitpläne erstellt</div>';return;}const days=['So','Mo','Di','Mi','Do','Fr','Sa'];list.innerHTML=schedules.map(s=>`<div class="shade-item"><div><strong>${String(s.hour).padStart(2,'0')}:${String(s.minute).padStart(2,'0')}</strong> - ${s.shadeName}<br><small style="color:#666">${days.filter((d,i)=>s.days&(1<<i)).join(', ')}</small></div><button class="btn btn-sm" onclick="deleteSchedule(${s.id})" title="Löschen">🗑️</button></div>`).join('');}
function updateScheduleShadeSelect(){const sel=document.getElementById('scheduleShade');sel.innerHTML=shades.map(s=>`<option value="${s.id}">${s.name}</option>`).join('');}
async function addSchedule(e){e.preventDefault();const shadeId=parseInt(document.getElementById('scheduleShade').value);const time=document.getElementById('scheduleTime').value.split(':');const hour=parseInt(time[0]);const minute=parseInt(time[1]);const command=parseInt(document.getElementById('scheduleAction').value);const target=command===3?parseInt(document.getElementById('scheduleTarget').value):0;let days=0;document.querySelectorAll('.scheduleDay:checked').forEach(cb=>{days|=1<<parseInt(cb.value);});try{await fetch('/api/schedules',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({shadeId,hour,minute,command,target,days})});closeModal('addScheduleModal');loadSchedules();showToast('Zeitplan erstellt','success');}catch(e){showToast('Fehler','error');}}
async function deleteSchedule(id){if(!confirm('Zeitplan löschen?'))return;try{const res=await fetch('/api/schedules/'+id,{method:'DELETE'});const data=await res.json();if(data.success){loadSchedules();showToast('Zeitplan gelöscht','success');}else{showToast('Fehler: '+data.error,'error');}}catch(e){showToast('Fehler beim Löschen','error');}}
async function saveWifi(e){e.preventDefault();const ssid=document.getElementById('wifiSsid').value;const password=document.getElementById('wifiPass').value;try{await fetch('/api/config/wifi',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({ssid,password})});showToast('WLAN gespeichert. Gerät startet neu...','success');setTimeout(()=>location.reload(),5000);}catch(e){showToast('Fehler','error');}}
async function loadDeviceInfo(){try{const r=await fetch('/api/info');const info=await r.json();document.getElementById('deviceInfo').innerHTML=`<strong>Skyeo v${info.version}</strong><br>📡 ${info.shades}/${info.maxShades} Rolläden<br>🌐 IP: ${info.ip}<br>🔌 MAC: ${info.mac}<br>📶 Signal: ${info.rssi} dBm<br>⏰ ${info.schedules} Zeitpläne<br>🔄 Modus: ${info.apMode?'AP':'Station'}`;}catch(e){}}

async function saveTimezone(){
    const offset=parseInt(document.getElementById('timezoneOffset').value);
    try{
        await fetch('/api/config/timezone',{
            method:'POST',
            headers:{'Content-Type':'application/json'},
            body:JSON.stringify({offset})
        });
        showToast('Zeitzone gespeichert. Zeitplan-Neustart erforderlich.','success');
    }catch(e){showToast('Fehler beim Speichern','error');}
}

function updateTimeDisplay(){
    const now=new Date();
    document.getElementById('currentTimeDisplay').textContent='Aktuelle Zeit: '+now.toLocaleTimeString('de-DE');
}
setInterval(updateTimeDisplay,1000);
async function rebootDevice(){if(!confirm('System wirklich neustarten?'))return;try{await fetch('/api/reboot',{method:'POST'});showToast('System startet neu...','success');}catch(e){showToast('Fehler','error');}}
async function factoryReset(){if(!confirm('⚠️ ALLE Daten werden gelöscht! Fortfahren?'))return;try{await fetch('/api/reset',{method:'POST'});showToast('Werksreset durchgeführt','success');setTimeout(()=>location.reload(),3000);}catch(e){showToast('Fehler','error');}}
function showModal(id){document.getElementById(id).classList.add('active');}
function closeModal(id){document.getElementById(id).classList.remove('active');}
function showAddShadeModal(){showModal('addShadeModal');}
function showAddScheduleModal(){showModal('addScheduleModal');}
function showToast(msg,type='info'){const t=document.getElementById('toast');t.textContent=msg;t.className='toast '+type;t.classList.add('show');setTimeout(()=>t.classList.remove('show'),4000);}
function updateShadeInList(shade){const idx=shades.findIndex(s=>s.id===shade.id);if(idx!==-1){shades[idx]=shade;renderShades();}}
function setupEventListeners(){document.getElementById('scheduleAction').addEventListener('change',function(){document.getElementById('scheduleTargetGroup').style.display=this.value==='3'?'block':'none';});}

// LED Control Functions
async function loadLEDStatus() {
    try {
        // Lade LED-Konfiguration
        const r = await fetch('/api/led/config');
        const config = await r.json();
        
        // Update UI
        document.getElementById('ledEnabled').checked = config.enabled;
        document.getElementById('ledBrightness').value = config.brightness;
        document.getElementById('ledBrightnessValue').textContent = config.brightness;
        document.getElementById('ledModeSelect').value = config.mode;
        document.getElementById('ledBlinkOn').value = config.blinkOnMs;
        document.getElementById('ledBlinkOff').value = config.blinkOffMs;
        document.getElementById('ledDimLevel').value = config.dimLevel;
        document.getElementById('ledDimLevelValue').textContent = config.dimLevel + '%';
        document.getElementById('ledScheduleTrigger').checked = config.scheduleTrigger;
        
        // Zeige/verstecke Einstellungen basierend auf Modus
        updateLEDSettingsVisibility(config.mode);
        
        // Update status info
        const statusEl = document.getElementById('ledStatusInfo');
        const modeNames = ['Aus', 'An', 'Blinken', 'Dimmen', 'Zeitplan'];
        if (config.enabled) {
            statusEl.style.background = '#e8f5e9';
            statusEl.style.borderLeft = '4px solid #4CAF50';
            statusEl.innerHTML = `<strong>✓ LED aktiviert</strong><br>Modus: ${modeNames[config.mode]} | Helligkeit: ${config.brightness}/255`;
        } else {
            statusEl.style.background = '#ffebee';
            statusEl.style.borderLeft = '4px solid #f44336';
            statusEl.innerHTML = '<strong>✗ LED deaktiviert</strong><br>Das LED-Gimmick ist ausgeschaltet';
        }
    } catch(e) {
        console.error('Fehler beim Laden des LED-Status:', e);
    }
}

function updateLEDSettingsVisibility(mode) {
    document.getElementById('blinkSettings').style.display = (mode == 2) ? 'block' : 'none';
    document.getElementById('dimSettings').style.display = (mode == 3) ? 'block' : 'none';
    document.getElementById('scheduleTriggerSettings').style.display = (mode == 4) ? 'block' : 'none';
}

async function saveLEDConfig() {
    const config = {
        enabled: document.getElementById('ledEnabled').checked,
        brightness: parseInt(document.getElementById('ledBrightness').value),
        mode: parseInt(document.getElementById('ledModeSelect').value),
        blinkOnMs: parseInt(document.getElementById('ledBlinkOn').value),
        blinkOffMs: parseInt(document.getElementById('ledBlinkOff').value),
        dimLevel: parseInt(document.getElementById('ledDimLevel').value),
        scheduleTrigger: document.getElementById('ledScheduleTrigger').checked
    };
    
    try {
        await fetch('/api/led/config', {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify(config)
        });
        
        // Update visibility
        updateLEDSettingsVisibility(config.mode);
        
        showToast('LED-Konfiguration gespeichert', 'success');
        loadLEDStatus();
    } catch(e) {
        showToast('Fehler beim Speichern', 'error');
    }
}

async function ledOn() {
    try {
        await fetch('/api/led/on', {method: 'POST'});
        showToast('LED aktiviert', 'success');
        loadLEDStatus();
    } catch(e) {
        showToast('Fehler beim Aktivieren', 'error');
    }
}

async function ledOff() {
    try {
        await fetch('/api/led/off', {method: 'POST'});
        showToast('LED deaktiviert', 'success');
        loadLEDStatus();
    } catch(e) {
        showToast('Fehler beim Deaktivieren', 'error');
    }
}

async function ledSetBrightness(value) {
    document.getElementById('ledBrightnessValue').textContent = value;
    try {
        await fetch('/api/led/brightness', {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({brightness: parseInt(value)})
        });
    } catch(e) {
        showToast('Fehler beim Setzen der Helligkeit', 'error');
    }
}

async function ledSetPattern(pattern) {
    try {
        await fetch('/api/led/pattern', {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({pattern: pattern})
        });
        showToast('Muster gesetzt: ' + pattern, 'success');
        loadLEDStatus();
    } catch(e) {
        showToast('Fehler beim Setzen des Musters', 'error');
    }
}

function showLEDPatternInfo() {
    const infoEl = document.getElementById('ledPatternInfo');
    if (infoEl.style.display === 'none') {
        infoEl.style.display = 'block';
    } else {
        infoEl.style.display = 'none';
    }
}

document.addEventListener('DOMContentLoaded',init);
    </script>
</body>
</html>
)rawliteral";

const char* WebUI::getIndexHTML() {
    return INDEX_HTML;
}

size_t WebUI::getIndexHTMLSize() {
    return strlen_P((const char*)INDEX_HTML);
}

const char* WebUI::getMainCSS() {
    return "";
}

size_t WebUI::getMainCSSSize() {
    return 0;
}

const char* WebUI::getMainJS() {
    return "";
}

size_t WebUI::getMainJSSize() {
    return 0;
}
