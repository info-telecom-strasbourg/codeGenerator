all:
	gcc -c -o its_unit_test.o tests/its_unit_test.c -Wall -Wextra -Werror -g
	gcc -c -o binary_op.o binary_op.c -Wall -Wextra -Werror -g
	gcc -c -o test.o tests/test.c -I . -Wall -Wextra -Werror -g
	gcc -o test test.o its_unit_test.o binary_op.o -pthread -Wall -Wextra -Werror -g

test:
	@gcc -c -o its_unit_test.o tests/its_unit_test.c -Wall -Wextra -Werror -g
	@gcc -c -o binary_op.o binary_op.c -Wall -Wextra -Werror -g
	@gcc -c -o test.o tests/test.c -I . -Wall -Wextra -Werror -g
	@gcc -o test test.o its_unit_test.o binary_op.o -pthread -Wall -Wextra -Werror -g
	@echo "classic test"
	@./test || true
	@echo "memory test"
	@valgrind -q ./test || true
	@rm -f test.o
	@rm -f its_unit_test.o
	@rm -f binary_op.o
	@rm -f test

clean:
	rm -f test.o
	rm -f its_unit_test.o
	rm -f binary_op.o
	rm -f test
