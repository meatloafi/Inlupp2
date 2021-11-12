CC = gcc
FLAGS = -g -pedantic -Wall

store: storefront.o webstore.o utils.o hash_table.o linked_list.o
	$(CC) $(FLAGS) $^ -o store

test: store_tests.o utils.o hash_table.o linked_list.o webstore.o
	$(CC) $(FLAGS) $^ -o test -lcunit

memtest-store: store
	@valgrind --leak-check=full ./store

memtest-test: test
	@valgrind --leak-check=full ./test

storefront.o: storefront.c business.h utils.h hash_table.h linked_list.h
	$(CC) $(FLAGS) -c storefront.c

webstore.o: webstore.c hash_table.h linked_list.h
	$(CC) $(FLAGS) -c webstore.c

utils.o: utils.c
	$(CC) $(FLAGS) -c utils.c

hash_table.o: hash_table.c linked_list.h
	$(CC) $(FLAGS) -c hash_table.c

linked_list.o: linked_list.c
	$(CC) $(FLAGS) -c linked_list.c

clean:
	rm -f ./store ./test *.o