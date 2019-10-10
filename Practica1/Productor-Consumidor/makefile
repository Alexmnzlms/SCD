#Autor: ALEJANDRO MANZANARES LEMUS
# Grupo A1 2º GII

SRC = .
BIN = .
OBJ = .

#ALL

all: cabecera\
$(BIN)/Productor-Consumidor-[LIFO]-AML\
confirm

#COMPILACION

#Ejecutables
#................................................
$(BIN)/Productor-Consumidor-[LIFO]-AML: $(OBJ)/Productor-Consumidor-[LIFO]-AML.o $(OBJ)/Semaphore.o
	@echo "---------------------------------------------------------------------"
	g++ -o $@ $^ -std=c++11 -pthread -I.
	rm $^
	@echo "\n"
	@echo "----------------------Productor-Consumidor-[LIFO]-AML-------------------------"

#Objetos
#................................................
$(OBJ)/Productor-Consumidor-[LIFO]-AML.o: $(SRC)/Productor-Consumidor-[LIFO]-AML.cpp
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
	$(BIN)/Productor-Consumidor-[LIFO]-AML

confirm:
	@echo "---------------------------------------------------------------------"
	@echo "		Todas las acciones se han ejecutado correctamente"
	@echo "---------------------------------------------------------------------"

