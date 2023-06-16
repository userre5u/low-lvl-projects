# shellcodeGames
reverse_shell-x64.asm -> x64 intel reverse shell client

usage:
1) make build
2) run 'nc -lv 4444' (on terminal 'A')
3) run ./reverse_shell-x64 (on terminal 'B')


# TODO: 
1) add packer to x64 reverse shell (the client will land "encrypted" in memory and will be **decrypted** during **runtime**)
2) write reverse shell based on ARMv8
