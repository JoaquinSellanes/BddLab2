-- Se crea el grupo.
CREATE GROUP gSistema;

-- Se crea el usuario con una password autogenerada.
CREATE USER ventaSistema 	WITH IN GROUP gSistema 		PASSWORD 'aRbOliON';

-- ## Permisos ##

-- Da permisos de traer información.
GRANT SELECT, UPDATE
	ON config
	TO gSistema;

-- Permite select e insert.
GRANT SELECT, INSERT
	ON venta, ventaEntrada, ventaArticulo
	TO gSistema;

-- Permite select, update and insert.
GRANT SELECT, UPDATE, INSERT
	ON stock, articulo, reposicion
	TO gSistema;

-- Permite el uso de la sequence. 
GRANT USAGE
	ON SEQUENCE venta_id_seq
	TO gSistema;

-- Permite el uso de la sequence. 
GRANT USAGE
	ON SEQUENCE articulo_id_seq
	TO gSistema;

-- Permite el uso de la sequence. 
GRANT USAGE
	ON SEQUENCE reposicion_id_seq
	TO gSistema;