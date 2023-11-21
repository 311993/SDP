LIBRARYREPO := simulator_libraries

ifeq ($(OS),Windows_NT)	
	SHELL := CMD
endif

all:
ifeq ($(OS),Windows_NT)	
	@cd $(LIBRARYREPO) && mingw32-make
else
	@cd $(LIBRARYREPO) && make
endif

clean:
ifeq ($(OS),Windows_NT)	
	@cd $(LIBRARYREPO) && mingw32-make clean
else
	@cd $(LIBRARYREPO) && make clean
endif