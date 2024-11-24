# Hướng dẫn cấu hình và cập nhật hệ thống

## Bước 1: Thay đổi tần số hệ thống
Cấu hình tần số hệ thống: **48MHz**

---

## Bước 2: Thay đổi timer interupt ở TIM2
Cài đặt các tham số để đặt **timer = 10ms**:
- Prescaler: `4799`
- Counter Period: `99`
  
---
## Bước 3: Pinout & Configuration
1. Vào mục **Timer** -> chọn **TIM3**:
   - Clock Source: `Internal Clock`
   - Channel1: `PWM Generation CH1`

---

## Bước 4: Parameter Setting (TIMER)
Cài đặt các tham số:
- Prescaler: `20-1`
- Counter Period: `3-1`

---

## Bước 5: DMA Setting (TIMER)
1. Thêm DMA Request:
   - Request: `TIM3_CH1/TRIG`
   - Direction: `Memory To Peripheral`

---

## Bước 6: Thêm hàm xử lý PWM trong main.c

Thêm đoạn mã sau vào phần `/* USER CODE BEGIN 4 */` trong file `main.c`:

```c
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
        htim3.Instance->CCR1 = 0;
    }
}
```

## Bước 7: Thêm trạng thái ở global
Thêm trạng thái `UPDATE_LED`

`global.h`
```c
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
	UPDATE_LED, // Thêm trạng thái
} state;
```
`globlal.c`
`global_fsm()`
```c
	case UPDATE_LED:
	    // Dieu chinh LED 1 (nhiet do) và LED 2 (do am)
	    setting_led_RGB((int)dht20.temperature, (int)dht20.humidity);

	    status = CHECK_READY;
	    break;
```
## Bước 8: Thêm mã nguồn led_RGB.h và led_RGB.c để kết thúc cập nhật.

