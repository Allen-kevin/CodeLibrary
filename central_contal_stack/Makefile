#make name=main

CFLAG=-g -Wall
LFLAG=-lpthread

SRC=$(name).c
TARGET=$(name)
$(TARGET):$(SRC)
	cc $^ -o $@ $(CFLAG) $(LFLAG)

.PHONY:clean
clean:
	rm $(TARGET)
