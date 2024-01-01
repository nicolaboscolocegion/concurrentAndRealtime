all: mem_alloc

mem_alloc: project.c queue.c 
	${CC} -O2 -g3  $^ -o $@ 
