#include <MsTimer2.h>
#include <avr/wdt.h>

const int	light_in_pin = 0;
const int	led_pin = 11;

int				light_value = 0;
int				elapsed_time = 0;
unsigned long	start_time = 0;
bool			led_on = false;
bool			timer_start = false;
bool			dark_when_timer_start = false;
bool			now_dark = false;


void setup() {
	Serial.begin(9800);
	Serial.println("system starts");

	if (bitRead(MCUSR, WDRF)) {
		Serial.println("system reset");
	}

	wdt_enable(WDTO_2S);
	Serial.println("Watch Dog starts running...");

	MsTimer2::set(100, checkSensor);
	MsTimer2::start();

}

void loop() {
	errorOccurs();

	if (timer_start) {
		elapsed_time = millis() - start_time;
		if (now_dark != dark_when_timer_start) {
			timer_start = false;
			Serial.println("timer stop");
		}
		if (elapsed_time >= 3000) {
			if (!led_on) {
				light_led(1);
				led_on = true;
			} else {
				light_led(0);
				led_on = false;
			}
			timer_start = false;
		}
	}
	if (millis() >= (unsigned long)0-1 - 10000) {
		// 固定値でやってることが多いと思う（一週間とか一か月とか）
		// メンテのやつ
		// システムの稼働時間だけ変更できる？→多分できるしそういう方法取ってるのかも
		// 固定長でやってると思う
//	if (millis() >= 10000) {
		Serial.println("System uptime is too long. Perform a reset.");
		delay(10000);
	}
	wdt_reset();
	delay(100);
}

void	errorOccurs() {
	if (Serial.available()) {
		char input = Serial.read();
		if (input == '\n' || input == '\r') {
			return;
		}
		if (input == 'e') {
			Serial.println("An error occuers! System stopped!");
			delay(10000);
			Serial.println("System resumed!");
		} else {
			return;
		}
	}
}

void	checkSensor() {
	light_value = analogRead(light_in_pin);
	if (light_value < 20) {
		now_dark = true;
		if (!timer_start && !led_on) {
			timer_start = true;
			start_time = millis();
			dark_when_timer_start = true;
			Serial.println("timer starts on dark");
		}
	} else if (light_value > 20) {
		now_dark = false;
		if (!timer_start && led_on) {
			timer_start = true;
			start_time = millis();
			dark_when_timer_start = false;
			Serial.println("timer starts on bright");
		}
	}
}

void	light_led(bool on) {
	if (on) {
		digitalWrite(led_pin, HIGH);
	} else {
		digitalWrite(led_pin, LOW);
	}
}
