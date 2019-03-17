all:
	gcc src/main.c src/linked_list.c src/art_controller.c src/shell.c -o art_db -Wall -g

clean: 
	rm art_db
