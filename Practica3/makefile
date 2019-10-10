.SUFFIXES:
.PHONY:    pc,pc2,pcm,fp,f,fi,fc,clean

compilador := mpicxx -std=c++11
flagsc     := -g -Wall

pc: prodcons_exe
	mpirun -np  3 ./$<

pc2: prodcons2_exe
	mpirun -np  3 ./$<

pcm: prodcons2-mu_exe
	mpirun -np  10 ./$<

fp: filosofos-plantilla_exe
	mpirun -np 10 ./$<

fi: filosofos-interb_exe
	mpirun -np 10 ./$<

f: filosofos_exe
	mpirun -np 10 ./$<

fc: filosofos-cam_exe
	mpirun -np 11 ./$<

%_exe: %.cpp
	$(compilador) $(flagsc) -o $@ $<

clean:
	rm -rf *_exe *.dSYM
