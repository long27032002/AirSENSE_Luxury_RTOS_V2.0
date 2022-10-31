#pragma once
#ifndef _BUTTONDRIVER_H_
#define _BUTTONDRIVER_H_

#if(defined(PIN_BUTTON_1))

#include "config.h"

#define BUTTON_PRESS_DURATION	(uint32_t)4000 	// millisecond

#define ERROR_NONE				    		0x90
#define ERROR_BUTTON_LONGPRESS				0x91 	
#define ERROR_DFROBOT_SHORTPRESS 			0x92


/**
 * @brief  Kiem tra bam nut dai hay ngan
 * @param [out] lastPressButton thoi diem an nut gan nhat  
 * return true or false
*/
uint32_t Button_isLongPressed(uint32_t* lastPressButton)
{
	if (millis() - *lastPressButton > BUTTON_PRESS_DURATION && digitalRead(PIN_BUTTON_1) == 0) 			// kiem tra  thoi gian bam nut
	{
		return ERROR_BUTTON_LONGPRESS;
	}
	else if (digitalRead(PIN_BUTTON_1) == 1)
	{
		*lastPressButton = millis();				// lấy giời gian nếu bút nhấn vẫn được nhấn
	}
	return ERROR_DFROBOT_SHORTPRESS;
}


#endif
#endif