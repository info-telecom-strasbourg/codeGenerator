all:
	g++ -c -o its_unit_test.o its_unit_test.cpp -Wall -Wextra -Werror
	g++ -c -o test.o test.cpp -Wall -Wextra -Werror
	g++ -o test test.o its_unit_test.o -pthread -Wall -Wextra -Werror
run:
	@g++ -c -o its_unit_test.o its_unit_test.cpp -Wall -Wextra -Werror
	@g++ -c -o test.o test.cpp -Wall -Wextra -Werror
	@g++ -o test test.o its_unit_test.o -pthread -Wall -Wextra -Werror
	@./test || true
	@rm -f its_unit_test.o
	@rm -f test.o
	@rm -f test

clean:
	rm -f its_unit_test.o
	rm -f test.o
	rm -f test