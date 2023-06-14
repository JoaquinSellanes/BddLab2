-- Creacion de grupos.
-- ##
CREATE GROUP gVentas;
CREATE GROUP gServicios;
CREATE GROUP gGerencias;
CREATE GROUP gVisitas;

-- Creacion de usuarios.
-- ##
CREATE USER jmartinez 	WITH IN GROUP gVentas 		PASSWORD 'abc123';
CREATE USER pmorales 	WITH IN GROUP gServicios 	PASSWORD 'abc123';
CREATE USER rcastro 	WITH IN GROUP gGerencias 	PASSWORD 'abc123';
CREATE USER pantalla1 	WITH IN GROUP gVisitas 		PASSWORD 'abc123';

----------------------------------------------------------------------------
-- Permisos gVentas
-- ##
GRANT SELECT, INSERT
	ON venta, ventaEntrada, ventaArticulo
	TO gVentas;

GRANT USAGE
	ON SEQUENCE venta_id_seq
	TO gVentas;

GRANT SELECT, UPDATE, INSERT
	ON stock, articulo, reposicion
	TO gVentas;

GRANT USAGE
	ON SEQUENCE articulo_id_seq
	TO gVentas;

GRANT USAGE
	ON SEQUENCE reposicion_id_seq
	TO gVentas;

----------------------------------------------------------------------------
-- Permisos gServicios
-- ##
GRANT SELECT, INSERT, UPDATE, DELETE
	ON vehiculos, estadoVehiculo, fotosVehiculos
	TO gServicios;

GRANT USAGE
	ON SEQUENCE fotosvehiculos_foto_seq
	TO gServicios;

----------------------------------------------------------------------------
-- Permisos gVisitas
-- ##
GRANT SELECT, INSERT
	ON calificacion, fotoVisitante
	TO gVisitas;

GRANT USAGE
	ON SEQUENCE fotovisitante_idfoto_seq
	TO gVisitas;

GRANT USAGE
	ON SEQUENCE calificacion_id_seq
	TO gVisitas;

----------------------------------------------------------------------------
-- Permisos gGerencias
-- ##
GRANT SELECT, INSERT, UPDATE, DELETE
	ON ALL TABLES
	IN SCHEMA public
	TO gGerencias;

GRANT USAGE
	ON SEQUENCE venta_id_seq
	TO gGerencias;

GRANT USAGE
	ON SEQUENCE articulo_id_seq
	TO gGerencias;

GRANT USAGE
	ON SEQUENCE fotosvehiculos_foto_seq
	TO gGerencias;

GRANT USAGE
	ON SEQUENCE calificacion_id_seq
	TO gGerencias;

GRANT USAGE
	ON SEQUENCE fotovisitante_idfoto_seq
	TO gGerencias;

GRANT USAGE
	ON SEQUENCE reposicion_id_seq
	TO gGerencias;


----------------------------------------------------------------------------