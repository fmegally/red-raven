#include "maps.h"

uint16_t rpm[FUEL_MAP_WIDTH] = {600,700,800,1000,1200,1400,1600,2000,2400,2800,3200,3600,4000,5000,6000,8000};
uint16_t load[FUEL_MAP_WIDTH] = {0,5,10,15,20,25,30,35,40,50,60,70,80,90,100,110};

uint16_t lininterp(uint16_t x, uint16_t x1, uint16_t x2, uint16_t z1, uint16_t z2)
{
	return ((x - x1)/(x2 - x1))*z2 + ((x2 - x)/(x2 - x1))*z1;
}

uint16_t map(uint16_t x,uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
	uint16_t t;
	t = ((x - x1)/(x2 - x1))*(y2 - y1) + y1;
	return t;
}


uint16_t bilinear(uint16_t x, uint16_t y, uint16_t *z, struct map2d* map)
{
	uint8_t ix, iy;
	for (ix=0; i < map->ncols - 1; ix++)
	{
		if (x >= (maps->ix)[ix] && x < (maps->ix)[ix + 1]) break;
	}

	for (iy=0; i < map->nrows - 1; iy++)
	{
		if (x >= (maps->iy)[iy] && x < (maps->iy)[iy + 1]) break;
	}

	q1 = lininterp(x, (maps->ix)[ix],(maps->ix)[ix + 1], (maps->data)[ix][iy],(maps->data)[ix+1][iy]);
	q2 = lininterp(x, (maps->ix)[ix],(maps->ix)[ix + 1], (maps->data)[ix][iy+1],(maps->data)[ix+1][iy+1]);

	z = lininterp(y,(maps->iy)[iy],(maps->iy)[iy + 1], q1, q2);

	return z;
}

	
		

int map2d_alloc(struct map2d* table,uint8_t dim_x, uint8_t dim_y)
{
	table->ncols = dim_x;
	table->nrows = dim_y;
	
	table->ix = (uint16_t*) malloc(sizeof(uint16_t) * dim_x);
	if(table->ix == NULL){
		return -1;
	}

	table->iy = (uint16_t*) malloc(sizeof(uint16_t) * dim_y);
	if(table->iy == NULL){
		return -1;
	}

	table->data = (uint16_t**) malloc(sizeof(uint16_t*) * dim_y);
	if(table->data == NULL){
		return -1;
	}

	uint8_t i;
	for (i=0; i<dim_y; i++)
	{
		table->data[i] = (uint16_t*) malloc(sizeof(uint16_t) * dim_x);
		if (table->data[i] == NULL)
		{
			return -1;
		}
	}	
	return 0;
}

int linspace(uint16_t s, uint16_t e, uint16_t n, uint16_t *dst)
{	
	uint16_t i;
	uint16_t t;

	t = (e - s)/(n - 1);
	for (i=0; i <= (n-1); i++)
	{
		dst[i] = s + (i * t);
	}
	return 0;
}
	



