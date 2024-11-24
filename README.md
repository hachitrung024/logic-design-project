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

## B5: Thêm hàm xử lý PWM trong main.c
Chèn đoạn mã sau vào mục `/* USER CODE BEGIN 4 */`:

```c
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
    htim3.Instance->CCR1 = 0;
}

## B6: Thêm trạng thái trong global.h và global.c
1. **global.h**:
   ```c
   typedef enum {
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

