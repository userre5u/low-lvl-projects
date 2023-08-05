compile_packer:
	@gcc generate_payload.c -o generate_payload -masm=intel -z execstack -w

packer: compile_packer

compile_test:
	@echo "Run the ./test to test your generated shellcode..."
	@gcc test.c -o test -masm=intel -z execstack

	
test_shell: compile_test

