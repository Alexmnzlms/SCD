#Autor: ALEJANDRO MANZANARES LEMUS
# Grupo A1 2º GII

SRC = .
BIN = .
OBJ = .

#ALL

all: cabecera\
$(BIN)/Fumadores-AML\
confirm

#COMPILACION

#Ejecutables
#................................................
$(BIN)/Fumadores-AML: $(OBJ)/Fumadores-AML.o $(OBJ)/Semaphore.o
	@echo "---------------------------------------------------------------------"
	g++ -o $@ $^ -std=c++11 -pthread -I.
	rm $^
	@echo "\n"
	@echo "----------------------Fumadores-AML-------------------------"

#Objetos
#................................................
$(OBJ)/Fumadores-AML.o: $(SRC)/Fumadores-AML.cpp
	@echo "---------------------------------------------------------------------"
	g++ -c -o $@ $^ -std=c++11 -pthread -I.
	@echo "---------------------------------------------------------------------"

$(OBJ)/Semaphore.o: $(SRC)/Semaphore.cpp
	@echo "---------------------------------------------------------------------"
	g++ -c -o $@ $^ -std=c++11 -pthread -I.
	@echo "---------------------------------------------------------------------"

#UTILIDADES

cabecera :
	@echo "\n"
	@echo "---------------------------------------------------------------------"
	@echo "		Autor: Alejandro Manzanares Lemus"
	@echo "		Grupo A1 2º GII"
	@echo "---------------------------------------------------------------------"

ejecutar: 
	$(BIN)/Fumadores-AML

confirm:
	@echo "---------------------------------------------------------------------"
	@echo "		Todas las acciones se han ejecutado correctamente"
	@echo "---------------------------------------------------------------------"

