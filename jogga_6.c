#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERFIL_CLIENTE 1
#define PERFIL_ADMIN   2

#define OK                      0
#define ERRO_LOGIN              1
#define ERRO_PERMISSAO          2
#define ERRO_DADOS_INVALIDOS    3
#define ERRO_DATA_INVALIDA      4
#define ERRO_HORARIO_INVALIDO   5
#define ERRO_QUADRA_INEXISTENTE 6
#define ERRO_CONFLITO           7
#define ERRO_NAO_ENCONTRADO     8
#define ERRO_ARQUIVO            9
#define ERRO_QUADRA_COM_RESERVAS 10

#define HORA_ABERTURA    8
#define HORA_FECHAMENTO 23
#define TOTAL_HORARIOS  15
#define MAX_QUADRAS     50
#define MAX_AGENDA     200
#define MAX_REGISTROS 2000

#define ARQ_USUARIOS     "usuarios.dat"
#define ARQ_QUADRAS      "quadras.dat"
#define ARQ_AGENDAMENTOS "agendamentos.dat"

typedef struct {
    int  id;
    char nome[50];
    char login[20];
    char senha[20];
    int  perfil;
} Usuario;

typedef struct {
    int   id;
    char  nome[40];
    char  tipo[20];
    float valor_hora;
} Quadra;

typedef struct {
    int  id;
    int  quadra_id;
    int  usuario_id;
    char data[11];
    int  hora;
} Agendamento;

Usuario logado;
Usuario usuario_encontrado;
Quadra nova_quadra;
Quadra quadra_encontrada;
Quadra lista_quadras[MAX_QUADRAS];
Quadra todas_quadras[MAX_QUADRAS];
Agendamento novo_agendamento;
Agendamento agendamento_encontrado;
Agendamento lista_agenda[MAX_AGENDA];
Agendamento todos_agendamentos[MAX_REGISTROS];

void banco_inicializar() {
    FILE *arquivo;
    Usuario usuarios[2] = {
        {1, "Cliente Exemplo", "cliente", "cliente123", PERFIL_CLIENTE},
        {2, "Administrador",   "admin",   "admin123",   PERFIL_ADMIN}
    };
    Quadra quadras[3] = {
        {1, "Quadra 1 - Society", "society", 180.0},
        {2, "Quadra 2 - Futsal",  "futsal",  150.0},
        {3, "Quadra 3 - Areia",   "areia",   120.0}
    };

    arquivo = fopen(ARQ_USUARIOS, "rb");
    if (arquivo == NULL) {
        arquivo = fopen(ARQ_USUARIOS, "wb");
        fwrite(usuarios, sizeof(Usuario), 2, arquivo);
    }
    fclose(arquivo);

    arquivo = fopen(ARQ_QUADRAS, "rb");
    if (arquivo == NULL) {
        arquivo = fopen(ARQ_QUADRAS, "wb");
        fwrite(quadras, sizeof(Quadra), 3, arquivo);
    }
    fclose(arquivo);
}

