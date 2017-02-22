//gbch 1.0
#include <stdio.h>
#include <errno.h>

extern int errno;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		//when file was not specified
		printf("Usage: gbch <file>\r\n");
		return 0;
	} else {
		//accessing file
		FILE *f;
		f = fopen(argv[1], "rb+");
		if(f == NULL)
		{
			//error handling
			perror(argv[1]);
			return 1;
		} else {
			//declaring variables
			int i;
			unsigned char x = 0;
			unsigned short y = 0;
			
			//calculating header checksum
			fseek(f, 0x0134, 0);
			for(i = 0x0134; i <= 0x014C; i++)
				x = x - fgetc(f) - 1;
			
			//verifying header checksum
			fseek(f, 0x014D, 0);
			if(fgetc(f) == x)
			{
				printf("Header checksum is correct.\r\n");
			} else {
				fseek(f, 0x014D, 0);
				fputc(x, f);
				printf("Fixed header checksum: 0x%X\r\n", x);
			}
			
			//calculating global checksum
			fseek(f, 0x0000, 0);
			while(!feof(f))
				y = y + fgetc(f);
			
			//subtracting previous values
			fseek(f, 0x014E, 0);
			y = y - fgetc(f) - fgetc(f) + 1;
			
			//verifying global checksum
			fseek(f, 0x014E, 0);
			if(y == (((short)fgetc(f)) << 8 | (short)fgetc(f)))
			{
				printf("Global checksum is correct.\r\n");
			} else {
				fseek(f, 0x014E, 0);
				fputc((char)(y>>8), f);
				fputc((char)y, f);
				printf("Fixed global checksum: 0x%X\r\n", y);
			}
			
			//closing file
			fclose(f);
			return 0;
		}	
	}
}
