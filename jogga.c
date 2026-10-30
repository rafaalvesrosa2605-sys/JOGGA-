/*
 * JOGGA - Sistema de agendamento de quadras esportivas
 * Equipe DevTeam - PIM II - ESWA - UNIP
 *
 * VERSAO EM UM UNICO ARQUIVO: e o mesmo sistema do repositorio (pastas
 * src/apresentacao, src/servico, src/repositorio e src/modelo), juntado aqui
 * para facilitar: basta colar no main.c de um projeto "Console application"
 * em C no Code::Blocks e apertar F9.
 * Cada bloco abaixo indica de qual arquivo/camada do repositorio ele veio.
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ===================== modelo/usuario.h ===================== */
#ifndef USUARIO_H
#define USUARIO_H

/* Camada MODELO - modulo de usuarios */

typedef enum {
    PERFIL_CLIENTE = 1,
    PERFIL_ADMIN   = 2
} Perfil;

typedef struct {
    int    id;
    char   nome[50];
    char   login[20];
    char   senha[20];
    Perfil perfil;
} Usuario;

#endif

/* ===================== modelo/quadra.h ===================== */
#ifndef QUADRA_H
#define QUADRA_H

/* Camada MODELO - modulo de quadras */

typedef struct {
    int   id;
    char  nome[40];
    char  tipo[20];      /* society, futsal, areia, poliesportiva... */
    float valor_hora;
} Quadra;

#endif

/* ===================== modelo/agendamento.h ===================== */
#ifndef AGENDAMENTO_H
#define AGENDAMENTO_H

/* Camada MODELO - modulo de agendamentos */

#define HORA_ABERTURA    8   /* primeiro horario: 08h */
#define HORA_FECHAMENTO 23   /* ultimo horario comeca as 22h */
#define TOTAL_HORARIOS  (HORA_FECHAMENTO - HORA_ABERTURA)

typedef struct {
    int  id;
    int  quadra_id;
    int  usuario_id;
    char data[11];       /* formato AAAA-MM-DD */
    int  hora;           /* hora de inicio (1 hora de duracao) */
} Agendamento;

#endif

/* ===================== servico/erros.h ===================== */
#ifndef ERROS_H
#define ERROS_H

/* Camada SERVICO - codigos de retorno padronizados das regras de negocio */

typedef enum {
    OK = 0,
    ERRO_LOGIN,              /* login ou senha incorretos            */
    ERRO_PERMISSAO,          /* perfil sem permissao para a acao     */
    ERRO_DADOS_INVALIDOS,    /* campos vazios ou valores invalidos   */
    ERRO_DATA_INVALIDA,      /* data fora do formato ou no passado   */
    ERRO_HORARIO_INVALIDO,   /* fora do horario de funcionamento     */
    ERRO_QUADRA_INEXISTENTE,
    ERRO_CONFLITO,           /* quadra ja reservada no dia/horario   */
    ERRO_NAO_ENCONTRADO,
    ERRO_ARQUIVO             /* falha ao ler/gravar os dados         */
} CodigoErro;

const char *erro_mensagem(int codigo);

#endif

/* ===================== repositorio/banco.h ===================== */
#ifndef BANCO_H
#define BANCO_H


/* Camada REPOSITORIO - infraestrutura de persistencia.
 * Os dados ficam em arquivos binarios dentro de uma pasta (padrao: "dados"). */

#define ARQ_USUARIOS     "usuarios.dat"
#define ARQ_QUADRAS      "quadras.dat"
#define ARQ_AGENDAMENTOS "agendamentos.dat"

void        banco_definir_pasta(const char *pasta);
const char *banco_caminho(const char *arquivo);
int         banco_inicializar(void);
int         banco_proximo_id(const char *arquivo, size_t tamanho_registro);

#endif

/* ===================== repositorio/usuario_repo.h ===================== */
#ifndef USUARIO_REPO_H
#define USUARIO_REPO_H


/* Camada REPOSITORIO - modulo de usuarios.
 * Retorno: 1 = sucesso/encontrado, 0 = falha/nao encontrado. */

int usuario_repo_salvar(Usuario *u);   /* gera o id automaticamente */
int usuario_repo_buscar_por_login(const char *login, Usuario *saida);
int usuario_repo_contar(void);

