#pragma once
#if (defined(USING_MQTT))
#define ERROR_NONE					0x50		
#define ERROR_MQTT_POST_FAILED  	0x51
#define ERROR_WIFI_CONNECT_FAILED	0x52
#define ERROR_WIFI_CONNECT_SUCCESS  0x53
#define ERROR_MQTT_INIT_SUCCESS   0x54
#define ERROR_MQTT_INIT_FAILED   0x55
#define ERROR_MQTT_DATA_SEND_SUCCESS 0x56
#define ERROR_MQTT_DATA_SEND_FAILED 0x57
#define ERROR_MQTT_CLIENT_CONNECT_SUCCESS 0x58
#define ERROR_MQTT_CLIENT_CONNECT_FAILED 0x59

WiFiUDP ntpUDP;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 3600*7, 60000);

//const char mqttServerAddress[15] = "23.89.159.119";
const char mqttServerAddress[15] = "188.68.48.86";
const uint16_t mqttServerPort = 1883;

char topic[32];
char espID[10];
uint8_t MacAddress[6];


typedef enum 
{
    E_WIFI_DISCONNECT,
    E_WIFI_CONNECTED,
    E_WIFI_SCANNING
} WIFI_Status_et;

WIFI_Status_et TFT_wifiStatus = WIFI_Status_et::E_WIFI_DISCONNECT;		// bien trang thai ket noi WIFI


/**
 * @brief	ham de khoi tao ket noi mqtt
 * @param[in]	_topic - topic cua server tra ve
 * @param[in]	_espID - ID cua esp
 * @param[in]  	_mqttClient - bien luu tru ket noi
 * @return None
 */
uint16_t mqtt_initClient(char*			 _topic,
					 char*			 _espID,
					 PubSubClient& 	_mqttClient)
{
	uint8_t espMacAddress[6];				// mang luu dia chi MAC
	WiFi.macAddress(espMacAddress);			// lay dia chi MAC cua WIFI
	uint32_t macAddressDecimal = (espMacAddress[3] << 16) + (espMacAddress[4] << 8) + espMacAddress[5]; // bien luu dia chi MAC duoi dang decimal

	sprintf(_topic, "/AirSENSE v3_luxury/ESP32_%08d/", macAddressDecimal);
	sprintf(_espID, "%08d", macAddressDecimal);

	_mqttClient.setServer(mqttServerAddress, mqttServerPort);		// cai dat server voi dia chi ip va port
	_mqttClient.connect(_espID);									// ket noi 

	LOG_PRINT_INFORMATION(_topic)
	LOG_PRINT_INFORMATION(_espID);
	mqttClient.connect(espID);
	if(PubSubClient.connect()){
		#ifdef	DEBUG_SERIAL
			LOG_PRINT_NOTIFICATION("MQTT initialized successfully")
#endif
		
	return ERROR_MQTT_INIT_SUCCESS;
	}else{
		#ifdef	DEBUG_SERIAL
			LOG_PRINT_NOTIFICATION("MQTT is not initialized ")
#endif
	}
	return ERROR_MQTT_INIT_FAILED;
}

/**
 * @brief	ham de gui data len MQTT
 * @param[in]	humidity - gia tri do am
 * @param[in]	temperature - gia tri nhiet do
 * @param[in]	pm1_u32 - pm1.0
 * @param[in]	pm25_u32 - pm2.5
 * @param[in]	pm10_u32 - pm10
 * @param[in]	O3ppb_u32 - 03 don vi do ppb
 * @param[in]	O3ppm - 03 don vi do ppm
 * @param[in]	O3ug - 03 don vi do ug
 * @param[in]	pm25_min_u32 - nong do pm25 min
 * @param[in]	pm25_max_u32 - nong do pm25 max
 * @return  None
 */
uint32_t mqtt_postData ( float 		humidity,
					 float 		temperature,
					 uint32_t   pm1_u32,
					 uint32_t   pm25_u32,
					 uint32_t 	pm10_u32,
					 uint32_t 	O3ppb_u32,
					 float 		O3ppm,
					 float 		O3ug,
					 uint32_t 	pm25_min_u32,
					 uint32_t 	pm25_max_u32 )
{
	if (WiFi.status() == WL_CONNECTED)
	{	
#ifdef	DEBUG_SERIAL
			LOG_PRINT_NOTIFICATION("wifi is connected") 
#endif
		return ERROR_WIFI_CONNECT_SUCCESS;
		if (mqttClient.connected())
		{
#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("mqtt client connected successfully");
#endif
		return ERROR_MQTT_CLIENT_CONNECT_SUCCESS;		
	 
			timeClient.update();
			uint32_t epochTime_u32 = timeClient.getEpochTime();   	// lay thoi gian (tinh bang so giay)
			char message[256] = {0};

			// tao chuoi ki tu chua du lieu
			sprintf(message, 
					"{\"DATA\":{\"O3ppb\":%u,\"O3ppm\":%.3f,\"O3ug\":%.1f,\"Temperature\":%.1f,\"Humidity\":%.1f,\"Pm1\":%u,\"Pm25\":%u,\"Pm10\":%u,\"Pm25_min\":%u,\"Pm25_max\":%u,\"Time\":%u},\"NodeID\":\"%s\"}",
					O3ppb_u32,
					O3ppm,
					O3ug,
					temperature,
					humidity,
					pm1_u32,
					pm25_u32,
					pm10_u32,
					pm25_min_u32,
					pm25_max_u32,
					epochTime_u32,
					nameDevice );
			}else{
#ifdef	DEBUG_SERIAL
		LOG_PRINT_NOTIFICATION("mqtt client not connected");
#endif
		return ERROR_MQTT_CLIENT_CONNECT_FAILED;	
			}
			if (mqttClient.publish(topic, message, true))	// kiem tra co gui dulieu len MQTT thanh cong
			{
#ifdef	DEBUG_SERIAL

			LOG_PRINT_NOTIFICATION("gui du lieu len MQTT thanh cong") 
#endif
			}
			return ERROR_MQTT_DATA_SEND_SUCCESS;
			else{
#ifdef	DEBUG_SERIAL
			LOG_PRINT_NOTIFICATION("gui du lieu len MQTT that bai") 
#endif
		return ERROR_MQTT_DATA_SEND_FAILED;
			}


			mqttClient.loop();
		}
		
		else 
		{	
			mqttClient.connect(espID);
#ifdef	DEBUG_SERIAL
			LOG_PRINT_NONE("mqtt client reconnect");
			mqttClient.subscribe("huuhuong");
#endif
		}
	}
	else
	{
	LOG_PRINT_ERROR("Wifi is not connected");
	return ERROR_WIFI_CONNECT_FAILED
	}
}
#endif
