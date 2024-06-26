#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

// --------------------------------------------------------------------------------------------------------
//            CAPTURA DE ERRORES SQL
// --------------------------------------------------------------------------------------------------------

EXEC SQL WHENEVER SQLERROR SQLPRINT;
EXEC SQL WHENEVER SQLWARNING SQLPRINT;
EXEC SQL WHENEVER NOT FOUND SQLPRINT;

// --------------------------------------------------------------------------------------------------------
//            FIRMAS
// --------------------------------------------------------------------------------------------------------

void limpiarUltimaLineas(int num);
void limpiarBuffer();
void clearS();
void title(const char* titulo);
void conectar();
void desconectar();
void esperarEnter();
void listarArticulos();
int intValido();
int ventaEntrada();               
int ventaArticulo();
int registroReposicion(int OptId);
int altaArticulo();
int consultarVentas();            
int configPrecios();
int menu();

// --------------------------------------------------------------------------------------------------------
//            MAIN
// --------------------------------------------------------------------------------------------------------

int main(){
  if (sqlca.sqlcode != 0){
    printf("TIPO DE ERROR: %d\n", sqlca.sqlcode);
    printf("MENSAJE:%s\n", sqlca.sqlerrm.sqlerrmc);
  }
  menu();
  return 0;
}

// --------------------------------------------------------------------------------------------------------
//            METODOS
// --------------------------------------------------------------------------------------------------------

// Funcion para conectar conla base de datos.
void conectar(){
  EXEC SQL CONNECT TO lab02@localhost:5432 USER postgres;
}

// Funcion para desconectar todas las conecciones conla base de datos.
void desconectar(){
  EXEC SQL DISCONNECT ALL;
}

// Funcion para limpiar el buffer de memoria del teclado, normalmente usado cuando se utiliza fgets.
void limpiarBuffer(){
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {}
}

// Funcion para limpiar las ultimas n lineas de la consola, utilizada con fines esteticos
void limpiarUltimaLineas(int num) {
    printf("\033[%dA", num);
    printf("\033[0J");
}

// Funcion para limpiar pantalla.
void clearS(){
  system("cls||clear");
}

// Devuelve un entero, pidiendo el input del usuario hasta que este ingrese un entero que nosea negativo
int intValido(){
  int foo;
  bool valido = false;
  do {
  if ((scanf("%d", &foo) == 1) && foo >= 0) {
      valido = true;
  } else {
    printf("\nDebe ingresar un número entero válido. Inténtelo nuevamente.");
    printf("\n----------------------------------------------------------");
    printf("\n\n\t--> ");
    while (getchar() != '\n');
  }
  } while (!valido);
  return foo;
}

// Funcion que espera hasta que el usuario ingrese un enter
void esperarEnter(){
  printf("\nPresionar ENTER para continuar...");
  getchar();
  getchar();
  clearS();
}

// Funcion para imprimir titulos
void title(const char* titulo) {
  printf("\n\t### %s ###\n----------------------------------------------------------\n", titulo);
}

void listarArticulos(){
  EXEC SQL BEGIN DECLARE SECTION;    
  int tmpId;
  varchar(50) tmpNombre;
  EXEC SQL END DECLARE SECTION;
	EXEC SQL DECLARE cursor_articulo CURSOR FOR SELECT id, nombre FROM articulo ORDER BY id;
	EXEC SQL WHENEVER NOT FOUND DO BREAK;
	EXEC SQL OPEN cursor_articulo;

  printf("Lista de Articulos\n");
  printf("=============================================\n");

  while (1) {
      EXEC SQL FETCH FROM cursor_articulo INTO :tmpId, :tmpNombre;
      if (sqlca.sqlcode == ECPG_NOT_FOUND) {
          break;
      }

      printf("\nID: %d \t-%s", tmpId, tmpNombre);
  }

  printf("\n=============================================\n");

  EXEC SQL CLOSE cursor_articulo;
  EXEC SQL WHENEVER NOT FOUND CONTINUE;
};


