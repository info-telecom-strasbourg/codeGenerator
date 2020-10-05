all:
	gcc -c -o its_unit_test.o tests/its_unit_test.c -Wall -Wextra -Werror
	gcc -c -o queue.o queue.c -Wall -Wextra -Werror
	gcc -c -o test.o tests/test.c -I . -Wall -Wextra -Werror
	gcc -o test test.o its_unit_test.o queue.o -pthread -Wall -Wextra -Werror

test:
	@gcc -c -o its_unit_test.o tests/its_unit_test.c -Wall -Wextra -Werror
	@gcc -c -o queue.o queue.c -Wall -Wextra -Werror
	@gcc -c -o test.o tests/test.c -I . -Wall -Wextra -Werror
	@gcc -o test test.o its_unit_test.o queue.o -pthread -Wall -Wextra -Werror
	@./test || true
	@rm -f test.o
	@rm -f its_unit_test.o
	@rm -f queue.o
	@rm -f test

clean:
	rm -f test.o
	rm -f its_unit_test.o
	rm -f queue.o
	rm -f test