#endif

/* ===================== repositorio/quadra_repo.h ===================== */
#ifndef QUADRA_REPO_H
#define QUADRA_REPO_H


/* Camada REPOSITORIO - modulo de quadras */

int quadra_repo_salvar(Quadra *q);                 /* gera o id */
int quadra_repo_buscar(int id, Quadra *saida);     /* 1 = encontrada */
int quadra_repo_listar(Quadra lista[], int max);   /* devolve a quantidade */
int quadra_repo_contar(void);

#endif

/* ===================== repositorio/agenda_repo.h ===================== */
#ifndef AGENDA_REPO_H
#define AGENDA_REPO_H


/* Camada REPOSITORIO - modulo de agendamentos */

int agenda_repo_salvar(Agendamento *a);                 /* gera o id */
int agenda_repo_buscar(int id, Agendamento *saida);     /* 1 = encontrado */
int agenda_repo_remover(int id);                        /* 1 = removido */
int agenda_repo_listar_por_quadra_data(int quadra_id, const char *data,
                                       Agendamento lista[], int max);
int agenda_repo_listar_por_data(const char *data, Agendamento lista[], int max);
int agenda_repo_listar_por_usuario(int usuario_id, Agendamento lista[], int max);

#endif

/* ===================== servico/auth_service.h ===================== */
#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H


/* Camada SERVICO - modulo de usuarios (autenticacao) */

int auth_login(const char *login, const char *senha, Usuario *saida);
int auth_eh_admin(const Usuario *u);

#endif

/* ===================== servico/quadra_service.h ===================== */
#ifndef QUADRA_SERVICE_H
#define QUADRA_SERVICE_H


/* Camada SERVICO - modulo de quadras */

int quadra_listar(Quadra lista[], int max, int *quantidade);
int quadra_buscar(int id, Quadra *saida);
int quadra_cadastrar(const Usuario *quem, Quadra *nova);   /* somente ADMIN */

#endif

/* ===================== servico/agenda_service.h ===================== */
#ifndef AGENDA_SERVICE_H
#define AGENDA_SERVICE_H


/* Camada SERVICO - modulo de agendamentos (regras de negocio principais) */

int agenda_horarios_livres(int quadra_id, const char *data,
                           int livres[], int *quantidade);
int agenda_agendar(const Usuario *quem, int quadra_id, const char *data,
                   int hora, Agendamento *saida);
int agenda_cancelar(const Usuario *quem, int agendamento_id);
int agenda_minhas_reservas(const Usuario *quem, Agendamento lista[], int max,
                           int *quantidade);
int agenda_do_dia(const Usuario *quem, const char *data, Agendamento lista[],
                  int max, int *quantidade);                 /* somente ADMIN */

#endif

/* ===================== apresentacao/console.h ===================== */
#ifndef CONSOLE_H
#define CONSOLE_H

/* Camada APRESENTACAO - utilitarios de entrada e saida no terminal */

int  ler_inteiro(const char *mensagem);
void ler_texto(const char *mensagem, char destino[], int tamanho);
void pausar(void);
void titulo(const char *texto);

#endif

/* ===================== apresentacao/tela_login.h ===================== */
#ifndef TELA_LOGIN_H
#define TELA_LOGIN_H


/* Camada APRESENTACAO - modulo de usuarios.
 * Retorna 1 se o login deu certo, 0 se o usuario escolheu sair. */
int tela_login_exibir(Usuario *logado);

#endif

/* ===================== apresentacao/tela_quadras.h ===================== */
#ifndef TELA_QUADRAS_H
#define TELA_QUADRAS_H


/* Camada APRESENTACAO - modulo de quadras */
void tela_quadras_listar(void);
void tela_quadras_exibir(const Usuario *usuario);

#endif

/* ===================== apresentacao/tela_agenda.h ===================== */
#ifndef TELA_AGENDA_H
#define TELA_AGENDA_H


/* Camada APRESENTACAO - modulo de agendamentos */
void tela_agenda_exibir(const Usuario *usuario);

#endif

/* ===================== apresentacao/app.h ===================== */
#ifndef APP_H
#define APP_H

