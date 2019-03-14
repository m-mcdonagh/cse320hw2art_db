#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "warehouse.h"

struct art_collection* createArtCollection(char* name, int size, int price){
	struct art_collection* output = malloc(sizeof(struct art_collection));
	output->name = malloc(strlen(name)+1);
	strcpy(output->name, name);
	output->size = size;
	output->price = price;
	return output;
}

void insertArtCollection(struct art_collection* art_collection){
	struct warehouse_sf_list* sf_cursor = sf_head;
	while (sf_cursor->class_size < art_collection->size){
		sf_cursor = sf_cursor->sf_next_warehouse;
		if (!sf_cursor){
			printf("ERROR: There exists no Warehouse large enough to fit Art Collection \"%s\".\n", art_collection->name);
			return;
		}
	}
	struct warehouse_list* wl_cursor = sf_cursor->warehouse_list_head;
	while (1){
		while (!wl_cursor){
			sf_cursor = sf_cursor->sf_next_warehouse;
			if (!sf_cursor){
				printf("ERROR: There exists no Warehouse large enough to fit Art Collection \"%s\".\n", art_collection->name);
				return;
			}
			wl_cursor = sf_cursor->warehouse_list_head;
		}
		while(wl_cursor){
			if ((wl_cursor->meta_info) & 2){
				wl_cursor = wl_cursor->next_warehouse;
			}
			else{
				int newSize = sf_cursor->class_size - art_collection->size;
				if (newSize < 4){
					wl_cursor->warehouse->art_collection = art_collection;
					wl_cursor->meta_info = wl_cursor->meta_info | 2;
					return;
				}
				else{
					insertWarehouse(createWarehouse(	wl_cursor->warehouse->id, 	art_collection->size),	wl_cursor->meta_info&1);
					insertWarehouse(createWarehouse(	nextGoodID(),			newSize),		wl_cursor->meta_info&1);
					removeWarehouseList(wl_cursor);
				}
			}
		}
	}
}
