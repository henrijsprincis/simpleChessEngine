ifeq ($(OS),Windows_NT) 
run: clean
	echo TODO: Windows makefile
else
run: clean
	@./main.out

clean: link
	@rm *.o

link: object
	@gcc -lstdc++ *.o -o main.out

object:
	@gcc -std=c++11 -c *.cpp
endif
