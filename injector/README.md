# Injector

Injector is designed to infect binaries (of type elf) that have enough space (for our poison) between the text section and the data section.
the program support both single file and entire directory infection.


### Explanation (copy commands from 'usage' section below ):
    1. compile a test C source file and the injector ( with default poison )
    2. extract the opcodes from poison file after compilation since we cant inject C source code to memory
    3. test the a.out file without infection
    4. execute main with 2 arguments, the first argument can be a file or a directory to be infected, the second argument is our poison
    5. test the a.out file AFTER infection :)

### usage:
   
    1) make test
    
    2) echo -en $(objdump -dM intel -j .text poison | cut -f2 | sed 1,7d | sed 's/.*:.*//g' | tr '\n' ' ' | xargs | sed 's/[[:space:]]/\\\x/g' | sed 's/^/\\\x/') > inject 
    
    3) run ./a.out and see the message
    
    4) ./main a.out inject
    
    5) run ./a.out and see the NEW message

    
The following video shows how to infect a copy of the /bin/ folder


[label](poc.mp4)