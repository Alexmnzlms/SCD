///////////////////////////////////////////////////////////////////////////////
// Alejandro Manzanares Lemus
// 2º A1 GII
///////////////////////////////////////////////////////////////////////////////

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

const int num_fumadores = 3;
mutex escribir;

//**********************************************************************
// Monitor
class Estanco : public HoareMonitor
{
private:
	int mostrador;
  	CondVar esperando_recogida ;
	CondVar esperando_fumar[num_fumadores] ;     

public:                    
   Estanco(  ) ;
   void obtenerIngrediente(int ingr);
   void ponerIngrediente(int ingr	);
   void esperarRecogidaIngrediente();

} ;
//-----------------------------------------------------------------------------
Estanco::Estanco(  )
{
	mostrador = -1;
	esperando_recogida = newCondVar();
	for(int i = 0; i < num_fumadores; i++){
		esperando_fumar[i] = newCondVar();
	}
}
//-----------------------------------------------------------------------------
void Estanco::obtenerIngrediente(int ingr)
{
	if(mostrador != ingr){
    	escribir.lock();
   	cout << "Fumador  " << ingr << "  :No es mi ingrediente, esperaré" << endl;
		escribir.unlock();
		esperando_fumar[ingr].wait();
	}
	mostrador = -1;
	escribir.lock();
   cout << "Fumador  " << ingr << "  :Acabo de recoger mi ingrediente" << endl;
	escribir.unlock();
	esperando_recogida.signal();
}	
//-----------------------------------------------------------------------------
void Estanco::ponerIngrediente(int ingr)
{	
	mostrador = ingr;
	escribir.lock();
   cout << "Estanquero  :Ingrediente " << ingr << " puesto" << endl;
	escribir.unlock();
	esperando_fumar[ingr].signal();
}
//-----------------------------------------------------------------------------
void Estanco::esperarRecogidaIngrediente()
{
	if (mostrador != -1){
		escribir.lock();
   	cout << "Estanquero  :Espero a que el ingrediente sea retirado" << endl;
		escribir.unlock();
		esperando_recogida.wait();
	}
}
//----------------------------------------------------------------------
// función que produce los datos
int producirIngrediente( )
{
   int ingrediente_aleatorio( aleatorio<0,num_fumadores-1>() );
   chrono::milliseconds duracion_preparar( aleatorio<20,200>() );
	escribir.lock();
   cout << "Estanquero  :Comienzo la preparacion del ingrediente " << ingrediente_aleatorio << " (" 
   << duracion_preparar.count() << " milisegundos)" << endl;
	escribir.unlock();
   this_thread::sleep_for( duracion_preparar );
   return ingrediente_aleatorio;
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero
void funcion_hebra_estanquero( MRef<Estanco> estanco )
{
  int ingrediente;
  while( true )
  {
     ingrediente = producirIngrediente();
     estanco->ponerIngrediente(ingrediente);
     estanco->esperarRecogidaIngrediente();
  }
}
//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra
void fumar( int num_fumador )
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );
   // informa de que comienza a fumar
	escribir.lock();
   cout << "Fumador  " << num_fumador << "  :"
   << "Empiezo a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;
	escribir.unlock();
   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );
   // informa de que ha terminado de fumar
	escribir.lock();
   cout << "Fumador  " << num_fumador << "  :Termino de fumar, comienzo espera de ingrediente." << endl;
	escribir.unlock();
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( MRef<Estanco> estanco, int num_fumador )
{
   while( true )
   {
      estanco->obtenerIngrediente(num_fumador);
      fumar(num_fumador);
   }
}
//----------------------------------------------------------------------

int main()
{
  cout << "--------------------------" << endl
      << "Problema de los fumadores." << endl
      << "--------------------------" << endl
      << flush ;

   MRef<Estanco> estanco = Create<Estanco>( );
  
	thread hebra_estanquero ( funcion_hebra_estanquero, estanco );
	thread hebras_fumador[num_fumadores];
	for (int i = 0; i < num_fumadores; i++){
		hebras_fumador[i] = thread( funcion_hebra_fumador, estanco, i );
	}

  	hebra_estanquero.join() ;
	for (int i = 0; i < num_fumadores; i++){
		hebras_fumador[i].join();
	}

  
}