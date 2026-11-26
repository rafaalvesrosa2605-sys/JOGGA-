/* =====================================================================
   JOGGA - DADOS DE EXEMPLO (Arena Central)
   Execute este arquivo inteiro (F5) DEPOIS do jogga.sql.
   Pode executar quantas vezes quiser: ele apaga os dados antigos
   e cadastra tudo de novo, sempre com os mesmos codigos (1, 2, 3...).
   Dia da semana na tabela_preco: 1 = domingo ... 7 = sabado
   ===================================================================== */

USE JOGGA;
GO

/* ---------- 0. LIMPAR DADOS ANTIGOS (nao precisa explicar) ---------- */
DELETE FROM pagamento;
DELETE FROM lista_espera;
DELETE FROM reserva;
DELETE FROM tabela_preco;
DELETE FROM quadra_modalidade;
DELETE FROM quadra;
DELETE FROM modalidade;
DELETE FROM usuario;
DELETE FROM arena;

/* faz os codigos (IDENTITY) voltarem a comecar do 1 */
IF EXISTS (SELECT 1 FROM sys.identity_columns WHERE object_id = OBJECT_ID('dbo.pagamento')    AND last_value IS NOT NULL) DBCC CHECKIDENT ('dbo.pagamento',    RESEED, 0);
IF EXISTS (SELECT 1 FROM sys.identity_columns WHERE object_id = OBJECT_ID('dbo.lista_espera') AND last_value IS NOT NULL) DBCC CHECKIDENT ('dbo.lista_espera', RESEED, 0);
IF EXISTS (SELECT 1 FROM sys.identity_columns WHERE object_id = OBJECT_ID('dbo.reserva')      AND last_value IS NOT NULL) DBCC CHECKIDENT ('dbo.reserva',      RESEED, 0);
IF EXISTS (SELECT 1 FROM sys.identity_columns WHERE object_id = OBJECT_ID('dbo.tabela_preco') AND last_value IS NOT NULL) DBCC CHECKIDENT ('dbo.tabela_preco', RESEED, 0);
IF EXISTS (SELECT 1 FROM sys.identity_columns WHERE object_id = OBJECT_ID('dbo.quadra')       AND last_value IS NOT NULL) DBCC CHECKIDENT ('dbo.quadra',       RESEED, 0);
IF EXISTS (SELECT 1 FROM sys.identity_columns WHERE object_id = OBJECT_ID('dbo.modalidade')   AND last_value IS NOT NULL) DBCC CHECKIDENT ('dbo.modalidade',   RESEED, 0);
IF EXISTS (SELECT 1 FROM sys.identity_columns WHERE object_id = OBJECT_ID('dbo.usuario')      AND last_value IS NOT NULL) DBCC CHECKIDENT ('dbo.usuario',      RESEED, 0);
IF EXISTS (SELECT 1 FROM sys.identity_columns WHERE object_id = OBJECT_ID('dbo.arena')        AND last_value IS NOT NULL) DBCC CHECKIDENT ('dbo.arena',        RESEED, 0);
GO

/* ---------- 1. ARENA (cliente da JOGGA) ---------- */
INSERT INTO arena (nome, cnpj, endereco, cidade, telefone)
VALUES ('Arena Central', '12345678000199', 'Av. dos Esportes, 500', 'São José dos Campos', '12999990000');

/* ---------- 2. MODALIDADES (esportes) ---------- */
INSERT INTO modalidade (nome) VALUES
  ('futebol society'),   -- 1
  ('futsal'),            -- 2
  ('beach tennis'),      -- 3
  ('futevolei'),         -- 4
  ('volei de praia');    -- 5

/* ---------- 3. QUADRAS ---------- */
INSERT INTO quadra (id_arena, nome, tipo_piso, coberta) VALUES
  (1, 'Quadra 1 - Society', 'grama sintetica', 0),   -- 1
  (1, 'Quadra 2 - Futsal',  'madeira',         1),   -- 2
  (1, 'Quadra 3 - Areia',   'areia',           0);   -- 3

/* ---------- 4. QUAIS ESPORTES CADA QUADRA ACEITA ---------- */
INSERT INTO quadra_modalidade (id_quadra, id_modalidade) VALUES
  (1, 1),             -- Society: futebol society
  (2, 2),             -- Futsal: futsal
  (3, 3), (3, 4), (3, 5);  -- Areia: beach tennis, futevolei, volei de praia

/* ---------- 5. HORARIOS E PRECOS (os 7 dias da semana) ----------
   Cada quadra funciona das 08h as 23h, com o MESMO preco em qualquer
   horario. Uma linha por quadra para cada dia da semana (3 x 7 = 21).  */
