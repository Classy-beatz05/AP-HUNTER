#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

String officeDevices = "No data received yet";
String lastUpdated = "Never";
int deviceCount = 0;

// ---------- WEB PAGE ----------
String webpage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>AP-HUNTER</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="10">
  <style>
    body { font-family: Arial; background:#f1f5f9; margin:0; }
    header { background:#0f172a; color:white; padding:18px; text-align:center; }
    .container { padding:16px; }
    .card {
      background:white; padding:16px; margin-bottom:14px;
      border-radius:10px; box-shadow:0 4px 8px rgba(0,0,0,0.08);
    }
    .label { font-size:13px; color:#475569; }
    footer { text-align:center; padding:12px; color:#334155; }
  </style>
</head>

<body>

<header>
  <h2>🔐 AP-HUNTER</h2>
  <p>Office Wi-Fi Monitoring Dashboard</p>
</header>

<div class="container">

  <div class="card">
    <b>Office Wi-Fi Connected Devices</b><br>
    <span class="label">
      (Collected from ARP table of authorized laptop)
    </span>
    <hr>
    <b>Total Devices:</b> )rawliteral" + String(deviceCount) + R"rawliteral(<br><br>
    )rawliteral" + officeDevices + R"rawliteral(
  </div>

  <div class="card">
    <b>Last Updated:</b><br>
    )rawliteral" + lastUpdated + R"rawliteral(
  </div>

</div>

<footer>
  Made by <b>Aryan Zambare</b> & <b>Siddharam Bilure</b>
</footer>

</body>
</html>
)rawliteral";

  return html;
}

// ---------- RECEIVE DATA ----------
void handleUpdate() {
  if (server.hasArg("devices")) {
    officeDevices = server.arg("devices");

    deviceCount = 0;
    for (int i = 0; i < officeDevices.length(); i++) {
      if (officeDevices.charAt(i) == '<') deviceCount++;
    }

    lastUpdated = String(millis() / 1000) + " seconds since ESP32 start";

    server.send(200, "text/plain", "Data received");
  } else {
    server.send(400, "text/plain", "No data");
  }
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("AP-HUNTER", "12345678");

  Serial.print("Dashboard IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/html", webpage());
  });

  server.on("/update", handleUpdate);

  server.begin();
  Serial.println("Web Server Started");
}

void loop() {
  server.handleClient();
}
