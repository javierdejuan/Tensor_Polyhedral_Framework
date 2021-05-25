gcc -ggdb -o tensor tensor.c tuple.c stacks.c -fsanitize=address -fno-omit-frame-pointer $1