// Funcion que permite la venta de entradas incluyendo el registro del artículo de regalo
int ventaEntrada()
{
  EXEC SQL BEGIN DECLARE SECTION;  
  int id_venta_entrada;
  EXEC SQL END DECLARE SECTION;
  clearS();
  
  // Pedimos la opción de si el visitante es menor de edad o es mayor y es necesario cobrarle
  int opcion;
  do {
    title("VENTA ENTRADA");
    printf("\n\nEl visitante es menor de edad?");
    printf("\n\t1 - SI");
    printf("\n\t2 - NO\n\t--> ");

    opcion = intValido();
    
    if (opcion != 1 && opcion != 2) {
      printf("\n\nOpción inválida. Por favor, intente nuevamente.");
      esperarEnter();
    }
  } while (opcion != 1 && opcion != 2);

  switch (opcion) {
    case 1:
      // Creamos el registro de venta y venta entrada para un visitante menor de edad
      conectar();
      EXEC SQL BEGIN TRANSACTION; 
      EXEC SQL SELECT nextval('venta_id_seq') INTO :id_venta_entrada;
      EXEC SQL INSERT INTO ventaentrada (idventa, esmenor) VALUES (:id_venta_entrada, true); 
      EXEC SQL COMMIT;      
      desconectar();

      printf("\n\nEntrada registrada correctamente.");
      printf("\n----------------------------------------------------------");
      esperarEnter();
      clearS();
      break;

    case 2:
      conectar();
      EXEC SQL BEGIN TRANSACTION;
      EXEC SQL SELECT nextval('venta_id_seq') INTO :id_venta_entrada;
      EXEC SQL INSERT INTO ventaentrada (idventa, esmenor) VALUES (:id_venta_entrada, false); 
      EXEC SQL COMMIT;
      desconectar();

      printf("\n\nEntrada registrada correctamente.");
      printf("\n----------------------------------------------------------");
      esperarEnter();
      clearS();
      break;

    default:
      break;
  }
  return 0;
}

