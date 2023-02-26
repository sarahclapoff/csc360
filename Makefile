.PHONY all:
all: diskinfo disklist diskget diskput diskfix

#NOTE: to change parts to parts, just do a 
#replace all you silly goose.

diskinfo: parts.c
	gcc -Wall -D PART1 parts.c -o diskinfo
disklist: parts.c
	gcc -Wall -D PART2 parts.c -o disklist
diskget: parts.c
	gcc -Wall -D PART3 parts.c -o diskget
diskput: parts.c
	gcc -Wall -D PART4 parts.c -o diskput
diskfix: parts.c
	gcc -Wall -D PART5 parts.c -o diskfix

.PHONY clean:
clean:
	-rm diskinfo disklist diskget diskput diskfix



