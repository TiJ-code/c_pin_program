#include "pin_ctrl.h"

#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static inline void loggerf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

static bool is_number(const char *s) {
	if (*s == 0)
		return false;

	while (*s) {
		if (!isdigit((unsigned char) *s))
			return false;
		s++;
	}

	return true;
}

static PinCtrl_t ctrl;

static int clean_exit(int code) {
	pc_free(&ctrl);
	return code;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: \n");
		printf("  %s <pin>\n", argv[0]);
		printf("  %s <pin> <0|1>\n", argv[0]);
		printf("  %s <pin> t=<ms>\n", argv[0]);
		return 1;
	}

	int pin;
	if (is_number(argv[1])) {
		pin = atoi(argv[1]);
	} else {
		fprintf(stderr, "Invalid pin: %s\n", argv[1]);
		return 1;
	}

	ctrl = (PinCtrl_t) {0};
	if (pc_init(&loggerf, &ctrl) != PC_OK) {
		fprintf(stderr, "Failed to connect to pigpiod\n");
		return clean_exit(1);
	}

	if (argc == 2) {
		if (pc_toggle_pin(&ctrl, pin) != PC_OK) {
			fprintf(stderr, "Failed to toggle pin %d\n", pin);
			return clean_exit(1);
		}
		return clean_exit(0);
	}

	if (is_number(argv[2])) {
		int val = atoi(argv[2]);

		if (val != 0 && val != 1) {
			fprintf(stderr, "Value must be either 0 or 1\n");
			return clean_exit(1);
		}

		PinCtrlState_t state = val == 0 ? PC_STATE_OFF : PC_STATE_ON;
		if (pc_turn_pin(&ctrl, pin, state)) {
			fprintf(stderr, "Failed to set pin %d\n", pin);
			return clean_exit(1);
		}

		return clean_exit(0);
	}

	if (strncmp(argv[2], "t=", 2) == 0) {
		const char *time = argv[2] + 2;
		if (!is_number(time)) {
			fprintf(stderr, "Invalid time format! Expected t=<ms>\n");
			return clean_exit(1);
		}

		int time_ms = atoi(time);
		if (time_ms < 0)
			time_ms = 0;

		if (pc_trigger_pin(&ctrl, pin, time_ms, PC_STATE_ON) != PC_OK) {
			fprintf(stderr, "Failed to trigger pin %d\n", pin);
			return clean_exit(1);
		}

		return clean_exit(0);
	}

	fprintf(stderr, "Invalid argument %s\n", argv[2]);
	return clean_exit(1);
}
