// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 4. Implementación de sistemas de tiempo real.
//
// Archivo: tiempos.cpp
// Operaciones con duraciones e instantes.
//
// Historial:
// Creado en Diciembre de 2017
// -----------------------------------------------------------------------------

#include <string>
#include <iostream> // cout, cerr
#include <thread>
#include <chrono>   // utilidades de tiempo
#include <ratio>    // std::ratio_divide

using namespace std ;
using namespace std::chrono ;

// tipo para duraciones en segundos, en coma flotante:
typedef duration<float,ratio<1,1>> secondsf ;

void duraciones() ;
void medir_sleep_for() ;
void medir_sleep_until() ;

// -----------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
   duraciones();
   medir_sleep_for();
   medir_sleep_until();
}
// -----------------------------------------------------------------------------

void duraciones()
{
   milliseconds  d1 = milliseconds( 34 ) ; // 34 milisegundos con precisión entera
   seconds       d2 = seconds( 45 ) + 3*seconds( 15 ) ; // 90 segundos (como enteros)
   secondsf      d3 = d2/4.0 ; // 22.5 segundos (en coma flotante)
   minutes       d4 = minutes( 55 ) ; // 56 minutos (enteros) (supone conversión)
   duration<float,ratio<60,1>> // tipo para minutos en coma flotante
                 d5 = 0.5*d4 ; // 27.5 minutos (en coma flotante)
   milliseconds  d6 = seconds(1) ; // conversion segundos --> milisegundos (ok)
   // seconds    d7 = milliseconds( 20 ) ; // error: conversión a una unidad entera mayor
   secondsf      d8 = milliseconds( 20 ) ; // ok: conversión a valores flotantes

   cout  << "Duraciones: " << endl
         << "  d1 == " << d1.count() << " milisegundos." << endl
         << "  d2 == " << d2.count() << " segundos." << endl
         << "  d3 == " << d3.count() << " segundos (reales)." << endl
         << "  d4 == " << d4.count() << " minutos." << endl
         << "  d5 == " << d5.count() << " minutos (reales)." << endl
         << "  d6 == " << d6.count() << " milisegundos" << endl
         << "  d8 == " << d8.count() << " segundos (reales)" << endl ;
}
// -----------------------------------------------------------------------------

void medir_sleep_for()
{
   time_point<steady_clock> instante_inicio = steady_clock::now() ;
   this_thread::sleep_for( milliseconds(200) );
   time_point<steady_clock> instante_fin = steady_clock::now() ;
   steady_clock::duration duracion = instante_fin - instante_inicio ;

   cout << endl
        << "Medición de duración de 'sleep_for( 200ms )'" << endl
        << "   duración == " << secondsf(duracion).count() << " segundos." << endl ;
}
// -----------------------------------------------------------------------------

void medir_sleep_until()
{
   time_point<steady_clock> instante_inicio    = steady_clock::now(),
                            instante_despertar = instante_inicio + seconds(1) ;
   this_thread::sleep_until( instante_despertar );
   time_point<steady_clock> instante_fin = steady_clock::now() ;
   steady_clock::duration duracion = instante_fin - instante_inicio ;
   cout << endl
        << "Medición de duración de 'sleep_until( 1 segundo en el futuro )'" << endl
        <<"    duración = " << secondsf(duracion).count() <<" segundos." << endl;
}
