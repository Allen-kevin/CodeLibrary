#make name=main

CFLAG=-g -Wall -std=c++11
LFLAG=-lpthread

SRC=$(name).cpp
TARGET=$(name)
$(TARGET):$(SRC)
	g++ $^ -o $@ $(CFLAG) $(LFLAG)

.PHONY:clean
clean:
	rm $(TARGET)
