# Robert McKenna
# remckenna@wpi.edu
# Distributes Systems : Project 3

# Generic makefile for project 3 and all further projects.

# Compiler Variables
CC			=	g++
FILETYPE 	= 	cpp
CFLAGS 		= 	-Wall -g
MAIN 		= 	../nutella_client
LIBS		=


# Directory names
DIR_INCLUDES	:= Include
DIR_SOURCES		:= Src
DIR_OBJS		:= Objects
DIR_COMMON		:= ../Common

# Makefile inputs
INCLUDES 	:= -I $(DIR_INCLUDES)
INCLUDES 	+= -I $(DIR_COMMON)/$(DIR_INCLUDES)

SOURCES 	:= $(shell find $(DIR_SOURCES) -type f -name *.$(FILETYPE))
OBJECTS 	:= $(patsubst $(DIR_SOURCES)/%,$(DIR_OBJS)/%,$(SOURCES:.$(FILETYPE)=.o))

COMMON_SRC	:= $(shell find $(DIR_COMMON)/$(DIR_SOURCES) -type f -name *.$(FILETYPE))
COMMON_OBJ	:= $(patsubst $(DIR_COMMON)/$(DIR_SOURCES)/%,$(DIR_COMMON)/$(DIR_OBJS)/%,$(COMMON_SRC:.$(FILETYPE)=.o))

all: $(MAIN)


$(MAIN): $(OBJECTS) $(COMMON_OBJ)
	@echo "Compiling..."
	$(CC) $^ -o $(MAIN) $(LIBS)

$(DIR_OBJS)/%.o: $(DIR_SOURCES)/%.$(FILETYPE)
	@mkdir -p $(DIR_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(DIR_COMMON)/$(DIR_OBJS)/%.o: $(DIR_COMMON)/$(DIR_SOURCES)/%.$(FILETYPE)
	@mkdir -p $(DIR_COMMON)/$(DIR_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	$(RM) -r $(DIR_OBJS) $(MAIN)
	$(RM) -r $(DIR_COMMON)/$(DIR_OBJS)

.PHONY: clean

