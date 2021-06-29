//Bare bones altair emulator
//Margaret Black, 2019

#include <stdio.h>
#include <stdlib.h>
#include "8085.h"

#include <termios.h>
#include <sys/ioctl.h>

struct termios termios_old;
int DOEXIT=0;

P8085* p8085;
unsigned int ram[0x10000];

void port_write(P8085* p8085, unsigned int address, unsigned char data)
{
//	printf("port write %x %x\n",address,data);
	switch(address)
	{
		case 0x01:
			printf("%c",data&0x7f);
			fflush(stdout);
			break;
	}
}

unsigned int port_read(P8085* p8085, unsigned int address)
{
//	printf("port read %x\n",address);

	switch(address)
	{
//		case 0xff:
//			return 1<<5;
		case 0x00:
		{
//			return 0xff;
			int pending;
			ioctl(0,FIONREAD,&pending);
			if(pending==0) return 0;
			else return 2;
		}
		case 0x01:
		{
//			int pending;
//			ioctl(0,FIONREAD,&pending);
//printf("%d ",pending); fflush(stdout);
//			if(pending==0)
//				return 0;
			char c=getchar();
			if(c==0x1c)
				DOEXIT=1;
			else if(c=='\\')
				return 0xd;
			else
				return c;
		}
	}

        return 0xff;
}

int tohex(char c)
{
	if (c>='0' && c<='9')
		return c-'0';
	if (c>='A' && c<='F')
		return c-'A'+0xa;
	return c-'a'+0xa;
}

void reset_termios()
{
	tcsetattr(0,TCSANOW,&termios_old);
}

int main()
{
	p8085=(P8085*)malloc(sizeof(P8085));
	p8085->memory=ram;
	reset8085(p8085);

//	FILE* f=fopen("4kbas.bin","r");
//	FILE* f=fopen("basic4k10.bin","r");
	FILE* f=fopen("8kbas.bin","r");
//	for(int i=0; i<0x1000; i++)
//	for(int i=0; i<4575; i++)
	for(int i=0; i<0x2000; i++)
		ram[i]=fgetc(f);
	fclose(f);

	struct termios tios=termios_old;
	tcgetattr(0,&termios_old);
	atexit(reset_termios);
	cfmakeraw(&tios);
	tcsetattr(0,TCSANOW,&tios);

/*	FILE *f=fopen("4kbas_2krom_0.hex","r");
	for(int i=0x8000; i<0x8800; i++)
		ram[i]=(tohex(fgetc(f))<<4)|tohex(fgetc(f));
	fclose(f);
	f=fopen("4kbas_2krom_1.hex","r");
	for(int i=0x8800; i<0x9000; i++)
		ram[i]=(tohex(fgetc(f))<<4)|tohex(fgetc(f));
	fclose(f);
	p8085->PC=0x8000;
*/
/*
	FILE *f=fopen("8kbas_2krom_0.hex","r");
	for(int i=0x8000; i<0x8800; i++)
		ram[i]=(tohex(fgetc(f))<<4)|tohex(fgetc(f));
	fclose(f);
	f=fopen("8kbas_2krom_1.hex","r");
	for(int i=0x8800; i<0x9000; i++)
		ram[i]=(tohex(fgetc(f))<<4)|tohex(fgetc(f));
	fclose(f);
	f=fopen("8kbas_2krom_2.hex","r");
	for(int i=0x9000; i<0x9800; i++)
		ram[i]=(tohex(fgetc(f))<<4)|tohex(fgetc(f));
	fclose(f);
	f=fopen("8kbas_2krom_3.hex","r");
	for(int i=0x9800; i<0xa000; i++)
		ram[i]=(tohex(fgetc(f))<<4)|tohex(fgetc(f));
	fclose(f);
	p8085->PC=0x8000;
*/
//	for(unsigned long cycles=0; cycles<4000000; cycles++)
	for(unsigned long cycles=0; ; cycles++)
	{
		if(DOEXIT) break;
//		ram[0x10000>>1]=0xff;
		doInstruction8085(p8085);
	}

	return 0;
}
