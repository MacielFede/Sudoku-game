///Federico Maciel / Gabriel Suárez / Santiago Dono 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Jugador{
    char alias[5];
    char nombre[20];
    char apellido[20];
    int edad;
    int puntos;
    bool activo;
};

Jugador ranking[10];

void leer_linea(char str[], int tam);

bool linea_es_natural_act(char arr[], int tam, int &num);

///Imprime el sudoku con 0.
void imprimir_Sudoku(int s[][9]);
///Crea el sudoku y lo llena de 0.
void inicializar_sudoku(int s[9][9]);
///Genera elementos para empezar a jugar sudoku
void generar_sudoku_valido(int s[9][9], int cant_elem);
///Devuelve true si un num es candidato para una fila,columna y reion
bool es_candidato(int s[9][9], int num, int fila, int columna);
///Crea candidatos aleatorios dentro de un arreglo bool que cumplan con las condiciones
void crear_candidatos(int s[9][9], int fila, int columna, bool candidatos[9]);
///Revisa  si el sudoku esta completo sin reglas rotas
bool sudoku_resuelto(int s[9][9]);
///Revisa si el sudoku esta bien completado(puede estar incompleto)
bool sudoku_valido(int s[9][9]);
///Busca si el alias ingresado ya existe, si existe devuelve la pos y si no devuelve -1
int pos_jugador(char alias[5]);
///Crea un jugador si el alias no es repetido y si hay espacio para uno mas
void crear_jugador(char alias[5]);
///Modifica el jugador con el alias asignado que se encuentra en la posicion descubierta
void modificar_jugador(char alias[],int pos);
///Elimina un jugador verificando si su alias existe o no
void eliminar_jugador(int &cantJ);
///Si el alias que se le pasa existe muestra los datos del jugador
void datos_jugador();
/// Pide alias y dificultad al jugador para iniciar un juevo nuevo
void juego_nuevo();
/// Permite al usuario jugar, dandole opciones de ayuda y retirarse. Si completa el juego suma los puntos
void jugar (int lv, int user);

int main(){
    srand(time(NULL));
    char opc, aux[5], limpio;
    int cantJ=0, sudoku[9][9], pos;
    for(pos=0;pos<10;pos++){
        ranking[pos].activo=false;
        ranking[pos].puntos=0;
    }
    ///No ingresamos tildes para que se muestren todos los caracteres al imprimir
    printf("Bienvenidos al juego del sudoku! Debe indicar en el menu lo que quiere hacer con SOLO una letra\n");
    printf("Recuerde ingresar un jugador antes de comenzar a jugar.\n");
    do{
        printf("\nMenu\n(R)egistrar, (L)istado, (J)ugar, (D)atos, (M)odificar, (E)liminar, (S)alir: ");
        opc=getchar(); limpio=getchar();
        while(limpio!='\n')
            limpio=getchar();
        switch(opc){ ///No usamos if-else if para que sea mas modular y mas facil de ver cada opcion
            case 'R': case 'r':
                printf("Ingreso en Registrar:\n");
                printf("Ingrese el Alias: ");
                leer_linea(aux,5);
                pos=pos_jugador(aux);
                if(pos==-1){
                    crear_jugador(aux);
                    cantJ++;
                }else
                    printf("Ya existe un jugador con el alias %s\n",aux);
            break;
            case 'L': case 'l':
                printf("Ingreso en Listado:\n");
                if(cantJ==0)
                    printf("No hay jugadores ingresados.\n");
                else{
                    for(pos=0;pos<10;pos++)
                        if(ranking[pos].activo)
                            printf("Alias: %s puntaje total: %d\n", ranking[pos].alias, ranking[pos].puntos);
                }
            break;
            case 'J': case 'j':
                printf("Ingreso en Jugar:\n");
                if(cantJ==0)
                    printf("No hay jugadores ingresados.\n");
                else{
                   juego_nuevo();
                }
            break;
            case 'D': case 'd':
                printf("Ingreso en Datos:\n");
                if(cantJ==0)
                    printf("No hay jugadores ingresados.\n");
                else{
                    datos_jugador();
                }
            break;
            case 'M': case 'm':
                printf("Ingreso en Modificar:\n");
                if(cantJ==0)
                    printf("No hay jugadores ingresados.\n");
                else{
                    printf("Ingrese el alias del jugador a modificar: ");
                    leer_linea(aux,5);
                    pos=pos_jugador(aux);
                    if(pos==-1)
                        printf("No existe ese jugador.\n");
                    else
                        modificar_jugador(aux, pos);
                }
            break;
            case 'E': case 'e':
                printf("Ingreso en Eliminar:\n");
                if(cantJ==0)
                    printf("No hay jugadores ingresados.\n");
                else
                    eliminar_jugador(cantJ);
            break;
            default:
                if(opc!='s' && opc!='S')
                    printf("Comando invalido, intentelo de nuevo.\n");
            break;
        }
    }while(opc!='s' && opc!='S');
    printf("Chau, vuelva pronto!\n");
	return 0;
}