// Funcion que permite la venta de artículos de la tienda (tazas, lapiceras, llaveros, etc)
int ventaArticulo(){
  EXEC SQL BEGIN DECLARE SECTION;
  int idarticulo = -1, cantidadVenta = 0, precioUni = 0, precioTotal = 0, val_elec, id_venta_articulo;
  bool validCant, validId = false;
  EXEC SQL END DECLARE SECTION;
  conectar();
  // Pedimos el identificador del articulo que se quiera vender y comprobamos que este exista en nuestros registros
  do{
    clearS();
    listarArticulos();
    title("VENTA ARTICULO");
    if (idarticulo != -1){
      clearS();
      title("VENTA ARTICULO");
      printf("\n\nNo existe dicho articulo, intente nuevamente\n\n\t--> ");
    }else{
      printf("\n\nIngrese el identificador del articulo a vender\n\n\t--> ");
    }

    idarticulo = intValido();

    EXEC SQL SELECT sp_existeariculo(:idarticulo) INTO :validId;
  } while (!validId);
  clearS();

  // Pedimos la cantidad a vender y comprobamos que exista el stock anecesario para la cantidad solicitada
  title("VENTA ARTICULO");
  printf("\n\nIngrese la cantidad a vender\n\n\t--> ");
  cantidadVenta = intValido();
  EXEC SQL SELECT sp_existestock(:idarticulo, :cantidadVenta) INTO :validCant;
  if (!validCant)
  {
    printf("\n\nNO EXISTE DICHA CANTIDAD EN STOCK --- SE CANCELA LA VENTA\n");
    printf("\n----------------------------------------------------------");
    esperarEnter();
    desconectar();
    return 1;
  }

  // Generamos una consulta q devuelva el precio unitario del articulo a vender
  EXEC SQL SELECT sp_valorarticulo(:idarticulo) INTO :precioUni;

  // Calculamos el monto total de la venta
  precioTotal = precioUni * cantidadVenta;

  clearS();

  // Pedimos la confirmacion de la venta 
  
  do{
    printf("\n----------------------------------------------------------\n");
    printf("CONFIRMAR VENTA");
    printf("\n----------------------------------------------------------\n");
    printf("ID del Articulo: %d\n", idarticulo);
    printf("Cantidad a vender: %d\n", cantidadVenta);
    printf("Precio total: %d\n", precioTotal);
    printf("\n----------------------------------------------------------\n");
    printf("\n\t1 - CONFIRMAR");
    printf("\n\t2 - CANCELAR\n\n\t--> ");
    val_elec = intValido();
    if (val_elec != 1 && val_elec != 2) {
      printf("\n\nOpción inválida. Por favor, intente nuevamente.");
      esperarEnter();
      clearS();
    } 
  } while (val_elec != 1 && val_elec != 2);
  switch (val_elec)
  {
    // En caso de confirmal la venta registramos y desconectamos de stock las mismas
  case 1:
    EXEC SQL BEGIN TRANSACTION;
    EXEC SQL SELECT nextval('venta_id_seq') INTO :id_venta_articulo;
    EXEC SQL INSERT INTO venta(id, valor) VALUES (:id_venta_articulo, :precioTotal);
    EXEC SQL INSERT INTO ventaarticulo (idventa, idarticulo, cantidad) VALUES (:id_venta_articulo, :idarticulo, :cantidadVenta);
    EXEC SQL COMMIT;
    desconectar();
    return 0;
    break;

    // De ser necesario, cancelamos la operacion
  case 2:
    printf("\n\nSE CANCELA LA VENTA\n");
    esperarEnter();
    desconectar();
    return 0;

  default:
    break;
  }
}

// Funcion que permite el realizar la reposición de artículos, esta puede ser llamada ya con el id del articulo a reponer como parametro
// en caso de no ser asi sellama con un -1 (cialquier numero negativo) como parametro y esta ya pedira el id dentro de la funcion 
int registroReposicion(int OptId){
  EXEC SQL BEGIN DECLARE SECTION;  
  int idarticulo = -1, precioTotal, cant, val_elec;
  bool validId = false;
  EXEC SQL END DECLARE SECTION;
  conectar();
  clearS();
  // Pedimos el id del articulo a reponer en caso de ser necesario y valida de que estas existan
  if (OptId < 0){
    do
    {
      title("REPOSICION");
      if (idarticulo != -1){
        clearS();
        title("REPOSICION");
        printf("\n\nNo existe dicho articulo, intente nuevamente\n\n\t--> ");
      }else
      {
        printf("\n\nIngrese el identificador del articulo a reponer\n\n\t--> ");
      }
      idarticulo = intValido();
      EXEC SQL SELECT sp_existeariculo(:idarticulo) INTO :validId;
    } while (!validId);
  }else{
    idarticulo = OptId;
  }
  clearS();

  // Pedimos la cantidad de elementos que se van a reponer al stock del articulo
  title("REPOSICION");
  printf("\n\nIngrese la cantidad a reponer\n\n\t--> ");
  cant = intValido();

  clearS();

  // Pedimos el costo de la reposicion
  title("REPOSICION");
  printf("\n\nIngrese el costo de la compra\n\n\t--> ");
  precioTotal = intValido();
  clearS();

  // Pedimos la confirmarmacion de la reposicion
  do{
    printf("\n----------------------------------------------------------\n");
    printf("CONFIRMAR REPOSICION");
    printf("\n----------------------------------------------------------\n");
    printf("Identificador del articulo: %d\n", idarticulo);
    printf("Cantidad a reponer: %d\n", cant);
    printf("Precio total: %d\n", precioTotal);
    printf("\n----------------------------------------------------------\n");
    printf("\n\t1 - CONFIRMAR");
    printf("\n\t2 - CANCELAR\n\n\t--> ");
    val_elec = intValido();
    if (val_elec != 1 && val_elec != 2) {
      printf("\n\nOpción inválida. Por favor, intente nuevamente.");
      esperarEnter();
    }
  } while (val_elec != 1 && val_elec != 2);

  switch (val_elec){
  case 1:
    // En caso de confirmar la operacion se genera la sentencias sql
    EXEC SQL BEGIN TRANSACTION;
    EXEC SQL INSERT INTO reposicion(idarticulo, costo, cantidad) VALUES (:idarticulo, :precioTotal, :cant);
    EXEC SQL COMMIT;
    desconectar();
    return 0;
    break;

  case 2:
    // En caso de cancelar la operacion 
    printf("\n\nSE CANCELA EL REGISTRO\n");
    esperarEnter();
    desconectar();
    return 0;

  default:
    break;
  }

}

