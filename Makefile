# Commands
MAKE = make

# Target
all: lib app

# Compile and run the programme
all_do: all run

# Run the programme
run: 
	./bin/app

# Launch driver
driver:
	./bin/driver

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