TARGET = sumvect

SOURCES = main.c utils.c add.c

CFLAGS = -g -Wno-deprecated-declarations -DCL_TARGET_OPENCL_VERSION=220
LDFLAGS = -lOpenCL

CC = gcc
DEL = rm -f

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	$(DEL) $(OBJECTS) $(TARGET)

