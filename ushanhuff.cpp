/**
 * Programa para codificar segun Shannon - Fano
 * 15 de septiembre del 2007
 * @author Rafa Rodriguez <rafageist@hotmail.com>
 */

#pragma hdrstop
#pragma argsused

#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>

/************************  Codificacion por Shannon - Fano ***************************/

  // CALCULO DEL CORTE
 //calcula donde debe darse el corte en un intervalo (ini,fin).
 //Se supone a arr[] ordenado descendentemente, y ini y fin correctos

 // sean
        // los elementos A={A1, A2, A3.....An}
        // S la suma de todos los elementos
        // Sk la suma parcial de los elementos A desde 1 hasta k, con k < n,
        // R el resto de S - Sk
        // se desea hallar la diferencia minima sea D entre dos conjuntos subconjuntos disjuntos de A
        // Si se sabe que Sk >= R entonces
                // si Sk = R entonces D = = |Sk - R| = 0 y es optima
                // si Sk > R entonces
                   // sea P el pivote dado por Ak  y se sabe que el corte es en P
                   // Pero aun queda hallar a cual de los dos conjuntos pertenece P
                   // existen 2 diferencias que es necesario comparar
                   // D1 = |Sk - R |
                   // D2 = |(Sk - P) - (R + P)|
                        // si D1 < D2 entonces D = D1 y se incluyen a P en el PRIMER conjunto
                                // en caso contrario,  D = D2, y el se incluye a P en el SEGUNDO
                        // si D1 = D2 entonces cualquiera de los dos casos anteriores es �ptimo

 int Corte(float arr[], int ini, int fin) {

     cout << "El corte entre " << ini+1 << " y " << fin+1;

     int cc=0; //toma el valor del �ndice donde se cortara

    //tomo la suma total
     float suma=0;  for (int l=ini; l<=fin; l++) suma+=arr[l];

     //se va tomando las sumas parciales y se compara con el resto de la suma
     float sumaparcial=0;
     float pivote=0;
     float diferencia1=0, diferencia2=0;
     float resto=0;
     bool conjunto;

     for (int m=ini; m<=fin;m++)
      {
       sumaparcial += arr[m];
       resto = suma - sumaparcial;
       pivote = arr[m];

      //cuando se cumpla la siguiente condicion se sabe que alrededor de pivote esta el optimo
      if (sumaparcial>=resto)
       {
        diferencia1 = sumaparcial - resto; //posible deferencia pinima
        //posible diferencia menor que la anterior
        float difpivote = sumaparcial - pivote;
        float addresto  = resto + pivote;
        if (difpivote < addresto) diferencia2= addresto - difpivote; else diferencia2 = difpivote - addresto;
        if ((diferencia1 <= diferencia2) || diferencia1==0) cc= m; else cc=m-1;
        conjunto=(cc==m-1);
        break;
       }
     }

   if (conjunto)  cout <<" es en " << cc << " incluyendolo en el PRIMER conjunto " << endl;
   else cout <<" es en " << cc + 1 << " incluyendolo en el SEGUNDO conjunto " << endl;

   return cc;
  }

 /*************** inserta un nuevo corte de forma ordenada mientras que no est� *************/
 /** retorna el nuevo valor de cant **/

 int InsertCorte(int arr[], int cant, int c) {
    for (int i=0; i<cant;i++)
    {
      if (c==arr[i]) return cant;
      if (c<arr[i])
       {
         for (int j=cant; j>=i+1; j--)  arr[j]=arr[j-1];
         arr[i]=c;
         return ++cant;
       }
    }
  return cant;
 }

 // CONDICION DE PARADA .. si todos los elementos ya han sido analizados
 bool SigoCortando(bool arr[], int cant)
  {
   for (int i=0; i<cant-1; i++) if (!arr[i]) return true;
   return false;
  }

 //******************** Algoritmo Shannon - Fano ************************
 void Shannon(float arr[], int cant) {

  //aqui se depositaran los codigos binarios
  char ** codes =  new char * [cant];
  for (int i=0; i<cant; i++)
   {
    codes[i] = new char[cant];
    for (int ll=0;ll<cant;ll++) codes[i][ll]='\0';
   }
  cout << "------------------------------" << endl;
  cout << " Segun Shannon - Fano " << endl;
  cout << "------------------------------" << endl;

  //ordeno decrecientemente por las probabilidades

  for (int j=0; j<cant-1; j++)
   for (int k=j+1; k<cant; k++)
    if (arr[j]<arr[k]) {
	float temp = arr[j];
	arr[j]=arr[k];
	arr[k]=temp;
	}

  //pico por diferencia minima y codifico la columna correspondiente

  //inicializo las varibles necesarias
  int * cortes = new int[cant-1];
  int * newcortes = new int[cant-1];
  bool * codesterminate = new bool[cant];
  for (int i=0; i<cant;i++)
        {
        codesterminate[i] = false;
        if (i<cant-1) cortes[i]=newcortes[i]=0;
        }

  int cantnewcortes=0;
  int cantcortes=0;
  int columna=-1;
  cortes[0]=0;
  cortes[1]=cant-1;
  cantcortes=2;

  while (SigoCortando(codesterminate,cant)) {
   columna++;
   cantnewcortes=0;
   for (int l=0; l<cantcortes-1;l++)
    {
      if (!codesterminate[l]) // si todavia no se ha terminado con este elemento
       {
        int c = Corte(arr,cortes[l],cortes[l+1]);  //busco donde cortar  en el intervalo l y l+1
        if (cortes[l]+1==cortes[l+1])  //si son coninuos
            {
              codesterminate[l]=true;    // los marco como terminados
              codesterminate[l+1]=true;
              if (l+2<cant) newcortes[cantnewcortes++]=l+2; //si es posible incluyo el nuevo comienzo
                                                           // de corte coninuo a estos
            }
        else
          if (cortes[l]==c && cortes[l]+1!=cortes[l+1]) // si el corte fue el primero del intervalo
              {
               codesterminate[l]=true;          // lo marco como terminado
               if (l+1<cant) newcortes[cantnewcortes++]=l+1;    // si es posible incluyo como comienzo
                                                                // de corte a quien le sigue
              }
                else
                 if (cortes[l+1]==c && cortes[l+1]!=cortes[l-1]+1) // si el corte fue el ultimo del intervalo
                         newcortes[cantnewcortes++]=l-1;        // incluyo a quien lo antecede como fin del otro corte
                                                                // no lo marco porque puede ser que sea comienzo de
                                                                //otro intervalo de corte y si es el ultimo de los elementos
                                                                // nunca se llegaro a el
         //codifico la columna
         for (int m=cortes[l]; m<=c;m++)      codes[m][columna]='1';            //los de arriba con uno
         for (int n=c+1; n<=cortes[l+1]; n++) codes[n][columna]='0';           // los de abajo con cero

        newcortes[cantnewcortes++]=c; //incluyo el corte buscado como comienzo de un nuevo intervalo
       }
    }
  //inserto los nuevos intervalos para la proxima iteracion
   for (int ll=0; ll<cantnewcortes;ll++) cantcortes=InsertCorte(cortes,cantcortes,newcortes[ll]);
 }

 for (int ii=0; ii<cant;ii++)
  {
    cout << "   P(" << arr[ii] << ") "  << " =>>>> ";
    for (int jj=0; jj<=columna;jj++) if (codes[ii][jj]!='\0') cout << codes[ii][jj];
    cout <<endl;
  }

/* try
 {
  delete [] cortes;
  delete [] newcortes;
  for (int del=0; del<cant; del++) delete [] codes[del];
  }
  catch (...)
   {
   cout << "No se pudo liberar la memoria. Se espera que el SO o IDE lo haga." << endl;
   }*/
}


