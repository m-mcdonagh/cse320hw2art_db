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

BOOLEAN executeCommand(char** arguments){
	if (equals(*arguments, "help")){
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
	else if (equals(*arguments, "load")){
	}
	else if (equals(*arguments, "printall")){
	}
	else if (equals(*arguments, "print")){
	}
	else if (equals(*arguments, "add")){
	}
	else if (equals(*arguments, "delete")){
	}
	else if (equals(*arguments, "utilization")){
	}
	else if (equals(*arguments, "exit")){
		return FALSE;
	}
	else{
		printf("ERROR: not a valid command, type \"help\" for a list of commands.\n");
	}
	return TRUE;
}

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

	shell_loop();

	printf("DONE.\n");
	freeAllWarehouseSFList();
	return 0;
}