bool linea_es_natural_act(char arr[], int tam, int &num){
    int pos=-1;
    bool soloNums=true;

    do{
        if(pos<tam-1)
            pos++;
        arr[pos]=getchar();
        if(arr[pos]!='\n' and (arr[pos]<'0' or arr[pos]>'9')and pos<tam-1)
            if(not (pos==0 and arr[pos]=='-'))
                soloNums=false;
    }while(arr[pos]!='\n');
    arr[pos]='\0';

    if(soloNums)
        num=atoi(arr);
        //num=a_entero(arr);

    return soloNums;
}

void leer_linea(char str[], int tam){
    int pos=-1;

    do{
        pos++;
        str[pos]=getchar();
    }while(pos<tam-1 and str[pos]!='\n');

    while(str[pos]!='\n')
         str[pos]=getchar();

    str[pos]='\0';
}

void imprimir_Sudoku(int s[9][9]){
	int pos1, pos2;
	for(pos1=0;pos1<9;pos1++){
        for(pos2=0;pos2<9;pos2++)
            printf("|%d|",s[pos1][pos2]);
        printf("\n");
    }
}
void inicializar_sudoku(int s[9][9]){
	int numero = 0;
	int pos1, pos2;
	for(pos1 = 0; pos1 < 9; pos1++){ // inicializo la matriz
		for(pos2 = 0; pos2 < 9; pos2++){
			s[pos1][pos2]=numero; //llenamos la matriz de 0
		}
	}
}
bool es_candidato(int s[9][9], int num, int fila, int columna) {
	int pos1, pos2;
	bool candidatoFil = true, candidatoCol = true, candidatoReg = true, candidato; //todas las variables inicializadas
	pos1 = fila;
	pos2 = 0;
	do{
		if (num == s[pos1][pos2])
			candidatoFil = false;
		pos2++;
	}while(pos2 < 9 && candidatoFil);///Revisamos fila
	pos1 = 0;
	pos2 = columna;
	do {
		if(num == s[pos1][pos2])
			candidatoCol = false;
		pos1++;
	} while(pos1 < 9 && candidatoCol);///Revisamos columna
	while(fila!=0 && fila!=3 && fila!=6)
        fila--;
    while(columna!=0 && columna!=3 && columna!=6)
        columna--;
    for(pos1=fila;pos1<fila+3 && candidatoReg;pos1++)///Revisamos region
        for(pos2=columna;pos2<columna+3;pos2++)
            if(num == s[pos1][pos2])
                candidatoReg = false;

	if(candidatoCol && candidatoFil && candidatoReg) ///preguntamos si el numero es candidato de fila, region y columna, si lo es sera un Candidato
		candidato = true;
    else
        candidato = false;

	return candidato;
}
bool sudoku_valido(int s[9][9]) {
	int pos1, pos2;
	bool sudokuValido = true;
	for(pos1 = 0; pos1 < 9; pos1++) {
		for(pos2 = 0; pos2 < 9; pos2++) {
			if(s[pos1][pos2] != 0 && !es_candidato(s, s[pos1][pos2], pos1, pos2)) ///Al recorrer el sudoku se pregunta si cada numero es candidato, dejando de lado los 0
				sudokuValido = false;
		}
	}

	return sudokuValido;
}
bool sudoku_resuelto(int s[9][9]) {
	int pos1 = 0, pos2 = 0;
	bool resuelto = true;
	while(pos1<9 && resuelto){
        pos2=0;
		while(pos2<9 && resuelto){ ///Inspecciona cada uno de los numeros del sudoku para ver si estan bien colocados
			if(s[pos1][pos2]==0 && !es_candidato(s, s[pos1][pos2], pos1, pos2))
				resuelto = false;
			pos2++;
		}
		pos1++;
	}
	return resuelto;
}

