all:
	gcc -c -o its_unit_test.o tests/its_unit_test.c -Wall -Wextra -Werror
	gcc -c -o hashmap.o hashmap.c -Wall -Wextra -Werror
	gcc -c -o test.o tests/test.c -I . -Wall -Wextra -Werror
	gcc -o test test.o its_unit_test.o hashmap.o -pthread -Wall -Wextra -Werror

test:
	@gcc -c -o its_unit_test.o tests/its_unit_test.c -Wall -Wextra -Werror
	@gcc -c -o hashmap.o hashmap.c -Wall -Wextra -Werror
	@gcc -c -o test.o tests/test.c -I . -Wall -Wextra -Werror
	@gcc -o test test.o its_unit_test.o hashmap.o -pthread -Wall -Wextra -Werror
	@./test || true
	@rm -f test.o
	@rm -f its_unit_test.o
	@rm -f hashmap.o
	@rm -f test

clean:
	rm -f test.o
	rm -f its_unit_test.o
	rm -f hashmap.o
	rm -f test
