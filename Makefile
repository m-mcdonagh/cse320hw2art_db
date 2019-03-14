all:
	gcc src/main.c src/linked_list.c src/art_controller.c -o art_db

clean: 
	rm art_db
