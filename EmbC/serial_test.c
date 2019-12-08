#include "serial.h"
#include "bbb.h"

int gi = 0;
int gj = 5;
int gk;
int c_entry(void)
{
	char str[32];

	serial_init(115200);

	serial_byte_rx();
	serial_tx("Welcome to SysPlay\r\n");

	for (;;)
	{
		serial_tx("Cmd> ");
		serial_rx(str, 32);
		serial_tx("You entered: ");
		serial_tx(str);
		serial_tx("\r\n");
		
//void debug_init(void);
//void debug_shut(void);
//void print_nl(void);
//void print_str(char *str);
//void print_str_nl(char *str);
	       print_num(123456789);
	       print_nl();
	       serial_tx("device id: ");
               print_hex(_REG(DEVICE_ID,0));
	       print_nl();
	       serial_tx("gi: ");
	       print_hex(gi);
	       print_nl();
	       serial_tx("gj: ");
	       print_num(gj);
	       print_nl();
	       serial_tx("gk: ");
	       print_num(gk);
//#define  DEVICE_ID 0x44e10600

//#define _REG_ADDR(base, off) ((volatile uint32_t *)((base) + (off)))
//#define _REG(base, off) (*_REG_ADDR(base, off))

	       
	}

	serial_shut();

	return 0;
}
