# Hướng dẫn cấu hình và cập nhật hệ thống

## B1: Thay đổi tần số hệ thống
Cấu hình tần số hệ thống: **48MHz**

---

## B2: Pinout & Configuration
1. Vào mục **Timer** -> chọn **TIM3**:
   - Clock Source: `Internal Clock`
   - Channel1: `PWM Generation CH1`

---

## B3: Parameter Setting (TIMER)
Cài đặt các tham số:
- Prescaler: `20-1`
- Counter Period: `3-1`

---

## B4: DMA Setting (TIMER)
1. Thêm DMA Request:
   - Request: `TIM3_CH1/TRIG`
   - Direction: `Memory To Peripheral`

---

## Bước 5: Thêm hàm xử lý PWM trong main.c

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

## Bước 6: Thêm trạng thái ở global
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
`globlal.c``global_fsm()`
```c
	case UPDATE_LED:
	    // Dieu chinh LED 1 (nhiet do) và LED 2 (do am)
	    setting_led_RGB((int)dht20.temperature, (int)dht20.humidity);

	    status = CHECK_READY;
	    break;
```
## Bước 7: Thêm mã nguồn led_RGB.h và led_RGB.c để kết thúc cập nhật.

