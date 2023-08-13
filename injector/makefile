build:
	@gcc main.c -o main -fno-stack-protector -z execstack -w

run: build
	@./main binaries poison
