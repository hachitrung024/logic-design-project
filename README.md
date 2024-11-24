B1: Thay đổi tần số hệ thống 48MHz
----------------------------------

B2: Ở mục Pinout & Configuration, chọn vào Timer -> cài đặt "TIM3"
------------------------------------------------------------------
  Clock Source : "Internal Clock"
  Channel1 : PWM Generation CH1
  
B3: Ở phần Parameter Setting (TIMER) thay đổi tham số
-----------------------------------------------------
  Prescaler = 20-1
  Counter = 3-1
  
B4: Ở mục DMA Setting (TIMER) chọn "add"
----------------------------------------
  Cấu hình DMA request "TIM3_CH1/TRIG"	và chuyển	Direction thành "Memory To Peripheral"
  
B5: Thêm hàm ở mục /* USER CODE BEGIN 4 */ ở main.c để xử lý với PWM
--------------------------------------------------------------------
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
    htim3.Instance->CCR1 = 0;
}

B7: Thêm trạng thái để chuyển đổi sang led ở global.h và global.c
-----------------------------------------------------------------
# global.h
typedef enum{
	INIT,
	IDLE,
	CHECK_CONNECTION,
	CHECK_READY,
    REQUEST_DATA,
    READ_DATA,
    CONVERT_DATA,
	SEND_DATA,
	ERROR_STATE,
 # UPDATE_LED, // Thêm trạng thái
} state;

# global.c
# Ở CASE SEND_DATA sẽ chuyển sang UPDATE_LED
case SEND_DATA:
	    if (isFlagTimer(UPDATE_TIMER)) {
	        setTimer(UPDATE_TIMER, UPDATE_CYCLE);
	        snprintf(lcd_buffer_1, 17, "Temp: %.2f %cC  ", dht20.temperature, 0xDF);
	        snprintf(lcd_buffer_2, 17, "Humi: %.2f %%   ", dht20.humidity);
	        lcd_send_buffer();

	        // Chuyen sang LED de dieu khien
	        # status = UPDATE_LED;
	    }
	    break;
# CASE UPDATE_LED (CASE mới)
	case UPDATE_LED:
	    // Dieu chinh LED 1 (nhiet do) và LED 2 (do am)
	    setting_led_RGB((int)dht20.temperature, (int)dht20.humidity);

	    status = CHECK_READY;
	    break;
     
B8: Cập nhật thêm file Led_RGB.h và Led_RGB.c và kết thúc việc cập nhật
---------------------------------------------
