#pragma once

#if(defined(PIN_TX_TFLP01) && defined(PIN_RX_TFLP01))

#include "config.h"
#define ERROR_SERIAL12_CONNECT_SUCCESS 	0x20
#define ERROR_SERIAL12_CONNECT_FAILED	0x21
#define ERROR_TFP01_INIT_SUCCESS		0x22
#define ERROR_TFLP01_INIT_FAILED		0x23
#define baudRate = 115200				

// uint32_t     TFLP01_pm10sum_u32		= 0;
// uint32_t     TFLP01_pm25sum_u32 	= 0;
// uint32_t     TFLP01_pm1sum_u32 		= 0;
// uint32_t	 dataTFLP01count_u32 	= 0;
bool 	     TFLP01_readStatus_bl  	= false;

uint32_t	 pm25_min_u32		   	= 1000;
uint32_t	 pm25_max_u32		   	= 0;
float 	 TFT_temperature_C 		= 0;
float 	 TFT_temperature_F		= 0;
float 	 TFT_humidity_percent 	= 0;

/**
 * @brief  Return CRC trong data response của TF-LP-01
 * @param[in] modbusdata // chua ro
 * @param[in] Length
 * @return  None
 */
uint16_t crc16_modbus(uint8_t *modbusdata , uint16_t Length) 
{
	uint16_t crc16 = 0xFFFF;
	for (uint16_t i = 0; i < Length; i++)
	{
		crc16 ^= modbusdata[i];
		for (uint16_t j = 0; j < 8; j++)
		{
			if ((crc16 & 0x01) == 1) crc16 = (crc16 >> 1) ^ 0xA001; 
			else  crc16 >>= 1;
		}
	}
	return crc16;
}


/**
 * @brief Setup TF-LP-01
 *
 * @return  none
 */
uint32_t TFLP01_init(baudRate)
{
	Serial2.begin();		// khoi dong Serial2
	
	if (Serial2.available() > 0) 
	{  
		#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("khoi dong Serial2 thanh cong");
		#endif

		return ERROR_SHT_INIT_SUCCESS;		// trang thai khoi tao sensor
	} else 
	{
#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("khoi tao Srial12 that bai\n");
#endif
		return ERROR_TFLP01_INIT_FAILED;			// trang thai khoi tao sensor
	}
}


/**
 * @brief Doc du lieu tu cam bien bui
 * 
 *@param[out] TFT_pm1_u32
	@param[out] TFT_pm25_u32
	@param[out] TFT_pm10_u32
 * @return  None
 */
uint32_t TFLP01_getData(uint32_t* TFT_pm1_u32,
						uint32_t*  TFT_pm25_u32,
						uint32_t*  TFT_pm10_u32)
{	

	uint8_t TFLP01_data_a8[17] = {0};
	if (Serial2.available() > 0) 
	{  
		LOG_PRINT_NOTIFICATION("khoi dong Serial2 thanh cong");
		LOG_PRINT_NOTIFICATION("Check: ");
		for(uint8_t i=0; i<17; i++)
		{
			LOG_PRINT_INFORMATION(i) // in ra index trong mang luu du lieu TFLP01_data_a8
			Serial.print(": "); 
			byte TFLP01_readData_byte = Serial2.read();			// doc tung byte du lieu tu TFLP01 qua Serial2
			TFLP01_data_a8[i] = (uint8_t)TFLP01_readData_byte;	// luu du lieu vua doc duoc vao mang du lieu cua TFLP01
			LOG_PRINT_INFORMATION("byte data of TFL01: %c", TFLP01_readData_byte); // in ra gia tri cua du lieu trong mang TFLP01_data_a8
		}
		
		TFLP01_readStatus_bl = true;
		#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("Serial2 is available");
#endif
		return ERROR_SERIAL12_CONNECT_SUCCESS;		
	} 
	} else {
		LOG_PRINT_ERROR("Serial2 not available") 
		return ERROR_SERIAL12_CONNECT_FAILED;
	}

	// lay du lieu tam thoi (chua co datasheet)
	if(TFLP01_readStatus_bl == true)
	{
		TFLP01_readStatus_bl = false;
		*TFT_pm1_u32  = TFLP01_data_a8[9]  + TFLP01_data_a8[10]*256 + pm1_calibInt_u32; // tinh gia tri bui pm1
		*TFT_pm25_u32 = TFLP01_data_a8[11] + TFLP01_data_a8[12]*256 + pm25_calibInt_u32;// tinh gia tri bui pm25
		*TFT_pm10_u32 = TFLP01_data_a8[13] + TFLP01_data_a8[14]*256 + pm10_calibInt_u32;// tinh gia tri bui pm10

		if(*TFT_pm25_u32 != 255)
		{
			if(pm25_max_u32 < *TFT_pm25_u32) pm25_max_u32 = *TFT_pm25_u32;
			if(pm25_min_u32 > *TFT_pm25_u32) pm25_min_u32 = *TFT_pm25_u32;
		} else{
			LOG_PRINT_NOTIFICATION("gia tri pm24_u32 dat gia tri max la 255") 
	
		}

#ifdef  DEBUG_SERIAL
		// in du lieu ra man hinh Serial
		for(uint8_t i=0; i < 17; i++)
		{
			LOG_PRINT_INFORMATION(i) // in ra index trong mang luu du lieu TFLP01_data_a8
			Serial.print(": ");
			LOG_PRINT_INFORMATION(TFLP01_data_a8[i])	
		}
		LOG_PRINT_INFORMATION("Pm2.5: " + String(TFT_pm25_u32) + "    Pm10: " + String(TFT_pm10_u32))
#endif

	}  
	
