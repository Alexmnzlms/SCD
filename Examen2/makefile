compilador:=g++
opcionesc:= -std=c++11 -pthread -Wfatal-errors
hmonsrcs:= HoareMonitor.h HoareMonitor.cpp Semaphore.h Semaphore.cpp

all: x1 x2

x1: prodcons_ex_exe
	./$< 4 4

x2: ejercicio2_exe
	./$<

prodcons_ex_exe: prodcons_ex.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

ejercicio2_exe: ejercicio2.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp
