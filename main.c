/*
 * JOGGA - Sistema de agendamento de quadras esportivas
 * PIM II - ESWA - UNIP
 *
 * main.c e o ponto de entrada: prepara a persistencia e abre a aplicacao.
 */
#include <stdio.h>
#include "src/repositorio/banco.h"
#include "src/apresentacao/app.h"

int main(void) {
    if (!banco_inicializar()) {
        printf("Erro ao preparar os arquivos de dados.\n");
        return 1;
    }
    app_executar();
    return 0;
}
