///////////////////////////////////////////////////////////////////////////////
// Alejandro Manzanares Lemus
// 2º A1 GII
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>

using namespace std ;
//-----------------------------------------------------------------------------
constexpr int num_items  = 40, productores = 4, consumidores = 8;    
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
class ProdCons2SC
{
private:
   static const int num_celdas_total = 10 ;   
   int buffer[num_celdas_total], primera_libre, primera_ocupada, num_celdas_ocupadas ;          
   mutex cerrojo_monitor ;        
   condition_variable ocupadas, libres ;                 

public:                    
   ProdCons2SC(  ) ;           
   int  leer();                
   void escribir( int valor );
} ;
//-----------------------------------------------------------------------------
ProdCons2SC::ProdCons2SC(  )
{
   primera_libre = 0 ;
   primera_ocupada = 0;
   num_celdas_ocupadas = 0;
}
//-----------------------------------------------------------------------------
int ProdCons2SC::leer(  )
{
   unique_lock<mutex> guarda( cerrojo_monitor );

   while ( num_celdas_ocupadas == 0 )
      ocupadas.wait( guarda );
   assert (0 < num_celdas_ocupadas);
   const int valor = buffer[primera_ocupada] ;
   primera_ocupada = (primera_ocupada+1)%num_celdas_total ;
   num_celdas_ocupadas-- ;

   libres.notify_one();

   return valor ;
}
//-----------------------------------------------------------------------------
void ProdCons2SC::escribir( int valor )
{
   unique_lock<mutex> guarda( cerrojo_monitor );

   while ( num_celdas_ocupadas == num_celdas_total )
      libres.wait( guarda );
	assert (num_celdas_ocupadas < num_celdas_total);
   buffer[primera_libre] = valor ;
   primera_libre = (primera_libre+1)%num_celdas_total ;
   num_celdas_ocupadas++ ;

   ocupadas.notify_one();
}
//-----------------------------------------------------------------------------
void funcion_hebra_productora( ProdCons2SC * monitor, int num_hebra )
{
	for( unsigned i = 0 ; i < num_items/productores ; i++ )
   {
		int valor = producir_dato(num_hebra) ;
		monitor->escribir( valor );
	}
}
//-----------------------------------------------------------------------------
void funcion_hebra_consumidora( ProdCons2SC * monitor, int num_hebra )
{
   for( unsigned i = 0 ; i < num_items/consumidores ; i++ )
   {
    int valor = monitor->leer();
    consumir_dato( valor );
   }
}
//----------------------------------------------------------------------------
int main()
{
   cout << "-------------------------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (" << productores
		  << " prod/ " << consumidores << " cons, Monitor SC, buffer FIFO). " << endl
        << "-------------------------------------------------------------------------------" << endl
        << flush ;

   ProdCons2SC monitor ;
	thread hebrap[productores], hebrac[consumidores];

	for(int k = 0; k < productores; k++){
		producidos[k] = 0;
	}

	for(int i = 0; i < productores; i++){
		hebrap[i] = thread ( funcion_hebra_productora, &monitor, i );
	}

	for(int j = 0; j < consumidores; j++){
		hebrac[j] = thread ( funcion_hebra_consumidora, &monitor, j );
	}

	for(int i = 0; i < productores; i++){
		hebrap[i].join();
	}

	for(int j = 0; j < consumidores; j++){
		hebrac[j].join();
	}
	
   test_contadores() ;

	for(int l = 0; l < productores; l++){
	cout << "la hebra " <<  l << " ha producido " << producidos[l] << " valores" << endl;
	}
}