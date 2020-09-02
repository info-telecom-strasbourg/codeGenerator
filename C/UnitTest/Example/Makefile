all:
	gcc -c -o its_unit_test.o its_unit_test.c -Wall -Wextra -Werror
	gcc -c -o test.o test.c -Wall -Wextra -Werror
	gcc -o test test.o its_unit_test.o -pthread -Wall -Wextra -Werror
run:
	@gcc -c -o its_unit_test.o its_unit_test.c -Wall -Wextra -Werror
	@gcc -c -o test.o test.c -Wall -Wextra -Werror
	@gcc -o test test.o its_unit_test.o -pthread -Wall -Wextra -Werror
	@./test || true
	@rm -f test.o
	@rm -f test
	@rm -f its_unit_test.o

clean:
	rm -f test.o
	rm -f test
	rm -f its_unit_test.o
