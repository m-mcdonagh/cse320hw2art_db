#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "warehouse.h"
#define BOOLEAN char
#define FALSE 0
#define TRUE 1

BOOLEAN equals(char* s1, char*s2){
	return !strcmp(s1, s2);
}

BOOLEAN sizeSort = FALSE;
BOOLEAN priceSort = FALSE;

BOOLEAN executeCommand(char** args){
	if (equals(*args, "help")){
		printf("help\t\t\t\tLists available commands.\n");
		printf("load warehouse \"filename\"\tLoads into the database warehouses from a file.\n");
		printf("load art \"filename\"\t\tLoads into the database art collections from a file.\n");
		printf("printall\t\t\tPrints all the art collections of the database to stdout.\n");
		printf("print public\t\t\tPrints all the art collections of the database in public warehouses to stdout.\n");
		printf("print private\t\t\tPrints all the art collections of the database in private warehouses to stdout.\n");
		printf("add art \"name\" \"size\" \"price\"\tEnters a new art collection in the database of a specified name, size, and price.\n");
		printf("delete art \"name\"\t\tRemoves any art collections with the specified name from the database.\n");
		printf("utilization\t\t\tPrints to stdout the ratio of occupied warehouses to the total and the ratio of the total size of art collections to\n\t\t\t\t\tthe total capacity of the warehouses.\n");
	}
	else if (equals(*args, "load")){
		if (equals(*++args, "warehouse")){
			if (*++args) {
				FILE* warehouseFile = fopen(*args, "r");
				if (warehouseFile){
					loadWarehouseFile(warehouseFile);
					fclose(warehouseFile);
				}
				else{
					printf("ERROR: failed to open %s\n", *args);
				}
			}
			else printf("ERROR: no file specified\n");
		}
		else if (equals(*args,  "art")){
			if (*++args) {
				FILE* artFile = fopen(*args, "r");
				if (artFile){
					loadArtFile(artFile);
					fclose(artFile);
				}
				else{
					printf("ERROR: failed to open %s\n", *args);
				}
			}
			else printf("ERROR: no file specified\n");
		}
		else{
			printf("ERROR: not a valid command, type \"help\" for a list of commands.\n");
		}
	}
	else if (equals(*args, "printall")){
		if (sizeSort){
			printBySize(1, 1);
		}
		else if (priceSort){
			printByPrice(1, 1);
		}
		else{
			printAll(1, 1);
		}	
	}
	else if (equals(*args, "print")){
		BOOLEAN private = equals(*++args, "private");
		if (sizeSort){
			printBySize(0, private);
		}
		else if (priceSort){
			printByPrice(0, private);
		}
		else{
			printAll(0, private);
		}
	}
	else if (equals(*args, "add") && *(args + 1) && *(args + 2) && *(args + 3) && *(args + 4)){
		if (equals(*++args, "art")){
			char* name = *++args;
			int size = atoi(*++args);
			int price = atoi(*++args);
			struct art_collection* artC = createArtCollection(name, size, price);
			insertArtCollection( artC );
			printf("Added ");
			printArtCollection( artC );
		}
	}
	else if (equals(*args, "delete") && *(args + 1) && *(args + 2)){
		if (equals(*++args, "art"))
			removeArtCollection(*++args);
	}
	else if (equals(*args, "utilization")){
		printUtilization();
	}
	else if (equals(*args, "exit")){
		return FALSE;
	}
	else{
		printf("ERROR: not a valid command, type \"help\" for a list of commands.\n");
	}
	return TRUE;
}

int main(int argc, char** argv) {
	sf_head = NULL;
	BOOLEAN quiet = FALSE;
	FILE* warehousesFile;
	FILE* art_collectionsFile;
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

	shell_loop(5);

	printf("DONE.\n");
	freeAllWarehouseSFList();
	return 0;
}
