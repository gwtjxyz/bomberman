PROGRAM=udaviyur

CC=g++
CFLAGS = -std=c++14 -Wall -pedantic -Wextra -g -Wno-long-long -O0 -ggdb
CURSES = -lncurses

all: objs compile doc

run: compile
	./$(PROGRAM)

compile: objs $(PROGRAM)

doc: 
	doxygen Doxyfile

$(PROGRAM): objs/CGame.o objs/CGameEntityBomb.o objs/CGameEntityNPC.o \
 objs/CGameEntityPC.o objs/CLevelBuilder.o objs/CMenu.o \
 objs/CWindow.o objs/main.o
	$(CC) $(CFLAGS) $^ $(CURSES) -o $@
	
	
	
objs/CGame.o: src/CGame.cpp src/CGame.hpp src/CConstants.hpp \
 src/CLevelBuilder.hpp src/CGameEntityPC.hpp src/CGameEntity.hpp \
 src/CGameEntityNPC.hpp src/CGameEntityBomb.hpp
	$(CC) $(CFLAGS) -c $< $(CURSES) -o $@ 
	
objs/CGameEntityBomb.o: src/CGameEntityBomb.cpp src/CGameEntityBomb.hpp \
 src/CGameEntity.hpp src/CConstants.hpp
	$(CC) $(CFLAGS) -c $< $(CURSES) -o $@ 
	
objs/CGameEntityNPC.o: src/CGameEntityNPC.cpp src/CGameEntityNPC.hpp \
 src/CGameEntity.hpp src/CConstants.hpp
	$(CC) $(CFLAGS) -c $< $(CURSES) -o $@ 
	
objs/CGameEntityPC.o: src/CGameEntityPC.cpp src/CGameEntityPC.hpp \
 src/CGameEntity.hpp src/CConstants.hpp
	$(CC) $(CFLAGS) -c $< $(CURSES) -o $@
	
objs/CLevelBuilder.o: src/CLevelBuilder.cpp src/CLevelBuilder.hpp \
 src/CConstants.hpp
	$(CC) $(CFLAGS) -c $< $(CURSES) -o $@ 
	
objs/CMenu.o: src/CMenu.cpp src/CMenu.hpp src/CGame.hpp src/CConstants.hpp \
 src/CLevelBuilder.hpp src/CGameEntityPC.hpp src/CGameEntity.hpp \
 src/CGameEntityNPC.hpp src/CGameEntityBomb.hpp src/CWindow.hpp
	$(CC) $(CFLAGS) -c $< $(CURSES) -o $@ 
	
objs/CWindow.o: src/CWindow.cpp src/CWindow.hpp
	$(CC) $(CFLAGS) -c $< $(CURSES) -o $@
	
objs/main.o: src/main.cpp src/CMenu.hpp src/CGame.hpp src/CConstants.hpp \
 src/CLevelBuilder.hpp src/CGameEntityPC.hpp src/CGameEntity.hpp \
 src/CGameEntityNPC.hpp src/CGameEntityBomb.hpp src/CWindow.hpp
	$(CC) $(CFLAGS) -c $< $(CURSES) -o $@
	
	
objs: 
	mkdir objs

clean:
	rm -rf $(PROGRAM) objs/ doc/ 2>/dev/null
