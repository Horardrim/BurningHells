all:
	@make -C main

clean:
	@rm -rf output
	@rm -f `find -name "*.gcov"` main/valgrind.log

test:
	@make -C main test

gencov:
	@make -C main gencov

.PHONY: all clean test gencov
