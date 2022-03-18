#ifndef __DHT11_H
#define __DHT11_H

#include "reg52.h"

sbit Data = P1^7;

void DHT11_start();
unsigned char DHT11_rec_byte();
void DHT11_receive();

#endif
