#include <Arduino_FreeRTOS.h>

const int	led_red = 11;
const int	led_blue = 3;

TaskHandle_t	TaskSerial_Handler;

class	LEDController {
public:
	LEDController(int pin) : led_pin(pin), isOn(false), brightness(0), fadeTaskHandle(NULL){
		pinMode(led_pin, OUTPUT);
	}

	void	switching(void) {
		isOn = !isOn;
		if (isOn) {
			if (fadeTaskHandle != NULL) {
				vTaskDelete(fadeTaskHandle);
			}
			xTaskCreate(fadeInTask, "FadeInTask", 128, (void *)this, 1, &fadeTaskHandle);
			Serial.println("LED ON");
		} else {
			if (fadeTaskHandle != NULL) {
				vTaskDelete(fadeTaskHandle);
			}
			xTaskCreate(fadeOutTask, "FadeOutTask", 128, (void *)this, 1, &fadeTaskHandle);
			Serial.println("LED OFF");
		}
	}

	bool	getStatus(void) const {
		return isOn;
	}

private:
	int		led_pin;
	int		brightness;
	bool	isOn;
	TaskHandle_t	fadeTaskHandle

	static void	fadeInTask(void *param) {
		LEDController	*led = (LEDController *)param;
		for (brightness <= 255; ++brightness) {
			analogWrite(led->led_pin, brightness);
			vTaskDelay(50 / portTICK_PERIOD_MS);
		}
		vTaskDelete(NULL);
	}

	static void	fadeOutTask(void *param) {
		LEDController	*led = (LEDController *)param;
		for (brightness >= 0; --brightness) {
				analogWrite(led->led_pin, brightness);
				vTaskDelay(20 / portTICK_PERIOD_MS);
		}
		vTaskDelete(NULL);
	}
};

LEDController	redLED(led_red);
LEDController	blueLED(led_blue);

void setup() {
	Serial.begin(9600);
	Serial.println("*******\nSystem start");

	xTaskCreate(TaskSerial, "TaskSerial", 128, NULL, 1, &TaskSerial_Handler);
	vTaskStartScheduler();
}

void loop() {
}

void	TaskSerial(void *) {
	while (1) {
		while (Serial.available()) {
			switch(Serial.read()) {
				case 'r':
					redLED.switching();
					Serial.println(redLED.getStatus() ? "RED LED ON" : "RED LED OFF");
					break;
				case 'b':
					blueLED.switching();
					Serial.println(blueLED.getStatus() ? "BLUE LED ON" : "BLUE LED OFF");
					break;
			}
		}
		vTaskDelay(1);
	}
}