void crear_candidatos(int s[9][9], int fila, int columna, bool candidatos[9]) {
	int i;
	for(i = 0; i < 9; i++)
        candidatos[i] = es_candidato(s, i + 1, fila, columna); //Se fija si cada numero correspondiente a cada indice es un candidato
}

void generar_sudoku_valido(int s[9][9], int cant_elem){
	int pos1=0, pos2=0,pos1Falso, pos2Falso, num, i;
	bool cand[9], candFalsos[9];
	while(cant_elem > 0) {
        do{ ///Genera posiciones de otra celda hasta que llegue a una que haya un 0
            pos1Falso = rand() % 9;
            pos2Falso = rand() % 9;
        }while(s[pos1Falso][pos2Falso] != 0);
        crear_candidatos(s,pos1,pos2,cand); ///se fija los candidatos de la celda actual
        crear_candidatos(s,pos1Falso,pos2Falso,candFalsos);/// se fija los candidatos de la proxima celda
        do{
            num = rand()%9+1;
            i=num-1;
        }while(!candFalsos[i] && cand[i]);///genera un numero que sea candidato de la proxima celda pero no de la actual
        if(es_candidato(s, num, pos1, pos2)){///esto lo chequeamos por si las celdas comparadas no son adyacentes
            s[pos1Falso][pos2Falso]=num; //llenamos la matriz de elementos validos
            cant_elem--;
        }
        pos1=pos1Falso;///cambiamos la celda actual
        pos2=pos2Falso;
    }
}


int pos_jugador(char alias[]){
    int pos=9;
    bool encontre=false;

    do{
        if(ranking[pos].activo)
            if(strcmp(ranking[pos].alias, alias)==0 )///corrobora el alias solo si el jugador esta activo
                encontre=true;
            else
                pos--;
        else
            pos--;
    }while(!encontre && pos>-1);
    return pos;
}

void crear_jugador(char alias[]){
    int pos=0;
    char aux[5];
    while(pos<10 && ranking[pos].activo){ ///Busca la primer posicion libre
        pos++;
    }
    if(pos==10)
        printf("No hay lugar para un nuevo jugador\n");
    else{
        ranking[pos].activo=true;
        strcpy(ranking[pos].alias, alias);
        printf("Ingrese el nombre: ");
        leer_linea(ranking[pos].nombre,20);
        printf("Ingrese el apellido: ");
        leer_linea(ranking[pos].apellido,20);
        printf("Ingrese la edad(NO INGRESE LETRAS): ");
        while(!linea_es_natural_act(aux,5,ranking[pos].edad))
            printf("Ingreso letras, ingrese la edad nuevamente: ");
    }
}

void modificar_jugador(char alias[],int pos){
    char aux[5];
    printf("Ingrese el nombre: ");
    leer_linea(ranking[pos].nombre,20);
    printf("Ingrese el apellido: ");
    leer_linea(ranking[pos].apellido,20);
    printf("Ingrese la edad(NO INGRESE LETRAS): ");
    while(!linea_es_natural_act(aux,5,ranking[pos].edad))
        printf("Ingreso letras, ingrese la edad nuevamente: ");
}

