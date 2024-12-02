#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <locale.h>


#define MAX_COMISSARIOS 3

typedef struct {
    int codigo;
    char nome[50];
    char endereco[100];
    char telefone[15];
    int fidelidade;
    int pontos;
} Passageiro;

typedef struct {
    int codigo;
    char nome[50];
    char telefone[15];
    char cargo[20];
} Tripulacao;

typedef struct {
    int codigo;
    char data[11];
    char hora[6];
    char origem[50];
    char destino[50];
    char aviao[20];
    int piloto;
    int copiloto;
    int comissarios[MAX_COMISSARIOS];
    int status; // 1 ativo, 0 inativo
    float tarifa;
} Voo;

typedef struct {
    int numero;
    int codigoVoo;
    int status; // 1 ocupado, 0 livre
} Assento;

typedef struct {
    int codigoVoo;
    int numeroAssento;
    int codigoPassageiro;
} Reserva;


int validarDataHora(const char *data, const char *hora) {
    int dia, mes, ano, horas, minutos;
    time_t t = time(NULL);
    struct tm *tempoAtual = localtime(&t);

    int diaAtual = tempoAtual->tm_mday;
    int mesAtual = tempoAtual->tm_mon + 1;
    int anoAtual = tempoAtual->tm_year + 1900;
    int horaAtual = tempoAtual->tm_hour;
    int minutoAtual = tempoAtual->tm_min;

    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3 ||
        sscanf(hora, "%d:%d", &horas, &minutos) != 2) {
        printf("Formato de data ou hora inválido!\n");
        return 0;
    }

    if (ano < anoAtual || 
        (ano == anoAtual && mes < mesAtual) || 
        (ano == anoAtual && mes == mesAtual && dia < diaAtual)) {
        printf("Data anterior à atual!\n");
        return 0;
    }

    if (ano == anoAtual && mes == mesAtual && dia == diaAtual) {
        if (horas < horaAtual || (horas == horaAtual && minutos < minutoAtual)) {
            printf("Hora anterior à atual!\n");
            return 0;
        }
    }

    return 1;
}

void cadastrarPassageiro() {
    FILE *file = fopen("passageiros.bin", "ab+");
    Passageiro p;
    printf("Digite o código do passageiro: ");
    scanf("%d", &p.codigo);
    Passageiro temp;
    while (fread(&temp, sizeof(Passageiro), 1, file)) {
        if (temp.codigo == p.codigo) {
            printf("Código já existente!\n");
            fclose(file);
            return;
        }
    }
    printf("Digite o nome do passageiro: ");
    scanf(" %[^\n]", p.nome);
    printf("Digite o endereço: ");
    scanf(" %[^\n]", p.endereco);
    printf("Digite o telefone: ");
    scanf(" %[^\n]", p.telefone);
    printf("Fidelidade (1 para sim, 0 para não): ");
    scanf("%d", &p.fidelidade);
    p.pontos = 0;
    fwrite(&p, sizeof(Passageiro), 1, file);
    fclose(file);
    printf("Passageiro cadastrado com sucesso!\n");
}

void listarPassageiros() {
    FILE *file = fopen("passageiros.bin", "rb");
    if (!file) {
        printf("Nenhum passageiro cadastrado.\n");
        return;
    }
    Passageiro p;
    printf("\n--- Lista de Passageiros ---\n");
    while (fread(&p, sizeof(Passageiro), 1, file)) {
        printf("Código: %d, Nome: %s, Fidelidade: %d, Pontos: %d\n",
               p.codigo, p.nome, p.fidelidade, p.pontos);
    }
    fclose(file);
}

void cadastrarTripulacao() {
    FILE *file = fopen("tripulacao.bin", "ab+");
    if (!file) {
        printf("Erro ao abrir arquivo de tripulação.\n");
        return;
    }

    Tripulacao t;
    printf("Digite o código da tripulação: ");
    scanf("%d", &t.codigo);

    Tripulacao temp;
    while (fread(&temp, sizeof(Tripulacao), 1, file)) {
        if (temp.codigo == t.codigo) {
            printf("Código de tripulante já existente! Cadastro não realizado.\n");
            fclose(file);
            return;
        }
    }

    printf("Digite o nome: ");
    scanf(" %[^\n]", t.nome);
    printf("Digite o telefone: ");
    scanf(" %[^\n]", t.telefone);

    do {
        printf("Digite o cargo (Piloto, Copiloto, Comissario): ");
        scanf(" %[^\n]", t.cargo);
        if (strcmp(t.cargo, "Piloto") != 0 && strcmp(t.cargo, "Copiloto") != 0 && strcmp(t.cargo, "Comissario") != 0) {
            printf("Cargo inválido! Tente novamente.\n");
        } else {
            break;
        }
    } while (1);

    fwrite(&t, sizeof(Tripulacao), 1, file);
    fclose(file);
    printf("Tripulante cadastrado com sucesso!\n");
}

