#make name=server

CFLAG=-g -Wall
LFLAG=-lssl -lcrypto

SRC=ssl-$(name).c
TARGET=$(name)
$(TARGET):$(SRC)
	cc $^ -o $@ $(CFLAG) $(LFLAG)

.PHONY:clean
clean:
	rm $(TARGET)
