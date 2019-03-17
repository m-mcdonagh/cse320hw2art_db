#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "warehouse.h"

/*
 * createArtCollection()
 * allocates memory for an art collection with a specified name, size, price
 *
 * Params:
 * 	name
 * 	string for the name of the art collection which will be copied to a new location and pointed to by the new art collection
 *
 * 	size
 * 	value for the smallest size warehouse that can fit this art collection
 *
 * 	price
 * 	value to measure worth of art collection
 *
 * Return:
 * 	pointer to newly malloc'd art collection struct
 */
struct art_collection* createArtCollection(char* name, int size, int price){
	struct art_collection* output = malloc(sizeof(struct art_collection));
	output->name = malloc(strlen(name)+1);
	strcpy(output->name, name);
	output->size = size;
	output->price = price;
	return output;
}

/*
 * insertArtCollection
 * finds an empty, sizable warehouse to store the specified art collection, or reports the failure to the user
 *
 * Params:
 * 	art_collection
 * 	art collection to be stored
 *
 * Return:
 * 	void
 */
void insertArtCollection(struct art_collection* art_collection){
	struct warehouse_sf_list* sf_cursor = sf_head;
	while (sf_cursor->class_size < art_collection->size){
		sf_cursor = sf_cursor->sf_next_warehouse;
		if (!sf_cursor){
			printf("ERROR: There exists no unoccupied warehouse large enough to fit Art Collection \"%s\".\n", art_collection->name);
			return;
		}
	}
	struct warehouse_list* wl_cursor = sf_cursor->warehouse_list_head;
	struct warehouse_list* wl_prev = NULL;
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
				wl_prev = wl_cursor;
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
					if (wl_prev)
						wl_prev->next_warehouse = wl_cursor->next_warehouse;
					else 
						sf_cursor->warehouse_list_head = wl_cursor->next_warehouse;
					freeWarehouse(wl_cursor->warehouse);
					free(wl_cursor);
					insertArtCollection(art_collection);
					return;
				}
			}
		}
	}
}

void loadArtFile(FILE* artFile){
	char* commandLine = malloc(256 * sizeof(char*));
	char** args;
	char* name;
	int size;
	int price;
	while (fgets(commandLine, 255, artFile) != NULL){
		args = commandSplitter(commandLine, 3);
		if (args && (args+1) && (args+2)){
			name = *args;
			size = atoi(*++args);
			price = atoi(*++args);
			insertArtCollection( createArtCollection(name, size, price));
		}
	}
}
