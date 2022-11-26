# PSEXE2ROM GNUMakeFile by Alex Free

CC=gcc
VER=1.0.2

psexe2rom: clean
	$(CC) -Wall psexe2rom.c -o psexe2rom

fedora-deps:
	sudo dnf install zip gcc glibc-devel glibc-devel.i686 glibc-static glibc-static.i686 make mingw32-gcc mingw64-gcc

clean:
	rm -f psexe2rom.exe psexe2rom

clean-zip:
	rm -f psexe2rom.exe psexe2rom *.zip

cross-win32: clean
	make psexe2rom CC="i686-w64-mingw32-gcc"

cross-win64: clean
	make psexe2rom CC="x86_64-w64-mingw32-gcc"

linux-x86: clean
	make psexe2rom CC="gcc -m32 -static"

linux-x86_64: clean
	make psexe2rom CC="gcc -static"

release:
	rm -rf psexe2rom_$(VER)_$(PLATFORM) psexe2rom_$(VER)_$(PLATFORM).zip
	mkdir psexe2rom_$(VER)_$(PLATFORM)
	cp -rv psexe2rom readme.md readme.html license.txt psexe2rom_$(VER)_$(PLATFORM)
	chmod -R 777 psexe2rom_$(VER)_$(PLATFORM)
	zip -r psexe2rom_$(VER)_$(PLATFORM).zip psexe2rom_$(VER)_$(PLATFORM)
	rm -rf psexe2rom_$(VER)_$(PLATFORM)
	make clean

release-win:
	rm -rf psexe2rom_$(VER)_$(PLATFORM) psexe2rom_$(VER)_$(PLATFORM).zip
	mkdir psexe2rom_$(VER)_$(PLATFORM)
	cp -rv psexe2rom.exe readme.md readme.html license.txt psexe2rom_$(VER)_$(PLATFORM)
	chmod -R 777 psexe2rom_$(VER)_$(PLATFORM)
	zip -r psexe2rom_$(VER)_$(PLATFORM).zip psexe2rom_$(VER)_$(PLATFORM)
	rm -rf psexe2rom_$(VER)_$(PLATFORM)
	make clean

linux-x86-release: linux-x86
	make release PLATFORM=linux_x86_static

linux-x86_64-release: linux-x86_64
	make release PLATFORM=linux_x86_64_static

win-x86-release: cross-win32
	make release-win PLATFORM=windows_x86

win-x86_64-release: cross-win64
	make release-win PLATFORM=windows_x86_64

all: linux-x86-release linux-x86_64-release win-x86-release win-x86_64-release
