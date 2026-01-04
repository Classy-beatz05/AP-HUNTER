import subprocess
import re
import requests
from datetime import datetime

ESP32_URL = "http://192.168.4.1/update"


def get_arp_devices():
    try:
        output = subprocess.check_output("arp -a", shell=True).decode()
    except:
        return "ARP command failed<br>"

    devices = []
    for line in output.splitlines():
        match = re.search(r"(\d+\.\d+\.\d+\.\d+)\s+([-\w]+)", line)
        if match:
            devices.append(f"{match.group(1)} - {match.group(2)}")

    if not devices:
        return "No active devices found<br>"

    result = ""
    for i, d in enumerate(devices, 1):
        result += f"{i}) {d}<br>"
    return result


device_list = get_arp_devices()

payload = {
    "devices": device_list
}

try:
    r = requests.post(ESP32_URL, data=payload, timeout=5)
    print("ESP32 Response:", r.text)
except Exception as e:
    print("Error sending data:", e)