void listarTripulacao() {
    FILE *file = fopen("tripulacao.bin", "rb");
    if (!file) {
        printf("Nenhum membro da tripulação cadastrado.\n");
        return;
    }
    Tripulacao t;
    printf("\n--- Lista de Tripulantes ---\n");
    while (fread(&t, sizeof(Tripulacao), 1, file)) {
        printf("Código: %d, Nome: %s, Cargo: %s\n", t.codigo, t.nome, t.cargo);
    }
    fclose(file);
}

void cadastrarVoo() {
    FILE *file = fopen("voos.bin", "ab+");
    if (!file) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Voo v;
    printf("Digite o código do voo: ");
    scanf("%d", &v.codigo);

<<<<<<< HEAD
    do 
        printf("Digite a data do voo (dd/mm/aaaa): ");
        scanf(" %[^\n]", v.data);
        printf("Digite a hora do voo (hh:mm): ");
        scanf(" %[^\n]", v.hora);
    } while (!validarDataHora(v.data, v.hora));
=======
    // Obtendo a data e hora atuais
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Formatando a data e hora
    snprintf(v.data, sizeof(v.data), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    snprintf(v.hora, sizeof(v.hora), "%02d:%02d", tm.tm_hour, tm.tm_min);

    printf("Data mínima do voo: %s\n", v.data);
    printf("Hora mínima do voo: %s\n", v.hora);

    // Verificando se a data é vál ida
    char data_input[11];
    printf("Digite a data do voo (dd/mm/aaaa): ");
    scanf(" %[^\n]", data_input);

    struct tm data_v;
    if (strptime(data_input, "%d/%m/%Y", &data_v) == NULL) {
        printf("Data inválida! Formato deve ser dd/mm/aaaa.\n");
        fclose(file);
        return;
    }

    // Verificando se a data é no futuro
    time_t data_v_time = mktime(&data_v);
    if (difftime(data_v_time, t) < 0) {
        printf("Data não pode ser no passado!\n");
        fclose(file);
        return;
    }

    strcpy(v.data, data_input);
    printf("Digite a hora do voo (hh:mm): ");
    scanf(" %[^\n]", v.hora);

    // Verificando se a hora está no formato correto
    if (sscanf(v.hora, "%2d:%2d", &tm.tm_hour, &tm.tm_min) != 2 || tm.tm_hour > 23 || tm.tm_min > 59) {
        printf("Hora inválida! Formato deve ser hh:mm.\n");
        fclose(file);
        return;
    }
>>>>>>> aa4525cf65696d84fb2fc28bb540fac2d51275be

    printf("Digite a origem: ");
    scanf(" %[^\n]", v.origem);
    printf("Digite o destino: ");
    scanf(" %[^\n]", v.destino);
    printf("Digite o avião: ");
    scanf(" %[^\n]", v.aviao);
    printf("Digite o código do piloto: ");
    scanf("%d", &v.piloto);
    printf("Digite o código do copiloto: ");
    scanf("%d", &v.copiloto);
    printf("Digite a tarifa: ");
    scanf("%f", &v.tarifa);
    for (int i = 0; i < MAX_COMISSARIOS; i++) {
        printf("Digite o código do comissario %d (0 para nenhum): ", i + 1);
        scanf("%d", &v.comissarios[i]);
    }
    v.status = (v.piloto > 0 && v.copiloto > 0) ? 1 : 0;
    fwrite(&v, sizeof(Voo), 1, file);
    fclose(file);
    printf("Voo cadastrado com sucesso!\n");

}

void listarVoos() {
    FILE *file = fopen("voos.bin", "rb");
    if (!file) {
        printf("Nenhum voo cadastrado.\n");
        return;
    }
    Voo v;
    printf("\n--- Lista de Voos ---\n");
    while (fread(&v, sizeof(Voo), 1, file)) {
        printf("Código: %d, Origem: %s, Destino: %s, Status: %s\n",
               v.codigo, v.origem, v.destino, v.status ? "Ativo" : "Inativo");
    }
    fclose(file);
}

void cadastrarAssentos(int codigoVoo) {
    FILE *file = fopen("assentos.bin", "ab+");
    if (!file) {
        printf("Erro ao abrir o arquivo de assentos.\n");
        return;
    }
    int numAssentos;
    printf("Digite o número de assentos para o voo %d: ", codigoVoo);
    scanf("%d", &numAssentos);

    for (int i = 1; i <= numAssentos; i++) {
        Assento a;
        a.numero = i;
        a.codigoVoo = codigoVoo;
        a.status = 0;
        fwrite(&a, sizeof(Assento), 1, file);
    }
    fclose(file);
    printf("Assentos cadastrados com sucesso!\n");
}

int main() {
    int opcao;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Cadastrar Passageiro\n");
        printf("2. Listar Passageiros\n");
        printf("3. Cadastrar Tripulação\n");
        printf("4. Listar Tripulação\n");
        printf("5. Cadastrar Voo\n");
        printf("6. Listar Voos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                cadastrarPassageiro();
                break;
            case 2:
                listarPassageiros();
                break;
            case 3:
                cadastrarTripulacao();
                break;
            case 4:
                listarTripulacao();
                break;
            case 5:
                cadastrarVoo();
                break;
            case 6:
                listarVoos();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    return 0;
}
