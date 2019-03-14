#include <stdio.h>
#include "warehouse.h"
#define BOOLEAN char
#define FALSE 0
#define TRUE 1

extern struct warehouse* createWarehouse(int id, int size);
extern void insertWarehouse(struct warehouse* warehouse, BOOLEAN private);
extern void freeAllWarehouseSFList();

int main(int argc, char** argv) {



	freeAllWarehouseSFList();
	return 0;
}