/* Camada APRESENTACAO - fluxo principal: login -> menu conforme o perfil */
void app_executar(void);

#endif

/* ===================== repositorio/banco.c ===================== */

#ifdef _WIN32
#include <direct.h>
#define CRIAR_PASTA(p) _mkdir(p)
#else
#include <sys/stat.h>
#define CRIAR_PASTA(p) mkdir(p, 0755)
#endif

static char pasta_dados[200] = "dados";

void banco_definir_pasta(const char *pasta) {
    strncpy(pasta_dados, pasta, sizeof(pasta_dados) - 1);
    pasta_dados[sizeof(pasta_dados) - 1] = '\0';
}

const char *banco_caminho(const char *arquivo) {
    static char caminho[300];
    snprintf(caminho, sizeof(caminho), "%s/%s", pasta_dados, arquivo);
    return caminho;
}

/* Le todos os registros do arquivo e devolve o maior id + 1.
 * Funciona para qualquer struct cujo primeiro campo seja "int id". */
int banco_proximo_id(const char *arquivo, size_t tamanho_registro) {
    unsigned char buffer[512];
    int maior = 0, id;
    FILE *f;

    if (tamanho_registro > sizeof(buffer)) return -1;
    f = fopen(banco_caminho(arquivo), "rb");
    if (f == NULL) return 1;
    while (fread(buffer, tamanho_registro, 1, f) == 1) {
        memcpy(&id, buffer, sizeof(int));
        if (id > maior) maior = id;
    }
    fclose(f);
    return maior + 1;
}

/* Cria a pasta de dados e, na primeira execucao, os dados de exemplo. */
int banco_inicializar(void) {
    CRIAR_PASTA(pasta_dados); /* se ja existir, apenas ignora */

    if (usuario_repo_contar() == 0) {
        Usuario cliente = {0, "Cliente Exemplo", "cliente", "cliente123", PERFIL_CLIENTE};
        Usuario admin   = {0, "Administrador",   "admin",   "admin123",   PERFIL_ADMIN};
        if (!usuario_repo_salvar(&cliente) || !usuario_repo_salvar(&admin)) return 0;
    }
    if (quadra_repo_contar() == 0) {
        Quadra q1 = {0, "Quadra 1 - Society", "society", 180.0f};
        Quadra q2 = {0, "Quadra 2 - Futsal",  "futsal",  150.0f};
        Quadra q3 = {0, "Quadra 3 - Areia",   "areia",   120.0f};
        if (!quadra_repo_salvar(&q1) || !quadra_repo_salvar(&q2) || !quadra_repo_salvar(&q3)) return 0;
    }
    return 1;
}

/* ===================== repositorio/usuario_repo.c ===================== */

int usuario_repo_salvar(Usuario *u) {
    FILE *f;
    u->id = banco_proximo_id(ARQ_USUARIOS, sizeof(Usuario));
    f = fopen(banco_caminho(ARQ_USUARIOS), "ab");
    if (f == NULL) return 0;
    fwrite(u, sizeof(Usuario), 1, f);
    fclose(f);
    return 1;
}

