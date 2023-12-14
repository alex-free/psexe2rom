#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "exe_loader.h"

#define VERSION "1.0.3"

char yn_response[2];

unsigned char byte;
unsigned char dat_header_128_fs[] = { 0x00, 0x00, 0x02, 0x00 };
unsigned char dat_header_256_fs[] = { 0x00, 0x00, 0x04, 0x00 };
unsigned char dat_header_512_fs[] = { 0x00, 0x00, 0x08, 0x00 };
unsigned char dat_header_rom_offset[] = { 0x00, 0x20, 0x00, 0x00 }; // This is technically 'dynamic' according to the ROMFILE.DAT proccessing of XFLASH and could be something else that isn't 0x2000 bytes (total 'header' file size before actual .rom file), but romfile.exe always hardcodes this value so PSEXE2ROM does the same to generate byte-identical romfile.dat files
unsigned char *bytes;
unsigned char fn_in_rom[53]; // an example max string BEFORE NXFLASH/XFLASH starts generating a '02:' slot for a second .rom file listing is tonyhax-v1.1.2-international12345678910111213141.rom which is 52 characters long. + 1 for the null terminator. I think it would actually still work with a few more characters, but the '02:' slot should NOT be generated for just one file in the romfile.dat archive so I'm gonna keep this as the max. Found through trial and error :)

FILE *exe, *rom, *dat;

typedef enum { FALSE, TRUE } bool;

int c; 
int i;
int exe_size;
int rom_size;
int checksum;
int argv3_len;
int fn_in_rom_len; 

#define ROM_MAX_512 524288 // 512KiB
#define ROM_MAX_256 262144 // 256KiB
#define ROM_MAX_128 131072 // 128KiB
#define EXE_MAX_512_CART 523264 // 512KB - 0x400 (exe loader is 1024 bytes)
#define EXE_MAX_256_CART 261120 // 256KB - 0x400 (exe loader is 1024 bytes)
#define EXE_MAX_128_CART 130048 // 128KB - 0x400 (exe loader is 1024 bytes)

bool make512;
bool make256;
bool make128;
bool valid_input;

