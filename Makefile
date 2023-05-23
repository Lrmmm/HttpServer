CXX = gcc
INCDIR = ./include
CXXFLAGS = -Wall -pthread -Wextra -I$(INCDIR) -O0 -g
SRCDIR = ./src
OBJDIR = ./output
BINDIR = ./output/bin
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/*.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/http_server

$(TARGET) : $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ && echo "Complie success!" || echo "Complie fail!"

$(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR) :
	mkdir -p $(OBJDIR)

$(BINDIR) :
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: clean