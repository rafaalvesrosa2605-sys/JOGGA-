/* =====================================================================
   JOGGA - ROTEIRO DE DEMONSTRACAO
   Antes: execute o jogga_dados_exemplo.sql uma vez.

   COMO USAR NA APRESENTACAO:
   selecione com o mouse SO o bloco do passo (do comentario ate o ;)
   e aperte F5. O SSMS executa apenas o que estiver selecionado.
   ===================================================================== */

USE JOGGA;
GO

/* ---------- PASSO 1: quadras cadastradas e seus esportes ---------- */
SELECT q.id_quadra                                  AS codigo,
       q.nome                                       AS quadra,
       q.tipo_piso                                  AS piso,
       CASE WHEN q.coberta = 1 THEN 'Sim' ELSE 'Não' END AS coberta,
       STRING_AGG(m.nome, ', ')                     AS esportes
FROM quadra q
JOIN quadra_modalidade qm ON qm.id_quadra    = q.id_quadra
JOIN modalidade m         ON m.id_modalidade = qm.id_modalidade
GROUP BY q.id_quadra, q.nome, q.tipo_piso, q.coberta
ORDER BY q.id_quadra;


/* ---------- PASSO 2: horario de funcionamento e preco de cada quadra (sabado) ----------
   O preco da hora e o mesmo em qualquer horario.                                 */
SELECT q.nome                                    AS quadra,
       'Sábado'                                  AS dia,
       CONVERT(VARCHAR(5), p.hora_inicio, 108)   AS das,
       CONVERT(VARCHAR(5), p.hora_fim, 108)      AS ate,
       p.valor_hora                              AS valor_por_hora
FROM tabela_preco p
JOIN quadra q ON q.id_quadra = p.id_quadra
WHERE p.dia_semana = 7          -- 1 = domingo ... 7 = sabado
ORDER BY q.nome, p.hora_inicio;


/* ---------- PASSO 3: todas as reservas (quem, quadra, dia e hora) ---------- */
SELECT r.id_reserva                                 AS codigo,
       u.nome                                       AS jogador,
       q.nome                                       AS quadra,
       CONVERT(VARCHAR(10), r.data_reserva, 103)    AS dia,
       CASE DATEDIFF(DAY, '19000107', r.data_reserva) % 7
            WHEN 0 THEN 'domingo'  WHEN 1 THEN 'segunda' WHEN 2 THEN 'terça'
            WHEN 3 THEN 'quarta'   WHEN 4 THEN 'quinta'  WHEN 5 THEN 'sexta'
            ELSE 'sábado' END                       AS dia_da_semana,
       CONVERT(VARCHAR(5), r.hora_inicio, 108)      AS horario,
       r.status,
       r.valor_total
FROM reserva r
JOIN usuario u ON u.id_usuario = r.id_usuario
JOIN quadra  q ON q.id_quadra  = r.id_quadra
ORDER BY r.data_reserva, r.hora_inicio;


/* ---------- PASSO 4: CADASTRAR UMA QUADRA NOVA "AO VIVO" ---------- */
INSERT INTO quadra (id_arena, nome, tipo_piso, coberta)
VALUES (1, 'Quadra 4 - Beach Tennis', 'areia', 1);

INSERT INTO quadra_modalidade (id_quadra, id_modalidade)
VALUES (4, 3);                  -- 3 = beach tennis

INSERT INTO tabela_preco (id_quadra, dia_semana, hora_inicio, hora_fim, valor_hora)
VALUES (4, 7, '08:00', '23:00', 200.00);   -- sabado, das 08h as 23h, R$ 200 a hora

SELECT * FROM quadra;           -- a Quadra 4 aparece na lista


/* ---------- PASSO 5: FAZER UMA RESERVA NA QUADRA NOVA ---------- */
INSERT INTO reserva (id_usuario, id_quadra, data_reserva, hora_inicio, valor_total)
VALUES (4, 4, '2026-12-05', '18:00', 200.00);   -- Carla, sabado 05/12 as 18h

SELECT * FROM reserva WHERE id_quadra = 4;   -- status PENDENTE entra sozinho (DEFAULT)


/* ---------- PASSO 6: A REGRA PRINCIPAL (vai dar ERRO de proposito) ----------
   Tenta reservar a MESMA quadra, no MESMO dia e na MESMA hora do passo 5.
   O banco recusa: "Violação da restrição UNIQUE KEY".                        */
INSERT INTO reserva (id_usuario, id_quadra, data_reserva, hora_inicio, valor_total)
VALUES (5, 4, '2026-12-05', '18:00', 200.00);   -- Diego tenta o mesmo horario


/* ---------- PASSO 7: horarios LIVRES da Quadra 1 no sabado 05/12 ---------- */
SELECT CONCAT(h.hora, 'h') AS horario_livre
FROM (VALUES (8),(9),(10),(11),(12),(13),(14),(15),
             (16),(17),(18),(19),(20),(21),(22)) AS h (hora)
WHERE NOT EXISTS (
    SELECT 1 FROM reserva r
    WHERE r.id_quadra    = 1
      AND r.data_reserva = '2026-12-05'
      AND DATEPART(HOUR, r.hora_inicio) = h.hora
);                              -- o 19h nao aparece: a Ana ja reservou


/* ---------- PASSO 8: UPDATE - Carla paga a 2a metade do Pix ---------- */
UPDATE pagamento SET status = 'PAGO', pago_em = GETDATE() WHERE id_pagamento = 4;
UPDATE reserva   SET status = 'CONFIRMADA'                WHERE id_reserva   = 3;

SELECT * FROM pagamento WHERE id_reserva = 3;
SELECT * FROM reserva   WHERE id_reserva = 3;


/* ---------- PASSO 9: DELETE - cancelar a reserva 5 ----------
   O pagamento dela some junto (ON DELETE CASCADE).                  */
SELECT * FROM pagamento WHERE id_reserva = 5;   -- antes: 1 pagamento
DELETE FROM reserva WHERE id_reserva = 5;
SELECT * FROM pagamento WHERE id_reserva = 5;   -- depois: vazio


/* ---------- PASSO 10: PROCEDURE - agenda do dia 05/12 ---------- */
EXEC sp_agenda_do_dia '2026-12-05';


/* ---------- PASSO 11: lista de espera ---------- */
SELECT u.nome AS jogador, q.nome AS quadra,
       CONVERT(VARCHAR(10), e.data_reserva, 103) AS dia,
       CONVERT(VARCHAR(5), e.hora_inicio, 108)   AS horario
FROM lista_espera e
JOIN usuario u ON u.id_usuario = e.id_usuario
JOIN quadra  q ON q.id_quadra  = e.id_quadra;


/* ---------- PASSO 12: senhas guardadas como hash (LGPD) ---------- */
SELECT nome, perfil, senha_hash FROM usuario;
