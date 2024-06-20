CC=gcc
CFLAG =-std=c11 -Wall -Werror -Wextra


CPP_FILES = $(wildcard *.cpp *.cxx *.c++ *.cc *.cp *.c)
INC_FILES = $(wildcard *.i *.ii *.h *.h++ *.hpp *.hxx *.hh *.inl *.inc *.ipp *.ixx *.txx *.tpp *.tcc *.tpl)

.PHONY: format clean all test
	clang-format -i $(CPP_FILES) $(INC_FILES)

all: s21_string.a test gcov_report

test: test s21_string.a
	$(CC) $(CFLAG) test.c  -lm -lchek -lpthread -D MAC -o test
	./test

s21_string: s21_string.c
	$(CC) $(CFLAG) s21_string.c -o s21_string

s21_string.a :
	$(CC) -c s21_*.c
	ar rcs s21_string.a *.o

test :  s21_string.a
	$(CC) -c test.c -o test_1.o
	$(CC) test_1.o s21_string.a -lcheck -o test_2.o
	./test_2.o
gcov_report:
	$(CC) -coverage test.c -o s21_test s21_string.c\ s21_sprintf.c\ s21_sscanf.c\ -lcheck
	./s21_test
	icov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info


clean:
	rm -f ./uni_test ./s21_test ./*.a
	rm -f report/
	rm -f test
	rm -f a.*
	rm -f ./*.gcnc ./.gcda ./s21_test.info ./s21_string.gcno ./s21_string.gcda ./*.o

rebuild: clean all



