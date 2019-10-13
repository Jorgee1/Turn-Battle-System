CC = cl
PROGRAM_NAME = main.exe
FILE = main.cpp

main: $(FILE)
	$(CC) $(FILE) -o $(PROGRAM_NAME)
