
copy:
	@cp /bin/ls .

build: copy
	@gcc injector.c -o injector -fno-stack-protector -z execstack


run: build
	@./injector ls a.out