all: dir date shell

dir: dir.c
	gcc -o dir dir.c

date: date.c
	gcc -o date date.c

shell: shell.c
	gcc -o shell shell.c

clean:
	rm -f shell date dir