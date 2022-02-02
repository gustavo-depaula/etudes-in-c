build:
	gcc src/*.c -o exec

run-only:
	./exec

run: build run-only