INSERT INTO tabela_preco (id_quadra, dia_semana, hora_inicio, hora_fim, valor_hora)
SELECT q.id_quadra, d.dia, '08:00', '23:00', q.preco
FROM (VALUES (1, 180.00),      -- Quadra 1 - Society
             (2, 150.00),      -- Quadra 2 - Futsal
             (3, 120.00)) AS q (id_quadra, preco)   -- Quadra 3 - Areia
CROSS JOIN (VALUES (1), (2), (3), (4), (5), (6), (7)) AS d (dia);

/* ---------- 6. USUARIOS (senha guardada como hash SHA-256) ---------- */
INSERT INTO usuario (nome, cpf, email, telefone, senha_hash, perfil, aceite_lgpd) VALUES
  ('Recepção Arena Central', '11111111111', 'recepcao@arenacentral.com', '12999990000',
     CONVERT(VARCHAR(64), HASHBYTES('SHA2_256', 'admin123'), 2), 'ADMIN',   GETDATE()),  -- 1
  ('Ana Souza',    '22222222222', 'ana@email.com',    '11990000001',
     CONVERT(VARCHAR(64), HASHBYTES('SHA2_256', 'ana123'), 2),   'CLIENTE', GETDATE()),  -- 2
  ('Bruno Lima',   '33333333333', 'bruno@email.com',  '11990000002',
     CONVERT(VARCHAR(64), HASHBYTES('SHA2_256', 'bruno123'), 2), 'CLIENTE', GETDATE()),  -- 3
  ('Carla Mendes', '44444444444', 'carla@email.com',  '11990000003',
     CONVERT(VARCHAR(64), HASHBYTES('SHA2_256', 'carla123'), 2), 'CLIENTE', GETDATE()),  -- 4
  ('Diego Santos', '55555555555', 'diego@email.com',  '11990000004',
     CONVERT(VARCHAR(64), HASHBYTES('SHA2_256', 'diego123'), 2), 'CLIENTE', GETDATE());  -- 5

/* ---------- 7. RESERVAS (quem, qual quadra, dia e hora) ---------- */
INSERT INTO reserva (id_usuario, id_quadra, data_reserva, hora_inicio, status, valor_total) VALUES
  (2, 1, '2026-12-05', '19:00', 'CONFIRMADA', 180.00),  -- 1 Ana   - Society - sab 05/12 19h
  (3, 3, '2026-12-05', '10:00', 'CONFIRMADA', 120.00),  -- 2 Bruno - Areia   - sab 05/12 10h
  (4, 2, '2026-12-05', '20:00', 'PENDENTE',   150.00),  -- 3 Carla - Futsal  - sab 05/12 20h
  (5, 1, '2026-12-07', '18:00', 'PENDENTE',   180.00),  -- 4 Diego - Society - seg 07/12 18h
  (2, 3, '2026-12-08', '09:00', 'PENDENTE',   120.00);  -- 5 Ana   - Areia   - ter 08/12 09h

/* ---------- 8. PAGAMENTOS ---------- */
INSERT INTO pagamento (id_reserva, forma, valor, status, pago_em) VALUES
  (1, 'PIX',      180.00, 'PAGO',     GETDATE()),  -- 1 reserva da Ana paga
  (2, 'CARTAO',   120.00, 'PAGO',     GETDATE()),  -- 2 reserva do Bruno paga
  (3, 'PIX',       75.00, 'PAGO',     GETDATE()),  -- 3 Carla: sinal dividido, 1a metade paga
  (3, 'PIX',       75.00, 'PENDENTE', NULL),       -- 4 Carla: 2a metade ainda pendente
  (5, 'DINHEIRO', 120.00, 'PENDENTE', NULL);       -- 5 Ana vai pagar no balcao

/* ---------- 9. LISTA DE ESPERA ---------- */
/* Bruno queria a Society no sabado 05/12 as 19h, mas a Ana ja reservou */
INSERT INTO lista_espera (id_usuario, id_quadra, data_reserva, hora_inicio, criado_em)
VALUES (3, 1, '2026-12-05', '19:00', GETDATE());
GO

/* ---------- 10. PROCEDURE: agenda de um dia ---------- */
CREATE OR ALTER PROCEDURE sp_agenda_do_dia
    @data DATE
AS
BEGIN
    SELECT q.nome                                AS quadra,
           CONVERT(VARCHAR(5), r.hora_inicio, 108) AS horario,
           u.nome                                AS jogador,
           r.status,
           r.valor_total
    FROM reserva r
    JOIN quadra  q ON q.id_quadra  = r.id_quadra
    JOIN usuario u ON u.id_usuario = r.id_usuario
    WHERE r.data_reserva = @data
    ORDER BY q.nome, r.hora_inicio;
END;
GO

PRINT 'Dados de exemplo da Arena Central cadastrados com sucesso!';
