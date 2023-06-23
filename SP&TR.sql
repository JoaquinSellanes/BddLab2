---------------------------------------------------------------------------------------
-- INICIO DE TRIGGER
---------------------------------------------------------------------------------------

-- Trigger que se ejecuta antes de insertar un nuevo registro en "ventaentrada" que crea el registro necesario en la tabla "ventas".
CREATE OR REPLACE FUNCTION SP_VEntrada() RETURNS TRIGGER AS $$
DECLARE
	idventa integer;
    precio integer := 0;
BEGIN
	idventa = NEW.idventa;
	IF NEW.esmenor THEN
		SELECT valor_i INTO precio FROM config WHERE elemento = 'valor_entrada_menor';
	ELSE
		SELECT valor_i INTO precio FROM config WHERE elemento = 'valor_entrada';
	END IF;

	INSERT INTO venta(id, valor) VALUES (idventa, precio);
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER TR_VEntrada
BEFORE INSERT ON ventaentrada
FOR EACH ROW
EXECUTE PROCEDURE SP_VEntrada();

---------------------------------------------------------------------------------------

-- Trigger que se ejecuta despues de insertar un nuevo registro en "ventaentrada" el cual descuenta el stock de los tickets y regalos de entrada
CREATE OR REPLACE FUNCTION SP_Entradas() RETURNS TRIGGER AS $$
DECLARE
    idRegalo integer;
    stockRegalo integer;
BEGIN
	SELECT valor_i INTO idRegalo FROM config WHERE elemento = 'id_regalo_entrada';
	SELECT stock INTO stockRegalo FROM stock WHERE idarticulo = idRegalo;

	IF stockRegalo = 0 THEN
		RAISE WARNING 'No hay stock de regalos.';
	ELSE
		UPDATE stock SET stock = stock - 1 WHERE idarticulo = idRegalo;
	END IF;
	
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER TR_Entradas
AFTER INSERT ON ventaentrada
FOR EACH ROW
EXECUTE FUNCTION SP_Entradas();

---------------------------------------------------------------------------------------

-- Trigger qie se ejecuta al crear un registro en "ventaarticulo" el cual descuenta el stock del articulo vendido a la tabla stock
-- (No es necesario comprobar o lanzar exepcion en lo que si el stock es suficiente para la venta, se comprueba previamente en codigo co la SP_ExisteStock)
CREATE OR REPLACE FUNCTION SP_VArticulos() RETURNS TRIGGER AS $$
BEGIN
	UPDATE stock
	SET stock = stock - NEW.cantidad
	WHERE idarticulo = NEW.idarticulo;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER TR_VArticulos
AFTER INSERT ON ventaarticulo
FOR EACH ROW
EXECUTE FUNCTION SP_VArticulos();

---------------------------------------------------------------------------------------

-- Trigger que al crear un nuevo articulo este crea un nuevo registro de "stock" y lo setea en 0
CREATE OR REPLACE FUNCTION SP_AStock() RETURNS TRIGGER AS $$
BEGIN

	INSERT INTO stock(idarticulo, stock) VALUES (NEW.id, 0);
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER TR_AStock
AFTER INSERT ON articulo
FOR EACH ROW
EXECUTE FUNCTION SP_AStock();

---------------------------------------------------------------------------------------

-- Trigger que al registrar una reposicion esta le suma la cantidad adquirida al articulo correspondiente
CREATE OR REPLACE FUNCTION SP_ActualizarStock() RETURNS TRIGGER AS $$
BEGIN

	-- Validar que el id del articulo exista en stock.

	UPDATE stock
	SET stock = stock + NEW.cantidad
	WHERE idarticulo = NEW.idarticulo;

	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER TR_ActualizarStock
AFTER INSERT ON reposicion
FOR EACH ROW
EXECUTE FUNCTION SP_ActualizarStock();

---------------------------------------------------------------------------------------
-- FIN DE LOS TRIGGERS // INICIO DE LAS SP's SIMPLES
---------------------------------------------------------------------------------------

-- Proceso almacenado que devuelve un booleano en condicion de si el id del articulo pasado por parametro existe en la base de datos o no
CREATE OR REPLACE FUNCTION SP_ExisteAriculo(newid integer) RETURNS boolean AS $$
DECLARE
    contador integer;
BEGIN
    SELECT COUNT(*) INTO contador
    FROM articulo
    WHERE id = newid;

    RETURN contador > 0;
END;
$$ LANGUAGE plpgsql;

---------------------------------------------------------------------------------------

-- Proceso almacenado que devuelve un booleano en condicion si existe igual o menor cantidad de stock del articulo y cantidad pasada por parametro
CREATE OR REPLACE FUNCTION SP_ExisteStock(newid integer, newcant integer) RETURNS boolean AS $$
DECLARE
    foo integer;
BEGIN
    SELECT stock INTO foo
    FROM stock
	WHERE idarticulo = newid;

    RETURN newcant <= foo;
END;
$$ LANGUAGE plpgsql;

---------------------------------------------------------------------------------------

-- Proceso almacenado que devuelve el valor de venta del articulo solicitado por parametro
CREATE OR REPLACE FUNCTION SP_ValorArticulo(newid integer) RETURNS integer AS $$
DECLARE
	foo integer;
BEGIN
	SELECT valorventa INTO foo
	FROM articulo
	WHERE id = newid;

	RETURN foo;
END;
$$ LANGUAGE plpgsql;

---------------------------------------------------------------------------------------

-- Proceso almacenado que devuelve el total recaudado en ventas del mes y año pasado por parametros, si en este periodo no hay ventas devuelve 0
CREATE OR REPLACE FUNCTION SP_Ventas(mes integer, anio integer) RETURNS integer AS $$
DECLARE
	foo integer := 0;
BEGIN
	SELECT SUM(valor) INTO foo
	FROM venta
	WHERE EXTRACT(MONTH FROM fecha) = mes
	AND EXTRACT(YEAR FROM fecha) = anio	
	AND valor != 0;

	IF foo IS NULL THEN 
		RETURN 0;
	END IF;

RETURN foo;
END;
$$ LANGUAGE plpgsql;

---------------------------------------------------------------------------------------