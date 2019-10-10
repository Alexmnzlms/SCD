compilador:=g++
opcionesc:= -std=c++11 -pthread -Wfatal-errors
hmonsrcs:= HoareMonitor.h HoareMonitor.cpp Semaphore.h Semaphore.cpp

all: fumadoresAML_exe barberoAML_exe 

x1: fumadoresAML_exe
	./$<

x2: barberoAML_exe
	./$<

fumadoresAML_exe: fumadoresAML.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

barberoAML_exe: barberoAML.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

clean:
	rm -f *_exe
