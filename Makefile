all:
	gcc main.c parser.c parser64.c -I ./ -o elfread
