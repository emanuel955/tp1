COMP = gcc
OBJS =  $(patsubst %.c,%.o,$(wildcard *.c)) # Todos los .o en carpeta
HEADERS = $(patsubst %.c,%.h,$(wildcard *.c)) # Todos los .h en carpeta
CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror
EXE = pruebas
# Si escribo make se ejecuta las reglas que indique a continuacion
all:	compilar	valgrind	clean_all
# Correr el EXE
run:	$(EXE)
	./$(EXE)
# Compila el programa principal
compilar:	$(OBJS)
	$(COMP)	$(CFLAGS)	-o	$(EXE)	$(OBJS)
# Genera los archivos.o individuales
%.o:	%.c	$(HEADERS)
# Elimina los archivos.o
clean:
	rm -f $(OBJS)
# Elimina el archivo EXE
clean_exe:
	rm -f $(EXE)
# Elimina los arhivos o y el EXE
clean_all:	clean	clean_exe
# Correr pruebas con valgrind
valgrind:	$(EXE)
	valgrind	--leak-check=full	--track-origins=yes	--show-reachable=yes	./$(EXE)
# Corre pruebas con valgreen (valgrind de la catedra)
valgreen:	$(EXE)
	valgreen	./$(EXE)
