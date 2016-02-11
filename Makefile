# Robert McKenna
# remckenna@wpi.edu
# Distributes Systems : Project 1

# Generic makefile for project 1 and all further projects.

# Compiler Variables


all: 
	cd Client && $(MAKE)

clean:
	cd Client && $(MAKE) clean

.PHONY: clean