int usuario_repo_buscar_por_login(char login[]) {
    FILE *arquivo;
    Usuario u;
    arquivo = fopen(ARQ_USUARIOS, "rb");
    if (arquivo == NULL) return 0;
    while (fread(&u, sizeof(Usuario), 1, arquivo) == 1) {
        if (strcmp(u.login, login) == 0) {
            usuario_encontrado = u;
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return 0;
}

int quadra_repo_salvar() {
    FILE *arquivo;
    Quadra q;
    int maior = 0;
    arquivo = fopen(ARQ_QUADRAS, "rb");
    if (arquivo != NULL) {
        while (fread(&q, sizeof(Quadra), 1, arquivo) == 1) {
            if (q.id > maior) maior = q.id;
        }
        fclose(arquivo);
    }
    nova_quadra.id = maior + 1;
    arquivo = fopen(ARQ_QUADRAS, "ab");
    if (arquivo == NULL) return 0;
    fwrite(&nova_quadra, sizeof(Quadra), 1, arquivo);
    fclose(arquivo);
    return 1;
}

int quadra_repo_buscar(int id) {
    FILE *arquivo;
    Quadra q;
    arquivo = fopen(ARQ_QUADRAS, "rb");
    if (arquivo == NULL) return 0;
    while (fread(&q, sizeof(Quadra), 1, arquivo) == 1) {
        if (q.id == id) {
            quadra_encontrada = q;
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return 0;
}

int quadra_repo_listar(int max) {
    FILE *arquivo;
    int total = 0;
    arquivo = fopen(ARQ_QUADRAS, "rb");
    if (arquivo == NULL) return 0;
    while (total < max && fread(&lista_quadras[total], sizeof(Quadra), 1, arquivo) == 1) {
        total++;
    }
    fclose(arquivo);
    return total;
}

int quadra_repo_remover(int id) {
    FILE *arquivo;
    Quadra q;
    int total = 0, achou = 0, i;
    arquivo = fopen(ARQ_QUADRAS, "rb");
    if (arquivo == NULL) return 0;
    while (fread(&q, sizeof(Quadra), 1, arquivo) == 1) {
        if (q.id == id) {
            achou = 1;
        } else {
            if (total >= MAX_QUADRAS) {   /* evita perder quadras ao regravar */
                fclose(arquivo);
                return 0;
            }
            todas_quadras[total] = q;
            total++;
        }
    }
    fclose(arquivo);
    if (achou == 0) return 0;

    arquivo = fopen(ARQ_QUADRAS, "wb");
    if (arquivo == NULL) return 0;
    for (i = 0; i < total; i++) {
        fwrite(&todas_quadras[i], sizeof(Quadra), 1, arquivo);
    }
    fclose(arquivo);
    return 1;
}

int agenda_repo_salvar() {
    FILE *arquivo;
    Agendamento a;
    int maior = 0;
    arquivo = fopen(ARQ_AGENDAMENTOS, "rb");
    if (arquivo != NULL) {
        while (fread(&a, sizeof(Agendamento), 1, arquivo) == 1) {
            if (a.id > maior) maior = a.id;
        }
        fclose(arquivo);
    }
    novo_agendamento.id = maior + 1;
    arquivo = fopen(ARQ_AGENDAMENTOS, "ab");
    if (arquivo == NULL) return 0;
    fwrite(&novo_agendamento, sizeof(Agendamento), 1, arquivo);
    fclose(arquivo);
    return 1;
}

int agenda_repo_buscar(int id) {
    FILE *arquivo;
    Agendamento a;
    arquivo = fopen(ARQ_AGENDAMENTOS, "rb");
    if (arquivo == NULL) return 0;
    while (fread(&a, sizeof(Agendamento), 1, arquivo) == 1) {
        if (a.id == id) {
            agendamento_encontrado = a;
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return 0;
}

int agenda_repo_remover(int id) {
    FILE *arquivo;
    Agendamento a;
    int total = 0, achou = 0, i;
    arquivo = fopen(ARQ_AGENDAMENTOS, "rb");
    if (arquivo == NULL) return 0;
    while (total < MAX_REGISTROS && fread(&a, sizeof(Agendamento), 1, arquivo) == 1) {
        if (a.id == id) {
            achou = 1;
        } else {
            todos_agendamentos[total] = a;
            total++;
        }
    }
    fclose(arquivo);
    if (achou == 0) return 0;

    arquivo = fopen(ARQ_AGENDAMENTOS, "wb");
    if (arquivo == NULL) return 0;
    for (i = 0; i < total; i++) {
        fwrite(&todos_agendamentos[i], sizeof(Agendamento), 1, arquivo);
    }
    fclose(arquivo);
    return 1;
}

int agenda_repo_listar_por_quadra_data(int quadra_id, char data[], int max) {
    FILE *arquivo;
    Agendamento a;
    int total = 0;
    arquivo = fopen(ARQ_AGENDAMENTOS, "rb");
    if (arquivo == NULL) return 0;
    while (total < max && fread(&a, sizeof(Agendamento), 1, arquivo) == 1) {
        if (a.quadra_id == quadra_id && strcmp(a.data, data) == 0) {
            lista_agenda[total] = a;
            total++;
        }
    }
    fclose(arquivo);
    return total;
}

int agenda_repo_contar_por_quadra(int quadra_id) {
    FILE *arquivo;
    Agendamento a;
    int total = 0;
    arquivo = fopen(ARQ_AGENDAMENTOS, "rb");
    if (arquivo == NULL) return 0;
    while (fread(&a, sizeof(Agendamento), 1, arquivo) == 1) {
        if (a.quadra_id == quadra_id) total++;
    }
    fclose(arquivo);
    return total;
}

int agenda_repo_listar_por_data(char data[], int max) {
    FILE *arquivo;
    Agendamento a;
    int total = 0;
    arquivo = fopen(ARQ_AGENDAMENTOS, "rb");
    if (arquivo == NULL) return 0;
    while (total < max && fread(&a, sizeof(Agendamento), 1, arquivo) == 1) {
        if (strcmp(a.data, data) == 0) {
            lista_agenda[total] = a;
            total++;
        }
    }
    fclose(arquivo);
    return total;
}

int agenda_repo_listar_por_usuario(int usuario_id, int max) {
    FILE *arquivo;
    Agendamento a;
    int total = 0;
    arquivo = fopen(ARQ_AGENDAMENTOS, "rb");
    if (arquivo == NULL) return 0;
    while (total < max && fread(&a, sizeof(Agendamento), 1, arquivo) == 1) {
        if (a.usuario_id == usuario_id) {
            lista_agenda[total] = a;
            total++;
        }
    }
    fclose(arquivo);
    return total;
}

void mostrar_erro(int codigo) {
    switch (codigo) {
        case ERRO_LOGIN:              printf("\nLogin ou senha incorretos.\n"); break;
        case ERRO_PERMISSAO:          printf("\nVoce nao tem permissao para esta acao.\n"); break;
        case ERRO_DADOS_INVALIDOS:    printf("\nDados invalidos. Verifique os campos.\n"); break;
        case ERRO_DATA_INVALIDA:      printf("\nData invalida (use AAAA-MM-DD).\n"); break;
        case ERRO_HORARIO_INVALIDO:   printf("\nHorario fora do funcionamento (8h as 22h).\n"); break;
        case ERRO_QUADRA_INEXISTENTE: printf("\nQuadra nao encontrada.\n"); break;
        case ERRO_CONFLITO:           printf("\nJa existe agendamento nesta quadra, dia e horario.\n"); break;
        case ERRO_NAO_ENCONTRADO:     printf("\nRegistro nao encontrado.\n"); break;
        case ERRO_ARQUIVO:            printf("\nErro ao acessar os arquivos de dados.\n"); break;
        case ERRO_QUADRA_COM_RESERVAS: printf("\nA quadra possui reservas. Cancele as reservas antes de remover.\n"); break;
        default:                      printf("\nErro desconhecido.\n");
    }
}

int auth_login(char login[], char senha[]) {
    if (login[0] == '\0' || senha[0] == '\0') return ERRO_DADOS_INVALIDOS;
    if (usuario_repo_buscar_por_login(login) == 0) return ERRO_LOGIN;
    if (strcmp(usuario_encontrado.senha, senha) != 0) return ERRO_LOGIN;
    logado = usuario_encontrado;
    return OK;
}

int auth_eh_admin() {
    if (logado.perfil == PERFIL_ADMIN) return 1;
    return 0;
}

int quadra_listar(int max, int *qtd) {
    *qtd = quadra_repo_listar(max);
    return OK;
}

int quadra_cadastrar() {
    if (auth_eh_admin() == 0) return ERRO_PERMISSAO;
    if (nova_quadra.nome[0] == '\0' || nova_quadra.tipo[0] == '\0' || nova_quadra.valor_hora <= 0) {
        return ERRO_DADOS_INVALIDOS;
    }
    if (quadra_repo_salvar() == 0) return ERRO_ARQUIVO;
    return OK;
}

int quadra_remover(int id) {
    if (auth_eh_admin() == 0) return ERRO_PERMISSAO;
    if (quadra_repo_buscar(id) == 0) return ERRO_QUADRA_INEXISTENTE;
    if (agenda_repo_contar_por_quadra(id) > 0) return ERRO_QUADRA_COM_RESERVAS;
    if (quadra_repo_remover(id) == 0) return ERRO_ARQUIVO;
    return OK;
}

int dias_no_mes(int mes, int ano) {
    int dias[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0)) return 29;
    return dias[mes - 1];
}

int validar_data(char data[]) {
    int i, ano, mes, dia;
    if (data[10] != '\0' || data[4] != '-' || data[7] != '-') return ERRO_DATA_INVALIDA;
    for (i = 0; i < 10; i++) {
        if (i != 4 && i != 7 && (data[i] < '0' || data[i] > '9')) return ERRO_DATA_INVALIDA;
    }
    ano = (data[0] - '0') * 1000 + (data[1] - '0') * 100 + (data[2] - '0') * 10 + (data[3] - '0');
    mes = (data[5] - '0') * 10 + (data[6] - '0');
    dia = (data[8] - '0') * 10 + (data[9] - '0');
    if (mes < 1 || mes > 12) return ERRO_DATA_INVALIDA;
    if (dia < 1 || dia > dias_no_mes(mes, ano)) return ERRO_DATA_INVALIDA;
    return OK;
}

int validar_horario(int hora) {
    if (hora < HORA_ABERTURA || hora >= HORA_FECHAMENTO) return ERRO_HORARIO_INVALIDO;
    return OK;
}

int validar_conflito(int quadra_id, char data[], int hora) {
    int i, total;
    total = agenda_repo_listar_por_quadra_data(quadra_id, data, MAX_AGENDA);
    for (i = 0; i < total; i++) {
        if (lista_agenda[i].hora == hora) return ERRO_CONFLITO;
    }
    return OK;
}

int agenda_horarios_livres(int quadra_id, char data[], int livres[], int *qtd) {
    int ocupado[TOTAL_HORARIOS] = {0};
    int i, total, r;
    *qtd = 0;
    if (quadra_repo_buscar(quadra_id) == 0) return ERRO_QUADRA_INEXISTENTE;
    r = validar_data(data);
    if (r != OK) return r;
    total = agenda_repo_listar_por_quadra_data(quadra_id, data, MAX_AGENDA);
    for (i = 0; i < total; i++) {
        ocupado[lista_agenda[i].hora - HORA_ABERTURA] = 1;
    }
    for (i = 0; i < TOTAL_HORARIOS; i++) {
        if (ocupado[i] == 0) {
            livres[*qtd] = HORA_ABERTURA + i;
            (*qtd)++;
        }
    }
    return OK;
}

int agenda_agendar(int quadra_id, char data[], int hora) {
    int r;
    if (quadra_repo_buscar(quadra_id) == 0) return ERRO_QUADRA_INEXISTENTE;
    r = validar_data(data);
    if (r != OK) return r;
    r = validar_horario(hora);
    if (r != OK) return r;
    r = validar_conflito(quadra_id, data, hora);
    if (r != OK) return r;

    novo_agendamento.quadra_id = quadra_id;
    novo_agendamento.usuario_id = logado.id;
    strcpy(novo_agendamento.data, data);
    novo_agendamento.hora = hora;
    if (agenda_repo_salvar() == 0) return ERRO_ARQUIVO;
    return OK;
}

int agenda_cancelar(int agendamento_id) {
    if (agenda_repo_buscar(agendamento_id) == 0) return ERRO_NAO_ENCONTRADO;
    if (auth_eh_admin() == 0 && agendamento_encontrado.usuario_id != logado.id) return ERRO_PERMISSAO;
    if (agenda_repo_remover(agendamento_id) == 0) return ERRO_ARQUIVO;
    return OK;
}

int agenda_minhas_reservas(int *qtd) {
    *qtd = agenda_repo_listar_por_usuario(logado.id, MAX_AGENDA);
    return OK;
}

int agenda_do_dia(char data[], int *qtd) {
    *qtd = 0;
    if (auth_eh_admin() == 0) return ERRO_PERMISSAO;
    *qtd = agenda_repo_listar_por_data(data, MAX_AGENDA);
    return OK;
}

void ler_texto(char mensagem[], char destino[], int tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, tamanho, stdin) == NULL) {
        printf("\nAte a proxima partida!\n");
        exit(0);
    }
    destino[strcspn(destino, "\n")] = '\0';
}

int ler_inteiro(char mensagem[]) {
    char linha[20];
    int i, valor, valido;
    do {
        ler_texto(mensagem, linha, 20);
        valor = 0;
        valido = 1;
        if (linha[0] == '\0') valido = 0;
        for (i = 0; linha[i] != '\0'; i++) {
            if (linha[i] >= '0' && linha[i] <= '9') {
                valor = valor * 10 + (linha[i] - '0');
            } else {
                valido = 0;
            }
        }
        if (valido == 0) printf("Digite apenas numeros.\n");
    } while (valido == 0);
    return valor;
}

void pausar() {
    char tecla[5];
    ler_texto("\nPressione ENTER para continuar...", tecla, 5);
}

void titulo(char texto[]) {
    printf("\n========================================\n");
    printf("  %s\n", texto);
    printf("========================================\n");
}

int tela_login_exibir() {
    char login[20], senha[20];
    int r;
    while (1) {
        titulo("JOGGA - Agendamento de Quadras");
        printf("Digite 0 no login para sair.\n\n");
        ler_texto("Login: ", login, 20);
        if (strcmp(login, "0") == 0) return 0;
        ler_texto("Senha: ", senha, 20);
        r = auth_login(login, senha);
        if (r == OK) {
            printf("\nBem-vindo(a), %s!\n", logado.nome);
            return 1;
        }
        mostrar_erro(r);
    }
}

void tela_quadras_listar() {
    int i, qtd;
    quadra_listar(MAX_QUADRAS, &qtd);
    if (qtd == 0) {
        printf("\nNenhuma quadra cadastrada.\n");
        return;
    }
    /* larguras fixas: %-Ns completa o texto com espacos ate N caracteres */
    printf("\n%-4s %-30s %-15s %s\n", "ID", "NOME", "TIPO", "VALOR/HORA");
    for (i = 0; i < qtd; i++) {
        printf("%-4d %-30s %-15s R$ %.2f\n", lista_quadras[i].id, lista_quadras[i].nome,
               lista_quadras[i].tipo, lista_quadras[i].valor_hora);
    }
}

void tela_quadras_cadastrar() {
    char valor[20];
    int r;
    ler_texto("Nome da quadra: ", nova_quadra.nome, 40);
    ler_texto("Tipo (society, futsal, areia...): ", nova_quadra.tipo, 20);
    printf("Valor por hora (ex: 150.00): ");
    scanf("%f", &nova_quadra.valor_hora);
    fgets(valor, 20, stdin);
    r = quadra_cadastrar();
    if (r == OK) printf("\nQuadra cadastrada com o ID %d.\n", nova_quadra.id);
    else mostrar_erro(r);
}

void tela_quadras_remover() {
    char resposta[5];
    int id, r;
    tela_quadras_listar();
    id = ler_inteiro("\nID da quadra a remover: ");
    if (quadra_repo_buscar(id) == 0) {
        mostrar_erro(ERRO_QUADRA_INEXISTENTE);
        return;
    }
    printf("Remover \"%s\"? (S/N): ", quadra_encontrada.nome);
    ler_texto("", resposta, 5);
    if (resposta[0] != 'S' && resposta[0] != 's') {
        printf("\nRemocao cancelada.\n");
        return;
    }
    r = quadra_remover(id);
    if (r == OK) printf("\nQuadra removida.\n");
    else mostrar_erro(r);
}

void tela_quadras_exibir() {
    int opcao;
    do {
        titulo("QUADRAS");
        printf("1 - Listar quadras\n");
        if (auth_eh_admin() == 1) {
            printf("2 - Cadastrar quadra\n");
            printf("3 - Remover quadra\n");
        }
        printf("0 - Voltar\n");
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1:
                tela_quadras_listar();
                pausar();
                break;
            case 2:
                if (auth_eh_admin() == 1) {
                    tela_quadras_cadastrar();
                    pausar();
                } else {
                    printf("Opcao invalida.\n");
                }
                break;
            case 3:
                if (auth_eh_admin() == 1) {
                    tela_quadras_remover();
                    pausar();
                } else {
                    printf("Opcao invalida.\n");
                }
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

void imprimir_agendamentos(int qtd) {
    int i;
    if (qtd == 0) {
        printf("\nNenhum agendamento encontrado.\n");
        return;
    }
    printf("\nCODIGO\tDATA\t\tHORARIO\t\tQUADRA\n");
    for (i = 0; i < qtd; i++) {
        printf("%d\t%s\t%dh as %dh\t", lista_agenda[i].id, lista_agenda[i].data,
               lista_agenda[i].hora, lista_agenda[i].hora + 1);
        if (quadra_repo_buscar(lista_agenda[i].quadra_id) == 1) printf("%s\n", quadra_encontrada.nome);
        else printf("?\n");
    }
}

void consultar_livres() {
    char data[20];
    int livres[TOTAL_HORARIOS];
    int qtd, i, r, quadra_id;
    tela_quadras_listar();
    quadra_id = ler_inteiro("\nID da quadra: ");
    ler_texto("Data (AAAA-MM-DD): ", data, 20);
    r = agenda_horarios_livres(quadra_id, data, livres, &qtd);
    if (r != OK) {
        mostrar_erro(r);
        return;
    }
    if (qtd == 0) {
        printf("\nNenhum horario livre.\n");
        return;
    }
    printf("\nHorarios livres em %s:\n", data);
    for (i = 0; i < qtd; i++) {
        printf("  %dh", livres[i]);
    }
    printf("\n");
}

void agendar() {
    char data[20];
    int quadra_id, hora, r;
    tela_quadras_listar();
    quadra_id = ler_inteiro("\nID da quadra: ");
    ler_texto("Data (AAAA-MM-DD): ", data, 20);
    hora = ler_inteiro("Hora de inicio (8 a 22) - cada reserva dura 1 hora: ");
    r = agenda_agendar(quadra_id, data, hora);
    if (r == OK) {
        printf("\nReserva confirmada! Codigo: %d (%dh as %dh)\n", novo_agendamento.id,
               novo_agendamento.hora, novo_agendamento.hora + 1);
    } else {
        mostrar_erro(r);
    }
}

void minhas_reservas() {
    int qtd;
    agenda_minhas_reservas(&qtd);
    imprimir_agendamentos(qtd);
}

void cancelar() {
    int id, r;
    if (auth_eh_admin() == 0) minhas_reservas();
    id = ler_inteiro("\nCodigo da reserva a cancelar: ");
    r = agenda_cancelar(id);
    if (r == OK) printf("\nReserva cancelada.\n");
    else mostrar_erro(r);
}

void agenda_geral() {
    char data[20];
    int qtd, r;
    ler_texto("Data (AAAA-MM-DD): ", data, 20);
    r = agenda_do_dia(data, &qtd);
    if (r != OK) mostrar_erro(r);
    else imprimir_agendamentos(qtd);
}

void tela_agenda_exibir() {
    int opcao;
    do {
        titulo("AGENDAMENTOS");
        printf("1 - Consultar horarios livres\n");
        printf("2 - Agendar horario\n");
        printf("3 - Minhas reservas\n");
        printf("4 - Cancelar reserva\n");
        if (auth_eh_admin() == 1) printf("5 - Agenda geral do dia\n");
        printf("0 - Voltar\n");
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1: consultar_livres(); pausar(); break;
            case 2: agendar(); pausar(); break;
            case 3: minhas_reservas(); pausar(); break;
            case 4: cancelar(); pausar(); break;
            case 5:
                if (auth_eh_admin() == 1) {
                    agenda_geral();
                    pausar();
                } else {
                    printf("Opcao invalida.\n");
                }
                break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

void menu_principal() {
    int opcao;
    do {
        if (auth_eh_admin() == 1) titulo("MENU - ADMINISTRADOR");
        else titulo("MENU - CLIENTE");
        printf("1 - Agendamentos\n");
        printf("2 - Quadras\n");
        printf("0 - Sair (logout)\n");
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1: tela_agenda_exibir(); break;
            case 2: tela_quadras_exibir(); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

int main() {
    banco_inicializar();
    while (tela_login_exibir() == 1) {
        menu_principal();
    }
    printf("\nAte a proxima partida!\n");
    return 0;
}
