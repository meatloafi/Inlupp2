store: storefront.c utils.c hash_table.c linked_list.c list_iterator.c webstore.c
	gcc -g -pedantic -Wall storefront.c utils.c hash_table.c linked_list.c list_iterator.c webstore.c -o store



memtest-store: store
	@valgrind --leak-check=full ./store
run:
	./a.out

clean:
	rm -f ./a.out
