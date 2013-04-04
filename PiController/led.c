#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define GPIO_PIN_17 0
#define GPIO_PIN_18 1
#define ON 1
#define OFF 0

int main() {
	if (wiringPiSetup() == -1) {
		exit(1);
	}
	pinMode(GPIO_PIN_17, OUTPUT);
	pinMode(GPIO_PIN_18, OUTPUT);
	digitalWrite(GPIO_PIN_17, LOW);
	digitalWrite(GPIO_PIN_18, LOW);
	while (1) {
		digitalWrite(GPIO_PIN_17, HIGH);
		digitalWrite(GPIO_PIN_18, HIGH);
		delay(1000);
		digitalWrite(GPIO_PIN_17, LOW);
		digitalWrite(GPIO_PIN_18, LOW);
		delay(1000);
	}
	return 0;
}
