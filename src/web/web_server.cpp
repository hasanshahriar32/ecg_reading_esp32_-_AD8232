/*
 * ECG Web Server Class Implementation
 * 
 * Provides web interface for real-time ECG monitoring
 */

#include "web_server.h"
#include "../config/config.h"

ECGWebServer::ECGWebServer() : server(WEB_SERVER_PORT) {
  wifiConnected = false;
  serverStarted = false;
  currentECGValue = 0;
  currentHeartRate = 0;
  currentSignalQuality = 0;
  leadsConnected = false;
  lastDataUpdate = 0;
}

bool ECGWebServer::begin() {
  Serial.println("Starting web server...");
  
  // Connect to WiFi
  if (!connectToWiFi()) {
    Serial.println("ERROR: Failed to connect to WiFi");
    return false;
  }
  
  // Setup server routes
  setupRoutes();
  
  // Start server
  server.begin();
  serverStarted = true;
  
  Serial.println("✓ Web server started successfully");
  Serial.print("✓ Access the ECG monitor at: http://");
  Serial.println(WiFi.localIP());
  
  return true;
}

bool ECGWebServer::connectToWiFi() {
  Serial.print("Connecting to WiFi network: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println();
    Serial.println("✓ WiFi connected successfully");
    Serial.print("✓ IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("✓ Signal strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    return true;
  } else {
    Serial.println();
    Serial.println("✗ WiFi connection failed");
    return false;
  }
}

void ECGWebServer::setupRoutes() {
  // Bind route handlers to this instance
  server.on("/", [this]() { this->handleRoot(); });
  server.on("/data", [this]() { this->handleData(); });
  server.on("/status", [this]() { this->handleStatus(); });
  server.onNotFound([this]() { this->handleNotFound(); });
}

void ECGWebServer::handleClient() {
  if (serverStarted) {
    server.handleClient();
  }
}

void ECGWebServer::updateECGData(int ecgValue, int heartRate, int signalQuality) {
  currentECGValue = ecgValue;
  currentHeartRate = heartRate;
  currentSignalQuality = signalQuality;
  lastDataUpdate = millis();
}

void ECGWebServer::updateLeadStatus(bool connected) {
  leadsConnected = connected;
}

String ECGWebServer::getIPAddress() {
  if (wifiConnected) {
    return WiFi.localIP().toString();
  }
  return "Not connected";
}

String ECGWebServer::getConnectionInfo() {
  if (wifiConnected) {
    return "Connected to " + String(WIFI_SSID) + " (" + WiFi.localIP().toString() + ")";
  }
  return "Not connected to WiFi";
}

void ECGWebServer::handleRoot() {
  String html = generateHTML();
  server.send(200, "text/html", html);
}

void ECGWebServer::handleData() {
  DynamicJsonDocument doc(200);
  
  doc["ecgValue"] = currentECGValue;
  doc["timestamp"] = millis();
  doc["lastUpdate"] = lastDataUpdate;
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}

void ECGWebServer::handleStatus() {
  DynamicJsonDocument doc(400);
  
  doc["heartRate"] = currentHeartRate;
  doc["signalQuality"] = currentSignalQuality;
  doc["leadsConnected"] = leadsConnected;
  doc["sampleRate"] = SAMPLE_RATE;
  doc["uptime"] = millis();
  doc["wifiConnected"] = wifiConnected;
  doc["ipAddress"] = WiFi.localIP().toString();
  doc["rssi"] = WiFi.RSSI();
  doc["freeHeap"] = ESP.getFreeHeap();
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}

void ECGWebServer::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: " + server.uri() + "\n";
  message += "Method: " + (server.method() == HTTP_GET ? "GET" : "POST") + "\n";
  message += "Arguments: " + String(server.args()) + "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}

String ECGWebServer::generateHTML() {
  return R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 ECG Monitor</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        
        body { 
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container { 
            max-width: 1400px; 
            margin: 0 auto; 
            background: rgba(255, 255, 255, 0.95);
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            padding: 30px;
            backdrop-filter: blur(10px);
        }
        
        .header {
            text-align: center;
            margin-bottom: 30px;
        }
        
        .header h1 {
            color: #2c3e50;
            font-size: 2.5em;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.1);
        }
        
        .header p {
            color: #7f8c8d;
            font-size: 1.1em;
        }
        
        .status-grid { 
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .status-card { 
            background: linear-gradient(135deg, #f8f9fa, #e9ecef);
            border-radius: 15px;
            padding: 25px;
            text-align: center;
            box-shadow: 0 5px 15px rgba(0,0,0,0.1);
            transition: transform 0.3s ease;
        }
        
        .status-card:hover {
            transform: translateY(-5px);
        }
        
        .status-value { 
            font-size: 3em; 
            font-weight: bold; 
            margin-bottom: 10px;
            background: linear-gradient(45deg, #007bff, #0056b3);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            background-clip: text;
        }
        
        .status-label { 
            color: #6c757d; 
            font-size: 1.1em;
            font-weight: 500;
        }
        
        .connection-status {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 10px;
        }
        
        .led-indicator { 
            width: 24px; 
            height: 24px; 
            border-radius: 50%; 
            box-shadow: 0 0 10px rgba(0,0,0,0.3);
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0% { transform: scale(1); opacity: 1; }
            50% { transform: scale(1.1); opacity: 0.7; }
            100% { transform: scale(1); opacity: 1; }
        }
        
        .connected { 
            background: linear-gradient(45deg, #28a745, #20c997);
            box-shadow: 0 0 20px rgba(40, 167, 69, 0.5);
        }
        
        .disconnected { 
            background: linear-gradient(45deg, #dc3545, #fd7e14);
            box-shadow: 0 0 20px rgba(220, 53, 69, 0.5);
        }
        
        .chart-container { 
            background: white;
            border-radius: 15px;
            padding: 20px;
            box-shadow: 0 10px 25px rgba(0,0,0,0.1);
            margin-bottom: 30px;
        }
        
        .footer {
            text-align: center;
            color: #6c757d;
            font-size: 0.9em;
            line-height: 1.6;
        }
        
        .footer p {
            margin-bottom: 5px;
        }
        
        .system-info {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
            margin-top: 20px;
            padding: 20px;
            background: rgba(108, 117, 125, 0.1);
            border-radius: 10px;
        }
        
        .info-item {
            text-align: center;
            padding: 10px;
        }
        
        .info-label {
            font-size: 0.8em;
            color: #6c757d;
            margin-bottom: 5px;
        }
        
        .info-value {
            font-weight: bold;
            color: #495057;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🫀 Advanced ECG Monitor</h1>
            <p>Real-time cardiac monitoring with ESP32 & AD8232</p>
        </div>
        
        <div class="status-grid">
            <div class="status-card">
                <div class="status-value" id="heartRate">--</div>
                <div class="status-label">Heart Rate (BPM)</div>
            </div>
            <div class="status-card">
                <div class="status-value" id="signalQuality">--</div>
                <div class="status-label">Signal Quality (%)</div>
            </div>
            <div class="status-card">
                <div class="connection-status">
                    <span class="led-indicator" id="connectionLed"></span>
                    <div>
                        <div class="status-value" style="font-size: 1.5em;" id="connectionStatus">--</div>
                        <div class="status-label">Lead Connection</div>
                    </div>
                </div>
            </div>
        </div>
        
        <div class="chart-container">
            <div id="ecgChart" style="width:100%;height:500px;"></div>
        </div>
        
        <div class="footer">
            <p><strong>ESP32 ECG Monitoring System</strong></p>
            <p>Ensure electrodes are properly connected for optimal results</p>
            <p>⚠️ For educational purposes only - not for medical diagnosis</p>
            
            <div class="system-info">
                <div class="info-item">
                    <div class="info-label">Uptime</div>
                    <div class="info-value" id="uptime">--</div>
                </div>
                <div class="info-item">
                    <div class="info-label">Sample Rate</div>
                    <div class="info-value" id="sampleRate">--</div>
                </div>
                <div class="info-item">
                    <div class="info-label">WiFi Signal</div>
                    <div class="info-value" id="wifiSignal">--</div>
                </div>
                <div class="info-item">
                    <div class="info-label">Free Memory</div>
                    <div class="info-value" id="freeMemory">--</div>
                </div>
            </div>
        </div>
    </div>

    <script>
        let ecgData = [];
        let timeData = [];
        let maxPoints = 1000;
        let startTime = Date.now();
        
        // Initialize plot with beautiful styling
        let trace = {
            x: timeData,
            y: ecgData,
            type: 'scatter',
            mode: 'lines',
            line: {
                color: '#007bff',
                width: 2
            },
            name: 'ECG Signal',
            fill: 'tonexty',
            fillcolor: 'rgba(0, 123, 255, 0.1)'
        };
        
        let layout = {
            title: {
                text: 'Real-time ECG Waveform',
                font: { size: 20, color: '#2c3e50' }
            },
            xaxis: {
                title: 'Time (seconds)',
                gridcolor: '#e9ecef',
                linecolor: '#dee2e6'
            },
            yaxis: {
                title: 'Amplitude (ADC Value)',
                gridcolor: '#e9ecef',
                linecolor: '#dee2e6'
            },
            showlegend: false,
            plot_bgcolor: '#f8f9fa',
            paper_bgcolor: 'white',
            margin: {l: 60, r: 30, t: 60, b: 60}
        };
        
        let config = {
            responsive: true,
            displayModeBar: false
        };
        
        Plotly.newPlot('ecgChart', [trace], layout, config);
        
        function updateData() {
            fetch('/data')
                .then(response => response.json())
                .then(data => {
                    let currentTime = (Date.now() - startTime) / 1000;
                    
                    ecgData.push(data.ecgValue);
                    timeData.push(currentTime);
                    
                    if (ecgData.length > maxPoints) {
                        ecgData.shift();
                        timeData.shift();
                    }
                    
                    Plotly.redraw('ecgChart');
                })
                .catch(error => console.error('Data update error:', error));
        }
        
        function updateStatus() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    // Update main status values
                    document.getElementById('heartRate').textContent = data.heartRate || '--';
                    document.getElementById('signalQuality').textContent = data.signalQuality || '--';
                    
                    // Update connection status
                    let led = document.getElementById('connectionLed');
                    let status = document.getElementById('connectionStatus');
                    
                    if (data.leadsConnected) {
                        led.className = 'led-indicator connected';
                        status.textContent = 'Connected';
                    } else {
                        led.className = 'led-indicator disconnected';
                        status.textContent = 'Disconnected';
                    }
                    
                    // Update system info
                    document.getElementById('uptime').textContent = formatUptime(data.uptime);
                    document.getElementById('sampleRate').textContent = data.sampleRate + ' Hz';
                    document.getElementById('wifiSignal').textContent = data.rssi + ' dBm';
                    document.getElementById('freeMemory').textContent = formatBytes(data.freeHeap);
                })
                .catch(error => console.error('Status update error:', error));
        }
        
        function formatUptime(ms) {
            let seconds = Math.floor(ms / 1000);
            let minutes = Math.floor(seconds / 60);
            let hours = Math.floor(minutes / 60);
            
            if (hours > 0) {
                return hours + 'h ' + (minutes % 60) + 'm';
            } else if (minutes > 0) {
                return minutes + 'm ' + (seconds % 60) + 's';
            } else {
                return seconds + 's';
            }
        }
        
        function formatBytes(bytes) {
            if (bytes < 1024) return bytes + ' B';
            if (bytes < 1048576) return (bytes / 1024).toFixed(1) + ' KB';
            return (bytes / 1048576).toFixed(1) + ' MB';
        }
        
        // Update intervals
        setInterval(updateData, 20);     // 50 Hz data updates
        setInterval(updateStatus, 1000); // 1 Hz status updates
        
        // Initial updates
        updateData();
        updateStatus();
        
        // Add some visual feedback
        console.log('🫀 ESP32 ECG Monitor Web Interface Loaded');
        console.log('📊 Real-time data streaming active');
    </script>
</body>
</html>
)";
}
