typedef struct 
{
	int A, B, C, D, E, H, L, SP, PC;
	int c, p, ac, z, s, i, m75, m65, m55;
	int flags;
	int interrupt_deferred, interrupts;
	int halted;

	int opcode, value, immediate, cycles;
	char* name;
	unsigned int* memory;
} P8085;

void doInstruction8085(P8085*);
void reset8085(P8085*);
void throwInterrupt8085(P8085*,int);
void port_write(P8085* p8085, unsigned int address, unsigned char data);
unsigned int port_read(P8085* p8085, unsigned int address);
