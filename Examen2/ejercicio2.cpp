#include <iostream>
#include <cassert>
#include <thread>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "Semaphore.h"
#include "HoareMonitor.h"

using namespace std ;
using namespace HM ;

//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}
//**********************************************************************
// variables compartidas

const int num_alumno_SCD = 10;

//**********************************************************************
// Monitor
class Bareto : public HoareMonitor
{
private:
	int n_bebidas[2];

	CondVar camarero;
	CondVar esperando_bebidas[2];

public:                    
	Bareto();
	void pedir (int beb);
	void servir ();
} ;
//-----------------------------------------------------------------------------
Bareto::Bareto(  )
{
	for( int i = 0; i < 2; i++){
		n_bebidas[i] = 4;	
	}
	camarero = newCondVar();
	esperando_bebidas[0] = newCondVar();
	esperando_bebidas[1] = newCondVar();
}
//-----------------------------------------------------------------------------
void Bareto::pedir( int beb )
{
	if(n_bebidas[beb] == 0){
   	cout << "Alumno    :Despierte señor camarero" << endl;
		camarero.signal();
		esperando_bebidas[beb].wait();	
	}
	n_bebidas[beb]--;
	cout << "Alumno    :Cojo la bebida " << beb << ", aun quedan " << n_bebidas[beb] << endl;
	for(int i = 0; i < n_bebidas[beb]; i++){
		esperando_bebidas[beb].signal();
	}
}
//-----------------------------------------------------------------------------
void Bareto::servir()
{
	if(n_bebidas[0] != 0 && n_bebidas[1] != 0){
		cout << "Camarero  :Hay bebidas, a dormir" << endl;
		camarero.wait();	
	}
	cout << "Camarero  :Bebidas puestas de nuevo" << endl;
	n_bebidas[0] = 4;
	n_bebidas[1] = 4;
	esperando_bebidas[0].signal();
	esperando_bebidas[1].signal();
	
}
//----------------------------------------------------------------------
// función que simula la beber como cosacos
void Beber( )
{
	chrono::milliseconds duracion_beber( aleatorio<20,200>() );
   cout << "Alumno    :¡A beber como un cosaco! (" << duracion_beber.count() << " milisegundos)" << endl;
   this_thread::sleep_for( duracion_beber );
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del Barbero

void funcion_hebra_camarero( MRef<Bareto> bareto )
{
	while(true){
		bareto->servir();
	}
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del alumno_SCD
void  funcion_hebra_alumno_SCD( MRef<Bareto> bareto, int num_alumno_SCD )
{
	while(true){
		int tipo_bebida( aleatorio<0, 1>() );
		bareto->pedir( tipo_bebida );
		Beber();
	}
}
//----------------------------------------------------------------------

int main()
{
  cout << "-------------------------------" << endl
      << "Ejercicio 2 examen." << endl
      << "--------------------------------" << endl
      << flush ;

   MRef<Bareto> bareto = Create<Bareto>( );

	thread hebra_camarero ( funcion_hebra_camarero, bareto );
   thread hebras_alumno_SCD[num_alumno_SCD];
	for (int i = 0; i < num_alumno_SCD; i++){
		hebras_alumno_SCD[i] = thread( funcion_hebra_alumno_SCD, bareto, i );
	}

  	hebra_camarero.join() ;
	for (int i = 0; i < num_alumno_SCD; i++){
		hebras_alumno_SCD[i].join();
	}
  
}