// Funcion que nos permite el crear articulos (dar de alta)
int altaArticulo() {
  EXEC SQL BEGIN DECLARE SECTION;  
  int idarticulo, precioUni = 0, val_elec;
  char articulo_nombre[50], articulo_desc[100];
  EXEC SQL END DECLARE SECTION;
  clearS();

  // Limpiamos el buffer de entrada del teclado previamente sucio
  limpiarBuffer();

  conectar();

  // Ingresamos el nombre del articulo a dar de alta
  title("ALTA ARTICULO");
  printf("\n\nIngrese el nombre del articulo\n\n\t--> ");
  do {
    if (strlen(articulo_nombre) == 50 - 1 && articulo_nombre[50 - 2] != '\n') {
      clearS();
      title("ALTA ARTICULO");
      printf("\n\nLa longitud del nombre es demasiado larga, intente nuevamente\n\n\t--> ");
      if (strlen(articulo_nombre) == 50 - 1) {
        limpiarBuffer();
      }
    }

    fgets(articulo_nombre, sizeof(articulo_nombre), stdin);

    // se elimina el carácter de nueva línea al final del nombre
    articulo_nombre[strcspn(articulo_nombre, "\n")] = '\0';
  } while (strlen(articulo_nombre) == 0);

  clearS();

  // Ingresamos la descripcion del articulo a dar de alta
  title("ALTA ARTICULO");
  printf("\n\nIngrese la descripcion del articulo\n\n\t--> ");
  do {
    if (strlen(articulo_desc) == 100 - 1 && articulo_desc[100 - 2] != '\n') {
      clearS();
      title("ALTA ARTICULO");
      printf("\n\nLa longitud de la descripcion es demasiado larga, intente nuevamente\n\n\t--> ");
      if (strlen(articulo_desc) == 100 - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
      }
    }

    fgets(articulo_desc, sizeof(articulo_desc), stdin);

    // se elimina el carácter de nueva línea al final de la descripción
    articulo_desc[strcspn(articulo_desc, "\n")] = '\0';
  } while (strlen(articulo_desc) == 0);

  clearS();

  // Pedimos el valor por el cual se va a vender el articulo a futuro
  title("ALTA ARTICULO");
  printf("\n\nIngrese el precio de venta del articulo\n\n\t--> ");
  precioUni = intValido();
  clearS();

  // Pedimos confirmar el alta del articulo y de ser nesario el ya registrar la reposicion, tomando el id del articulo siendo inecesario consultarlo posteriormente
  do{
    printf("\n----------------------------------------------------------\n");
    printf("CONFIRMAR ARTICULO");
    printf("\n----------------------------------------------------------\n");
    printf("Nombre del articulo: %s\n", articulo_nombre);
    printf("Descripcion del articulo: %s\n", articulo_desc);
    printf("Precio de venta: %d\n", precioUni);
    printf("\n----------------------------------------------------------\n");
    printf("\n\t1 - CONFIRMAR");
    printf("\n\t2 - ASIGNAR REPOSICION");
    printf("\n\t3 - CANCELAR\n\n\t--> ");

    val_elec = intValido();

    if (val_elec != 1 && val_elec != 2 && val_elec != 3) {
      printf("\n\nOpción inválida. Por favor, intente nuevamente");
      esperarEnter();
    } 
  } while (val_elec != 1 && val_elec != 2 && val_elec != 3);

  switch (val_elec)
  {
  case 1:
    // Se registra el articulo sin reponer
    EXEC SQL BEGIN TRANSACTION;
    EXEC SQL INSERT INTO articulo(nombre, descripcion, valorventa) VALUES (:articulo_nombre, :articulo_desc, :precioUni);
    EXEC SQL COMMIT;
    desconectar();
    return 0;
    break;

  case 2:
    // Se registra el articulo y se llama a la funcion de reposicion con el id del articulo creado
    EXEC SQL BEGIN TRANSACTION;
    EXEC SQL INSERT INTO articulo(nombre, descripcion, valorventa) VALUES (:articulo_nombre, :articulo_desc, :precioUni);
    EXEC SQL SELECT currval('articulo_id_seq') INTO :idarticulo;
    EXEC SQL COMMIT;
    desconectar();
    registroReposicion(idarticulo);
    return 0;
    break;

  case 3:
    // Cancelamos la operacion
    printf("\n\nSE CANCELA EL REGISTRO\n");
    esperarEnter();
    desconectar();
    return 0;

  default:
    break;
  }
}

