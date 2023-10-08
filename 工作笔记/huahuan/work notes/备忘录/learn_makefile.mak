
#..........version 1..........

edit : main.o kbd.o command.o display.o \
			insert.o search.o files.o utils.o
		cc -o edit main.o kbd.o command.o display.o \
				insert.o search.o files.o utils.o

main.o : main.c defs.h
		cc -c main.c
kbd.o : kbd.c defs.h command.h
		cc -c kbd.cc
command.o : command.c defs.h command.h
		cc -c command.c
display.o : display.c defs.h buffer.h
		cc -c display.c
insert.o : insert.c defs.h buffer.h
		cc -c insert.c
search.o : search.c defs.h buffer.h
		cc -c search.c
files.o : files.c defs.h buffer.h command.h
		cc -c files.c
utils.o : utils.c defs.h
		cc -c utils.c

clean : 
		rm edit main.o kbd.o command.o display.o \
			insert.o search.o files.o utils.o
			
			
#..........version 2..........

object = main.o kbd.o command.o display.o \
			insert.o search.o files.o utils.o

edit : $(object)
		cc -o edit $(object)

main.o : main.c defs.h
		cc -c main.c
kbd.o : kbd.c defs.h command.h
		cc -c kbd.cc
command.o : command.c defs.h command.h
		cc -c command.c
display.o : display.c defs.h buffer.h
		cc -c display.c
insert.o : insert.c defs.h buffer.h
		cc -c insert.c
search.o : search.c defs.h buffer.h
		cc -c search.c
files.o : files.c defs.h buffer.h command.h
		cc -c files.c
utils.o : utils.c defs.h
		cc -c utils.c

clean : 
		rm edit $(object)
		
		
#..........version 3..........

object = main.o kbd.o command.o display.o \
			insert.o search.o files.o utils.o
cc = gcc

edit : $(object)
		cc -o edit $(object)

main.o : main.c defs.h
kbd.o : kbd.c defs.h command.h
command.o : command.c defs.h command.h
display.o : display.c defs.h buffer.h
insert.o : insert.c defs.h buffer.h
search.o : search.c defs.h buffer.h
files.o : files.c defs.h buffer.h command.h
utils.o : utils.c defs.h

.PHONY : clean
clean : 
		-rm edit $(object)
		
		
		

		



		
