#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>
#include <random>
#include <cstdlib>
#include "HoareMonitor.h"
#include "Semaphore.h"

using namespace std ;
using namespace HM ;

//-----------------------------------------------------------------------------
constexpr int num_items  = 40, productores = 4, consumidores = 4;   
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
class ProdCons : public HoareMonitor
{
private:
   static const int num_celdas_total_par = 10;
	static const int num_celdas_total_impar = 10;
   
   int bufferpar[num_celdas_total_par]; 
	int bufferimpar[num_celdas_total_impar];
	int primera_libre_par, primera_libre_impar ;          
   CondVar ocupadas, libres_par, libres_impar ;                 

public:                    
   ProdCons() ;           
   int  leer();                
   void escribir( int valor , int num_hebra );
} ;
//-----------------------------------------------------------------------------
ProdCons::ProdCons()
{
   primera_libre_par = 0 ;
	primera_libre_impar = 0 ;
   ocupadas = newCondVar();
   libres_par = newCondVar();
	libres_impar = newCondVar();
}
//-----------------------------------------------------------------------------
int ProdCons::leer(  )
{
	while (primera_libre_par == 0 && primera_libre_impar == 0){
		ocupadas.wait();
	}
   if(primera_libre_par != 0){
		assert( 0 < primera_libre_par);
   	const int valor = bufferpar[primera_libre_par - 1 ] ;
		cout << "                  Leido valor " << valor << " de buffer par" << endl;
   	primera_libre_par-- ;
		libres_par.signal();
		return valor;
	}
	else{
		if(primera_libre_impar != 0){
			assert( 0 < primera_libre_impar);
   		const int valor = bufferimpar[primera_libre_impar - 1] ;
			cout << "                  Leido valor " << valor << " de buffer impar" << endl;
   		primera_libre_impar-- ;
			libres_impar.signal();
			return valor;
		}	
	}
}
//-----------------------------------------------------------------------------
void ProdCons::escribir( int valor , int num_hebra)
{
	const int hebra = num_hebra;
	if (hebra%2 == 0){
	  	while ( primera_libre_par == num_celdas_total_par ){
		   libres_par.wait();
		}
		assert( primera_libre_par < num_celdas_total_par );
		cout << "Escrito valor " << valor << " por hebra " << hebra << " par" << endl;
   	bufferpar[primera_libre_par] = valor ;
  		primera_libre_par++ ;
	}
	else{
		while ( primera_libre_impar == num_celdas_total_impar ){
		   libres_impar.wait();
		}
		assert( primera_libre_impar < num_celdas_total_impar );
		cout << "Escrito valor " << valor << " por hebra " << hebra << " impar" << endl;
		bufferimpar[primera_libre_impar] = valor ;
		primera_libre_impar++ ;
	}
	ocupadas.signal();
}
//-----------------------------------------------------------------------------
void funcion_hebra_productora( MRef<ProdCons> monitor, int num_hebra )
{
	for( unsigned i = 0 ; i < num_items/productores ; i++ )
   {
		int valor = producir_dato(num_hebra) ;
		monitor->escribir( valor , num_hebra );
	}
}
//-----------------------------------------------------------------------------
void funcion_hebra_consumidora( MRef<ProdCons> monitor, int num_hebra )
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
        << "Problema de los productores-consumidores examen (" << productores
		  << " prod/ " << consumidores << " cons, Monitor SU, buffer LIFO). " << endl
        << "-------------------------------------------------------------------------------" << endl
        << flush ;

   MRef<ProdCons> monitor = Create<ProdCons>();
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
