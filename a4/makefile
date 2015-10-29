COMPILER = gcc

# My program file is cars.c...
SOURCE = main.c
# ...and I want my output to go to cars
OUTPUT = main

# When make is run with no arguments, do this stuff
all:
	$(COMPILER) -o $(OUTPUT) $(SOURCE)

# Clean up after ourselves
clean:
	# Note to self: don't do rm -rf /
	rm -f $(OUTPUT)


