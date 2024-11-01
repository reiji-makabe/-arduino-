#define	DEBUG

#ifdef	DEBUG
	const int	night_standby_time = 3;
	const int	daytime_standby_time = 5;
#else
	const int	night_standby_time = 300;
	const int	daytime_standby_time = 3600;
#endif

const int	buzzer_pin = 13;
const int	button_in_pin = 2;
const int	light_in_pin = 0;
const int	led_pin = 11;

const int	bps = 30;
// オーラリー
const char	*score = "-5,0.25 0,0.25 -1,0.25 0,0.25 2,0.25 -3,0.25 2,0.5 100,0.01 0,0.25 -1,0.25 -3,0.25 -1,0.25 0,0.5 100,0.5 \
-5,0.25 0,0.25 -1,0.25 0,0.25 2,0.25 -3,0.25 2,0.5 100,0.01 0,0.25 -1,0.25 -3,0.25 -1,0.25 0,0.5 100,0.5 \
16,0.325 16,0.125 16,0.5 100,0.01 16,0.325 16,0.125 16,0.5 100,0.01 \
16,0.25 14,0.25 12,0.25 14,0.25 16,0.5 100,0.5 \
16,0.25 16,0.25 17,0.25 16,0.25 14,0.25 9,0.25 14,0.5 12,0.25 11,0.25 9,0.25 11,0.25 12,0.5";

int	is_night = 0;
int	timecount = 0;
int	light_value = 0;
int	button_state = 0;

void	setup() {
	Serial.begin(9800);
	pinMode(button_in_pin, INPUT_PULLUP);
}

float	parse_phoneme(float scale) {
	switch ((int)scale) {
		// C4 = 0;
		// C#4 = 1;
		case -5: // G3
			return 201.741;
		case -3: // A3
			return 226.446;
		case -1:  // B3
			return 254.178;
		case 0: // C4
			return 269.292;
		case 1:
			return 285.305;
		case 2: // D4
			return 302.270;
		case 3:
			return 320.244;
		case 4: // E4
			return 339.286;
		case 5: // F4
			return 359,461;
		case 6:
			return 380.836;
		case 7: // G4
			return 403,482;
		case 8:
			return 427.474;
		case 9: // A4
			return 452.893;
		case 10:
			return 479.823;
		case 11: // B4
			return 508.355;
		case 12: // C5
			return 538.584;
		case 13:
			return 570.609;
		case 14: // D5
			return 604.540;
		case 16: // E5
			return 678.573;
		case 17: // F5
			return 718.923;
		default:
			return 0;
	}
}

float	strtof(char *str, char **ret) {
	float	result = 0;
	float	decimal_place = 1.0;
	int		is_negative = 0;

	while (*str == ' ' || *str == ',') {
		str++;
	}
	if (*str == '-') {
		is_negative = 1;
		str++;
	} else if (*str == ('+')) {
		str++;
	}
	while ('0' <= *str && *str <= '9') {
		result = result * 10.0 + (*str - '0');
		str++;
	}
	if (*str == '.') {
		str++;
		while ('0' <= *str && *str <= '9') {
			decimal_place *= 0.1;
			result += (*str - '0') * decimal_place;
			str++;
		}
	}

	*ret = str;
	if (is_negative) {
		return result * -1.0;
	} else {
		return result;
	}
}

void	alarm(int bps, char *score) {
	char	*tmp;
	char	*now;
	float	phoneme;
	float	time;

	now = score;
	while (1) {
		// 曲をループさせたい
		if (*now == '\0') {
			Serial.println("test");
			now = score;
			delay(3000);
		}
		phoneme = strtof(now, &tmp);
		if (now == tmp) {
			break;
		}
		now = tmp;
		phoneme = parse_phoneme(phoneme);
		time = strtof(now, &tmp);
		if (now == tmp) {
			break;
		}
		now = tmp;
		Serial.println(time);
		time = (60 / bps) * time * 1000;
		Serial.println(time);
		tone(buzzer_pin, phoneme);
		delay((int)time);
		noTone(buzzer_pin);
		delay(1);

		// if button on, alarm off
		button_state = digitalRead(button_in_pin);
		Serial.println(button_state);
		if (button_state == LOW) {
			break ;
		}
	}
}

void	light_led(int is_on) {
	if (is_on) {
		analogWrite(led_pin, 50);
	} else {
		digitalWrite(led_pin, LOW);
	}
}

// char	*convers_score(char *score) {
// 	char	*ret_p;
// 	char	*tmp;
// 
// 	ret_p = (char *)calloc(sizeof(score));
// 	tmp = ret_p;
// 	while (*score) {
// 		if (*tmp == ' ' || *tmp == ',') {
// 			continue;
// 		} else if ('A' <= *tmp && *tmp <= 'G') {
// 			switch (*tmp) {
// 
// 	}
// }

void	bright() {
	char	*new_score;

	timecount += 1;
	if (is_night && timecount >= night_standby_time) {
// 		new_score = convvers_score(score);
// 		if (new_score == NULL) {
// 			return;
// 		}
// 		alarm(bps, new_score);
		alarm(bps, score);
		timecount = 0;
		is_night = 0;
		light_led(0);
	}
	else if (!is_night) {
		timecount = 0;
	}
}

void	dark() {
	timecount += 1;
	if (is_night) {
		timecount = 0;
	}
	else if (!is_night && timecount >= daytime_standby_time) {
		light_led(1);
		timecount = 0;
		is_night = 1;
	}
}

void	loop() {
	light_value = analogRead(light_in_pin);
	if (light_value >= 20) {
		bright();
	} else {
		dark();
	}
#ifdef DEBUG
	Serial.print("is_night:");
	Serial.println(is_night);
	Serial.print("timecount:");
	Serial.println(timecount);
	Serial.print("light_value:");
	Serial.println(light_value);
	Serial.println("****");
#endif
	delay(1000);
}
