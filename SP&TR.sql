
CREATE FUNCTION SP_Entradas() RETURNS TRIGGER AS $$
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

INSERT INTO venta (fecha, valor) VALUES ('2023-05-11', 0);
INSERT INTO ventaEntrada (idVenta, esMenor, diaDeLaSemana) VALUES (39, true, 'jueves');

---------------------------------------------------------------------------------------

