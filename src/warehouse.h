#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <stdint.h>

struct art_collection {
    char* name;
    int size;
    int price;
};

struct warehouse {
    int id;
    int size;
    struct art_collection* art_collection;
};

/* This defines the internal structure of meta_info field. The two least significant bits
 * represents if the warehouse is occupied and if it is private or public. The rest of the
 * bits are for its size
structure of meta_info
    uint64_t size:62; // Size of the warehouse and should be multiple of 2 and at least 4
    uint64_t occupied:1; // Bit that indicates of the warehouse is occupied or not (1 is occupied and 0 is free)
    uint64_t is_private:1; // Defines ownership of the warehouse (1 is private and 0 is public)
*/

struct warehouse_list {
    uint64_t meta_info; // Meta information about warehouse node; it is mimicking memory block header
    struct warehouse* warehouse; // Useful information about actual warehouse; think of payload
    struct warehouse_list* next_warehouse;
};

struct warehouse_sf_list {
    // `class_size' represents warehouse sizes that correspond to the list this node points to
    int class_size;
    struct warehouse_list* warehouse_list_head;
    struct warehouse_sf_list* sf_next_warehouse;
};

struct warehouse_sf_list* sf_head;
#endif /* WAREHOUSE_H */
