all:
	gcc -g main.c parser64.c print64.c -I ./ -o elfread
