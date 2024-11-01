const int	buttonPin = 2;
const int	ledPin = 8;

int	buttonState = 0;
int	counter = 0;

void	setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT);
	Serial.begin(9600);
}

void	loop() {
	buttonState = digitalRead(buttonPin);

	counter++;
	if (counter >= 10000) {
		Serial.println(buttonState);
		counter = 0;
	}
	if (buttonState == HIGH) {
		digitalWrite(ledPin, HIGH);
	} else {
		digitalWrite(ledPin, LOW);
	}
}
