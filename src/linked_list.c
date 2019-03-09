#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "warehouse.h"
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

extern struct warehouse_sf_list* sf_head;	// Head of the Segregated Free List for Warehouses
BOOLEAN empty = TRUE;				// Value to keep track of the the elements in the Segregated Free List for initializing sf_head

/*
 * createWarehouseSFList()
 * allocates memory for a new class size of warehouses for the segregated free list
 * 
 * Params:
 * 	class_size
 *	size all members of its warehouse list will be
 *	
 *	warehouse_list_head
 *	Since this function is only called if a warehouse is created, but an entry in the Segregated List is yet to exist, that newly created warehouse will be the beginning of the list for its class size
 * 
 * Return:
 * 	pointer to the newly malloc'd warehouse_sf_list [member]
 */
struct warehouse_sf_list* createWarehouseSFList(int class_size, struct warehouse_list* warehouse_list_head){
	struct warehouse_sf_list* output = malloc(sizeof(struct warehouse_sf_list));
	output->class_size = class_size;
	output->warehouse_list_head = warehouse_list_head;
	return output;
}

/*
 * insertWarehouseSFList()
 * inserts a the new member of the segrated free list in its correct position relative to the already inserted members
 *
 * Params:	
 * 	toBeInserted
 * 	the member of the segregated free list that this function will insert into the free list
 *
 * Return:
 * 	void	
 */
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

/*
 * freeWarehouse()
 * Frees the memory allocated to the specified warehouse and its dependencies if any
 *
 * Params:
 * 	warehouse
 * 	pointer to the warehouse to be freed
 *
 * Return:
 * 	void
 */
void freeWarehouse(struct warehouse* warehouse){
	if (warehouse->art_collection){
		if (warehouse->art_collection->name)
			free(warehouse->art_collection->name);
		free(warehouse->art_collection);
	}
	free(warehouse);
}

/*
 * freeAllWarehouseList()
 * frees all the warehouse of the same class size
 *
 * Params:
 * 	warehouse_list_head 
 * 	head of the list of the class size to be freed
 *
 * Return:
 * 	void
 */
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

/*
 * freeAllWarehouseSFList()
 * frees the entirety of the Segregated List including all dependacies (warehouse_lists, warehouses, art_collections)
 *
 * Params:
 * 	void
 * 
 * Return:
 * 	void
 */
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
