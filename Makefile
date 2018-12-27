output: lab4.o
	gcc lab4.o -o lab4
lab4.o: lab4.cc
	gcc -c lab4.cc
clean:
	rm *.o lab4
