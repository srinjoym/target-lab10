#include <stdint.h>

#ifndef COMMON_H
#define COMMON_H

struct Player {
	uint8_t sprite;
	int32_t score;
};
typedef  struct Player player;

#endif /* COMMON_H */
