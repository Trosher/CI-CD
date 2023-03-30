CC= gcc -std=c11 -D_GNU_SOURCE
WWW= -Wall -Wextra -Werror
CAT= s21_cat.c

all: uninstall s21_cat clean style autotest

s21_cat:
	$(CC) $(WWW) $(CAT) -o s21_cat

test:
	chmod 777 t_cat.sh
	./t_cat.sh

style:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	cp ../../materials/linters/.clang-format ./
	clang-format -i s21_cat.c
	rm .clang-format

uninstall:
	rm -rf s21_cat

clean:
	rm -rf *.a *.out *.o log.txt
