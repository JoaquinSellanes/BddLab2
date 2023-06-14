CREATE DATABASE museo;

CREATE TABLE vehiculos(
    matricula       VARCHAR(8)      NOT NULL,
    marca           VARCHAR(32)     NOT NULL,
    modelo          VARCHAR(32)     NOT NULL,
    descripcion     VARCHAR(100)    DEFAULT 'none',
    anoFabricacion  INT             NOT NULL,
    paisOrigen      VARCHAR(32)     NOT NULL,
    color           VARCHAR(32)     DEFAULT 'none'
);

CREATE TABLE fotosVehiculos(
    foto            SERIAL,
    matricula       VARCHAR(8)      NOT NULL,
    encode          BYTEA           NOT NULL
);

CREATE TABLE estadoVehiculo(
    estado              BOOLEAN     NOT NULL,
    fechaActualizacion  DATE        NOT NULL,
    funcResponsable     VARCHAR(8)  NOT NULL,
    matricula           VARCHAR(8)  NOT NULL
);

CREATE TABLE venta(
    ID              SERIAL,
    fecha           DATE            DEFAULT current_date,
    valor           INT             DEFAULT 0
);

CREATE TABLE ventaEntrada(
    idVenta         INT             NOT NULL,
    esMenor         BOOLEAN         default true
    -- diaDeLaSemana   VARCHAR(10)     NOT NULL
);

CREATE TABLE articulo(
    ID              SERIAL,
    nombre          VARCHAR(50)     NOT NULL,
    descripcion     VARCHAR(100)    default 'none',
    valorVenta      INT             NOT NULL
);

CREATE TABLE ventaArticulo(
    idVenta         INT             NOT NULL,
    idArticulo      INT             NOT NULL,
    cantidad        INT             DEFAULT 1
);

CREATE TABLE stock(
    idArticulo      INT             NOT NULL,
    Stock           INT             NOT NULL
);

CREATE TABLE reposicion(
    ID              SERIAL,
    idarticulo      INT             NOT NULL,
    fecha           DATE            DEFAULT current_date,
    costo           INT             NOT NULL,
    cantidad        INT             NOT NULL
);

CREATE TABLE calificacion(
    ID              SERIAL,
    estrellas       INT             NOT NULL,
    fecha           DATE            DEFAULT current_date
);

CREATE TABLE fotoVisitante(
    IDfoto          SERIAL,
    fecha           DATE            DEFAULT current_date,
    encode          BYTEA           NOT NULL
);