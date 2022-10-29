#ifdef O3_SENSOR_DF_ROBOT
#include "DFRobot_OzoneSensor.h"


#define O3_GETDATA_PERIOD       (uint32_t)5000
#define COLLECT_NUMBER          20              // collect number, the collection range is 1-100
#define Ozone_IICAddress        ADDRESS_3
/*   iic slave Address, The default is ADDRESS_3
	ADDRESS_0               0x70      // iic device address
	ADDRESS_1               0x71
	ADDRESS_2               0x72
	ADDRESS_3               0x73
*/
DFRobot_OzoneSensor Ozone;
#define ERROR_NONE				0x20
#define ERROR_DFROBOT_INIT_SUCCESS	ERROR_NONE 	
#define ERROR_DFROBOT_INIT_FAILED  	0x21

/**
 * @brief	Khoi tao module Ozone
 * @param[in] Ozone_IIAddress : Khoi tao dia chi Address
 * @return  Success or Failed
 */
uint32_t O3_init(Ozone_IIAddress)
{
	if(!Ozone.begin(Ozone_IICAddress)) 
	{
#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("I2c device number error !");
#endif
		return ERROR_DFROBOT_INIT_FAILED;
	}  else {
#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("I2c connect success !");
#endif
		return ERROR_DFROBOT_INIT_SUCCESS;
	}

	Ozone.SetModes(MEASURE_MODE_PASSIVE);
}


/**
 * @brief	Luu tru gia tri vao bien TFF_o3_ppm, TFT_o3_ug
 *@param [in] min_o3_ppb gia tri nho nhat cua O3
 *@param [in] max_o3_ppb gia tri lon nhat cua O3
 *@param [out] min_o3_ug gia tri nho nhat lay ra
 *@param [out] max_o3_ug gia tri lon nhat lay ra
 * @return  None
 */
uint32_t O3_getData(uint32_t min_o3_ppb,
					uint32_t max_o3_ppb,
					uint32_t* min_o3_ug,
					uint32_t* max_o3_ug )
{
		TFT_o3_ppb_u32 = Ozone.ReadOzoneData(COLLECT_NUMBER);
		if(min_o3_ppb > TFT_o3_ppb_u32) min_o3_ppb_u32 = TFT_o3_ppb;
		if(max_o3_ppb < TFT_o3_ppb_u32) max_o3_ppb = TFT_o3_ppb;
		TFT_o3_ppm = float(TFT_o3_ppb)/1000;
		TFT_o3_ug  = float(TFT_o3_ppb)*1.96;
		min_o3_ug  = float(min_o3_ppb)*1.96;
		min_o3_ppm = float(min_o3_ppb)/1000;
		max_o3_ug  = float(max_o3_ppb)*1.96;
		max_o3_ppm = float(max_o3_ppb)/1000;
		LOG_PRINT_NOTIFICATION("Get data success");
		return ERROR_NONE;
	
}
#endif
