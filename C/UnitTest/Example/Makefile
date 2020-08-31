all:
	gcc -c -o its_unit_test.o its_unit_test.c -Wall -Wextra -Werror
	gcc -c -o test.o test.c -Wall -Wextra -Werror
	gcc -o test test.o its_unit_test.o -pthread -Wall -Wextra -Werror
run:
	@gcc -c -o its_unit_test.o its_unit_test.c -Wall -Wextra -Werror
	@gcc -c -o test.o test.c -Wall -Wextra -Werror
	@gcc -o test test.o its_unit_test.o -pthread -Wall -Wextra -Werror
	@./test || true
