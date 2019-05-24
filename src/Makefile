CC = gcc
COMPILER_FLAGS = -std=c99 -fstack-protector-all
LINKER_FLAGS = -lm
EXEC_NAME = siguel
OBJECTS = main.o geometry.o object.o circle.o rectangle.o file_util.o static_list.o svg.o commands.o

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXEC_NAME) -Wall

main.o: main.c file_util.h modules/sig/object.h commands.h modules/data_structures/static_list.h
	$(CC) -c main.c $(COMPILER_FLAGS)

geometry.o: modules/sig/geometry.c modules/sig/geometry.h modules/sig/object.h
	$(CC) -c modules/sig/geometry.c $(COMPILER_FLAGS)

object.o: modules/sig/object.c modules/sig/object.h
	$(CC) -c modules/sig/object.c $(COMPILER_FLAGS)

circle.o: modules/sig/circle.c modules/sig/circle.h
	$(CC) -c modules/sig/circle.c $(COMPILER_FLAGS)

rectangle.o: modules/sig/rectangle.c modules/sig/rectangle.h
	$(CC) -c modules/sig/rectangle.c $(COMPILER_FLAGS)

file_util.o: file_util.c file_util.h
	$(CC) -c file_util.c $(COMPILER_FLAGS)

static_list.o: modules/data_structures/static_list.c modules/data_structures/static_list.h
	$(CC) -c modules/data_structures/static_list.c $(COMPILER_FLAGS)

svg.o: modules/sig/svg.c modules/sig/svg.h modules/sig/object.h modules/sig/circle.h modules/sig/rectangle.h
	$(CC) -c modules/sig/svg.c $(COMPILER_FLAGS)

commands.o: commands.c commands.h file_util.h modules/data_structures/static_list.h modules/sig/svg.h modules/sig/geometry.h modules/sig/object.h
	$(CC) -c commands.c $(COMPILER_FLAGS)

clean:
	rm -rf *.o