// Funcion que nos permite el obtener el total recaudado en un mes / año.
int consultarVentas(){
  EXEC SQL BEGIN DECLARE SECTION;  
  int foo;
  int anio = 0;
  int mes = 0;
  EXEC SQL END DECLARE SECTION;
  clearS();

  // Pedimos el año en el cual queremos consultar el total de ventas
  do {
    title("CONSULTAR VENTAS");
    printf("\n\nDe que AÑO quiere realizar la consulta? (2000 - 2050)\n\n\t-->");
    anio = intValido();

    if (anio < 2000 || anio > 2050) {
      printf("\n\nOpción inválida. Por favor, debe ingresar un año valido (2000 - 2050), intente nuevamente.");
      esperarEnter();
    }
  } while (( anio < 2000 || anio > 2050) || anio == 0);
  clearS();

  // Pedimos el mes en el cual queremos consultar el total de ventas
  do{
    title("CONSULTAR VENTAS");
    printf("\n\nDe que MES quiere realizar la consulta? (1-12)\n\n\t-->");
    mes = intValido();

    if (mes > 12) {
      printf("\n\nOpción inválida. Por favor, debe ingresar un mes valido (1-12), intente nuevamente.");
      esperarEnter();
    }
  } while (mes > 12 || mes == 0);

  // Realizamos la consulta que devuelve el total de ventas
  conectar();
  EXEC SQL SELECT sp_ventas(:mes, :anio) INTO :foo;
  desconectar();

  // Mostramos el resultado 
  if(foo == 0)
  {
    printf("\n\nNo se realizaron ventas en el periodo dado");
    printf("\n----------------------------------------------------------\n");
    esperarEnter();
  }else
  {
    printf("\n\nEl total recaudado en ventas es de: $%d", foo);
    printf("\n----------------------------------------------------------\n");
    esperarEnter();
  }
  
  return 0;
}

