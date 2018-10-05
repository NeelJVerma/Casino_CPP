CC = g++
OPTS = -c -g -std=c++14 -Wall

# Project name
PROJECT = casino

# Directories
OBJDIR = obj
SRCDIR = src

# Libraries
LIBS = -lstdc++

# Files and folders
SRCS    = $(shell find $(SRCDIR) -name '*.cc')
SRCDIRS = $(shell find . -name '*.cc' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
OBJS    = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRCS))

# Targets
casino: builddevrepo $(OBJS)
	@echo "*** Linking object files"
	$(CC) $(OBJS) $(LIBS) -o bin/$@
	@echo "**** Running binary"
	bin/$@

obj/%.o: src/%.cc
	@echo "**** Creating object files"
	$(CC) $(OPTS) -c $< -o $@

clean:
	@echo "**** Removing all files"
	rm $(PROJECT) $(OBJDIR) -Rf
	@echo "**** Reconstructing bin folder"
	rm $(PROJECT) bin -Rf
	mkdir $(PROJECT) bin

install:
	@echo "*** Installing app"
	mv bin/casino  ~/usr/bin/

builddevrepo:
	@echo "**** Building repository"
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef