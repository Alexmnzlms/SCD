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

const int num_clientes = 10;
mutex escribir;

//**********************************************************************
// Monitor
class Barberia : public HoareMonitor
{
private:
   CondVar barbero, clientes, silla;

public:                    
   Barberia(  ) ;
   void CortarPelo(int i);
   void SiguienteCliente();
   void FinCliente();
} ;
//-----------------------------------------------------------------------------
Barberia::Barberia(  )
{
   barbero = newCondVar();
   clientes = newCondVar();
   silla = newCondVar();
}
//-----------------------------------------------------------------------------
void Barberia::CortarPelo( int i )
{
	escribir.lock();
	cout << "Cliente  " << i << ": entra a la barberia" << endl;
   escribir.unlock();
   while (!silla.empty()){
      escribir.lock();
      cout << "Cliente  " << i << ": el barbero esta ocupado, me esperare" << endl;
      escribir.unlock();
      clientes.wait();
   }
   if (!barbero.empty()){
      escribir.lock();
      cout << "Cliente  " << i << ": el barbero esta dormido, ¡¡DESPIERTE!!" << endl;
      escribir.unlock();
      barbero.signal();
   }
   escribir.lock();
   cout << "Cliente  " << i << ": ¡me toca!" << endl;
   escribir.unlock();
   silla.wait();
}
//-----------------------------------------------------------------------------
void Barberia::SiguienteCliente(  )
{
   if (clientes.empty() && silla.empty()){
      escribir.lock();
      cout << "Barbero   : no hay nadie, hora de dormir" << endl;
      escribir.unlock();
      barbero.wait();
   }
   escribir.lock();
   cout << "Barbero   : Manos a la obra, ¡¡¡QUE PASE EL SIGUIENTE!!!" << endl;
   escribir.unlock();
   clientes.signal();
   
}
//-----------------------------------------------------------------------------
void Barberia::FinCliente(  )
{
   escribir.lock();
   cout << "Barbero   : ya he acabado con este cliente" << endl;
   escribir.unlock();
   silla.signal();
}
//----------------------------------------------------------------------
// función que simula cortarle el pelo un cliente
void CortarPeloACliente()
{
   chrono::milliseconds duracion_corte( aleatorio<20,200>() ); 
   // informa de que comienza a cortar
   escribir.lock();
   cout << "Barbero   : comienzo a cortar el pelo a un cliente (" 
   << duracion_corte.count() << " milisegundos)" << endl; // espera bloqueada un tiempo igual a ''duracion_corte' en milisegundos
   escribir.unlock();
   this_thread::sleep_for( duracion_corte );
   // informa de que ha terminado de cortar
   escribir.lock();
   cout << "Barbero   : ya he acabado de cortar el pelo" << endl;
   escribir.unlock();
}
//----------------------------------------------------------------------
// función que simula la espera fuera de la barberia
void EsperarFueraBarberia( int i )
{
   chrono::milliseconds duracion_esperar( aleatorio<20,200>() ); 
   // informa de que comienza a esperar
   escribir.lock();
   cout << "Cliente  " << i << ":" << " se va hasta necesitar otro corte de pelo (" 
   << duracion_esperar.count() << " milisegundos)" << endl; // espera bloqueada un tiempo igual a ''duracion_esperar' en milisegundos
   escribir.unlock();
   this_thread::sleep_for( duracion_esperar);
   // informa de que ha terminado de esperar
   escribir.lock();
   cout << "Cliente  " << i << ": vuelve a necesitar un corte de pelo" << endl;
   escribir.unlock();
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del Barbero

void funcion_hebra_barbero( MRef<Barberia> barberia )
{
   while(true){
      barberia->SiguienteCliente();
      CortarPeloACliente();
      barberia->FinCliente();
   }
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del cliente
void  funcion_hebra_cliente( MRef<Barberia> barberia, int num_cliente )
{
   while(true){
      barberia->CortarPelo(num_cliente);
      EsperarFueraBarberia(num_cliente);
   }
}
//----------------------------------------------------------------------

int main()
{
  cout << "-------------------------------" << endl
      << "Problema del barbero durmiente." << endl
      << "Numero de clientes: " << num_clientes << endl
      << "--------------------------------" << endl
      << flush ;

   MRef<Barberia> barberia = Create<Barberia>( );

	thread hebra_barbero ( funcion_hebra_barbero, barberia );
   thread hebras_clientes[num_clientes];
	for (int i = 0; i < num_clientes; i++){
		hebras_clientes[i] = thread( funcion_hebra_cliente, barberia, i );
	}

  	hebra_barbero.join() ;
	for (int i = 0; i < num_clientes; i++){
		hebras_clientes[i].join();
	}
  
}