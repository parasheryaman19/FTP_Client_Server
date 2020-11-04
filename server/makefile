CC := gcc
CFLAGS := -Wall -g -Os

SHDIR := ../common

OBJS = ftpserver.o $(SHDIR)/common.o

all: ftpserver

ftpserver: $(OBJS)
	@$(CC) -o ftpserver $(CFLAGS) $(OBJS)

$(OBJS) : %.o: %.c 
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY:
clean:
	@rm -f *.o ftpserver
	@rm -f ../common/*.o
	@rm -f client.txt
	@echo Done cleaning
