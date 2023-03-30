CC= gcc -std=c11 -D_GNU_SOURCE
WWW= -Wall -Wextra -Werror
GREP= s21_grep.c

all: uninstall s21_grep clean style autotest

s21_grep:
	$(CC) $(WWW) $(GREP) -o s21_grep

test:
	chmod 777 t_grep.sh
	./t_grep.sh

style:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	cp ../../materials/linters/.clang-format ./
	clang-format -i s21_grep.c
	rm .clang-format

uninstall:
	rm -rf s21_grep

clean:
	rm -rf *.a *.out *.o log.txt
