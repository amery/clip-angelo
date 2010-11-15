
                Clip-Glade2 - (c) Sergio Zayas Tor�, 2.005
                --------------------------------------------


Licencia
--------
        Clip-Glade2 se distribuye bajo la GNU General Public License (GPL),
como se describe en el archivo COPYING (ingl�s).


Descripci�n
-----------
       Clip-Glade2 es una librer�a para el compilador Clip
(http://www.itk.ru). Permite usar las facilidades ofrecidas por libGlade
dentro de una aplicaci�n compilada con Clip.


Requisitos
----------
       Para compilar Clip-Glade2 se necesita tener instalado :
               1.- Compilador Clip con soporte para gtk2
               2.- libglade2 
               3.- libglade2-devel 

       Para compilar y ejecutar programas (.prg) que utilicen Clip-libGlade2
no es necesario tener instalado libglade2-devel.
   


Construcci�n de Clip-Glade2
---------------------------
       Para compilar Clip-Glade2 en el directorio fuente ejecutar:

               $ make

y si no ha habido problemas se habr� creado el archivo 'libclip_glade2.so'.


       Para instalar la librer�a

	       # make install


       Para compilar y ejecutar el ejemplo :
               $cd example
               $make
               $./eglade


Compilaci�n de programas Clip
-----------------------------
       Para compilar programas Clip (.prg) que hagan uso de Clip-Glade2 
hay que a�adir la librer�a en la l�nea de generaci�n del
ejecutable.

       Ejemplo : 
   
    $ clip -e miaplicacion.prg -clip-gtk2 -lclip-glade2
   

       

Uso de Clip-Glade2 desde Clip
----------------------------
       Consultar el archivo 'REFERENCIA.txt' para informaci�n de uso de
las llamadas a clip-glade2.
       Mas informaci�n (en ingl�s) de libglade se encuentra aqu� : 
       http://developer.gnome.org/doc/API/2.0/libglade/index.html 




Compatibilidad
--------------
       Clip-Glade2 est� desarrollado usando la versi�n 2.4.2 de libglade-2.0
con versiones anteriores o posteriores no est� probado.



Soporte
-------
       Cualquier duda, sugerencia o aportaci�n ser� bien recibida en :
       
       <Segio Zayas> sergio_zayas@yahoo.com.ar 

