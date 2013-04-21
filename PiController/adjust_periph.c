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
}
