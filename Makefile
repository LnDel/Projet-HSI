# Commands
MAKE = make

# Target
all: lib app

# Library compilation
lib:
	$(MAKE) -f Makefile_lib

# Application compilation
app:
	$(MAKE) -f Makefile_app

clean:
	$(MAKE) -f Makefile_lib clean
	$(MAKE) -f Makefile_app clean

.PHONY: all lib app clean