#ifndef LED_H
#define LED_H

#define GPIO_BASE              0x3F200000
#define PIN 4

#define PAGE_SIZE    (4*1024)
#define BLOCK_SIZE   (4*1024)

void INP_GPIO(char g);
void OUT_GPIO(char g);
void GPIO_SET(char g);
void GPIO_CLR(char g);
void setup_io();

#endif