int usuario_repo_buscar_por_login(const char *login, Usuario *saida) {
    Usuario u;
    FILE *f = fopen(banco_caminho(ARQ_USUARIOS), "rb");
    if (f == NULL) return 0;
    while (fread(&u, sizeof(Usuario), 1, f) == 1) {
        if (strcmp(u.login, login) == 0) {
            *saida = u;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int usuario_repo_contar(void) {
    Usuario u;
    int total = 0;
    FILE *f = fopen(banco_caminho(ARQ_USUARIOS), "rb");
    if (f == NULL) return 0;
    while (fread(&u, sizeof(Usuario), 1, f) == 1) total++;
    fclose(f);
    return total;
}

/* ===================== repositorio/quadra_repo.c ===================== */

int quadra_repo_salvar(Quadra *q) {
    FILE *f;
    q->id = banco_proximo_id(ARQ_QUADRAS, sizeof(Quadra));
    f = fopen(banco_caminho(ARQ_QUADRAS), "ab");
    if (f == NULL) return 0;
    fwrite(q, sizeof(Quadra), 1, f);
    fclose(f);
    return 1;
}

int quadra_repo_buscar(int id, Quadra *saida) {
    Quadra q;
    FILE *f = fopen(banco_caminho(ARQ_QUADRAS), "rb");
    if (f == NULL) return 0;
    while (fread(&q, sizeof(Quadra), 1, f) == 1) {
        if (q.id == id) {
            *saida = q;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int quadra_repo_listar(Quadra lista[], int max) {
    int total = 0;
    FILE *f = fopen(banco_caminho(ARQ_QUADRAS), "rb");
    if (f == NULL) return 0;
    while (total < max && fread(&lista[total], sizeof(Quadra), 1, f) == 1) total++;
    fclose(f);
    return total;
}

int quadra_repo_contar(void) {
    Quadra q;
    int total = 0;
    FILE *f = fopen(banco_caminho(ARQ_QUADRAS), "rb");
    if (f == NULL) return 0;
    while (fread(&q, sizeof(Quadra), 1, f) == 1) total++;
    fclose(f);
    return total;
}

/* ===================== repositorio/agenda_repo.c ===================== */

#define MAX_REGISTROS 2000

int agenda_repo_salvar(Agendamento *a) {
    FILE *f;
    a->id = banco_proximo_id(ARQ_AGENDAMENTOS, sizeof(Agendamento));
    f = fopen(banco_caminho(ARQ_AGENDAMENTOS), "ab");
    if (f == NULL) return 0;
    fwrite(a, sizeof(Agendamento), 1, f);
    fclose(f);
    return 1;
}

int agenda_repo_buscar(int id, Agendamento *saida) {
    Agendamento a;
    FILE *f = fopen(banco_caminho(ARQ_AGENDAMENTOS), "rb");
    if (f == NULL) return 0;
    while (fread(&a, sizeof(Agendamento), 1, f) == 1) {
        if (a.id == id) {
            *saida = a;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

/* Le todos os registros, descarta o removido e regrava o arquivo. */
int agenda_repo_remover(int id) {
    static Agendamento todos[MAX_REGISTROS];
    Agendamento a;
    int total = 0, achou = 0, i;
    FILE *f = fopen(banco_caminho(ARQ_AGENDAMENTOS), "rb");
    if (f == NULL) return 0;
    while (total < MAX_REGISTROS && fread(&a, sizeof(Agendamento), 1, f) == 1) {
        if (a.id == id) achou = 1;
        else todos[total++] = a;
    }
    fclose(f);
    if (!achou) return 0;

    f = fopen(banco_caminho(ARQ_AGENDAMENTOS), "wb");
    if (f == NULL) return 0;
    for (i = 0; i < total; i++) fwrite(&todos[i], sizeof(Agendamento), 1, f);
    fclose(f);
    return 1;
}

int agenda_repo_listar_por_quadra_data(int quadra_id, const char *data,
                                       Agendamento lista[], int max) {
    Agendamento a;
    int total = 0;
    FILE *f = fopen(banco_caminho(ARQ_AGENDAMENTOS), "rb");
    if (f == NULL) return 0;
    while (total < max && fread(&a, sizeof(Agendamento), 1, f) == 1) {
        if (a.quadra_id == quadra_id && strcmp(a.data, data) == 0) lista[total++] = a;
    }
    fclose(f);
    return total;
}

int agenda_repo_listar_por_data(const char *data, Agendamento lista[], int max) {
    Agendamento a;
    int total = 0;
    FILE *f = fopen(banco_caminho(ARQ_AGENDAMENTOS), "rb");
    if (f == NULL) return 0;
    while (total < max && fread(&a, sizeof(Agendamento), 1, f) == 1) {
        if (strcmp(a.data, data) == 0) lista[total++] = a;
    }
    fclose(f);
    return total;
}

int agenda_repo_listar_por_usuario(int usuario_id, Agendamento lista[], int max) {
    Agendamento a;
    int total = 0;
    FILE *f = fopen(banco_caminho(ARQ_AGENDAMENTOS), "rb");
    if (f == NULL) return 0;
    while (total < max && fread(&a, sizeof(Agendamento), 1, f) == 1) {
        if (a.usuario_id == usuario_id) lista[total++] = a;
    }
    fclose(f);
    return total;
}

/* ===================== servico/erros.c ===================== */

const char *erro_mensagem(int codigo) {
    switch (codigo) {
        case OK:                      return "Operacao realizada com sucesso.";
        case ERRO_LOGIN:              return "Login ou senha incorretos.";
        case ERRO_PERMISSAO:          return "Voce nao tem permissao para esta acao.";
        case ERRO_DADOS_INVALIDOS:    return "Dados invalidos. Verifique os campos.";
        case ERRO_DATA_INVALIDA:      return "Data invalida ou no passado (use AAAA-MM-DD).";
        case ERRO_HORARIO_INVALIDO:   return "Horario fora do funcionamento (08h as 22h).";
        case ERRO_QUADRA_INEXISTENTE: return "Quadra nao encontrada.";
        case ERRO_CONFLITO:           return "Ja existe agendamento nesta quadra, dia e horario.";
        case ERRO_NAO_ENCONTRADO:     return "Registro nao encontrado.";
        case ERRO_ARQUIVO:            return "Erro ao acessar os arquivos de dados.";
        default:                      return "Erro desconhecido.";
    }
}

/* ===================== servico/auth_service.c ===================== */

int auth_login(const char *login, const char *senha, Usuario *saida) {
    Usuario u;
    if (login == NULL || senha == NULL || login[0] == '\0' || senha[0] == '\0')
        return ERRO_DADOS_INVALIDOS;
    if (!usuario_repo_buscar_por_login(login, &u)) return ERRO_LOGIN;
    if (strcmp(u.senha, senha) != 0) return ERRO_LOGIN;
    *saida = u;
    return OK;
}

int auth_eh_admin(const Usuario *u) {
    return u != NULL && u->perfil == PERFIL_ADMIN;
}

/* ===================== servico/quadra_service.c ===================== */

int quadra_listar(Quadra lista[], int max, int *quantidade) {
    *quantidade = quadra_repo_listar(lista, max);
    return OK;
}

int quadra_buscar(int id, Quadra *saida) {
    if (!quadra_repo_buscar(id, saida)) return ERRO_QUADRA_INEXISTENTE;
    return OK;
}

int quadra_cadastrar(const Usuario *quem, Quadra *nova) {
    if (!auth_eh_admin(quem)) return ERRO_PERMISSAO;               /* RN04 */
    if (nova->nome[0] == '\0' || nova->tipo[0] == '\0' || nova->valor_hora <= 0)
        return ERRO_DADOS_INVALIDOS;                                /* RN07 */
    if (!quadra_repo_salvar(nova)) return ERRO_ARQUIVO;
    return OK;
}

/* ===================== servico/agenda_service.c ===================== */

/* ---------- funcoes internas (static = privadas do modulo) ---------- */

static void data_de_hoje(char hoje[11], int *hora_atual) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    strftime(hoje, 11, "%Y-%m-%d", t);
    if (hora_atual != NULL) *hora_atual = t->tm_hour;
}

static int dias_no_mes(int mes, int ano) {
    int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int bissexto = (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
    if (mes == 2 && bissexto) return 29;
    return dias[mes - 1];
}

/* RN05: data no formato AAAA-MM-DD, existente e nao anterior a hoje */
static int validar_data(const char *data) {
    int ano, mes, dia;
    char hoje[11];
    if (data == NULL || strlen(data) != 10) return ERRO_DATA_INVALIDA;
    if (sscanf(data, "%4d-%2d-%2d", &ano, &mes, &dia) != 3) return ERRO_DATA_INVALIDA;
    if (data[4] != '-' || data[7] != '-') return ERRO_DATA_INVALIDA;
    if (mes < 1 || mes > 12 || dia < 1 || dia > dias_no_mes(mes, ano)) return ERRO_DATA_INVALIDA;
    data_de_hoje(hoje, NULL);
    if (strcmp(data, hoje) < 0) return ERRO_DATA_INVALIDA;
    return OK;
}

/* RN02: horario dentro do funcionamento; se for hoje, ainda nao pode ter passado */
static int validar_horario(const char *data, int hora) {
    char hoje[11];
    int hora_atual;
    if (hora < HORA_ABERTURA || hora >= HORA_FECHAMENTO) return ERRO_HORARIO_INVALIDO;
    data_de_hoje(hoje, &hora_atual);
    if (strcmp(data, hoje) == 0 && hora <= hora_atual) return ERRO_HORARIO_INVALIDO;
    return OK;
}

/* RN01 (regra principal): nunca dois agendamentos na mesma quadra, dia e horario */
static int validar_conflito(int quadra_id, const char *data, int hora) {
    Agendamento lista[TOTAL_HORARIOS];
    int i, total = agenda_repo_listar_por_quadra_data(quadra_id, data, lista, TOTAL_HORARIOS);
    for (i = 0; i < total; i++) {
        if (lista[i].hora == hora) return ERRO_CONFLITO;
    }
    return OK;
}

/* ---------- funcoes publicas ---------- */

int agenda_horarios_livres(int quadra_id, const char *data,
                           int livres[], int *quantidade) {
    Quadra q;
    int ocupado[TOTAL_HORARIOS] = {0};
    Agendamento lista[TOTAL_HORARIOS];
    int i, total, r;

    *quantidade = 0;
    if (!quadra_repo_buscar(quadra_id, &q)) return ERRO_QUADRA_INEXISTENTE;
    if ((r = validar_data(data)) != OK) return r;

    total = agenda_repo_listar_por_quadra_data(quadra_id, data, lista, TOTAL_HORARIOS);
    for (i = 0; i < total; i++) ocupado[lista[i].hora - HORA_ABERTURA] = 1;

    for (i = 0; i < TOTAL_HORARIOS; i++) {
        int hora = HORA_ABERTURA + i;
        if (!ocupado[i] && validar_horario(data, hora) == OK) livres[(*quantidade)++] = hora;
    }
    return OK;
}

int agenda_agendar(const Usuario *quem, int quadra_id, const char *data,
                   int hora, Agendamento *saida) {
    Quadra q;
    Agendamento novo;
    int r;

    if (quem == NULL) return ERRO_PERMISSAO;
    if (!quadra_repo_buscar(quadra_id, &q)) return ERRO_QUADRA_INEXISTENTE;   /* RN03 */
    if ((r = validar_data(data)) != OK) return r;                             /* RN05 */
    if ((r = validar_horario(data, hora)) != OK) return r;                    /* RN02 */
    if ((r = validar_conflito(quadra_id, data, hora)) != OK) return r;        /* RN01 */

    memset(&novo, 0, sizeof(novo));
    novo.quadra_id  = quadra_id;
    novo.usuario_id = quem->id;
    strcpy(novo.data, data);
    novo.hora = hora;
    if (!agenda_repo_salvar(&novo)) return ERRO_ARQUIVO;
    if (saida != NULL) *saida = novo;
    return OK;
}

/* RN06: cliente so cancela as proprias reservas; admin cancela qualquer uma */
int agenda_cancelar(const Usuario *quem, int agendamento_id) {
    Agendamento a;
    if (quem == NULL) return ERRO_PERMISSAO;
    if (!agenda_repo_buscar(agendamento_id, &a)) return ERRO_NAO_ENCONTRADO;
    if (!auth_eh_admin(quem) && a.usuario_id != quem->id) return ERRO_PERMISSAO;
    if (!agenda_repo_remover(agendamento_id)) return ERRO_ARQUIVO;
    return OK;
}

int agenda_minhas_reservas(const Usuario *quem, Agendamento lista[], int max,
                           int *quantidade) {
    *quantidade = 0;
    if (quem == NULL) return ERRO_PERMISSAO;
    *quantidade = agenda_repo_listar_por_usuario(quem->id, lista, max);
    return OK;
}

int agenda_do_dia(const Usuario *quem, const char *data, Agendamento lista[],
                  int max, int *quantidade) {
    *quantidade = 0;
    if (!auth_eh_admin(quem)) return ERRO_PERMISSAO;                          /* RN04 */
    *quantidade = agenda_repo_listar_por_data(data, lista, max);
    return OK;
}

/* ===================== apresentacao/console.c ===================== */

void ler_texto(const char *mensagem, char destino[], int tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    if (strchr(destino, '\n') == NULL) {         /* descarta o excesso digitado */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    destino[strcspn(destino, "\r\n")] = '\0';
}

int ler_inteiro(const char *mensagem) {
    char linha[32];
    char *fim;
    long valor;
    for (;;) {
        ler_texto(mensagem, linha, sizeof(linha));
        if (feof(stdin)) return 0;
        valor = strtol(linha, &fim, 10);
        if (fim != linha && *fim == '\0') return (int)valor;
        printf("Digite apenas numeros.\n");
    }
}

void pausar(void) {
    char tmp[4];
    ler_texto("\nPressione ENTER para continuar...", tmp, sizeof(tmp));
}

void titulo(const char *texto) {
    printf("\n========================================\n");
    printf("  %s\n", texto);
    printf("========================================\n");
}

/* ===================== apresentacao/tela_login.c ===================== */

int tela_login_exibir(Usuario *logado) {
    char login[20], senha[20];
    int r;
    for (;;) {
        titulo("JOGGA - Agendamento de Quadras");
        printf("Digite 0 no login para sair.\n\n");
        ler_texto("Login: ", login, sizeof(login));
        if (feof(stdin) || (login[0] == '0' && login[1] == '\0')) return 0;
        ler_texto("Senha: ", senha, sizeof(senha));

        r = auth_login(login, senha, logado);
        if (r == OK) {
            printf("\nBem-vindo(a), %s!\n", logado->nome);
            return 1;
        }
        printf("\n%s\n", erro_mensagem(r));
    }
}

/* ===================== apresentacao/tela_quadras.c ===================== */

#define MAX_QUADRAS 50

void tela_quadras_listar(void) {
    Quadra lista[MAX_QUADRAS];
    int i, qtd;
    quadra_listar(lista, MAX_QUADRAS, &qtd);
    printf("\n%-4s %-25s %-15s %s\n", "ID", "NOME", "TIPO", "VALOR/HORA");
    for (i = 0; i < qtd; i++) {
        printf("%-4d %-25s %-15s R$ %.2f\n", lista[i].id, lista[i].nome,
               lista[i].tipo, lista[i].valor_hora);
    }
    if (qtd == 0) printf("Nenhuma quadra cadastrada.\n");
}

static void cadastrar(const Usuario *usuario) {
    Quadra nova = {0};
    char valor[20];
    int r;
    ler_texto("Nome da quadra: ", nova.nome, sizeof(nova.nome));
    ler_texto("Tipo (society, futsal, areia...): ", nova.tipo, sizeof(nova.tipo));
    ler_texto("Valor por hora (ex: 150.00): ", valor, sizeof(valor));
    nova.valor_hora = (float)atof(valor);

    r = quadra_cadastrar(usuario, &nova);
    if (r == OK) printf("\nQuadra cadastrada com o ID %d.\n", nova.id);
    else         printf("\n%s\n", erro_mensagem(r));
}

void tela_quadras_exibir(const Usuario *usuario) {
    int opcao;
    do {
        titulo("QUADRAS");
        printf("1 - Listar quadras\n");
        if (auth_eh_admin(usuario)) printf("2 - Cadastrar quadra\n");
        printf("0 - Voltar\n");
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1: tela_quadras_listar(); pausar(); break;
            case 2:
                if (auth_eh_admin(usuario)) { cadastrar(usuario); pausar(); }
                else printf("Opcao invalida.\n");
                break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0 && !feof(stdin));
}

/* ===================== apresentacao/tela_agenda.c ===================== */

#define MAX_LISTA 200

static void imprimir_agendamentos(Agendamento lista[], int qtd) {
    Quadra q;
    int i;
    if (qtd == 0) {
        printf("Nenhum agendamento encontrado.\n");
        return;
    }
    printf("\n%-4s %-12s %-6s %s\n", "ID", "DATA", "HORA", "QUADRA");
    for (i = 0; i < qtd; i++) {
        const char *nome = quadra_buscar(lista[i].quadra_id, &q) == OK ? q.nome : "?";
        printf("%-4d %-12s %02dh    %s\n", lista[i].id, lista[i].data, lista[i].hora, nome);
    }
}

static void consultar_livres(void) {
    char data[16];
    int livres[TOTAL_HORARIOS], qtd, i, r, quadra_id;

    tela_quadras_listar();
    quadra_id = ler_inteiro("\nID da quadra: ");
    ler_texto("Data (AAAA-MM-DD): ", data, sizeof(data));

    r = agenda_horarios_livres(quadra_id, data, livres, &qtd);
    if (r != OK) {
        printf("\n%s\n", erro_mensagem(r));
        return;
    }
    printf("\nHorarios livres em %s:\n", data);
    for (i = 0; i < qtd; i++) printf("  %02dh", livres[i]);
    printf(qtd == 0 ? "Nenhum horario livre.\n" : "\n");
}

static void agendar(const Usuario *usuario) {
    char data[16];
    Agendamento novo;
    int quadra_id, hora, r;

    tela_quadras_listar();
    quadra_id = ler_inteiro("\nID da quadra: ");
    ler_texto("Data (AAAA-MM-DD): ", data, sizeof(data));
    hora = ler_inteiro("Hora de inicio (8 a 22): ");

    r = agenda_agendar(usuario, quadra_id, data, hora, &novo);
    if (r == OK) printf("\nReserva confirmada! Codigo: %d\n", novo.id);
    else         printf("\n%s\n", erro_mensagem(r));
}

static void minhas_reservas(const Usuario *usuario) {
    Agendamento lista[MAX_LISTA];
    int qtd;
    agenda_minhas_reservas(usuario, lista, MAX_LISTA, &qtd);
    imprimir_agendamentos(lista, qtd);
}

static void cancelar(const Usuario *usuario) {
    int id, r;
    if (!auth_eh_admin(usuario)) minhas_reservas(usuario);
    id = ler_inteiro("\nCodigo da reserva a cancelar: ");
    r = agenda_cancelar(usuario, id);
    printf("\n%s\n", r == OK ? "Reserva cancelada." : erro_mensagem(r));
}

static void agenda_geral(const Usuario *usuario) {
    Agendamento lista[MAX_LISTA];
    char data[16];
    int qtd, r;
    ler_texto("Data (AAAA-MM-DD): ", data, sizeof(data));
    r = agenda_do_dia(usuario, data, lista, MAX_LISTA, &qtd);
    if (r != OK) printf("\n%s\n", erro_mensagem(r));
    else         imprimir_agendamentos(lista, qtd);
}

void tela_agenda_exibir(const Usuario *usuario) {
    int opcao, admin = auth_eh_admin(usuario);
    do {
        titulo("AGENDAMENTOS");
        printf("1 - Consultar horarios livres\n");
        printf("2 - Agendar horario\n");
        printf("3 - Minhas reservas\n");
        printf("4 - Cancelar reserva\n");
        if (admin) printf("5 - Agenda geral do dia\n");
        printf("0 - Voltar\n");
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1: consultar_livres(); pausar(); break;
            case 2: agendar(usuario); pausar(); break;
            case 3: minhas_reservas(usuario); pausar(); break;
            case 4: cancelar(usuario); pausar(); break;
            case 5:
                if (admin) { agenda_geral(usuario); pausar(); }
                else printf("Opcao invalida.\n");
                break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0 && !feof(stdin));
}

/* ===================== apresentacao/app.c ===================== */

static void menu_principal(const Usuario *usuario) {
    int opcao;
    do {
        titulo(usuario->perfil == PERFIL_ADMIN ? "MENU - ADMINISTRADOR" : "MENU - CLIENTE");
        printf("1 - Agendamentos\n");
        printf("2 - Quadras\n");
        printf("0 - Sair (logout)\n");
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1: tela_agenda_exibir(usuario); break;
            case 2: tela_quadras_exibir(usuario); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0 && !feof(stdin));
}

void app_executar(void) {
    Usuario logado;
    while (tela_login_exibir(&logado)) {
        menu_principal(&logado);
        if (feof(stdin)) break;
    }
    printf("\nAte a proxima partida!\n");
}

/* ===================== main.c ===================== */
/*
 * JOGGA - Sistema de agendamento de quadras esportivas
 * PIM II - ESWA - UNIP
 *
 * main.c e o ponto de entrada: prepara a persistencia e abre a aplicacao.
 */

int main(void) {
    if (!banco_inicializar()) {
        printf("Erro ao preparar os arquivos de dados.\n");
        return 1;
    }
    app_executar();
    return 0;
}