/************************** Algoritmo Huffman *****************/

void Huffman(float arr[], int cant)
 {
 cout << "huff entro";
 for (int i=0; i<cant; i++)
   cout << arr[i] << endl;

 }

/***********************************************************************************************/
/*----------- MAIN -----------------------------*/

int main(int argc, char* argv[]) {

 float * prob;
 long cantprob = 0;

 clrscr();
 textcolor(9); cprintf("///////////////////////////////////////////////////////////////////////////////" ); cout << endl;
 textcolor(CYAN);cprintf(" Universidad: "); textcolor(11); cprintf(" ISPJAE, Instituto Superior Polit�cnico \"Jos� Antonio Echeverr�a\"" ); cout << endl;;
 textcolor(CYAN);cprintf(" Especialidad:"); textcolor(11); cprintf(" Ingenier�a Inform�tica" );   cout << endl;
 textcolor(CYAN);cprintf(" Programa:    "); textcolor(11); cprintf(" Codificaci�n por Shannon - Fano" );   cout << endl;
 textcolor(CYAN);cprintf(" Autor:       "); textcolor(11); cprintf(" Rafael Rodr�guez Ram�rez");    cout << endl;;
 textcolor(CYAN);cprintf(" Actualizado: "); textcolor(11); cprintf(" 15 de septiembre del 2007" );   cout << endl;;
 textcolor(9); cprintf("//////////////////////////////////////////////////////////////////////////////" ); cout << endl;

restart:
cantprob=0;
 while (cantprob<2)
 {
 cout << endl;
 cout << " Entre la cantidad de s�mbolos: ";
 cin >> cantprob;
 }
 prob=new float[cantprob];

 restartprob:
float suma=0;
 for (long i =0; i<cantprob;i++)
  {
   textcolor(3);
   cout << endl;

   cprintf ("   - >> Entre la probabilidad "); cout << i+1 << " (0.." << 1-suma << "): " ;
 restartprobi:
   cin >> prob[i];
   suma += prob[i];
   if (suma>1)
   {
   textcolor(12); cprintf("La suma de las probabilidades debe ser igual a 1 !!");cout << endl << "Entre un n�mero menor que " << suma - 1 << endl;
   suma -=prob[i];
   goto restartprobi;
   }
 }
 suma=0;
 for (int i=0; i<cantprob; i++) suma+=prob[i];
  if (suma!=1)
  {
  textcolor(12); cprintf("La suma de las probabilidades debe ser igual a 1 !!"); cout << endl;
  goto restartprob;
  }
   Shannon(prob,cantprob);

   //pendiente
   // Huffman(prob,cantprob);

   char a;
   cout << "Desea continuar codificando (S/N)? "; a=getch();
   if (a=='s' || a=='S') goto restart;
   delete [] prob;
   return 0;
 }
