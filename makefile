CC = gcc
CFLAGS = -Wall -W -pedantic -O2 -std=c11 $(shell pkg-config --cflags MLV)
LIBS = $(shell pkg-config --libs-only-other --libs-only-L MLV) $(shell pkg-config --libs-only-l MLV)

OBJDIR = obj
SRCDIR = src
TARGET = projet

SRCS = $(SRCDIR)/main.c $(SRCDIR)/lutins.c $(SRCDIR)/plannings.c $(SRCDIR)/fichiers.c $(SRCDIR)/ihm.c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)

