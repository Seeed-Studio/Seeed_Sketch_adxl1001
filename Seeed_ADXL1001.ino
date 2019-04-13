/*
 *  
 * Copyright (c) 2019 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : downey
 * Create Time: Jan 2018
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
  #define SYS_VOL    		3.3
  #define ADC_RESOLUTION    12
#else
  #define SERIAL Serial
  #define SYS_VOL   		5
  #define ADC_RESOLUTION    10
#endif

float cali_data;
int16_t scale;

#define MODUEL_RANGE           20

#define CALI_BUF_LEN           15
#define CALI_INTERVAL_TIME     250


float cali_buf[CALI_BUF_LEN];

#define INPUT_PIN              A0

float deal_cali_buf(float *buf)
{
	float cali_val = 0;
	
	for(int i = 0;i < CALI_BUF_LEN;i++)
	{
		cali_val += buf[i];
	}
	cali_val = cali_val/CALI_BUF_LEN;
	return (float)cali_val;
}


void calibration(void)
{
	SERIAL.println("Please Place the module horizontally!");
	delay(1000);
	SERIAL.println("Start calibration........");
	
	for(int i=0;i<CALI_BUF_LEN;i++)
	{
		cali_buf[i] = analogRead(INPUT_PIN);

		cali_buf[i] = cali_buf[i] * 3 / 2; 
		
		delay(CALI_INTERVAL_TIME);
	}
	cali_data =  deal_cali_buf(cali_buf);
	SERIAL.println("Calibration OK!!");
	scale = (float)1000.0 * SYS_VOL * 1000 / (20 * 1024);
	SERIAL.println(cali_data);
	SERIAL.println(scale);
	
}



void AccMeasurement(void)
{
	int16_t val = 0;
	val = analogRead(INPUT_PIN);
	val = val * 3 / 2; 
	
	SERIAL.print("Raw data   = ");
	SERIAL.println(val);
	
	SERIAL.println(" ");
	
	SERIAL.print("single axis acc is ");
	SERIAL.print((val - cali_data) * scale / 1000.0);
	SERIAL.println(" g ");
	SERIAL.println(" ");
	SERIAL.println(" ");
	SERIAL.println(" ");
	delay(1000);
}

void setup()
{
	SERIAL.begin(115200);
	calibration();
	SERIAL.print("Scale = ");
	SERIAL.println(scale);
	
}



void loop()
{
	//int val = analogRead(A0);
	//SERIAL.print("val = ");
	//SERIAL.println(val);
	//delay(500);
	AccMeasurement();
}