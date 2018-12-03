CC=g++
CFLGAS = -1
DEPS = BinaryTree.h algorithm.h ArrayStack.h LinkStack.h ArrayQueue.h LinkQueue.h
OBJ = main.o BinaryTree.o sort.o subarry.o heap.o baseclass.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
main:$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f main *.o
