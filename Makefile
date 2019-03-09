all:
	gcc src/main.c src/linked_list.c -o art_db

clean: 
	rm art_db
