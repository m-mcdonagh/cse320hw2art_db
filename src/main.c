#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "warehouse.h"
#define BOOLEAN char
#define FALSE 0
#define TRUE 1

int main(int argc, char** argv) {
	BOOLEAN quiet = FALSE;
	FILE* warehousesFile;
	FILE* art_collectionsFile;
	BOOLEAN sizeSort = FALSE;
	BOOLEAN priceSort = FALSE;
	int opt;
	while ((opt = getopt(argc, argv, "qw:a:s:")) != -1){
		switch (opt){
			case 'q':
				quiet = TRUE;
				break;
			case 'w':
				if (!quiet){
					printf("ERROR: warehouses files can only be opened by commandline when in quiet mode (-q).\n");
					exit(1);
				}
				warehousesFile = fopen(optarg, "r");
				if (!warehousesFile){
					printf("ERROR: failed to open Warehouses File \"%s\".\n", optarg);
					exit(1);
				}
				break;
			case 'a':
				if (!quiet){
					printf("ERROR: art collections files can only be opened by commandline when in quiet mode (-q).\n");
					exit(1);
				}
				art_collectionsFile = fopen(optarg, "r");
				if (!art_collectionsFile){
					printf("ERROR: failed to open Art Collections File \"%s\".\n", optarg);
					exit(1);
				}
				break;
			case 's':
			      if (optarg[0] == 's' && optarg[1] == '\0')
				      sizeSort = TRUE;
			      else if (optarg[0] == 'p' && optarg[1] == '\0')
				      priceSort = TRUE;
			      else{
				      printf("ERROR: \"%s\" is not a valid argument for -s. Valid arguments: \"p\" and \"s\".\n", optarg);
				      exit(1);
			      }
			      break;
			case '?':
			      exit(1);
		}
	}
	if (quiet && (!warehousesFile || !art_collectionsFile)){
		printf("ERROR: no Query Provided\n");
		exit(1);
	};
	printf("DONE.\n");
	freeAllWarehouseSFList();
	return 0;
}
