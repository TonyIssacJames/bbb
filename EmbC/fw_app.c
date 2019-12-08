/*
 * Command Requirements of the firmware:
 * + l 0 - Switch off LED 0
 * + l 1 - Switch on LED 0
 * + b <msecs> - Blink LED 1 at an interval of <msecs> milliseconds
 * + b 0 - Disable the blink of LED 1
 * + r <addr> <cnt> - Read <acnt> bytes from EEPROM starting at address <addr>
 */
#include "debug.h"
#include "leds.h"
#include "timer.h"
#include "eeprom.h"
#include "interrupt.h"
#include "bbb.h"
#include "debug.h"
#include "common.h"
#include "debug.h"
#include "eeprom.h"


void handler(void)
{
	TIMER0_IRQSTATUS = (1 << 1);
	leds_toggle(3);
}

static void init_shell(void)
{
	debug_init();

	leds_init();
	eeprom_init();
	
	interrupt_init();
	timer_handler_register(handler);
	  
	scan_char();
}
static void shut_shell(void)
{
	eeprom_shut();
	leds_shut();

	debug_shut();
	
        timer_handler_unregister();
	interrupt_shut();
}





static void command_l(int off)
{
  if(!(off))
  {
    leds_off(0);
  }
  else
  {
    leds_on(0);    
  }
  
}

static void command_b(int msec)
{
	int i;
	if(msec != 0)
	{
	  timer_init(msec);

	}
	else
	{
	  leds_off(3);
	  timer_shut();
	}
  
}


static void command_r(uint8_t *addr, int cnt)
{
        int i;
  	union
	{
		uint8_t c[4];
		uint32_t i;
	} u;

	
	
	scan_char();
	print_str_nl("Welcome to SysPlay");

	eeprom_init();

	print_str("Press any key: ");
	scan_char();
	print_nl();
	print_str("EEPROM Content: ");
	
	for(i = 0; i< (cnt >> 2); i++)
	{
	  if (eeprom_read(addr, u.c, sizeof(u)) != 0)
	  {
		  print_str_nl("Read Error");
	  }
	  else
	  {
		  print_hex(u.i);
		  print_nl();
		  addr += 4;
	  }
	}

	print_nl();
	print_str("EEPROM Reading Done: ");
	eeprom_shut();
	//debug_shut();
  
}
int atoi(char *str, int *next)
{
	int i = 0, num = 0;

	while ((str[i]) && ((str[i] == ' ') || (str[i] == '\t')))
		i++;

	if (!str[i])
	{
		if (next)
			*next = i;
		return num;
	}

	while ((str[i] >= '0') && (str[i] <= '9'))
	{
		// TODO: Fill in your code for atoi implementation
	        num *= 10;
		num += (str[i] - '0');

		i++;
	}

	if (next)
		*next = i;

	return num;
}

static void loop_forever(void)
{
	char cmd[128];
	int st, cnt, next;
	uint32_t msecs; 
	uint8_t *addr;

	print_str("$ ");
	scan_line(cmd, 128);

	switch (cmd[0])
	{
		case 0: // Ignore empty string
			break;
		case 'l':
			st = atoi(cmd + 1, NULL);
			// TODO: Fill in your code for LED implementation
			print_str(" :");
			print_num(st);
			command_l(st);
			print_nl();
			break;
		case 'b':
			msecs = atoi(cmd + 1, NULL);
			// TODO: Fill in your code for timer implementation
			print_str(" :");
			print_num(msecs);
			command_b(msecs);
			print_nl();
			break;
		case 'r':
			addr = (uint8_t *)(atoi(cmd + 1, &next));
			cnt = atoi(cmd + 1 + next, NULL);
			// TODO: Fill in your code for EEPROM implementation
			print_str(" :");
			print_hex(addr);
			print_str(" :");
			print_num(cnt);	
			print_str("start reading: ");
			command_r(addr, cnt);
			print_nl();
			break;
		default:
			print_str("Invalid cmd: ");
			print_str(cmd);
			print_nl();
	}
}

int c_entry(void)
{
	init_shell();

	for (;;)
		loop_forever();

	shut_shell();

	return 0;
}
