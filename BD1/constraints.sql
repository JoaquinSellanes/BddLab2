ALTER TABLE vehiculos ADD CONSTRAINT pk_vehiculos PRIMARY KEY (matricula);

ALTER TABLE fotosvehiculos ADD CONSTRAINT pk_fotosVehiculos PRIMARY KEY (matricula, foto);
ALTER TABLE fotosVehiculos ADD CONSTRAINT fk_fotosVehiculos FOREIGN KEY (matricula) REFERENCES vehiculos(matricula);

ALTER TABLE estadovehiculo ADD CONSTRAINT pk_estadoVehiculo PRIMARY KEY (matricula);
ALTER TABLE estadoVehiculo ADD CONSTRAINT fk_estadoVehiculo FOREIGN KEY (matricula) REFERENCES vehiculos(matricula);

ALTER TABLE venta ADD CONSTRAINT pk_venta PRIMARY KEY (id);

ALTER TABLE ventaentrada ADD CONSTRAINT pk_ventaEntrada PRIMARY KEY (idventa);
ALTER TABLE ventaEntrada ADD CONSTRAINT fk_ventaEntrada FOREIGN KEY (idventa) REFERENCES venta(id);

ALTER TABLE articulo ADD CONSTRAINT pk_articulo PRIMARY KEY (id);

ALTER TABLE ventaarticulo ADD CONSTRAINT pk_ventaArticulo PRIMARY KEY (idventa);
ALTER TABLE ventaArticulo ADD CONSTRAINT fk_ventaArticulo FOREIGN KEY (idArticulo) REFERENCES articulo(ID);
ALTER TABLE ventaArticulo ADD CONSTRAINT fk_ventaArticuloVentaId FOREIGN KEY (idventa) REFERENCES venta(id);

ALTER TABLE stock ADD CONSTRAINT pk_stock PRIMARY KEY (idarticulo);
ALTER TABLE stock ADD CONSTRAINT fk_stock FOREIGN KEY (idArticulo) REFERENCES articulo(ID);

ALTER TABLE reposicion ADD CONSTRAINT  pk_reposicion PRIMARY KEY (id);
ALTER TABLE reposicion ADD CONSTRAINT  fk_reposicion FOREIGN KEY (idarticulo) REFERENCES stock(idarticulo);

ALTER TABLE calificacion ADD CONSTRAINT pk_calificacion PRIMARY KEY (id);
ALTER TABLE calificacion ADD CONSTRAINT ck_estrellas CHECK ( estrellas > 0 AND estrellas < 6);

ALTER TABLE fotovisitante ADD CONSTRAINT pk_fotoVisitante PRIMARY KEY (idfoto);