int main (int argc, const char * argv[]) 
{
    make512 = FALSE;
    make256 = FALSE;
    make128 = FALSE;
	valid_input = FALSE;
	
	printf("PSEXE2ROM v%s\nBy Alex Free & MottZilla (C)2022-2023 (3-BSD)\nhttps://alex-free.github.io/psexe2rom\n", VERSION);
	
	if(argc != 4)
	{
		printf("\nUsage:\npsexe2rom <MODE> <PS-EXE> <OUTPUT.ROM>\n\n<MODE> must be either -r (output .rom) or -d (output .rom AND NXFLASH/XFlash compatible romfile.dat)\n\n<PS-EXE> is a Playstation 1 .EXE file.\n\n<OUTPUT.ROM> is the output ROM file created by PSEXE2ROM that can be flashed to a cheat device.\n\nNote: romfile.dat will be created in the current directory if you use the -d option.\n");
		return(1);
	}
	
    argv3_len = strlen(argv[3]);
    fn_in_rom_len = 0;

    //printf("full file path length: %d\n", argv3_len);

     if(((argv[3][(argv3_len - 1)] != 'm') && (argv[3][(argv3_len - 2)] != 'o') && (argv[3][(argv3_len - 3)] != 'r') && (argv[3][(argv3_len - 4)] != '.')) || ((argv[3][(argv3_len - 1)] != 'M') && (argv[3][(argv3_len - 2)] != 'O') && (argv[3][(argv3_len - 3)] != 'R') && (argv[3][(argv3_len - 4)] != '.'))) // ignore termination unsigned char
	{
		printf("Error: output file given in 3rd argument must have the file extension .rom or .ROM\n");
		return(1);
	}

	argv3_len = argv3_len - 4; // remove '.rom'

	if((rom = fopen(argv[3], "r"))!= NULL)
	{
		printf("\nWarning: the desired ROM output file %s already exists. Is it OK to overwrite it with a new file of the same name?\n", argv[3]);

		while(!valid_input)
		{
			printf("(y/n): ");
			fgets(yn_response, 2, stdin);
			scanf("%*[^\n]");
			scanf("%*c");
		
			if(strcmp(yn_response,"y") == 0) 
			{
				valid_input = TRUE;
                fclose(rom);
				if(remove(argv[3]) != 0)
				{
					printf("Error: failed to delete the file %s, aborting conversion\n", argv[3]);
					return(1);
				} 
				else 
				{
					printf("Successfully deleted the file %s for overwriting\n", argv[3]);
				} 
			} 
			else if(strcmp(yn_response,"n") == 0) 
			{
				valid_input = TRUE;
				printf("Aborted conversion\n");
				fclose(rom);
				return(1);
			} 
			else 
			{
				printf("Invalid input, please enter y or n\n");
			}
		}
	}

	rom = fopen(argv[3], "w+b");

	if((exe = fopen(argv[2], "r+b"))!= NULL)
	{
        fseek(exe, 0L, SEEK_END);
        exe_size = ftell(exe);
        fseek(exe, 0, SEEK_SET);

        if(exe_size > EXE_MAX_512_CART)
        {
            printf("Error: PS-EXE file %s is over the file size limit for 512KiB cheat carts and can not be converted to a ROM\n", argv[1]);
	        fclose(exe);
	        fclose(rom);
            return(1);
        }
        else
        {
            make512 = TRUE;

            if(exe_size > EXE_MAX_256_CART)
            {
                make512 = FALSE;
                make256 = TRUE;

                if(exe_size > EXE_MAX_128_CART)
                {
                    make256 = FALSE;
                    make128 = TRUE;    
                }
            }
        }

    	if((strcmp(argv[1], "-r") != 0) && (strcmp(argv[1], "-d") != 0))
    	{
    		printf("Error: first argument must be -r or -d to select output mode\n");
    		fclose(rom);
    		fclose(exe);
    		return(1);
    	}
        
        printf("Writing MottZilla's PS-EXE loader assembly...\n");
    
        for(i = 0; i < 0x400; i++)       
            fputc(exe_loader[i], rom);

		printf("Writing PS-EXE...\n");
		fseek(exe, 0, SEEK_SET);
	    
        while((c = fgetc(exe)) != EOF)
	    {
		    byte = c;
	    	fputc(byte, rom);
    	}
    	fclose(exe);
        byte = 0;
        fseek(rom, 0L, SEEK_END);
        rom_size = ftell(rom);

        if(make512)
        {
            printf("%s can be flashed to 512KiB cheat carts\n", argv[3]);
            // Pad
            if(rom_size != ROM_MAX_512)
            {                
                while(rom_size != ROM_MAX_512)
                {
                    fputc(byte, rom);
                    rom_size++;
                }
            }
        }

        if(make256)
        {
            printf("%s can be flashed to 512KiB and 256KiB cheat carts\n", argv[3]);

            if(rom_size != ROM_MAX_256)
            {    
                while(rom_size != ROM_MAX_256)
                {
                    fputc(byte, rom);
                    rom_size++;
                }
            }
        }	    
        if(make128)
        {
            printf("%s can be flashed to 512KiB, 256KiB, and 128KiB cheat carts\n", argv[3]);

            if(rom_size != ROM_MAX_128)
            {            
                while(rom_size != ROM_MAX_128)
                {
                    fputc(byte, rom);
                    rom_size++;
                }
            }
        }

    } else {
	       printf("Error: Can not open the input PS-EXE file: %s for conversion\n", argv[3]);
	       fclose(rom);
	       return(1);
	}

    if(strcmp(argv[1], "-d") == 0)
    {
		unsigned char rom_buf[rom_size];
		fseek(rom, 0, SEEK_SET);
    	fread(rom_buf, 1, rom_size, rom);
  		checksum = 0;
		for(int i=0; i<rom_size;i++) 
  			checksum += rom_buf[i];

		if((dat = fopen("romfile.dat", "r")) != NULL)
		{
			printf("\nWarning: romfile.dat already exists in the current directory. Is it OK to overwrite it?\n");
			valid_input = FALSE;

			while(!valid_input)
			{
				printf("(y/n): ");
				fgets(yn_response, 2, stdin);
				scanf("%*[^\n]");
				scanf("%*c");
		
				if(strcmp(yn_response,"y") == 0) 
				{
					valid_input = TRUE;
        	        fclose(dat);
					if(remove("romfile.dat") != 0)
					{
						printf("Error: failed to delete romfile.dat, aborting conversion\n");
						return(1);
					} else {
						printf("Successfully deleted romfile.dat for overwriting\n");
					} 
				} 			
				else if(strcmp(yn_response,"n") == 0) 
				{
					valid_input = TRUE;
					printf("Aborted conversion\n");
					fclose(dat);
					fclose(rom);
					return(1);
				} else {
					printf("Invalid input, please enter y or n\n");
				}
			}	
		}

		dat = fopen("romfile.dat", "w+b");
		fseek(dat, 0, SEEK_SET);
        printf("Writing header filesize...\n"); // We pad up to one of 3 different sizes, so we can actually just hardcode the filesize and set it like this instead of dynamically figuring it out and then writing it
        
		if(make128)
		{
        	for(i = 0; i < 4; i++)       
            	fputc(dat_header_128_fs[i], dat);
		} else if(make256) {
        	for(i = 0; i < 4; i++)       
        	    fputc(dat_header_256_fs[i], dat);
		} else if(make512) {
			for(i = 0; i < 4; i++)       
            	fputc(dat_header_512_fs[i], dat);
		}

		printf("Writing ROM file offset...\n");
			for(i = 0; i < 4; i++)       
            	fputc(dat_header_rom_offset[i], dat);

        printf("Writing checksum: %08X...\n", checksum);
        bytes=(unsigned char *)&checksum;
        //printf("0x08: %02X\n0x09: %02X\n0xA: %02X\n0xB: %02X\n", bytes[0], bytes[1], bytes[2], bytes[3]);
		
		for(int i = 0; i < 4; i++)
        	fputc(bytes[i], dat);

		for(int i = 0; i < 4; i++) // 4 leading 00 bytes to filename
        	fputc(0x00, dat);

        printf("Writing filename...\n");

        while((argv[3][argv3_len] != '\\') && (argv[3][argv3_len] != '/') && (argv3_len >= 0)) // Windows delimiter could be either
        {
        	fn_in_rom[fn_in_rom_len] = argv[3][argv3_len];
        	argv3_len--;
        	fn_in_rom_len++;
        }

		if(fn_in_rom_len > (53 - 4)) // We already stripped away the .rom file extension
		{
			printf("Error: The filename in ROM is %d characters long, which is over the 53 character limit\n", (fn_in_rom_len + 4));
			fclose(rom);
			fclose(dat);
			return(1);
		}

        for(int i = (fn_in_rom_len -1); i > 0; i--) // ignore termination char!
        {
        	//printf("%02X\n", fn_in_rom[i]);
        	fputc(fn_in_rom[i], dat);
        }

        uint32_t dat_fn_fpos = ftell(dat);
        uint32_t dat_padding = (0x2000 - dat_fn_fpos); // Again, the 0x2000 header file size is hardcoded in the original romfile.exe so we do the same here.

        printf("Writing header data 2/2...\n");

        for(int i = 0; i < dat_padding; i++) // Pad up to 0x2000 bytes total before PS-EXE
        	fputc(0x00, dat);       

        printf("Writing %s data...\n", argv[3]);

        for(int i = 0; i < rom_size; i++)
        	fputc(rom_buf[i], dat);

        fclose(dat);
	}

	fclose(rom);

	printf("Complete\n");
	return 0;
}
