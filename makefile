store: storefront.c utils.c hash_table.c linked_list.c list_iterator.c webstore.c
	gcc -g -pedantic -Wall storefront.c utils.c hash_table.c linked_list.c list_iterator.c webstore.c -o store

memtest-store: store
	@valgrind --leak-check=full ./store

test: store_tests.c utils.c hash_table.c linked_list.c list_iterator.c webstore.c
	gcc -g -pedantic -Wall store_tests.c utils.c hash_table.c linked_list.c list_iterator.c webstore.c -o test -lcunit

run:
	./store

clean:
	rm -f ./store ./test
