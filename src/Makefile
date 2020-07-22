all: server.exe client.exe
build_dir = .build

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread -pthread
# INC_FLAGS = -I/Server/header

SOURCES1 = \
	server.c\
	connection_thread.c\
	linked_list.c\
	log.c\
	control_thread.c\
	communication.c\

SOURCES2 = \
	client.c\
	read_thread.c\

OBJECTS1 = $(addprefix $(build_dir)/,$(SOURCES1:.c=.o)) 
OBJECTS2 = $(addprefix $(build_dir)/,$(SOURCES2:.c=.o)) 

TARGET1 = server.exe
TARGET2 = client.exe

$(TARGET1) : $(OBJECTS1)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS1) -o $@

$(TARGET2) : $(OBJECTS2)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS2) -o $@ 

$(build_dir)/%.o: %.c | $(build_dir)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: $(build_dir) clean 

$(build_dir): 
	@mkdir -p $@

clean:
	rm -f $(TARGET1) $(TARGET2) $(OBJECTS1) $(OBJECTS2)

VPATH = \
	Server\
	Client\
