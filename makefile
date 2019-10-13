CC = cl
PROGRAM_NAME = battle-system.exe
FILE = main.cpp

main: $(FILE)
	$(CC) $(FILE) -o $(PROGRAM_NAME)
