#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "warehouse.h"
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

extern struct warehouse_sf_list* sf_head;
BOOLEAN empty = TRUE;

struct warehouse_sf_list* createWarehouseSFList(int class_size){
	struct warehouse_sf_list* output = malloc(sizeof(struct warehouse_sf_list));
	output->class_size = class_size;
	output->warehouse_list_head = NULL;
	return output;
}

void insertWarehouseSFList(struct warehouse_sf_list* toBeInserted){
	if (empty)
		sf_head = toBeInserted;
	else{
		struct warehouse_sf_list* cursor = sf_head->sf_next_warehouse;
		struct warehouse_sf_list* prev = cursor;
		while (cursor){
			if (cursor->class_size > toBeInserted->class_size){
				toBeInserted->sf_next_warehouse = cursor;
				prev->sf_next_warehouse = toBeInserted;
				return;
			}
			prev = cursor;
			cursor = cursor->sf_next_warehouse;
		}
		prev->sf_next_warehouse = toBeInserted;
		toBeInserted->sf_next_warehouse = NULL;
	}
}

void freeWarehouse(struct warehouse* warehouse){
	free(warehouse->art_collection->name);
	free(warehouse->art_collection);
	free(warehouse);
}

void freeAllWarehouseList(struct warehouse_list* warehouse_list_head){
	struct warehouse_list* cursor = warehouse_list_head;
	struct warehouse_list* temp;
	while (cursor){
		freeWarehouse(cursor->warehouse);
		temp = cursor;
		cursor = cursor->next_warehouse;
		free(temp);
	}
}

void freeAllWarehouseSFList(){
	if (empty)
		return;
	struct warehouse_sf_list* cursor = sf_head;
	struct warehouse_sf_list* temp;
	while (cursor){
		freeAllWarehouseList(cursor->warehouse_list_head);
		temp = cursor;
		cursor = cursor->sf_next_warehouse;
		free(temp);
	}
}
