store: storefront.c utils.c hash_table.c linked_list.c webstore.c
	gcc -g -pedantic -Wall storefront.c utils.c hash_table.c linked_list.c webstore.c -o store

memtest-store: store
	@valgrind --leak-check=full ./store

memtest-test: test
	@valgrind --leak-check=full ./test

test: store_tests.c utils.c hash_table.c linked_list.c webstore.c
	gcc -g -pedantic -Wall store_tests.c utils.c hash_table.c linked_list.c webstore.c -o test -lcunit

clean:
	rm -f ./store ./test
