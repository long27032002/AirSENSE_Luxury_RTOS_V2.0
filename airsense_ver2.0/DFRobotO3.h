#ifdef O3_SENSOR_DF_ROBOT
#include "DFRobot_OzoneSensor.h"


#define O3_GETDATA_PERIOD       (uint32_t)5000

#define Ozone_IICAddress        OZONE_ADDRESS_3
/*   iic slave Address, The default is ADDRESS_3
	OZONE_ADDRESS_0               0x70      // iic device address
	OZONE_ADDRESS_1               0x71
	OZONE_ADDRESS_2               0x72
	OZONE_ADDRESS_3               0x73
*/
DFRobot_OzoneSensor Ozone;
#define ERROR_NONE				    	0x80
#define ERROR_DFROBOT_INIT_SUCCESS		ERROR_NONE 	
#define ERROR_DFROBOT_INIT_FAILED  		0x81
#define ERROR_DFROBOT_GETDATA_SUCCESS 	ERROR_NONE
#define ERROR_DFROBOT_GETDATA_FAILED 	0x82

/**
 * @brief	Khoi tao module Ozone
 * @param[in] Ozone_IIAddress :  Dia chi OZONE_ADDRESS_3   0x73
 * @return  Success or Failed
 */
uint32_t O3_init(uint32_t Ozone_IIAddress)
{
	if(!Ozone.begin(Ozone_IICAddress)) 
	{
#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("I2c device number error !");
#endif
		Ozone.setModes(MEASURE_MODE_PASSIVE);
		return ERROR_DFROBOT_INIT_FAILED;
	}  else {
#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("I2c connect success !");
#endif
		Ozone.setModes(MEASURE_MODE_PASSIVE);
		return ERROR_DFROBOT_INIT_SUCCESS;
	}
}


/**
 *@brief							Luu tru gia tri vao bien TFF_o3_ppm, TFT_o3_ug
 *@param [in] 	COLLECT_NUMBER 		so lan do
 *
 *@param [out] o3_ppb_min_u32 		gia tri nho nhat lay ra theo don vi ppb
 *@param [out] o3_ug_min    		gia tri nho nhat lay ra theo don vi ug
 *@param [out] o3_ppm_min   		gia tri nho nhat lay ra theo don vi ppm
 *@param [out] o3_ppb_max_u32		gia tri lon nhat lay ra theo don vi ppb
 *@param [out] 03_ug_max    		gia tri lon nhat lay ra theo don vi ug
 *@param [out] 03_ppm_max   		gia tri lon nhat lay ra theo don vi ppm
 * @return  ERROR_NONE or ERROR_DFROBOT_GETDATA_FAILE
 */
uint32_t O3_getData(uint32_t 	COLLECT_NUMBER,
					uint32_t* 	o3_ppb_min_u32,
					float* 		o3_ug_min,
					float* 		o3_ppm_min,
					uint32_t* 	o3_ppb_max_u32,
					float* 		o3_ug_max,
					float* 		o3_ppm_max )
{
	uint32_t o3_newValue_ppb_u32 = Ozone.readOzoneData(COLLECT_NUMBER);
	if( o3_newValue_ppb_u32 != -1)
	{
		if(*o3_ppb_min_u32 > o3_newValue_ppb_u32) *o3_ppb_min_u32 = o3_newValue_ppb_u32;
		if(*o3_ppb_max_u32 < o3_newValue_ppb_u32) *o3_ppb_max_u32 = o3_newValue_ppb_u32;
		*o3_ppm 	= float(o3_newValue_ppb_u32)/1000;
		*o3_ug  	= float(o3_newValue_ppb_u32)*1.96;
		*o3_ug_min  = float(*o3_ppb_min_u32)*1.96;
		*o3_ppm_min = float(*o3_ppb_min_u32)/1000;
		*o3_ug_max  = float(*o3_ppb_max_u32)*1.96;
		*o3_ppm_max = float(*o3_ppb_max_u32)/1000;
		LOG_PRINT_NOTIFICATION("Get data success !");
		return ERROR_NONE;
	}
	else
	{
		LOG_PRINTF_NOTIFICATION("Get data fail !");
		return ERROR_DFROBOT_GETDATA_FAILED;
	}
}
#endif
