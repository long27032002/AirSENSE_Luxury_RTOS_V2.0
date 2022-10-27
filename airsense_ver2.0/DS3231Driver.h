#include "RTClib.h"
#include <ESP32Ping.h>
#include "log.h"

/* Wifi status */
enum WIFI_status {WIFI_IS_CONNECTED, WIFI_UNCONNECTED, WIFI_PING_CONNECTED};

const char* remote_host_string = "www.google.com";
RTC_DS3231 realTime;


/**
 * @brief	Khoi tao module thoi gian thuc
 *
 * @return  WIFI_PING_CONNECTED 2: Wifi da ket noi va ping duong dan "www.google.com"
 * @return  WIFI_IS_CONNECTED   1: Wifi da ket noi nhung ping duong dan khac "www.google.com"
 * @return  WIFI_UNCONNECTED    0: Wifi khong ket noi
 */
uint32_t DS3231_init()
{
	realTime.begin();									// khoi dong module RTC
	if (WiFi.status() == wl_status_t::WL_CONNECTED)		// kiem tra co ket noi wifi
	{
		if (Ping.ping(remote_host_string))				// kiem tra ping duong dan "www.google.com"
		{
			// cap nhat thoi gian cho RTC
			timeClient.update();				
			uint32_t epochTime_u32 = timeClient.getEpochTime();
			realTime.adjust(DateTime(epochTime_u32));			// Set the date and flip the Oscillator Stop Flag
			LOG_PRINT_NOTIFICATION("Updatetime DS3231....");
			return WIFI_PING_CONNECTED;							// Wifi da ket noi va ping duong dan "www.google.com"
		}
		return WIFI_IS_CONNECTED;								// Wifi da ket noi nhung ping duong dan khac "www.google.com"
	}
	return WIFI_UNCONNECTED;									// Wifi khong ket noi
	//LOG_PRINT_NONE(getDateTime("YYYY-MMM-DD hh:mm:ss"));
//#ifdef	DEBUG_SERIAL
//	LOG_PRINT_NOTIFICATION(getDateTime("YYYY-MMM-DD hh:mm:ss")); 		// in thoi gian hien tai ra Serial
//#endif
}


/**
 * @brief	luu tru thoi gian thuc vao bien TFT_time_string
 *
 * @return true 	neu lay duoc thoi gian hop le
 * @return false 	neu thoi gian khong hop le
 */
bool DS3231_getData()
{
	if (realTime.now().isValid())		// kiem tra thoi gian co hop le
	{
#ifdef  DEBUG_SERIAL
	LOG_PRINT_NOTIFICATION(getDateTime("hh:mm DD-MMM-YY"));		// in thoi gian hien tai ra Serial
#endif
		return true;
	} else {
#ifdef  DEBUG_SERIAL
	LOG_PRINT_ERROR("------ *** CAN'T GET TIME *** -----");
#endif
		return false;
	}
}
