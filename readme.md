# [alex-free.github.io](https://alex-free.github.io)

# PSEXE2ROM : Convert PS1/PSX Executables To Flashable GameShark/Cheat Cart ROM files

_Brought to you by [MottZilla](http://www.psxdev.net/forum/memberlist.php?mode=viewprofile&u=867) and [Alex Free](http://www.psxdev.net/forum/memberlist.php?mode=viewprofile&u=6018)_

Have you ever wanted to run your own code instead of the PSX BIOS/Shell at boot on an unmodified PS1 console? This tool allows you to convert PSX executables into a `.rom` file that you can use to overwrite the existing executable on a GameShark or similar PSX cheat cart with **your own**. 

You can flash your cheat cart using the [NXFlash CD](https://github.com/danhans42/nxflash) [method](#nxflashxflash-cd-guide) with the `romfile.dat` file generated by PS-EXE2ROM, and then the cheat cart will boot that PS-EXE automatically if the cheat cart is installed in a console and you turn it on.

Alternatively, you can send over the `.rom` file generated by PSEXE2ROM with a serial cable to transfer the `.rom` file from your computer to your PS1 console using a program like [UNIROM](https://unirom.github.io) or [NXFlash](https://github.com/danhans42/nxflash).

PSEXE2ROM can generate byte-identical `romfile.dat` files, exactly the same as when generated by the old, non-open source program `romfile.exe` included in the original NXFLASH/XFLASH releases.

## External Links

*   [PSEXE2ROM Homepage](https://alex-free.github.io/psexe2rom)
*   [PSEXE2ROM GitHub](https://github.com/alex-free/psexe2rom)
*   [Tonyhax International](https://alex-free.github.io/tonyhax-international)
*   [Stealth ROM](http://www.psxdev.net/forum/viewtopic.php?f=66&t=3966)
*   [My ROMFILE.EXE Reversing Thread On PSXDev](http://www.psxdev.net/forum/viewtopic.php?f=24&t=4005)
*   [MKPSXISO](https://github.com/LameGuy64/mkpsxiso)
*   [NXFLASH](https://github.com/danhans42/nxflash)
*   [Shadow's XFLASH Source Code Thread On PSXDev](http://www.psxdev.net/forum/viewtopic.php?t=1530)

## Downloads

### Version 1.0.1 (11/24/2022)

Changes:

*   PSEXE2ROM now has 2 modes, specified by the first argument. The `-d` argument will output a `romfile.dat` file in the current directory, **and** a `.rom` file specified by the 3rd argument given. The `-r` argument will output only the `.rom` file asspecified by the 3rd argument given.
*   Rewrote documentation in markdown/html, and added more info about how to flash `.rom` and or use `romfile.dat` files.

*   [Windows x86](https://github.com/alex-free/psexe2rom/releases/download/v1.0.1/psexe2rom_1.0.1_windows_x86.zip) _For Windows 95 OSR 2.5 and above._
*   [Windows x86\_64](https://github.com/alex-free/psexe2rom/releases/download/v1.0.1/psexe2rom_1.0.1_windows_x86_64.zip) _For 64-bit Windows._
*   [Linux x86](https://github.com/alex-free/psexe2rom/releases/download/v1.0.1/psexe2rom_1.0.1_linux_x86_static.zip) _For modern 32-bit Linux distros (static build)._
*   [Linux x86\_64](https://github.com/alex-free/psexe2rom/releases/download/v1.0.1/psexe2rom_1.0.1_linux_x86_64_static.zip) _For modern 64-bit Linux distros (static build)._
*   [Source](https://github.com/alex-free/psexe2rom/archive/refs/tags/v1.0.1.zip)

### Version 1.0 (7/6/2022)

*   [Windows x86](https://github.com/alex-free/psexe2rom/releases/download/v1.0/psexe2rom_1.0_windows_x86.zip) _For Windows 95 OSR 2.5 and above._
*   [Windows x86\_64](https://github.com/alex-free/psexe2rom/releases/download/v1.0/psexe2rom_1.0_windows_x86_64.zip) _For 64-bit Windows._
*   [Linux x86](https://github.com/alex-free/psexe2rom/releases/download/v1.0/psexe2rom_1.0_linux_x86_static.zip) _For modern 32-bit Linux distros (static build)._
*   [Linux x86\_64](https://github.com/alex-free/psexe2rom/releases/download/v1.0/psexe2rom_1.0_linux_x86_64_static.zip) _For modern 64-bit Linux distros (static build)._
*   [Source](https://github.com/alex-free/psexe2rom/archive/refs/tags/v1.0.zip)

## Usage

PSEXE2ROM is a command line program that requires 3 arguments. The program is the `psexe2rom.exe` (Windows) or `psexe2rom` (Linux) file in the releases downloadable from above.

PS1 cheat carts such as the GameShark have capacities of either 128KB, 256KB, or 512KB (such as the GameShark Pro v3.0 that I have). Due to the fact that MottZilla's primary loader that runs the PS-EXE on the cheat cart takes up space in itself, the actual maximum PS-EXE file size you can use is slightly less:

*   512KB Cheat Device: 523,264 bytes
*   256KB Cheat Device: 261,120 bytes
*   128KB Cheat Device: 130,048 bytes

If you specify a PS-EXE file bigger then 523,264 bytes psexe2rom will flat out refuse to continue after telling you that the executable you specified as the first argument is too large to be converted into a ROM file. Otherwise PSEXE2ROM will generate a ROM file that is padded exactly to 128KB, 256KB, or 512KB in size depending on the least common denominator for maximum compatibility between flash carts.

The first argument is the output mode of PSEXE2ROM. `-d` as the first argument will output a `romfile.dat` file in the current directory, **and** a `.rom` file specified by the 3rd argument given. `-r` as the first argument will output only the `.rom` file asspecified by the 3rd argument given.

The second argument is the file path to the PS-EXE file you want to run on your GameShark or other cheat device. 

The third argument is the desired output file path of the ROM file that you want to make out of the PS-EXE file you specify in the first argument. **The third argument must have the file extension `.rom` or `.ROM` specified**. Example usage below:

_Linux (In Terminal), generate my.rom from my.exe_

    ./psexe2rom -r my.exe my.rom

_Linux (In Terminal), generate my.rom from my.exe **and** a romfile.dat file in the current directory_

    ./psexe2rom -d my.exe my.rom

_Windows (In CMD.EXE) generate my.rom from my.exe_

    psexe2rom.exe -r my.exe my.rom

_Windows (In CMD.EXE), generate my.rom from my.exe **and** a romfile.dat file in the current directory_

    psexe2rom.exe -d my.exe my.rom

## NXFLASH/XFLASH CD Guide

You can build an NXFLASH or XFLASH CD with a `romfile.dat` file, which when booted on a PS1 console allows you to flash the `.rom` file contained within the `romfile.dat` file on the CD to a cheat cartridge connected to the PS1. If you don't have a serial cable and everything setup for that, this is the easiest way to flash your cheat cartridge. The only requirement is a CD-R to flash your cartridge this way.

_Steps:_

*   Download and install [MKPSXISO](https://github.com/LameGuy64/mkpsxiso).
*   Download [NXFLASH](https://github.com/danhans42/nxflash) to a local directory.
*   Download the Japanese license file to use for the CD (from the PSYQ SDK): [licensej.dat](https://github.com/alex-free/tonyhax/raw/master/boot-cd/licensej.dat), this license file ensures that the CD will boot with **any boot method for any console with a cartridge port that supports cheat carts** (SCPH-1000 to SCPH-7502).
*   Copy `nxflash/CDFILES/system.cnf`, `rom-flasher-cd.xml` (found in the psexe2rom release/source), and the `licensej.dat` files to the same directory as your `romfile.dat` file (which was previously generated with the `-d` mode of PSEXE2ROM).
*   Run `mkpsxiso rom-flasher-cd.xml`. 2 files will be generated, `rom-flasher-cd.bin`, and `rom-flasher-cd.cue`.
*   Burn the `rom-flasher-cd.cue` file and boot it in your PS1 with a cheat cartridge connected.
*   Press the `R2` button on your controller to detect your cheat cartridge (this is stored at the top left of the XFLASH/NXFLASH interface below the logo).
*   After confirming that your cheat cartridge is detected and works with NXFLASH/XFLASH, press the `start` button on your controller. 
*   Press the `X` button on your controller to flash the ROM, and wait for the loading bar to complete.
*   Reset your console, and it will immediately boot to your original PS-EXE!

## Building From Source

PSEXE2ROM is written in C with no external dependencies. To compile PSEXE2ROM, simply [download](#downloads) the latest source, `cd` into the `psexe2rom` source directory, and  execute `make`.

## License

PSEXE2ROM is released under the 3-BSD license. See the file `license.txt` in each release for more info.

## Credits

*   [MottZilla](http://www.psxdev.net/forum/memberlist.php?mode=viewprofile&u=867) : PS-EXE loader assembly and specifications of how it works.
*   Alex Free : Testing (GameShark Pro v3.0) and program written to MottZilla's specifications using his loader assembly.
*   [Shadow](http://www.psxdev.net/forum/memberlist.php?mode=viewprofile&u=211) of [PSXDev](http://www.psxdev.net/): Released the original XFLASH source code, very helpful for reversing how `romfile.exe` works.
*   [DanHans42](https://github.com/danhans42) for the NXFLASH documentation and program fork.