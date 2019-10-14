CC = cl
PROGRAM_NAME = battle-system.exe
FILE = main.cpp
DEPENDENCIES = Inventory.cpp Character.cpp Item.cpp

main:
	$(CC) $(FILE) $(DEPENDENCIES) -o $(PROGRAM_NAME)
