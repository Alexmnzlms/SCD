compilador:=g++ -g
opcionesc:= -std=c++11 -pthread -Wfatal-errors
hmonsrcs:= HoareMonitor.h HoareMonitor.cpp Semaphore.h Semaphore.cpp

all: x1 x2 x3 x4 clean

x1: prodcons1SCLIFO_exe
	./$<

x2: prodcons2SCFIFO_exe
	./$<

x3: prodcons3SULIFO_exe
	./$<

x4: prodcons4SUFIFO_exe
	./$<

prodcons1SCLIFO_exe: prodcons1SCLIFO.cpp 
	$(compilador) $(opcionesc)  -o $@ $<

prodcons2SCFIFO_exe: prodcons2SCFIFO.cpp 
	$(compilador) $(opcionesc)  -o $@ $< 

prodcons3SULIFO_exe: prodcons3SULIFO.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

prodcons4SUFIFO_exe: prodcons4SUFIFO.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

clean:
	rm -f *_exe
