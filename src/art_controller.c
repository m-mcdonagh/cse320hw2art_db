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
			wl_prev = NULL;
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

/*
 * loadArtFile()
 * inserts into the database art collections from a specified file each of the format: "%s %d %d\n", name, size, price
 *
 * Params:
 * 	artFile
 * 	pointer to the file to be read
 *
 * Return:
 * 	void
 */
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

/*
 * printArtCollection()
 * prints the info of the specified art collection to stdout
 *
 * Params:
 * 	artC
 * 	the art collection to be printed
 *
 * Return:
 * 	void
 */
void printArtCollection(struct art_collection* artC){
	printf("%s %d %d", artC->name, artC->size,  artC->price);
}

/*
 * printAll()
 * prints the info of all art collections of the database to stdout
 *
 * Params:
 * 	void
 *
 * Return:
 * 	void
 */
void printAll(BOOLEAN all, BOOLEAN private){
	struct warehouse_sf_list* sf_cursor = sf_head;
	struct warehouse_list* wl_cursor;
	int total = 0;
	while (sf_head){
		wl_cursor = sf_cursor->warehouse_list_head;
		while(wl_cursor){
			if (((wl_cursor->meta_info) & 2) && (all  || !((wl_cursor->meta_info) ^ private))){
				printArtCollection( wl_cursor->warehouse->art_collection);
				total += wl_cursor->warehouse->art_collection->price;
			}
			wl_cursor = wl_cursor->next_warehouse;
		}
		sf_head = sf_head->sf_next_warehouse;
	}
}

struct art_collection_list{
	struct art_collection* art_collection;
	struct art_collection_list* next;
};

void printAndFreeACL(struct art_collection_list* acl_cursor){
	struct art_collection_list* acl_prev;
	while (acl_cursor){
		printArtCollection(acl_cursor->art_collection);
		acl_prev = acl_cursor;
		free(acl_prev);
		acl_cursor = acl_cursor->next;
	}
}

void printBySize(BOOLEAN all, BOOLEAN private){
	struct warehouse_sf_list* sf_cursor = sf_head;
	struct warehouse_list* wl_cursor;
	struct art_collection_list* acl_head = NULL;
	struct art_collection_list* acl_cursor = NULL;
	struct art_collection_list* acl_prev;
	struct art_collection_list* acl_new;
	while (sf_cursor){
		wl_cursor = sf_cursor->warehouse_list_head;
		while(wl_cursor){
			if (((wl_cursor->meta_info) & 2) && (all || !((wl_cursor->meta_info) ^ private))){
				if (acl_head){
					if (acl_head->art_collection->size > wl_cursor->warehouse->art_collection->size){
						acl_new = malloc(sizeof(struct art_collection_list));
						acl_new->art_collection = wl_cursor->warehouse->art_collection;
						acl_new->next = acl_head;
						acl_head = acl_new;
					}
					else{
						acl_cursor = acl_head->next;
						acl_prev = acl_head;
						while (acl_cursor){
							if (acl_cursor->art_collection->size > wl_cursor->warehouse->art_collection->size){
								acl_new = malloc(sizeof(struct art_collection_list));
								acl_new->art_collection = wl_cursor->warehouse->art_collection;
								acl_new->next = acl_cursor;
								acl_prev->next = acl_new;
								break;
							}
							acl_prev = acl_cursor;
							acl_cursor = acl_cursor->next;
						}
						if (!acl_cursor){ 
							acl_new = malloc(sizeof(struct art_collection_list));
							acl_new->art_collection = wl_cursor->warehouse->art_collection;
							acl_new->next = NULL;
							acl_prev->next = acl_new;
						}
					}
				}
				else{
					acl_head = malloc(sizeof(struct art_collection_list));
					acl_head->art_collection = wl_cursor->warehouse->art_collection;
					acl_head->next = NULL;
				}
			}
		wl_cursor = wl_cursor->next_warehouse;
		}
	sf_cursor = sf_cursor->sf_next_warehouse;
	}
	printAndFreeACL(acl_head);
}

void printByPrice(BOOLEAN all, BOOLEAN private){	
	struct warehouse_sf_list* sf_cursor = sf_head;
	struct warehouse_list* wl_cursor;
	struct art_collection_list* acl_head = NULL;
	struct art_collection_list* acl_cursor = NULL;
	struct art_collection_list* acl_prev;
	struct art_collection_list* acl_new;
	while (sf_cursor){
		wl_cursor = sf_cursor->warehouse_list_head;
		while(wl_cursor){
			if (((wl_cursor->meta_info) & 2) && (all || !((wl_cursor->meta_info) ^ private))){
				if (acl_head){
					if (acl_head->art_collection->price > wl_cursor->warehouse->art_collection->price){
						acl_new = malloc(sizeof(struct art_collection_list));
						acl_new->art_collection = wl_cursor->warehouse->art_collection;
						acl_new->next = acl_head;
						acl_head = acl_new;
					}
					else{
						acl_cursor = acl_head->next;
						acl_prev = acl_head;
						while (acl_cursor){
							if (acl_cursor->art_collection->price > wl_cursor->warehouse->art_collection->price){
								acl_new = malloc(sizeof(struct art_collection_list));
								acl_new->art_collection = wl_cursor->warehouse->art_collection;
								acl_new->next = acl_cursor;
								acl_prev->next = acl_new;
								break;
							}
							acl_prev = acl_cursor;
							acl_cursor = acl_cursor->next;
						}
						if (!acl_cursor){ 
							acl_new = malloc(sizeof(struct art_collection_list));
							acl_new->art_collection = wl_cursor->warehouse->art_collection;
							acl_new->next = NULL;
							acl_prev->next = acl_new;
						}
					}
				}
				else{
					acl_head = malloc(sizeof(struct art_collection_list));
					acl_head->art_collection = wl_cursor->warehouse->art_collection;
					acl_head->next = NULL;
				}
			}
		wl_cursor = wl_cursor->next_warehouse;
		}
	sf_cursor = sf_cursor->sf_next_warehouse;
	}
	printAndFreeACL(acl_head);
}
