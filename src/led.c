#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include "led.h"

int mem_fd;
void *gpio_map;
volatile unsigned *gpio;

void INP_GPIO(char g)
{
	volatile unsigned *CR = (gpio+((g)/10));
	*CR &= ~(7<<(((g)%10)*3));
}

void OUT_GPIO(char g)
{
        volatile unsigned *CR = (gpio+((g)/10));
        *CR |= (1<<(((g)%10)*3));
}


void GPIO_SET(char g)
{
	volatile unsigned *CR = (gpio+7);
	*CR |= 1 << g;
}

void GPIO_CLR(char g)
{
        volatile unsigned *CR = (gpio+10);
	*CR |= 1 << g;
}


void setup_io()
{
	if((mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
	{
		printf("can't open /dev/mem \n");
		exit(-1);
	}

	gpio_map = mmap(
		NULL,
		BLOCK_SIZE,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		mem_fd,
		GPIO_BASE
	);

	close(mem_fd);

	if(gpio_map == MAP_FAILED)
	{
		printf("mmap error %d\n", (uintptr_t)gpio_map);
		exit(-1);
	}

	gpio = (volatile unsigned *)gpio_map;


   	OUT_GPIO(PIN); 
    	GPIO_SET(PIN);  
}


