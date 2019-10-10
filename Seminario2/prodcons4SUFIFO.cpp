#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>
#include <random>
#include "HoareMonitor.h"
#include "Semaphore.h"

using namespace std ;
using namespace HM ;

//-----------------------------------------------------------------------------
constexpr int num_items  = 40, productores = 4, consumidores = 2;     
mutex mtx ;  
unsigned cont_prod[num_items], cont_cons[num_items], producidos[productores]; 
//-----------------------------------------------------------------------------
template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}
//-----------------------------------------------------------------------------
int producir_dato( int i)
{
	if(producidos[i] < (num_items / productores)){
		static int contador = 0 ;
		this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
		mtx.lock();
		cout << "producido: " << contador << endl << flush ;
		producidos[i]++;
		mtx.unlock();
		cont_prod[contador] ++ ;
		return contador++;
	}
}
//-----------------------------------------------------------------------------
void consumir_dato( unsigned dato )
{
   if ( num_items <= dato )
   {
      cout << " dato === " << dato << ", num_items == " << num_items << endl ;
      assert( dato < num_items );
   }
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   mtx.lock();
   cout << "                  consumido: " << dato << endl ;
   mtx.unlock();
}
//-----------------------------------------------------------------------------
void ini_contadores()
{
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  cont_prod[i] = 0 ;
      cont_cons[i] = 0 ;
   }
}
//-----------------------------------------------------------------------------
void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." << flush ;

   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      if ( cont_prod[i] != 1 )
      {
         cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {
         cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}
//-----------------------------------------------------------------------------
class ProdCons4SU : public HoareMonitor
{
private:
   static const int num_celdas_total = 10 ;   
   int buffer[num_celdas_total], primera_libre, primera_ocupada, num_celdas_ocupadas ;          
   CondVar ocupadas, libres ;                 

public:                    
   ProdCons4SU(  ) ;           
   int  leer();                
   void escribir( int valor );
} ;
//-----------------------------------------------------------------------------
ProdCons4SU::ProdCons4SU(  )
{
   primera_libre = 0 ;
   primera_ocupada = 0;
   num_celdas_ocupadas = 0;
   ocupadas = newCondVar();
   libres = newCondVar();
}
//-----------------------------------------------------------------------------
int ProdCons4SU::leer(  )
{
   while ( num_celdas_ocupadas == 0 )
      ocupadas.wait();
   assert (0 < num_celdas_ocupadas);
   const int valor = buffer[primera_ocupada] ;
   primera_ocupada = (primera_ocupada+1)%num_celdas_total ;
   num_celdas_ocupadas-- ;

   libres.signal();

   return valor ;
}
//-----------------------------------------------------------------------------
void ProdCons4SU::escribir( int valor )
{
   while ( num_celdas_ocupadas == num_celdas_total )
      libres.wait();
	assert (num_celdas_ocupadas < num_celdas_total);
   buffer[primera_libre] = valor ;
   primera_libre = (primera_libre+1)%num_celdas_total ;
   num_celdas_ocupadas++ ;

   ocupadas.signal();
}
//-----------------------------------------------------------------------------
void funcion_hebra_productora( MRef<ProdCons4SU> monitor, int num_hebra )
{
	for( unsigned i = 0 ; i < num_items/productores ; i++ )
   {
		int valor = producir_dato(num_hebra) ;
		monitor->escribir( valor );
	}
}
//-----------------------------------------------------------------------------
void funcion_hebra_consumidora( MRef<ProdCons4SU> monitor, int num_hebra )
{
    for( unsigned i = 0 ; i < num_items/consumidores ; i++ )
   {
    int valor = monitor->leer();
    consumir_dato( valor );
   }
}
//-----------------------------------------------------------------------------
int main()
{
   cout << "-------------------------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (" << productores
		  << " prod/ " << consumidores << " cons, Monitor SU, buffer FIFO). " << endl
        << "-------------------------------------------------------------------------------" << endl
        << flush ;

   MRef<ProdCons4SU> monitor = Create<ProdCons4SU>( );
	thread hebrap[productores], hebrac[consumidores];
	for(int k = 0; k < productores; k++){
		producidos[k] = 0;
	}

	for(int i = 0; i < productores; i++){
		hebrap[i] = thread ( funcion_hebra_productora, monitor, i );
	}

	for(int j = 0; j < consumidores; j++){
		hebrac[j] = thread ( funcion_hebra_consumidora, monitor, j );
	}

	for(int i = 0; i < productores; i++){
		hebrap[i].join();
	}

	for(int j = 0; j < consumidores; j++){
		hebrac[j].join();
	}

	for(int l = 0; l < productores; l++){
		cout << "la hebra " <<  l << " ha producido " << producidos[l] << " valores" << endl;
	}

   test_contadores() ;
}