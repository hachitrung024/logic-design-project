	/*
	 * dht20.h
	 *
	 *  Created on: Oct 12, 2024
	 *      Author: thuyh
	 */
	#ifndef INS_DHT20_H_
	#define INS_DHT20_H_
	#include "main.h"

typedef enum {
	DHT20_OK                   , //Trạng thái không có lỗi, mọi thứ hoạt động tốt
    DHT20_ERROR_CHECKSUM       , // Lỗi khi kiểm tra tổng kiểm (checksum) không khớp.
    DHT20_ERROR_CONNECT        , // Lỗi khi không thể kết nối với cảm biến.
    DHT20_MISSING_BYTES        , // Lỗi do thiếu dữ liệu từ cảm biến.
    DHT20_ERROR_BYTES_ALL_ZERO , // Lỗi khi tất cả các byte dữ liệu đọc từ cảm biến đều bằng 0.
    DHT20_ERROR_READ_TIMEOUT   , // Lỗi khi quá trình đọc dữ liệu bị quá thời gian quy định.
    DHT20_ERROR_LASTREAD         // Lỗi liên quan đến lần đọc cuối cùng từ cảm biến.
} status_active;

typedef struct {
		I2C_HandleTypeDef *hi2c;  // Giao tiếp I2C được sử dụng
		float humidity; // Độ ẩm
		float temperature; // Nhiệt độ
		float humOffset; // Hiệu chỉnh độ ẩm
		float tempOffset;// Hiệu chỉnh nhiệt độ
		uint8_t status; // Trạng thái của cảm biến
		uint32_t lastRequest; // Thời gian (đếm theo ms) của lần yêu cầu đo cuối cùng từ cảm biến.
		uint32_t lastRead; //Thời gian (đếm theo ms) của lần đọc dữ liệu cuối cùng từ cảm biến.
		uint8_t bytes[7];// Mảng nhận dữ liệu
	} DHT20_t;

extern DHT20_t dht20;
extern status_active active ;
	// Hàm khởi tạo cảm biến DHT20 với giao tiếp I2C
	void DHT20_Init(DHT20_t *dht20, I2C_HandleTypeDef *hi2c);
	// Kiểm tra xem cảm biến DHT20 có được kết nối hay không. Trả về true nếu có kết nối, false nếu không.
	uint8_t DHT20_IsConnected(DHT20_t *dht20);

	// Gửi yêu cầu thu thập dữ liệu từ cảm biến. Trả về 0 nếu thành công hoặc mã lỗi nếu không.
	status_active DHT20_RequestData(DHT20_t *dht20);

	// Đọc dữ liệu thô từ cảm biến sau khi yêu cầu đọc được gửi. Trả về 0 nếu thành công hoặc mã lỗi nếu không.
	status_active DHT20_ReadData(DHT20_t *dht20);

	// Chuyển đổi dữ liệu thô nhận được từ cảm biến thành dữ liệu có ý nghĩa (nhiệt độ và độ ẩm). Trả về 0 nếu thành công hoặc mã lỗi nếu không.
	status_active DHT20_Convert(DHT20_t *dht20);

	// Gọi hàm để đọc và chuyển đổi dữ liệu từ cảm biến. Trả về 0 nếu thành công hoặc mã lỗi nếu không.
	status_active DHT20_Read(DHT20_t *dht20);

	// Đặt giá trị bù độ ẩm cho cảm biến (offset). Giá trị này sẽ được cộng thêm vào khi tính toán độ ẩm.
	void DHT20_SetHumOffset(DHT20_t *dht20, float offset);

	// Đặt giá trị bù nhiệt độ cho cảm biến (offset). Giá trị này sẽ được cộng thêm vào khi tính toán nhiệt độ.
	void DHT20_SetTempOffset(DHT20_t *dht20, float offset);

	// Hàm đọc trạng thái hiện tại của cảm biến DHT20. Trả về mã trạng thái hoặc mã lỗi nếu có.
	uint8_t DHT20_ReadStatus(DHT20_t *dht20);

	// Kiểm tra xem cảm biến DHT20 có được hiệu chuẩn (calibrated) hay không. Trả về true nếu có, false nếu không.
	uint8_t DHT20_IsCalibrated(DHT20_t *dht20);

	// Kiểm tra xem cảm biến DHT20 có đang đo đạc dữ liệu hay không. Trả về true nếu có, false nếu không.
	uint8_t DHT20_IsMeasuring(DHT20_t *dht20);

	// Trả về trạng thái nội bộ của cảm biến DHT20 (trạng thái cuối cùng sau lần đọc gần nhất).
	uint8_t DHT20_InternalStatus(DHT20_t *dht20);

	// Trả về thời gian (tính bằng mili giây) kể từ lần đọc dữ liệu cuối cùng từ cảm biến.
	uint32_t DHT20_LastRead(DHT20_t *dht20);

	// Trả về thời gian (tính bằng mili giây) kể từ lần yêu cầu đo dữ liệu cuối cùng từ cảm biến.
	uint32_t DHT20_LastRequest(DHT20_t *dht20);

	// Đặt lại cảm biến DHT20. Trả về số lượng thanh ghi được đặt lại hoặc mã lỗi nếu có.
	uint8_t DHT20_ResetSensor(DHT20_t *dht20);

	// Hàm tính toán CRC để kiểm tra lỗi dữ liệu từ cảm biến.
	uint8_t DHT20_CRC8(uint8_t *ptr, uint8_t len);

	// Hàm đặt lại một thanh ghi cụ thể của cảm biến, trả về trạng thái sau khi thực hiện. Trả về 1 nếu thành công, 0 nếu không.
	uint8_t DHT20_ResetRegister(DHT20_t *dht20, uint8_t reg);
#endif  /* INS_DHT20_H_ */
