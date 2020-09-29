#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FUEL_MAP_WIDTH 16
#define FUEL_MAP_HEIGHT 16

struct map2d
{
	uint8_t nrows;
	uint8_t ncols;
	
	uint16_t *ix;
	uint16_t *iy;
	uint16_t **data;
};


uint16_t lininterp(uint16_t x, uint16_t x1, uint16_t x2, uint16_t z1, uint16_t z2);
uint16_t map(uint16_t x,uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
uint16_t bilinear(uint16_t x, uint16_t y, uint16_t *z, struct map2d* map);

int map2d_alloc(struct map2d* table,uint8_t dim_x, uint8_t dim_y);
int linspace(uint16_t s, uint16_t e, uint16_t n, uint16_t *dst);

