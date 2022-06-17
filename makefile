run: clean
	@./main

clean: link
	@rm *.o

link: object
	@gcc -lstdc++ *.o -o main

object:
	@gcc -std=c++11 -c *.cpp
