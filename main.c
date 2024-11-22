#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char data[10];
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
    Tripulacao t;
    printf("Digite o código da tripulação: ");
    scanf("%d", &t.codigo);

    Tripulacao temp;
    while (fread(&temp, sizeof(Tripulacao), 1, file)) {
        if (temp.codigo == t.codigo) {
            printf("Código já existente!\n");
            fclose(file);
            return;
        }
    }

    printf("Digite o nome: ");
    scanf(" %[^\n]", t.nome);
    printf("Digite o telefone: ");
    scanf(" %[^\n]", t.telefone);
    printf("Digite o cargo (Piloto, Copiloto, Comissário): ");
    scanf(" %[^\n]", t.cargo);

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
    Voo v;
    printf("Digite o código do voo: ");
    scanf("%d", &v.codigo);
    printf("Digite a data (DD/MM/AAAA): ");
    scanf(" %[^\n]", v.data);
    printf("Digite a hora (HH:MM): ");
    scanf(" %[^\n]", v.hora);
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
        printf("Digite o código do comissário %d (0 para nenhum): ", i + 1);
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
