compile_test:
	@gcc test.c -fno-stack-protector -z execstack -no-pie

build: compile_test
	@gcc injector.c -o injector -fno-stack-protector -z execstack -w

run: build
	@./injector a.out test
