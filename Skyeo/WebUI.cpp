#include "WebUI.h"

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Skyeo - Somfy Controller</title>
    <style>
* { box-sizing: border-box; margin: 0; padding: 0; }
body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; background: #0f0f23; color: #fff; min-height: 100vh; padding: 1rem; }
.bg-anim { position: fixed; top: 0; left: 0; width: 100%; height: 100%; z-index: -1; overflow: hidden; }
.bg-anim::before { content: ''; position: absolute; width: 200%; height: 200%; top: -50%; left: -50%; background: radial-gradient(circle at 30% 30%, rgba(0,212,255,0.15), transparent 40%), radial-gradient(circle at 70% 70%, rgba(0,255,136,0.1), transparent 40%); animation: bgMove 20s infinite; }
@keyframes bgMove { 0%,100%{transform:translate(0,0)} 50%{transform:translate(-2%,-2%)} }
.orb { position: absolute; border-radius: 50%; filter: blur(60px); animation: float 15s infinite; }
.orb1 { width: 400px; height: 400px; background: rgba(0,212,255,0.2); top: 10%; left: 10%; }
.orb2 { width: 300px; height: 300px; background: rgba(0,255,136,0.15); bottom: 20%; right: 15%; animation-delay: -5s; }
.orb3 { width: 250px; height: 250px; background: rgba(138,43,226,0.15); top: 50%; left: 60%; animation-delay: -10s; }
@keyframes float { 0%,100%{transform:translate(0,0) scale(1)} 33%{transform:translate(30px,-30px) scale(1.1)} 66%{transform:translate(-20px,20px) scale(0.9)} }
h1 { text-align: center; font-size: 2.5rem; font-weight: 300; letter-spacing: 0.1em; margin-bottom: 0.3rem; background: linear-gradient(135deg,#00d4ff,#fff); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }
.version { text-align: center; color: #888; font-size: 0.9rem; margin-bottom: 1.5rem; }
.container { max-width: 800px; margin: 0 auto; }
.card { background: rgba(255,255,255,0.05); border: 1px solid rgba(255,255,255,0.1); border-radius: 16px; padding: 1.2rem; margin-bottom: 1rem; }
.card h2 { font-size: 1.1rem; color: #00d4ff; margin-bottom: 1rem; border-bottom: 1px solid rgba(0,212,255,0.3); padding-bottom: 0.5rem; }
.tabs { display: flex; gap: 0.5rem; margin-bottom: 1rem; flex-wrap: wrap; justify-content: center; }
.tab-btn { padding: 0.7rem 1.2rem; background: rgba(255,255,255,0.05); border: 1px solid rgba(255,255,255,0.1); border-radius: 25px; cursor: pointer; font-size: 0.9rem; color: #aaa; transition: 0.3s; }
.tab-btn:hover { background: rgba(0,212,255,0.1); color: #00d4ff; }
.tab-btn.active { background: linear-gradient(135deg,#00d4ff,#0099cc); color: #fff; border: none; }
.dropdown { position: relative; display: inline-block; }
.dropdown-content { display: none; position: absolute; right: 0; top: 100%; background: #1a1a2e; border: 1px solid rgba(0,212,255,0.3); border-radius: 10px; min-width: 150px; z-index: 100; box-shadow: 0 8px 16px rgba(0,0,0,0.3); margin-top: 5px; }
.dropdown-content.show { display: block; }
.dropdown-item { display: block; width: 100%; padding: 0.7rem 1rem; border: none; background: transparent; color: #aaa; text-align: left; cursor: pointer; font-size: 0.9rem; border-bottom: 1px solid rgba(255,255,255,0.05); }
.dropdown-item:last-child { border-bottom: none; }
.dropdown-item:hover { background: rgba(0,212,255,0.2); color: #00d4ff; }
.dropdown-btn { min-width: 120px; }
.tab-content { display: none; }
.tab-content.active { display: block; }
.subtabs { display: flex; gap: 0.3rem; margin-bottom: 0.8rem; flex-wrap: wrap; justify-content: center; }
.subtab-btn { padding: 0.4rem 0.8rem; background: rgba(255,255,255,0.03); border: 1px solid rgba(255,255,255,0.08); border-radius: 15px; cursor: pointer; font-size: 0.8rem; color: #777; }
.subtab-btn.active { background: rgba(0,212,255,0.2); color: #00d4ff; }
.btn { display: inline-block; padding: 0.7rem 1.2rem; border: none; border-radius: 10px; cursor: pointer; font-size: 0.9rem; margin: 0.2rem; background: rgba(255,255,255,0.1); color: #fff; }
.btn-primary { background: linear-gradient(135deg,#00d4ff,#0099cc); }
.btn-success { background: linear-gradient(135deg,#00ff88,#00cc66); color: #000; }
.btn-danger { background: linear-gradient(135deg,#ff3366,#cc0044); }
.btn-sm { padding: 0.4rem 0.8rem; font-size: 0.8rem; }
.form-group { margin-bottom: 0.8rem; }
.form-group label { display: block; margin-bottom: 0.4rem; font-size: 0.9rem; color: #ccc; }
.form-group input, .form-group select { width: 100%; padding: 0.7rem; background: rgba(0,0,0,0.3); border: 1px solid rgba(255,255,255,0.1); border-radius: 8px; color: #fff; font-size: 0.95rem; }
.form-group input:focus, .form-group select:focus { outline: none; border-color: #00d4ff; }
.grid-2 { display: grid; grid-template-columns: 1fr 1fr; gap: 0.8rem; }
.status-dot { display: inline-block; width: 8px; height: 8px; border-radius: 50%; margin-right: 0.4rem; vertical-align: middle; }
.status-online { background: #00ff88; }
.status-offline { background: #ff3366; }
.status-connecting { background: #ffaa00; animation: blink 1s infinite; }
@keyframes blink { 50% { opacity: 0.3; } }
.wifi-list { max-height: 250px; overflow-y: auto; border-radius: 10px; padding: 0.5rem; background: rgba(0,0,0,0.2); }
.wifi-item { padding: 0.7rem; border-radius: 8px; margin-bottom: 0.4rem; background: rgba(255,255,255,0.03); display: flex; justify-content: space-between; cursor: pointer; }
.wifi-item:hover { background: rgba(0,212,255,0.1); }
.wifi-sig { font-size: 0.8rem; }
.wifi-sig.s { color: #00ff88; }
.wifi-sig.m { color: #ffaa00; }
.wifi-sig.w { color: #ff3366; }
.logs { background: rgba(0,0,0,0.4); color: #bbb; padding: 0.8rem; border-radius: 10px; font-family: monospace; font-size: 0.8rem; height: 300px; overflow-y: auto; }
.log-entry { padding: 0.3rem 0; border-bottom: 1px solid rgba(255,255,255,0.05); display: flex; gap: 0.5rem; font-size: 0.75rem; }
.log-time { color: #666; }
.log-addr { color: #00d4ff; }
.log-dir { padding: 0.1rem 0.3rem; border-radius: 4px; font-size: 0.65rem; }
.log-dir.tx { background: rgba(0,212,255,0.3); }
.log-dir.rx { background: rgba(0,255,136,0.3); }
.modal { display: none; position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: rgba(0,0,0,0.8); z-index: 1000; justify-content: center; align-items: center; }
.modal.active { display: flex; }
.modal-content { background: #1a1a2e; padding: 1.5rem; border-radius: 16px; max-width: 400px; width: 90%; }
.modal-header h3 { color: #00d4ff; margin-bottom: 1rem; }
.toast { position: fixed; bottom: 1rem; right: 1rem; background: #1a1a2e; color: #fff; padding: 0.8rem 1rem; border-radius: 10px; transform: translateX(150%); transition: 0.3s; z-index: 2000; }
.toast.show { transform: translateX(0); }
.toast.success { border: 1px solid #00ff88; color: #00ff88; }
.toast.error { border: 1px solid #ff3366; color: #ff3366; }
.shade-item { display: flex; justify-content: space-between; align-items: center; padding: 0.8rem; border-radius: 10px; margin-bottom: 0.5rem; background: rgba(255,255,255,0.03); border: 1px solid rgba(255,255,255,0.05); flex-wrap: wrap; gap: 0.5rem; }
.shade-item:hover { background: rgba(0,212,255,0.05); }
.shade-controls { display: flex; gap: 0.25rem; align-items: center; flex-wrap: wrap; }
.shade-controls .btn-sm { padding: 0.5rem 0.6rem; font-size: 0.85rem; min-width: 36px; }
.shade-visual { width: 36px; height: 54px; border: 2px solid rgba(0,212,255,0.5); border-radius: 4px; position: relative; overflow: hidden; background: rgba(0,0,0,0.3); flex-shrink: 0; }
input[type=range] { -webkit-appearance: none; width: 80px; height: 4px; background: rgba(255,255,255,0.1); border-radius: 2px; }
input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; width: 14px; height: 14px; border-radius: 50%; background: #00d4ff; cursor: pointer; }
select { appearance: none; background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='12' fill='%2300d4ff'%3E%3Cpath d='M6 9L1 4h10z'/%3E%3C/svg%3E"); background-repeat: no-repeat; background-position: right 0.5rem center; padding-right: 2rem; }
.empty { text-align: center; padding: 2rem; color: #666; font-style: italic; }
.badge { display: inline-block; background: #00d4ff; color: #000; padding: 0.2rem 0.6rem; border-radius: 15px; font-size: 0.7rem; font-weight: bold; margin-left: 0.5rem; }
.shade-visual { width: 40px; height: 60px; border: 2px solid rgba(0,212,255,0.5); border-radius: 4px; position: relative; overflow: hidden; background: rgba(0,0,0,0.3); }
.shade-visual .shade-fill { position: absolute; bottom: 0; left: 0; right: 0; background: linear-gradient(180deg,rgba(0,212,255,0.6),rgba(0,212,255,0.3)); transition: height 0.5s ease; }
.shade-visual.moving-up .shade-fill { animation: shadeMoveUp 1s infinite; }
.shade-visual.moving-down .shade-fill { animation: shadeMoveDown 1s infinite; }
@keyframes shadeMoveUp { 0%,100%{opacity:0.8} 50%{opacity:1} }
@keyframes shadeMoveDown { 0%,100%{opacity:1} 50%{opacity:0.8} }
.shade-percent { font-size: 0.75rem; color: #00d4ff; text-align: center; margin-top: 2px; }
.shade-info { display: flex; align-items: center; gap: 0.8rem; }
.shade-name { font-weight: bold; }
.shade-status { font-size: 0.8rem; color: #888; }
.shade-status.moving { color: #00ff88; }
.wizard-step { padding: 1rem; text-align: center; }
.wizard-step h4 { color: #00d4ff; margin-bottom: 0.8rem; }
.wizard-step p { color: #ccc; margin-bottom: 1rem; line-height: 1.5; }
.wizard-progress { display: flex; justify-content: center; gap: 0.5rem; margin-bottom: 1rem; }
.wizard-dot { width: 10px; height: 10px; border-radius: 50%; background: #333; }
.wizard-dot.active { background: #00d4ff; }
.wizard-dot.done { background: #00ff88; }
.pairing-icon { font-size: 3rem; margin: 1rem 0; }
.pulse { animation: pulse 1.5s infinite; }
@keyframes pulse { 0%,100%{transform:scale(1)} 50%{transform:scale(1.1)} }
    </style>
</head>
<body>
    <div class="bg-anim"><div class="orb orb1"></div><div class="orb orb2"></div><div class="orb orb3"></div></div>
    <h1>Skyeo <span class="badge">433 MHz</span></h1>
    <div class="version">v1.0.0 | <span id="connectionStatus">Bereit</span></div>
    
    <div class="container">
        <div class="tabs">
            <button class="tab-btn active" onclick="showTab('shades')">Rolladen</button>
            <button class="tab-btn" onclick="showTab('schedules')">Zeitplane</button>
            <div class="dropdown">
                <button class="tab-btn dropdown-btn" onclick="toggleDropdown()">Einstellungen &#9662;</button>
                <div id="settingsDropdown" class="dropdown-content">
                    <button class="dropdown-item" onclick="showTab('system')">System</button>
                    <button class="dropdown-item" onclick="showTab('antenna')">Antenne</button>
                    <button class="dropdown-item" onclick="showTab('led')">LED</button>
                    <button class="dropdown-item" onclick="showTab('prog')">PROG</button>
                    <button class="dropdown-item" onclick="showTab('wifi')">WLAN</button>
                </div>
            </div>
        </div>
        
        <div id="tab-shades" class="tab-content active">
            <div class="card">
                <h2>Verbundene Rolladen</h2>
                <button class="btn btn-primary" onclick="showModal('addShadeModal')">+ Hinzufugen</button>
                <div id="shadesList" class="empty">Keine Rolladen konfiguriert</div>
            </div>
        </div>
        
        <div id="tab-schedules" class="tab-content">
            <div class="card">
                <h2>Zeitplane</h2>
                <button class="btn btn-primary" onclick="showModal('addScheduleModal')">+ Erstellen</button>
                <div id="schedulesList" class="empty">Keine Zeitplane</div>
            </div>
        </div>
        
        <div id="tab-system" class="tab-content">
            <div class="card">
                <h2>System-Info</h2>
                <div id="systemInfo" class="empty">Lade...</div>
            </div>
            <div class="card">
                <h2>Zeitzone</h2>
                <div class="form-group">
                    <label>Zeitversatz (UTC)</label>
                    <select id="timezoneOffset" onchange="saveTimezone()">
                        <option value="1" selected>UTC+1 (Deutschland)</option>
                        <option value="0">UTC+0</option>
                        <option value="2">UTC+2</option>
                    </select>
                </div>
                <p style="font-size:0.85rem;color:#888">Aktuelle Zeit: <span id="currentTime">--:--:--</span></p>
            </div>
            <div class="card">
                <h2>System-Aktionen</h2>
                <button class="btn btn-sm" onclick="rebootDevice()">Neustarten</button>
                <button class="btn btn-sm btn-danger" onclick="factoryReset()">Werksreset</button>
                <button class="btn btn-sm" onclick="downloadBackup()">Backup</button>
                <button class="btn btn-sm" onclick="document.getElementById('restoreFile').click()">Restore</button>
                <input type="file" id="restoreFile" style="display:none" onchange="uploadRestore(this)">
            </div>
        </div>
        
        <div id="tab-antenna" class="tab-content">
            <div class="card">
                <h2>Antennen-Status</h2>
                <div id="antennaStatus" class="empty">Lade...</div>
            </div>
            <div class="card">
                <h2>Pins Konfiguration</h2>
                <div class="grid-2">
                    <div class="form-group"><label>SCK Pin</label><input type="number" id="antennaSCK" value="18"></div>
                    <div class="form-group"><label>MOSI Pin</label><input type="number" id="antennaMOSI" value="23"></div>
                    <div class="form-group"><label>MISO Pin</label><input type="number" id="antennaMISO" value="19"></div>
                    <div class="form-group"><label>CSN Pin</label><input type="number" id="antennaCSN" value="5"></div>
                    <div class="form-group"><label>TX Pin</label><input type="number" id="antennaTX" value="13"></div>
                    <div class="form-group"><label>RX Pin</label><input type="number" id="antennaRX" value="12"></div>
                </div>
                <button class="btn btn-primary btn-sm" onclick="saveAntennaConfig()">Speichern</button>
            </div>
            <div class="card">
                <h2>Empfangs-Logs</h2>
                <button class="btn btn-sm" onclick="clearLogs()">Loschen</button>
                <div id="antennaLogs" class="logs"></div>
            </div>
        </div>
        
        <div id="tab-led" class="tab-content">
            <div class="card">
                <h2>LED Gimmick</h2>
                <div class="form-group">
                    <label><input type="checkbox" id="ledEnabled" onchange="saveLEDConfig()"> LED aktiviert</label>
                </div>
                <div class="form-group">
                    <label>Helligkeit (0-255)</label>
                    <input type="range" id="ledBrightness" min="0" max="255" value="128" onchange="saveLEDConfig()">
                </div>
                <div class="form-group">
                    <label>Modus</label>
                    <select id="ledMode" onchange="saveLEDConfig()">
                        <option value="0">Aus</option>
                        <option value="1">An</option>
                        <option value="2" selected>Blinken</option>
                        <option value="3">Dimmen</option>
                        <option value="4">Bei Zeitplan</option>
                    </select>
                </div>
                <div id="blinkSettings">
                    <div class="grid-2">
                        <div class="form-group"><label>An (ms)</label><input type="number" id="ledBlinkOn" value="500" onchange="saveLEDConfig()"></div>
                        <div class="form-group"><label>Aus (ms)</label><input type="number" id="ledBlinkOff" value="500" onchange="saveLEDConfig()"></div>
                    </div>
                </div>
            </div>
        </div>
        
        <div id="tab-prog" class="tab-content">
            <div class="card">
                <h2>PROG Senden</h2>
                <p style="color:#888;margin-bottom:1rem;">Wahlen Sie einen Rolladen und drucken Sie die PROG-Taste um das Signal zu senden.</p>
                <div class="form-group">
                    <label>Rolladen auswahlen</label>
                    <select id="progShadeSelect">
                        <option value="">-- Auswahl --</option>
                    </select>
                </div>
                <button class="btn btn-primary" onclick="sendProgSelected()">PROG Senden</button>
            </div>
        </div>
        
        <div id="tab-wifi" class="tab-content">
            <div class="card">
                <h2>Aktuelle Verbindung</h2>
                <div id="wifiStatus" class="empty">Lade...</div>
            </div>
            <div class="card">
                <h2>WLAN suchen</h2>
                <button class="btn btn-sm" onclick="scanWifi()">Suchen</button>
                <div id="wifiList" class="wifi-list"></div>
            </div>
            <div class="card">
                <h2>WLAN konfigurieren</h2>
                <div class="form-group"><label>SSID</label><input type="text" id="wifiSsidInput"></div>
                <div class="form-group"><label>Passwort</label><input type="password" id="wifiPassInput"></div>
                <button class="btn btn-primary btn-sm" onclick="saveWifi()">Verbinden</button>
            </div>
        </div>
    </div>
    
    <div id="addShadeModal" class="modal">
        <div class="modal-content" style="max-width:450px;">
            <div class="modal-header"><h3>Rolladen einbinden <span style="float:right;cursor:pointer;font-size:1.2rem;" onclick="closeModal('addShadeModal')">&times;</span></h3></div>
            <div id="addShadeContent">
                <div class="form-group"><label>Name</label><input type="text" id="newShadeName" placeholder="z.B. Wohnzimmer"></div>
                <div class="grid-2">
                    <div class="form-group"><label>Up Time (s)</label><input type="number" id="newShadeUp" value="30"></div>
                    <div class="form-group"><label>Down Time (s)</label><input type="number" id="newShadeDown" value="30"></div>
                </div>
                <button class="btn btn-primary" onclick="startAddShadePairing()">Einbinden</button>
                <button class="btn" onclick="closeModal('addShadeModal')">Abbrechen</button>
            </div>
            <div id="addShadePairing" style="display:none;">
                <div class="wizard-step">
                    <p style="margin-bottom:1rem;">Drücke und halte auf deiner Fernbedienung die PROG Taste für ca. 2 Sekunden.</p>
                    <p style="color:#888;font-size:0.85rem;margin-bottom:1rem;">Das Rollo zeigt dadurch seine Adresse. Drücke dann "Verbinden".</p>
                    <button class="btn btn-primary" id="btnPairConnect" onclick="doPairConnect()">Verbinden</button>
                    <button class="btn" id="btnPairOk" onclick="finishAddShade()" disabled>OK</button>
                    <button class="btn" onclick="cancelAddShade()">Abbrechen</button>
                </div>
            </div>
        </div>
    </div>

    <div id="addScheduleModal" class="modal">
        <div class="modal-content">
            <div class="modal-header"><h3>Zeitplan erstellen</h3></div>
            <form onsubmit="addSchedule(event)">
                <div class="form-group"><label>Rolladen</label><select id="scheduleShade"></select></div>
                <div class="form-group"><label>Zeit</label><input type="time" id="scheduleTime" required></div>
                <div class="form-group"><label>Aktion</label>
                    <select id="scheduleAction">
                        <option value="0">Hoch</option>
                        <option value="1">Runter</option>
                        <option value="2">My/Stop</option>
                        <option value="3">Position</option>
                    </select>
                </div>
                <div class="form-group"><label>Tage</label>
                    <div><label><input type="checkbox" class="scheduleDay" value="0"> So</label>
                    <label><input type="checkbox" class="scheduleDay" value="1" checked> Mo</label>
                    <label><input type="checkbox" class="scheduleDay" value="2" checked> Di</label>
                    <label><input type="checkbox" class="scheduleDay" value="3" checked> Mi</label>
                    <label><input type="checkbox" class="scheduleDay" value="4" checked> Do</label>
                    <label><input type="checkbox" class="scheduleDay" value="5" checked> Fr</label>
                    <label><input type="checkbox" class="scheduleDay" value="6"> Sa</label></div>
                </div>
                <button type="submit" class="btn btn-primary">Erstellen</button>
                <button type="button" class="btn" onclick="closeModal('addScheduleModal')">Abbrechen</button>
            </form>
        </div>
    </div>
    
    <div id="toast" class="toast"></div>
    
    <div id="confirmModal" class="modal">
        <div class="modal-content" style="max-width:300px;">
            <div class="modal-header"><h3 id="confirmTitle">Bestatigen</h3></div>
            <p id="confirmText" style="margin:1rem 0;color:#ccc;">Mochten Sie fortfahren?</p>
            <p id="confirmName" style="margin:0.5rem 0;color:#00d4ff;font-weight:bold;"></p>
            <div style="display:flex;gap:0.5rem;justify-content:center;margin-top:1rem;">
                <button class="btn btn-danger" id="confirmYes">Ja</button>
                <button class="btn" onclick="closeModal('confirmModal')">Abbrechen</button>
            </div>
        </div>
    </div>
    
    <script>
let shades=[], schedules=[];
function showTab(id){document.querySelectorAll('.tab-btn').forEach(b=>b.classList.remove('active'));event.target.classList.add('active');document.querySelectorAll('.tab-content').forEach(c=>c.classList.remove('active'));document.getElementById('tab-'+id).classList.add('active');document.getElementById('settingsDropdown').classList.remove('show');}
function toggleDropdown(){document.getElementById('settingsDropdown').classList.toggle('show');}
function showModal(id){document.getElementById(id).classList.add('active');}
function closeModal(id){document.getElementById(id).classList.remove('active');}
function showToast(msg,type='info'){const t=document.getElementById('toast');t.textContent=msg;t.className='toast '+type+' show';setTimeout(()=>t.classList.remove('show'),3000);}
function confirmAction(title,text,name,onYes){document.getElementById('confirmTitle').textContent=title;document.getElementById('confirmText').textContent=text;document.getElementById('confirmName').textContent=name;document.getElementById('confirmYes').onclick=function(){closeModal('confirmModal');onYes();};showModal('confirmModal');}
async function loadShades(){try{const r=await fetch('/api/shades');shades=await r.json();renderShades();updateProgSelect();}catch(e){console.error(e);}}
function updateProgSelect(){const s=document.getElementById('progShadeSelect');if(!s||!shades)return;const cur=s.value;s.innerHTML='<option value="">-- Auswahl --</option>'+(shades.map(x=>'<option value="'+x.id+'">'+x.name+'</option>').join(''));if(cur)s.value=cur;}
async function sendProgSelected(){const id=document.getElementById('progShadeSelect').value;if(!id){showToast('Bitte Rolladen auswahlen','error');return;}await sendProg(parseInt(id));}
function renderShades(){const l=document.getElementById('shadesList');if(!shades||shades.length===0){l.innerHTML='<div class="empty">Keine Rolladen</div>';return;}l.innerHTML=shades.map(s=>'<div class="shade-item"><div class="shade-info"><div class="shade-visual '+(s.moving?'moving-'+(s.direction>0?'down':'up'):'')+'"><div class="shade-fill" style="height:'+(100-s.position)+'%"></div></div><div><div class="shade-name">'+s.name+'</div><div class="shade-percent">'+s.position+'%</div><div class="shade-status '+(s.moving?'moving':'')+'">'+(s.moving?(s.direction>0?'Runter':'Hoch'):'Stopp')+'</div></div></div><div class="shade-controls"><button class="btn btn-sm" onclick="sendCmd('+s.id+',0)">&#9650;</button><button class="btn btn-sm" onclick="sendCmd('+s.id+',2)">MY</button><button class="btn btn-sm" onclick="sendCmd('+s.id+',1)">&#9660;</button><button class="btn btn-sm btn-danger" onclick="confirmDel('+s.id+',\''+s.name.replace(/'/g,"\\'")+'\')">&#10005;</button></div></div>').join('');}
async function sendCmd(id,cmd){try{const res=await fetch('/api/shades/command',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({shadeId:id,command:cmd})});if(!res.ok){const err=await res.text();showToast('Fehler: '+err,'error');}}catch(e){showToast('Fehler','error');}}
async function sendProg(id){try{const res=await fetch('/api/shades/command',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({shadeId:id,command:16})});if(res.ok)showToast('PROG gesendet!','success');else showToast('Fehler','error');}catch(e){showToast('Fehler','error');}}
async function addShade(e){if(e)e.preventDefault();const n=document.getElementById('newShadeName').value;const u=parseInt(document.getElementById('newShadeUp').value);const d=parseInt(document.getElementById('newShadeDown').value);try{await fetch('/api/shades',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({name:n,remoteAddress:null,upTime:u,downTime:d})});closeModal('addShadeModal');loadShades();showToast('Hinzugefugt','success');}catch(e){showToast('Fehler','error');}}
let pendingAddShade={name:'',upTime:30,downTime:30,pairedAddress:null};
async function startAddShadePairing(){pendingAddShade.name=document.getElementById('newShadeName').value;pendingAddShade.upTime=parseInt(document.getElementById('newShadeUp').value);pendingAddShade.downTime=parseInt(document.getElementById('newShadeDown').value);if(!pendingAddShade.name){showToast('Bitte Name eingeben','error');return;}document.getElementById('addShadeContent').style.display='none';document.getElementById('addShadePairing').style.display='block';document.getElementById('btnPairOk').disabled=true;document.getElementById('btnPairConnect').disabled=false;pendingAddShade.pairedAddress=null;}
async function doPairConnect(){document.getElementById('btnPairConnect').disabled=true;document.getElementById('btnPairConnect').textContent='Sende...';try{const r=await fetch('/api/shades/pair',{method:'POST'});const d=await r.json();if(d.address){pendingAddShade.pairedAddress=d.address;showToast('Signal empfangen!','success');}else{showToast('Kein Signal - nochmal versuchen','error');document.getElementById('btnPairConnect').disabled=false;}}catch(e){showToast('Fehler','error');document.getElementById('btnPairConnect').disabled=false;}document.getElementById('btnPairConnect').textContent='Verbinden';setTimeout(()=>{document.getElementById('btnPairOk').disabled=false;},5000);}
function cancelAddShade(){document.getElementById('addShadeContent').style.display='block';document.getElementById('addShadePairing').style.display='none';pendingAddShade={name:'',upTime:30,downTime:30,pairedAddress:null};loadShades();}
async function finishAddShade(){try{await fetch('/api/shades',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({name:pendingAddShade.name,remoteAddress:pendingAddShade.pairedAddress||null,upTime:pendingAddShade.upTime,downTime:pendingAddShade.downTime})});closeModal('addShadeModal');document.getElementById('addShadeContent').style.display='block';document.getElementById('addShadePairing').style.display='none';pendingAddShade={name:'',upTime:30,downTime:30,pairedAddress:null};loadShades();showToast('Rolladen hinzugefugt','success');}catch(e){showToast('Fehler','error');}}
function confirmDel(id,name){confirmAction('Rolladen loschen','Mochten Sie diesen Rolladen wirklich loschen?',name,()=>delShade(id));}
async function delShade(id){try{const res=await fetch('/api/shade/delete?id='+id,{method:'POST'});if(res.ok){loadShades();showToast('Geloscht','success');}else{const err=await res.text();showToast('Fehler: '+err,'error');}}catch(e){showToast('Fehler','error');}}

async function loadSchedules(){try{const r=await fetch('/api/schedules');schedules=await r.json();renderSchedules();}catch(e){}}
function renderSchedules(){const l=document.getElementById('schedulesList');if(schedules.length===0){l.innerHTML='<div class="empty">Keine Zeitplane</div>';return;}const days=['So','Mo','Di','Mi','Do','Fr','Sa'];l.innerHTML=schedules.map(s=>'<div class="shade-item"><div><strong>'+String(s.hour).padStart(2,'0')+':'+String(s.minute).padStart(2,'0')+'</strong> - '+s.shadeName+'<br><small style="color:#888">'+days.filter((d,i)=>s.days&(1<<i)).join(', ')+'</small></div><button class="btn btn-sm btn-danger" onclick="delSchedule('+s.id+')">X</button></div>').join('');}
async function addSchedule(e){e.preventDefault();const sid=parseInt(document.getElementById('scheduleShade').value);const t=document.getElementById('scheduleTime').value.split(':');const h=parseInt(t[0]);const m=parseInt(t[1]);const cmd=parseInt(document.getElementById('scheduleAction').value);let days=0;document.querySelectorAll('.scheduleDay:checked').forEach(c=>{days|=1<<parseInt(c.value);});try{await fetch('/api/schedules',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({shadeId:sid,hour:h,minute:m,command:cmd,days:days})});closeModal('addScheduleModal');loadSchedules();showToast('Erstellt','success');}catch(e){showToast('Fehler','error');}}
async function delSchedule(id){if(!confirm('Loschen?'))return;try{await fetch('/api/schedules/'+id,{method:'DELETE'});loadSchedules();showToast('Geloscht','success');}catch(e){showToast('Fehler','error');}}
async function saveTimezone(){const o=parseInt(document.getElementById('timezoneOffset').value);try{await fetch('/api/config/timezone',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({offset:o})});showToast('Gespeichert','success');}catch(e){showToast('Fehler','error');}}
async function loadSystemInfo(){try{const r=await fetch('/api/info');const s=await r.json();document.getElementById('systemInfo').innerHTML='<p><strong>Version:</strong> '+s.version+'</p><p><strong>IP:</strong> '+s.ip+'</p><p><strong>MAC:</strong> '+s.mac+'</p><p><strong>Signal:</strong> '+s.rssi+' dBm</p><p><strong>Uptime:</strong> '+s.uptime+'s</p><p><strong>CPU Temp:</strong> '+(s.cpuTemp||'N/A')+'</p><p><strong>Rolladen:</strong> '+s.shades+'/'+s.maxShades+'</p><p><strong>Zeitplane:</strong> '+s.schedules+'</p>';document.getElementById('wifiStatus').innerHTML='<p><strong>SSID:</strong> '+(s.wifiSsid||'Nicht verbunden')+'</p><p><strong>IP:</strong> '+s.ip+'</p><p><strong>Signal:</strong> '+s.rssi+' dBm</p>';}catch(e){document.getElementById('systemInfo').innerHTML='<div class="empty">Fehler beim Laden</div>';}}
async function loadAntennaStatus(){try{const r=await fetch('/api/antenna/status');const a=await r.json();document.getElementById('antennaStatus').innerHTML='<p><strong>Status:</strong> '+(a.initialized?'Initialisiert':'Nicht initialisiert')+'</p><p><strong>Version:</strong> '+a.version+'</p><p><strong>RSSI:</strong> '+a.rssi+' dBm</p><p><strong>Empfangen:</strong> '+a.framesReceived+'</p><p><strong>Gesendet:</strong> '+a.framesSent+'</p>';}catch(e){document.getElementById('antennaStatus').innerHTML='<div class="empty">Fehler beim Laden</div>';}}
async function loadAntennaLogs(){try{const r=await fetch('/api/antenna/logs');const l=await r.json();const logs=document.getElementById('antennaLogs');if(l.length===0){logs.innerHTML='<div class="empty">Keine Logs</div>';return;}logs.innerHTML=l.slice(-20).map(x=>'<div class="log-entry"><span class="log-time">'+x.time+'</span><span class="log-addr">'+x.address+'</span><span>'+x.command+'</span><span class="log-dir '+x.direction+'">'+x.direction.toUpperCase()+'</span></div>').join('');}catch(e){}}
async function clearLogs(){document.getElementById('antennaLogs').innerHTML='<div class="empty">Gelöscht</div>';}
async function saveAntennaConfig(){const c={SCK:parseInt(document.getElementById('antennaSCK').value),MOSI:parseInt(document.getElementById('antennaMOSI').value),MISO:parseInt(document.getElementById('antennaMISO').value),CSN:parseInt(document.getElementById('antennaCSN').value),TX:parseInt(document.getElementById('antennaTX').value),RX:parseInt(document.getElementById('antennaRX').value)};try{await fetch('/api/antenna/config',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(c)});showToast('Antenne gespeichert - Neustart erforderlich','success');}catch(e){showToast('Fehler','error');}}
async function scanWifi(){try{const r=await fetch('/api/wifi/scan');const w=await r.json();document.getElementById('wifiList').innerHTML=w.map(n=>'<div class="wifi-item" onclick="document.getElementById(\'wifiSsidInput\').value=\''+n.ssid+'\'"><span>'+n.ssid+'</span><span class="wifi-sig '+(n.rssi>-50?'s':n.rssi>-70?'m':'w')+'">'+n.rssi+' dBm</span></div>').join('');}catch(e){showToast('Scan fehlgeschlagen','error');}}
async function saveWifi(){const ssid=document.getElementById('wifiSsidInput').value;const pass=document.getElementById('wifiPassInput').value;try{await fetch('/api/config/wifi',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({ssid:ssid,password:pass})});showToast('WLAN gespeichert - Neustart...','success');setTimeout(()=>location.reload(),3000);}catch(e){showToast('Fehler','error');}}
async function saveLEDConfig(){const c={enabled:document.getElementById('ledEnabled').checked,brightness:parseInt(document.getElementById('ledBrightness').value),mode:parseInt(document.getElementById('ledMode').value),blinkOnMs:parseInt(document.getElementById('ledBlinkOn')?.value||500),blinkOffMs:parseInt(document.getElementById('ledBlinkOff')?.value||500)};try{await fetch('/api/led/config',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(c)});showToast('LED gespeichert','success');}catch(e){}}
async function rebootDevice(){if(!confirm('Neustarten?'))return;try{await fetch('/api/reboot',{method:'POST'});showToast('Neustarte...','success');}catch(e){}}
async function factoryReset(){if(!confirm('ALLES loschen?'))return;try{await fetch('/api/reset',{method:'POST'});showToast('Reset abgeschlossen','success');setTimeout(()=>location.reload(),2000);}catch(e){showToast('Fehler','error');}}
function downloadBackup(){window.location='/api/backup';}
async function uploadRestore(input){const file=input.files[0];if(!file)return;const reader=new FileReader();reader.onload=async function(e){try{const res=await fetch('/api/restore',{method:'POST',headers:{'Content-Type':'application/json'},body:e.target.result});if(res.ok){showToast('Restore erfolgreich','success');setTimeout(()=>location.reload(),2000);}else{showToast('Restore fehlgeschlagen','error');}}catch(err){showToast('Fehler','error');}};reader.readAsText(file);}
function updateClock(){document.getElementById('currentTime').textContent=new Date().toLocaleTimeString('de-DE');}
setInterval(updateClock,1000);
setInterval(loadShades,3000);
setInterval(loadSystemInfo,5000);
setInterval(loadAntennaLogs,2000);
loadShades();loadSchedules();loadSystemInfo();loadAntennaStatus();
    </script>
</body>
</html>
)rawliteral";

const char* WebUI::getIndexHTML() { return INDEX_HTML; }
size_t WebUI::getIndexHTMLSize() { return strlen(INDEX_HTML); }