// Funcion (fuera de alcance) la cual permite modificar los precios de las entradas, tanto de mayores de edad como menores
int configPrecios(){
  EXEC SQL BEGIN DECLARE SECTION; 
  int actualPrecio, actualPrecioMenor, newValue, val_elec;
  EXEC SQL END DECLARE SECTION;
  clearS();

  // Pedimos los precios actualmente seteados 
  conectar();
  EXEC SQL SELECT valor_i INTO :actualPrecio FROM config WHERE elemento = 'valor_entrada';
  EXEC SQL SELECT valor_i INTO :actualPrecioMenor FROM config WHERE elemento = 'valor_entrada_menor';

  // Mostrasmos los precios
  title("CONFIGURAR PRECIOS");
  printf("Precio actual de las entradas: \t\t\t%d\n", actualPrecio);
  printf("Precio actual de las entradas para menores: \t%d\n", actualPrecioMenor);

  // Pedimos la accion a desear
  printf("\n\nQue desea hacer?");
  printf("\n\t1 - Cambiar el valor de la entrada");
  printf("\n\t2 - Cambiar el valor de la entrada para menores ");
  printf("\n\t3 - Salir\n\n\t--> ");
  val_elec = intValido();

  switch (val_elec){
  case 1:
    printf("\nIngrese el nuevo precio para la entrada\n\t--> $");
    newValue = intValido();
    EXEC SQL BEGIN TRANSACTION;
    EXEC SQL UPDATE config SET valor_i = :newValue WHERE elemento = 'valor_entrada';
    EXEC SQL COMMIT;
    desconectar();
    return 0;
    break;

  case 2:
    printf("\nIngrese el nuevo precio para la entrada para menores\n\t--> $");
    newValue = intValido();
    EXEC SQL BEGIN TRANSACTION;
    EXEC SQL UPDATE config SET valor_i = :newValue WHERE elemento = 'valor_entrada_menor';
    EXEC SQL COMMIT;
    desconectar();
    return 0;
    break;
  
  case 3:
    printf("\nSaliendo de configuracion");
    esperarEnter();
    desconectar();
    return 0;

  default:
    clearS();
    printf("\nLa opción que ingreso es invalidad. Por favor, intente de nuevo.\n"); 
    esperarEnter();
    configPrecios();
  }
}

// Menu principal
int menu() {
    int opcion;
    clearS();
    while (1) {
        title("MENU PRINCIPAL");
        printf("1. Venta de entradas\n");
        printf("2. Venta de artículos\n");
        printf("3. Crear articulos\n");
        printf("4. Realizar resposición de artículos\n");
        printf("5. Obtener el total recaudado (mes/año)\n");
        printf("6. Configurar precios de entradas\n");
        printf("0. Salir\n");
        printf("\n\t--> Ingrese su opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                ventaEntrada();
                clearS();
                break;
            case 2:
                ventaArticulo();
                clearS();
                break;
            case 3:
                altaArticulo();
                clearS();
                break;
            case 4:
                registroReposicion(-1);
                clearS();
                break;
            case 5:
                consultarVentas();
                clearS();
                break;
            case 6:
                configPrecios();
                clearS();
                break;
            case 0:
                clearS();

                printf("\nSaliendo del programa...\n\nDEV's:\n\t- Ezequiel Blandin\n\t- Milagros Vairo\n\t- Joaquin Sellanes\n\n\n");
                printf(" **           **     ******       ******   *******   *******        **** \n");
                printf("/**          ****   /*////**     /*////** /**////** /**////**      */// *\n");
                printf("/**         **//**  /*   /**     /*   /** /**    /**/**    /**    /    /*\n");
                printf("/**        **  //** /******      /******  /**    /**/**    /**       *** \n");
                printf("/**       **********/*//// **    /*//// **/**    /**/**    /**      *//  \n");
                printf("/**      /**//////**/*    /**    /*    /**/**    ** /**    **      *     \n");
                printf("/********/**     /**/*******     /******* /*******  /*******      /******\n");
                printf("//////// //      // ///////      ///////  ///////   ///////       ////// \n\n");

                esperarEnter();
                return 0;
            default:
                clearS();
                printf("\nLa opción que ingreso es invalidad. Por favor, intente de nuevo.\n");                       
        }
        printf("\n");
    }
    return 0;
}