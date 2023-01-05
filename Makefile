c = compiled/ofiles

dirs:
	mkdir -p compiled
	mkdir -p compiled/ofiles

all : compiled/main dirs

# Links

compiled/main : $(c)/scheduler.o $(c)/manager.o $(c)/worker.o
	gcc $(c)/manager.o $(c)/worker.o $(c)/scheduler.o -pthread -o compiled/main

# Compilations

$(c)/manager.o : src/manager.c $(c)/buffer_lib.o $(c)/io_line_lib.o
	gcc -c src/manager.c -o $(c)/manager_compiled.o
	ld -relocatable $(c)/buffer_lib.o $(c)/io_line_lib.o $(c)/manager_compiled.o -o $(c)/manager.o

$(c)/worker.o : src/worker.c $(c)/buffer_lib.o $(c)/io_line_lib.o $(c)/remove_vowels_lib.o
	gcc -c src/worker.c -o $(c)/worker_compiled.o
	ld -relocatable $(c)/buffer_lib.o $(c)/io_line_lib.o $(c)/remove_vowels_lib.o $(c)/worker_compiled.o -o $(c)/worker.o

$(c)/manager.o : src/manager.c
	gcc -c src/manager.c -o $(c)/manager.o

$(c)/scheduler.o : src/scheduler.c
	gcc -c src/scheduler.c -o $(c)/scheduler.o

$(c)/buffer_lib.o : src/buffer/buffer.c
	gcc -c src/buffer/buffer.c -o $(c)/buffer_lib.o

$(c)/io_line_lib.o : src/io_line/io_line.c
	gcc -c src/io_line/io_line.c -o $(c)/io_line_lib.o

$(c)/remove_vowels_lib.o : src/remove_vowels/remove_vowels.c
	gcc -c src/remove_vowels/remove_vowels.c -o $(c)/remove_vowels_lib.o
