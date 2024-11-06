#include <Arduino_FreeRTOS.h>

const int	botton_pin = 2;
const int	led_0_pin = 7;
const int	led_1_pin = 10;

const	int	ctrlTable[3][2] = {{LOW, LOW}, {HIGH, LOW}, {HIGH, HIGH}};

int	toggle = 0;

void setup() {
	Serial.begin(9600);

	pinMode(botton_pin, INPUT_PULLUP);
	pinMode(led_0_pin, OUTPUT);
	pinMode(led_1_pin, OUTPUT);

	Serial.println("********\nSystem start");
	attachInterrupt(digitalPinToInterrupt(botton_pin), toggleLED, FALLING);
}

void	loop() {
	digitalWrite(led_0_pin, ctrlTable[toggle][0]);
	digitalWrite(led_1_pin, ctrlTable[toggle][1]);
}

void	toggleLED(void) {
	toggle = (toggle + 1) % 3;
}

/*
void loop() {
	int	btn_state = 0;
	// これboolにしたい...
	bool	latest_change = false;
	int	btn_on = 0;
	int	btn_off = 0;
	int	state = 0;

	while (1) {
		btn_state = digitalRead(botton_pin);
		if (btn_state == 0) {
			btn_on += 1;
			btn_off = 0;
		} else if (btn_on != 0 && btn_state == 1) {
			btn_off += 1;
			latest_change = false;
			if (btn_off >= 1000) {
				btn_on = 0;
			}
		}
		if (btn_on >= 10000 && latest_change == false) {
			latest_change = true;
			btn_off = 0;
			btn_on = 0;
			state = (state + 1) % 3;
		}
		digitalWrite(led_0_pin, ctrlTable[state][0]);
		digitalWrite(led_1_pin, ctrlTable[state][1]);
	}
}
*/
