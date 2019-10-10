#Autor: ALEJANDRO MANZANARES LEMUS
# Grupo A1 2º GII

SRC = src
BIN = bin
OBJ = obj

#ALL

all: cabecera\
help\
$(BIN)/Calcular_Pi_AML_A1_2GII\
clean\
confirm

#COMPILACION

#Ejecutables
#................................................
$(BIN)/Calcular_Pi_AML_A1_2GII: $(OBJ)/Calcular_Pi_AML_A1_2GII.o
	@echo "---------------------------------------------------------------------"
	g++ -o $@ $^ -std=c++11 -pthread
	@echo "\n"
	@echo "----------------------Calcular_Pi_AML_A1_2GII-------------------------"

#Objetos
#................................................
$(OBJ)/Calcular_Pi_AML_A1_2GII.o: $(SRC)/Calcular_Pi_AML_A1_2GII.cpp
	@echo "---------------------------------------------------------------------"
	g++ -c -o $@ $^ -std=c++11 -pthread
	@echo "---------------------------------------------------------------------"

#UTILIDADES

cabecera :
	@echo "\n"
	@echo "---------------------------------------------------------------------"
	@echo "		Autor: Alejandro Manzanares Lemus"
	@echo "		Grupo A1 2º GII"
	@echo "---------------------------------------------------------------------"

ejecutar: 
	$(BIN)/Calcular_Pi_AML_A1_2GII

clean:
	@-rm $(OBJ)/*
	@echo "Objetos eliminados"

mr_proper:
	@-rm $(BIN)/*
	@echo "Ejecutables eliminados"

comprime: mr_proper
	tar -cvf Calcular_Pi_AML_A1_2GII.tar makefile obj/ lib/ bin/ include/ src/

confirm:
	@echo "---------------------------------------------------------------------"
	@echo "		Todas las acciones se han ejecutado correctamente"
	@echo "---------------------------------------------------------------------"

help:
	@echo "---------------------------------------------------------------------"
	@echo "		Comandos disponibles:"
	@echo "		ejecutar  -> ejecuta los ejercios"
	@echo "		clean 	  -> elimina los objetos intermedios creados"
	@echo "		mr_proper -> elimina los ejecutables"
	@echo "		comprime  -> comprime en un tar las carpetas de obj lib src"
	@echo "		             include y bin asi como el makefile"
	@echo "		help      -> muestra este panel de ayuda"
	@echo "		cabecera  -> muestra datos del autor"
	@echo "---------------------------------------------------------------------"
