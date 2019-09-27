padre: main.c funct.c structs.c 
	gcc -o padre main.c funct.c structs.c -I.
	gcc -o hijo hijo.c -I.

