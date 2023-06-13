
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

EXEC SQL BEGIN DECLARE SECTION;
char articulo_nombre[50];
char articulo_desc[100];
int id_venta_enterada, valor_entrada, foo, val_elec, idarticulo, cantidadVenta, precioUni, precioTotal,
id_venta_articulo, cant, mes, anio;
bool valido, validId, validCant;

EXEC SQL END DECLARE SECTION;

EXEC SQL WHENEVER SQLERROR SQLPRINT;
EXEC SQL WHENEVER SQLWARNING SQLPRINT;
EXEC SQL WHENEVER NOT FOUND SQLPRINT;

void limpiarUltimaLineas(int num) {
    printf("\033[%dA", num);
    printf("\033[0J");
}

void clearS(){
  system("cls||clear");
}

int intValido(){
  int foo;
  bool valido = false;
  do {
  if ((scanf("%d", &foo) == 1) && foo > 0) {
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

void titleVentaEntrada(){
  printf("\n\t### REGISTRO DE ENTRADA ###\n----------------------------------------------------------\n");
}
void titleVentaArticulo(){
  printf("\n\t### REGISTRO DE VENTA ###\n----------------------------------------------------------\n");
}
void titleAltaArticulo(){
  printf("\n\t### REGISTRO ARTICULO ###\n----------------------------------------------------------\n");
}
void titleReposicion(){
  printf("\n\t### REGISTRO REPOSICION ###\n----------------------------------------------------------\n");
}
void titleConsultaVentas(){
  printf("\n\t### CONSULTAR VENTAS ###\n----------------------------------------------------------\n");
}

int ventaEntrada(){
    clearS();
    titleVentaEntrada();
    int opcion;
    do {
        printf("\n\nEl visitante es menor de edad?");
        printf("\n\t1 - SI");
        printf("\n\t2 - NO\n\t--> ");
        opcion = intValido();
        if (opcion != 1 && opcion != 2) {
            printf("\n\nOpción inválida. Por favor, intente nuevamente, Enter para continuar...");
            getchar();
            getchar();
            clearS();
        }
    } while (opcion != 1 && opcion != 2);
    if (opcion == 1)
    {
        int id_venta_enterada;
        EXEC SQL CONNECT TO lab02@localhost:5432 USER postgres;
        EXEC SQL INSERT INTO venta DEFAULT VALUES;
        EXEC SQL SELECT currval('venta_id_seq') INTO :id_venta_enterada;
        EXEC SQL INSERT INTO ventaentrada (idventa, esmenor) VALUES (:id_venta_enterada, true);
        EXEC SQL COMMIT;
        EXEC SQL DISCONNECT ALL;
        printf("\n\nEntrada registrada correctamente.");
        printf("\n----------------------------------------------------------");
        printf("\nPresiona para continuar...\n\n");
        getchar();
        getchar();
        clearS();
    }
    if (opcion == 2)
    {
        int id_venta_enterada, valor_entrada, val_elec;
        limpiarUltimaLineas(10);
        do {
        printf("\nQue precio quiere asignarle?");
        printf("\n\t1 - Recomendado ($350)");
        printf("\n\t2 - Otro (Ingresar)\n\n\t--> ");
        val_elec = intValido();
        if (val_elec != 1 && val_elec != 2) {
            printf("\n\nOpción inválida. Por favor, intente nuevamente, Enter para continuar...");
            getchar();
            getchar();
            clearS();
        }
        } while (val_elec != 1 && val_elec != 2);

        switch (val_elec)
        {
        case 1:
            valor_entrada = 350;
            break;
        case 2:
            printf("\nIngrese el valor de la entrada: ");
            valor_entrada = intValido();
        default:
            break;
        }
        EXEC SQL CONNECT TO lab02@localhost:5432 USER postgres;
        EXEC SQL INSERT INTO venta(valor) VALUES (:valor_entrada);
        EXEC SQL SELECT currval('venta_id_seq') INTO :id_venta_enterada;
        EXEC SQL INSERT INTO ventaentrada (idventa, esmenor) VALUES (:id_venta_enterada, false);
        EXEC SQL COMMIT;
        EXEC SQL DISCONNECT ALL;
        printf("\n\nEntrada registrada correctamente.");
        printf("\n----------------------------------------------------------");
        printf("\nPresiona para continuar...\n\n");
        getchar();
        getchar();
        clearS();
    }
    return 0;
}

int ventaArticulo(){
  int idarticulo = -1;
  int cantidadVenta = 0;
  int precioUni = 0;
  int precioTotal = 0;
  bool validId = false;
  bool validCant;
  int val_elec, id_venta_articulo;
  EXEC SQL CONNECT TO lab02@localhost:5432 USER postgres;
  clearS();
  do
  {
    titleVentaArticulo();
    if (idarticulo != -1){
        clearS();
        titleVentaArticulo();
        printf("\n\nNo existe dicho articulo, intente nuevamente\n\n\t--> ");
    }else
    {
      printf("\n\nIngrese el identificador del articulo a vender\n\n\t--> ");
    }
    idarticulo = intValido();
    EXEC SQL SELECT sp_existeariculo(:idarticulo) INTO :validId;
  } while (!validId);
  clearS();
  titleVentaArticulo();
  printf("\n\nIngrese la cantidad a vender\n\n\t--> ");
  cantidadVenta = intValido();
  EXEC SQL SELECT sp_existestock(:idarticulo, :cantidadVenta) INTO :validCant;
  if (!validCant)
  {
    printf("\n\nNO EXISTE DICHA CANTIDAD EN STOCK --- SE CANCELA LA VENTA\n");
    printf("\n----------------------------------------------------------");
    printf("\nPresiona para continuar...\n\n");
    getchar();
    getchar();
    EXEC SQL DISCONNECT ALL;
    return 1;
  }
  EXEC SQL SELECT sp_valorarticulo(:idarticulo) INTO :precioUni;
  precioTotal = precioUni * cantidadVenta;
  clearS();
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
  do{
    if (val_elec != 1 && val_elec != 2) {
      printf("\n\nOpción inválida. Por favor, intente nuevamente, Enter para continuar...");
      getchar();
      getchar();
    } 
  } while (val_elec != 1 && val_elec != 2);
  switch (val_elec)
  {
  case 1:
    EXEC SQL INSERT INTO venta(valor) VALUES (:precioTotal);
    EXEC SQL SELECT currval('venta_id_seq') INTO :id_venta_articulo;
    EXEC SQL INSERT INTO ventaarticulo (idventa, idarticulo, cantidad) VALUES (:id_venta_articulo, :idarticulo, :cantidadVenta);
    EXEC SQL COMMIT;
    EXEC SQL DISCONNECT ALL;
    return 0;
    break;
  case 2:
    printf("\n\nSE CANCELA LA VENTA\n");
    printf("Enter para continuar...");
    getchar();
    getchar();
    EXEC SQL DISCONNECT ALL;
    return 0;
  default:
    break;
  }
}

int registroReposicion(int OptId){
  int idarticulo = -1, precioTotal, cant;
  bool validId = false;
  EXEC SQL CONNECT TO lab02@localhost:5432 USER postgres;
  clearS();
  if (OptId < 0){
    do
    {
      titleReposicion();
      if (idarticulo != -1){
          clearS();
          titleReposicion();
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
  titleReposicion();
  printf("\n\nIngrese la cantidad a reponer\n\n\t--> ");
  cant = intValido();
  clearS();
  titleReposicion();
  printf("\n\nIngrese el costo de la compra\n\n\t--> ");
  precioTotal = intValido();
  clearS();
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
  do{
    if (val_elec != 1 && val_elec != 2) {
      printf("\n\nOpción inválida. Por favor, intente nuevamente, Enter para continuar...");
      getchar();
      getchar();
    } 
  } while (val_elec != 1 && val_elec != 2);
  switch (val_elec)
  {
  case 1:
    EXEC SQL INSERT INTO reposicion(idarticulo, costo, cantidad) VALUES (:idarticulo, :precioTotal, :cant);
    EXEC SQL COMMIT;
    EXEC SQL DISCONNECT ALL;
    return 0;
    break;
  case 2:
    printf("\n\nSE CANCELA EL REGISTRO\n");
    printf("Enter para continuar...");
    getchar();
    getchar();
    EXEC SQL DISCONNECT ALL;
    return 0;
  default:
    break;
  }

}

int altaArticulo(){
  int idarticulo;
  char articulo_nombre[50];
  char articulo_desc[100];
  int precioUni = 0;
  int val_elec;
  EXEC SQL CONNECT TO lab02@localhost:5432 USER postgres;
  clearS();
  titleAltaArticulo();
  printf("\n\nIngrese el nombre del articulo\n\n\t--> ");
  do
  {
    if (strlen(articulo_nombre) == 50 - 1 && articulo_nombre[50 - 2] != '\n'){
      clearS();
      titleAltaArticulo();
      printf("\n\nLa longitud del nombre es demasiado larga, intente nuevamente\n\n\t--> ");
      if (strlen(articulo_nombre) == 50 - 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF) {}
    }
    }
    fgets(articulo_nombre, sizeof(articulo_nombre), stdin);
  } while (strlen(articulo_nombre) == 50 - 1 && articulo_nombre[50 - 2] != '\n');
  clearS();
  titleAltaArticulo();
  printf("\n\nIngrese la descripcion del articulo\n\n\t--> ");
  do
  {
    if (strlen(articulo_desc) == 100 - 1 && articulo_desc[100 - 2] != '\n'){
      clearS();
      titleAltaArticulo();
      printf("\n\nLa longitud de la descripcion es demasiado larga, intente nuevamente\n\n\t--> ");
      if (strlen(articulo_desc) == 100 - 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF) {}
    }
    }
    fgets(articulo_desc, sizeof(articulo_desc), stdin);
  } while (strlen(articulo_desc) == 100 - 1 && articulo_desc[100 - 2] != '\n');
  clearS();
  titleAltaArticulo();
  printf("\n\nIngrese el precio de venta del articulo\n\n\t--> ");
  precioUni = intValido();
  clearS();
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
  do{
    if (val_elec != 1 && val_elec != 2 && val_elec != 3) {
      printf("\n\nOpción inválida. Por favor, intente nuevamente, Enter para continuar...");
      getchar();
      getchar();
    } 
  } while (val_elec != 1 && val_elec != 2 && val_elec != 3);
  switch (val_elec)
  {
  case 1:
    EXEC SQL INSERT INTO articulo(nombre, descripcion, valorventa) VALUES (:articulo_nombre, :articulo_desc, :precioUni);
    EXEC SQL COMMIT;
    EXEC SQL DISCONNECT ALL;
    return 0;
    break;
  case 2:
    EXEC SQL INSERT INTO articulo(nombre, descripcion, valorventa) VALUES (:articulo_nombre, :articulo_desc, :precioUni);
    EXEC SQL SELECT currval('articulo_id_seq') INTO :idarticulo;
    EXEC SQL COMMIT;
    EXEC SQL DISCONNECT ALL;
    registroReposicion(idarticulo);
    return 0;
    break;
  case 3:
    printf("\n\nSE CANCELA EL REGISTRO\n");
    printf("Enter para continuar...");
    getchar();
    getchar();
    EXEC SQL DISCONNECT ALL;
    return 0;

  default:
    break;
  }
}

int consultarVentas(){
  int foo;
  int anio = 0;
  int mes = 0;
  clearS();
  do {
    titleConsultaVentas();
    printf("\n\nDe que AÑO quiere realizar la consulta? (2000 - 2050)\n\n\t-->");
    anio = intValido();
    if (anio < 2000 || anio > 2050) {
      printf("\n\nOpción inválida. Por favor, debe ingresar un año valido (2000 - 2050), intente nuevamente.\nEnter para continuar...");
      getchar();
      clearS();
    }
  } while (( anio < 2000 || anio > 2050) || anio == 0);
  clearS();
  do {
      titleConsultaVentas();
      printf("\n\nDe que MES quiere realizar la consulta? (1-12)\n\n\t-->");
      mes = intValido();
      if (mes > 12) {
        printf("\n\nOpción inválida. Por favor, debe ingresar un mes valido (1-12), intente nuevamente.\nEnter para continuar...");
        getchar();
        clearS();
      }
  } while (mes > 12 || mes == 0);
  EXEC SQL CONNECT TO lab02@localhost:5432 USER postgres;
  EXEC SQL SELECT sp_ventas(:mes, :anio) INTO :foo;
  EXEC SQL DISCONNECT ALL;
  if(foo == 0)
  {
    printf("\n\nNo se realizaron ventas en el periodo dado");
    printf("\n----------------------------------------------------------\n");
    printf("\n\tPresione enter para continuar...");
    getchar();
    getchar();
  }else
  {
    printf("\n\nEl total recaudado en ventas es de: $%d", foo);
    printf("\n----------------------------------------------------------\n");
    printf("\n\tPresione enter para continuar...");
    getchar();
    getchar();
  }
  
  return 0;
}


// menu
int menu() {
    int opcion;
clearConsole();
    while (1) {
        printf("MENU\n");
        printf("1. Venta de entradas\n");
        printf("2. Venta de artículos\n");
        printf("3. Crear articulos\n");
        printf("4. Realizar resposición de artículos\n");
        printf("5. Obtener el total recaudado (mes o año)\n");
        printf("0. Salir\n");
        printf("\nIngrese su opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                ventaEntrada();
                clearConsole();
                break;
            case 2:
                ventaArticulo();
                clearConsole();
                break;
            case 3:
                registroReposicion();
                clearConsole();
                break;
            case 4:
                altaArticulo();
                clearConsole();
                break;
            case 5:
                consultarVentas();
                clearConsole();
                break;    
            default:
                printf("\tOpción inválida. Por favor, seleccione una opción válida.\n");
                
        }

        printf("\n");
        clearConsole();
    }

    return 0;
}


int main(){
  if (sqlca.sqlcode != 0){
    printf("TIPO DE ERROR: %d\n", sqlca.sqlcode);
    printf("MENSAJE:%s\n", sqlca.sqlerrm.sqlerrmc);
  }
  menu();
  // ventaEntrada();
  // ventaArticulo();
  // altaArticulo();
  // registroReposicion(-1);
  // consultarVentas();
  return 0;
}
