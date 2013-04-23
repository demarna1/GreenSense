#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define BUFF_SIZE 20
#define GPIO_PIN_17 0
#define GPIO_PIN_18 1
#define ON 1
#define OFF 0

int main() {
	char buff[BUFF_SIZE];
	int settings[7];
	int i = 0;
	if (wiringPiSetup() == -1) {
		exit(1);
	}
	while (fgets(buff, BUFF_SIZE, stdin) != NULL) {
		settings[i] = atoi(buff);
		i++;
		if (i == 7) {
			break;
		}
	}
	if (settings[6] == ON) {
		digitalWrite(GPIO_PIN_17, ON);
		digitalWrite(GPIO_PIN_18, ON);
	}
	else if (settings[6] == OFF) {
		digitalWrite(GPIO_PIN_17, OFF);
		digitalWrite(GPIO_PIN_18, OFF);
	}
	return 0;
}