void eliminar_jugador(int &cantJ){
    int pos=0;
    char aux[5];
    printf("Ingrese el alias del jugador a eliminar: ");
    leer_linea(aux, 5);
    pos=pos_jugador(aux);
    if(pos==-1)
        printf("No existe un jugador llamado %s\n",aux);
    else{
        ranking[pos].activo=false;
        cantJ--;
        printf("Se elimino el jugador exitosamente\n");
    }
}

void datos_jugador(){
    char aux[5];
    int pos;
    printf("Ingrese el alias del jugador: ");
    leer_linea(aux,5);
    pos=pos_jugador(aux);
    if(pos==-1)
        printf("No existe ese jugador.\n");
    else{
        printf("Nombre: %s\nApellido: %s\n",ranking[pos].nombre, ranking[pos].apellido);
        printf("Edad: %d\nPuntaje total: %d", ranking[pos].edad, ranking[pos].puntos);
    }
}

void juego_nuevo(){
    int d;
    char dif, nick [5];

    printf ("Alias?\n");
    leer_linea(nick, 5);
    if (pos_jugador(nick)==-1)
        printf("Ese alias no existe!\n");
    else{
        do{
            printf(" Dificultad? (F)acil, (N)ormal o (D)ificil\n");
            dif=getchar();
            getchar();  //  Getchar para limpiar el \n
            switch (dif){
                case 'D': case'd':
                    d=20;
                    break;
                case 'N': case 'n':
                    d=40;
                    break;
                case 'F': case 'f':
                    d=60;
                    break;
                default:
                    printf("Opcion invalida!\n");
                    d=0;
                    break;
            }
        }while(d==0);
        jugar (d, pos_jugador(nick));
    }
}

void jugar (int lv, int user){

    int sdk [9][9];
    int num, fila, col, pos1, pos2, i;
    bool exit=false, candi[9];
    //Rendirse: Para esto debe ingresar -1 en vez de un número nuevo a ingresarse en la matriz.
    //● Ayuda [Opcional]: Digitando un -2 se le brinda al jugad
    inicializar_sudoku(sdk);
    generar_sudoku_valido(sdk, lv);
    do{
        imprimir_Sudoku(sdk);
        do{
            printf("Numero:(-1 Rendirse, -2 Ayuda)");
            scanf("%d", &num); getchar();                           // Se lee respuesta y se usa el getchar para limpiar el enter
            if(num==-1){
                printf ("***Usted se Rindio***\n");
                exit=true;
            }
            else if (num==-2){
                do{
                    pos1=rand()%9;                      // Genera pos1 random entre 0 y 8
                    pos2=rand()%9;                      // Genera pos2 random entre 0 y 8
                }while(sdk[pos1][pos2]!=0);
                crear_candidatos(sdk, pos1, pos2, candi);
                printf (" Para la celda %d,%d los candidatos son:\n", pos1, pos2);
                for (i=0; i<9 ;i++){
                    if (candi[i])
                        printf("%d\n",i+1);
                }
            }
            else if (num>0 and num <10){
                do{
                    printf("Fila: ");                                       // Pregunta en que Fila guarda el elemento nuevo
                    scanf("%d", &fila);getchar();                         // Se lee respuesta y se usa el getchar para limpiar el enter
                    if(fila<1 or fila>9)
                        printf("Numero invalido\n");
                }while(fila<1 or fila>9);
                do{
                    printf("Columna: ");                                    // Pregunta en que Columna guarde el elemento nuevo
                    scanf("%d", &col);getchar();                        // Se lee respuesta y se usa el getchar para limpiar el enter
                    if(col<1 or col>9)
                        printf("Numero invalido\n");
                }while (col<1 or col>9);
                fila--;
                col--;
                if(es_candidato(sdk,num,fila,col)){
                    sdk[fila][col]=num;
                    exit=sudoku_resuelto(sdk);
                    if(exit){
                        printf("***Ganaste!!!***");
                        ranking[user].puntos=ranking[user].puntos+(81-lv);
                    }
                }
                else
                    printf("Ese numero no es valido para esa celda!\n");
            }
            else{
                num=0;
                printf ("Opcion Invalida!\n");
            }
        }while(num==0);
    }while(not exit);
}
