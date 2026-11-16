CREATE DATABASE JOGGA;
GO

USE JOGGA;
GO

CREATE TABLE arena (
  id_arena     INT          NOT NULL IDENTITY(1,1) PRIMARY KEY,
  nome         VARCHAR(80)  NOT NULL,
  cnpj         CHAR(14)     NOT NULL UNIQUE,
  endereco     VARCHAR(150) NOT NULL,
  cidade       VARCHAR(60)  NOT NULL DEFAULT 'São Paulo',
  telefone     VARCHAR(15),
  ativa        BIT          NOT NULL DEFAULT 1
);

CREATE TABLE quadra (
  id_quadra    INT          NOT NULL IDENTITY(1,1) PRIMARY KEY,
  id_arena     INT          NOT NULL,
  nome         VARCHAR(40)  NOT NULL,
  tipo_piso    VARCHAR(20)  NOT NULL,
  coberta      BIT          NOT NULL DEFAULT 0,
  ativa        BIT          NOT NULL DEFAULT 1,
  foreign key (id_arena) references arena (id_arena)
);

CREATE TABLE usuario (
  id_usuario   INT          NOT NULL IDENTITY(1,1) PRIMARY KEY,
  nome         VARCHAR(100) NOT NULL,
  cpf          CHAR(11)     NOT NULL UNIQUE,
  email        VARCHAR(120) NOT NULL UNIQUE,
  telefone     VARCHAR(15),
  senha_hash   VARCHAR(255) NOT NULL,
  perfil       VARCHAR(10)  NOT NULL DEFAULT 'CLIENTE',
  aceite_lgpd  DATETIME     NOT NULL
);

CREATE TABLE modalidade (
  id_modalidade INT         NOT NULL IDENTITY(1,1) PRIMARY KEY,
  nome          VARCHAR(30) NOT NULL UNIQUE
);

CREATE TABLE quadra_modalidade (
  id_quadra     INT NOT NULL,
  id_modalidade INT NOT NULL,
  primary key (id_quadra, id_modalidade),
  foreign key (id_quadra) references quadra (id_quadra),
  foreign key (id_modalidade) references modalidade (id_modalidade)
);

CREATE TABLE tabela_preco (
  id_preco     INT          NOT NULL IDENTITY(1,1) PRIMARY KEY,
  id_quadra    INT          NOT NULL,
  dia_semana   TINYINT      NOT NULL,
  hora_inicio  TIME         NOT NULL,
  hora_fim     TIME         NOT NULL,
  valor_hora   DECIMAL(8,2) NOT NULL,
  foreign key (id_quadra) references quadra (id_quadra)
);

CREATE TABLE reserva (
  id_reserva   INT          NOT NULL IDENTITY(1,1) PRIMARY KEY,
  id_usuario   INT          NOT NULL,
  id_quadra    INT          NOT NULL,
  data_reserva DATE         NOT NULL,
  hora_inicio  TIME         NOT NULL,
  status       VARCHAR(15)  NOT NULL DEFAULT 'PENDENTE',
  valor_total  DECIMAL(8,2) NOT NULL,
  unique (id_quadra, data_reserva, hora_inicio),
  foreign key (id_usuario) references usuario (id_usuario),
  foreign key (id_quadra) references quadra (id_quadra)
);

CREATE TABLE pagamento (
  id_pagamento INT          NOT NULL IDENTITY(1,1) PRIMARY KEY,
  id_reserva   INT          NOT NULL,
  forma        VARCHAR(10)  NOT NULL,
  valor        DECIMAL(8,2) NOT NULL,
  status       VARCHAR(10)  NOT NULL DEFAULT 'PENDENTE',
  pago_em      DATETIME,
  foreign key (id_reserva) references reserva (id_reserva)
  on delete cascade
);

CREATE TABLE lista_espera (
  id_espera    INT      NOT NULL IDENTITY(1,1) PRIMARY KEY,
  id_usuario   INT      NOT NULL,
  id_quadra    INT      NOT NULL,
  data_reserva DATE     NOT NULL,
  hora_inicio  TIME     NOT NULL,
  criado_em    DATETIME NOT NULL,
  foreign key (id_usuario) references usuario (id_usuario),
  foreign key (id_quadra) references quadra (id_quadra)
);
