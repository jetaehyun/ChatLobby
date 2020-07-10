all: server.exe client.exe
build_dir = .build

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread

SOURCES1 = server.c
SOURCES2 = client.c

OBJECTS1 = $(join $(build_dir)/ ,$(SOURCES1:.c=.o)) 
OBJECTS2 = $(join $(build_dir)/ ,$(SOURCES2:.c=.o)) 

TARGET1 = client.exe
TARGET2 = server.exe

$(TARGET1) : $(OBJECTS1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET2) : $(OBJECTS2)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(build_dir)/%.o: %.c | $(build_dir)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: $(build_dir) clean 

$(build_dir): 
	@mkdir -p $@

clean:
	rm -f $(TARGET1) $(TARGET2) $(OBJECTS1) $(OBJECTS2)

