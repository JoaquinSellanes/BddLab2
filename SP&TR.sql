
CREATE OR REPLACE FUNCTION SP_Entradas() RETURNS TRIGGER AS $$
BEGIN
	UPDATE stock
	SET stock = stock - 1
	WHERE idarticulo IN (4, 6);
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER TR_Entradas
AFTER INSERT ON ventaentrada
FOR EACH ROW
EXECUTE FUNCTION SP_Entradas();

---------------------------------------------------------------------------------------

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

CREATE OR REPLACE FUNCTION SP_Reposicion(idrep integer, cant integer, precio integer) RETURNS VOID AS $$
BEGIN
	UPDATE stock 
	SET stock = stock + cant
	WHERE idarticulo = idrep;

	INSERT INTO reposicion(idarticulo, costo, cantidad) VALUES (idrep, precio, cant);
END;
$$ LANGUAGE plpgsql;

---------------------------------------------------------------------------------------



