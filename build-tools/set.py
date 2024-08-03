import sys
import re
import subprocess
import os
import json

def EditHeader(ip: str, port: str, back: bool) -> None:
    HeaderFile = "include/macros.h"

    file = open(HeaderFile, 'r')
    HeaderContent = file.read()
    file.close()

    if back is False:
        HeaderContent = re.sub(r'IPHERE', ip, HeaderContent)
        HeaderContent = re.sub(r'PORTHERE', port, HeaderContent)
    else:
        HeaderContent = re.sub(r'^#define HOST_IP.*$', '#define HOST_IP         IPHERE', HeaderContent, flags=re.MULTILINE)
        HeaderContent = re.sub(r'^#define HOST_PORT.*$', '#define HOST_PORT       PORTHERE', HeaderContent, flags=re.MULTILINE)

    file = open(HeaderFile, 'w')
    file.write(HeaderContent)
    file.close()

def SetPortAndIp(ip, port):
    if sys.platform == "win32":
        ConvertExe = "convert.exe"
    else:
        ConvertExe = "convert"

    try:
        output = subprocess.check_output([ConvertExe, str(port), str(ip)], text=True)
    except subprocess.CalledProcessError as e:
        return False

    output = json.loads(output)
    ip = hex(int(output["ip"]))
    port = hex(int(output["port"]))

    EditHeader(ip, port, False)

    return True

if __name__ == "__main__":
    if sys.argv[1] != "back":
        SetPortAndIp(sys.argv[1], sys.argv[2])
    else:
        EditHeader(None, None, True)