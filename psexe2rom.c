#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "exe_loader.h"

char yn_response[2];

unsigned char byte;

FILE *exe, *rom;

typedef enum { FALSE, TRUE } bool;

int c; 
int i;
int exe_size;
int rom_size;

#define ROM_MAX_512 524288 // 512KB
#define ROM_MAX_256 262144 // 256KB
#define ROM_MAX_128 131072 // 128KB
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
	
	printf("PSEXE2ROM v1.0 By Alex Free & MottZilla (C)2022 3-BSD\nhttps://alex-free.github.io/psexe2rom\n");
	
	if(argc != 3)
	{
		printf("\nUsage:\npsexe2rom <PS-EXE> <OUTPUT.ROM>\n\n<PS-EXE> is a Playstation 1 .EXE file.\n\n<OUTPUT.ROM> is the output ROM file created by PSEXE2ROM that can be flashed to a cheat device.\n");
		return(1);
	}
	
	if((rom = fopen(argv[2], "r"))!= NULL)
	{
		printf("\nWarning: the desired ROM output file %s already exists. Is it OK to overwrite it with a new file of the same name?\n", argv[2]);

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
				if(remove(argv[2]) != 0)
				{
					printf("Error: failed to delete the file %s, aborting conversion\n", argv[2]);
					return(1);
				} 
				else 
				{
					printf("Successfully deleted the file %s for overwriting\n", argv[2]);
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

	rom = fopen(argv[2], "w+b");

	if((exe = fopen(argv[1], "r+b"))!= NULL)
	{
        fseek(exe, 0L, SEEK_END);
        exe_size = ftell(exe);
        fseek(exe, 0, SEEK_SET);

        if(exe_size > EXE_MAX_512_CART)
        {
            printf("Error: PS-EXE file %s is over the file size limit for 512KB cheat carts and can not be converted to a ROM\n", argv[1]);
	        fclose(exe);
	        fclose(rom);
            return(1);
        }
        else
        {
            make512 = TRUE;

            if(exe_size > EXE_MAX_256_CART)
            {
                printf("WARNING: PS-EXE file %s is over the file size limit for 256KB cheat carts and a 256KB ROM will not be made\n", argv[1]);
            }
            else
            {
                make512 = FALSE;
                make256 = TRUE;

                if(exe_size > EXE_MAX_128_CART)
                {
                    printf("WARNING: PS-EXE file %s is over the file size limit for 128KB cheat carts and a 128KB ROM will not be made\n", argv[1]);
                }
                else
                {
                    make256 = FALSE;
                    make128 = TRUE;    
                }
            }
        }

        printf("Writing PS-EXE loader assembly\n");
    
        for(i = 0; i < 0x400; i++)       
            fputc(exe_loader[i], rom);

		printf("Writing PS-EXE\n");
		fseek(exe, 0, SEEK_SET);
	    
        while((c = fgetc(exe)) != EOF)
	    {
		    byte = c;
	    	fputc(byte, rom);
    	}

        byte = 0;
        fseek(rom, 0L, SEEK_END);
        rom_size = ftell(rom);

        if(make512)
        {
            printf("INFO: ROM file %s can be flashed to 512KB cheat carts\n", argv[2]);
            
            if(rom_size != ROM_MAX_512)
            {
                printf("Padding ROM file to 512KB");
                
                while(rom_size != ROM_MAX_512)
                {
                    fputc(byte, rom);
                    rom_size++;
                }
            }
        }

        if(make256)
        {
            printf("INFO: ROM file %s can be flashed to 512KB and 256KB cheat carts\n", argv[2]);

            if(rom_size != ROM_MAX_256)
            {
                printf("Padding ROM file to 256KB");
    
                while(rom_size != ROM_MAX_256)
                {
                    fputc(byte, rom);
                    rom_size++;
                }
            }
        }	    
        if(make128)
        {
            printf("INFO: ROM file %s can be flashed to 512KB, 256KB, and 128KB cheat carts\n", argv[2]);

            if(rom_size != ROM_MAX_128)
            {
                printf("Padding ROM file to 128KB");
            
                while(rom_size != ROM_MAX_128)
                {
                    fputc(byte, rom);
                    rom_size++;
                }
            }
        }

    } else {
	       printf("Error: Can not open the input PS-EXE file for conversion\n");
	       fclose(rom);
	       return(1);
	}

	fclose(exe);
	fclose(rom);
	
	printf("\nDone! The output file %s is a NXFlash compatible .ROM\n", argv[2]);

	return 0;
}
