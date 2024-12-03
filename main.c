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
    int status;
    float tarifa;
} Voo;

typedef struct {
    int numero;
    int codigoVoo;
    int status;
    
} Assento;

typedef struct {
    int codigoVoo;
    int numeroAssento;
    int codigoPassageiro;
} Reserva;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lerOpcao() {
    int opcao;
    char entrada[10];

    while (1) {
        printf("Escolha uma opção: ");
        if (fgets(entrada, sizeof(entrada), stdin) != NULL) {
            if (sscanf(entrada, "%d", &opcao) == 1) {
                return opcao;
            } else {
                printf("Entrada inválida! Digite um número.\n");
            }
        } else {
            printf("Erro ao ler entrada!\n");
        }
    }
}

int validarTelefone(const char *telefone) {
    // Verifica se o telefone começa com 0 e tem 14 caracteres (incluindo o código de área)
    if (telefone[0] == '0' && strlen(telefone) == 14) {
        // Verifica se os primeiros 3 caracteres são o código de área e o resto são dígitos
        for (int i = 0; i < 14; i++) {
            if (i == 0 || i == 1 || i == 2) {
                if (!isdigit(telefone[i])) {
                    return 0; // Não é um dígito
                }
            } else {
                if (!isdigit(telefone[i]) && telefone[i] != '-') {
                    return 0; // Não é um dígito ou um hífen
                }
            }
        }
        return 1; // Telefone válido
    }
    return 0; // Telefone inválido
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

    // Validação do telefone
    do {
        printf("Digite o telefone (formato: 0XX-XXXXX-XXXX): ");
        scanf(" %[^\n]", p.telefone);
        if (!validarTelefone(p.telefone)) {
            printf("Telefone inválido! Tente novamente.\n");
        }
    } while (!validarTelefone(p.telefone));

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
    int codigoUnico = 1;

    // Garantir código único para o tripulante
    do {
        FILE *verificaFile = fopen("tripulacao.bin", "rb");
        if (!verificaFile) {
            printf("Erro ao abrir o arquivo de verificação.\n");
            fclose(file);
            return;
        }

        printf("Digite o código da tripulação: ");
        scanf("%d", &t.codigo);

        Tripulacao temp;
        while (fread(&temp, sizeof(Tripulacao), 1, verificaFile)) {
            if (temp.codigo == t.codigo) {
                printf("Código de tripulante já existente! Por favor, insira outro código.\n");
                codigoUnico = 0;
                break;
            }
        }
        fclose(verificaFile);
    } while (!codigoUnico);

    // Informações do tripulante
    printf("Digite o nome: ");
    scanf(" %[^\n]", t.nome);

    // Validação do telefone
    do {
        printf("Digite o telefone (formato: 0XX-XXXXXXXX): ");
        scanf(" %[^\n]", t.telefone);
        if (!validarTelefone(t.telefone)) {
            printf("Telefone inválido! Certifique-se de seguir o formato 0XX-XXXXXXXX.\n");
        }
    } while (!validarTelefone(t.telefone));

    // Validação do cargo
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

int validarTarifa(float tarifa) {
    return tarifa >= 0; // Tarifa não pode ser negativa
}

int verificarCargoTripulacao(int codigo, const char *cargoEsperado) {
    FILE *file = fopen("tripulacao.bin", "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo de tripulação.\n");
        return 0;
    }

    Tripulacao t;
    while (fread(&t, sizeof(Tripulacao), 1, file)) {
        if (t.codigo == codigo && strcmp(t.cargo, cargoEsperado) == 0) {
            fclose(file);
            return 1; // Código encontrado com o cargo correto
        }
    }

    fclose(file);
    return 0; // Código não encontrado ou cargo incorreto
}
void cadastrarVoo() {
    FILE *file = fopen("voos.bin", "ab+");
    if (!file) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Voo v;
    int codigoUnico;

    do {
        FILE *verificaFile = fopen("voos.bin", "rb");
        if (!verificaFile) {
            printf("Erro ao abrir o arquivo de verificação.\n");
            fclose(file);
            return;
        }

        printf("Digite o código do voo: ");
        scanf("%d", &v.codigo);

        codigoUnico = 1;
        Voo temp;
        while (fread(&temp, sizeof(Voo), 1, verificaFile)) {
            if (temp.codigo == v.codigo) {
                printf("Código já existente! Por favor, insira outro código.\n");
                codigoUnico = 0;
                break;
            }
        }
        fclose(verificaFile);
    } while (!codigoUnico);

    // Validação de data e hora
    do {
        printf("Digite a data do voo (dd/mm/aaaa): ");
        scanf(" %[^\n]", v.data);
        printf("Digite a hora do voo (hh:mm): ");
        scanf(" %[^\n]", v.hora);
    } while (!validarDataHora(v.data, v.hora));

    printf("Digite a origem: ");
    scanf(" %[^\n]", v.origem);
    printf("Digite o destino: ");
    scanf(" %[^\n]", v.destino);
    printf("Digite o avião: ");
    scanf(" %[^\n]", v.aviao);

    // Verificar o código do piloto (não é necessário ser único)
    printf("Digite o código do piloto: ");
    scanf("%d", &v.piloto);
    if (!verificarCargoTripulacao(v.piloto, "Piloto")) {
        printf("Código inválido ou não pertence a um Piloto! O voo será registrado como INATIVO.\n");
        v.piloto = -1; // Marcando como inválido
    }

    // Verificar o código do copiloto
    printf("Digite o código do copiloto: ");
    scanf("%d", &v.copiloto);
    if (!verificarCargoTripulacao(v.copiloto, "Copiloto")) {
        printf("Código inválido ou não pertence a um Copiloto! O voo será registrado como INATIVO.\n");
        v.copiloto = -1; // Marcando como inválido
    }

    // Verificar e adicionar comissários
    for (int i = 0; i < MAX_COMISSARIOS; i++) {
        printf("Digite o código do comissário %d (0 para nenhum): ", i + 1);
        scanf("%d", &v.comissarios[i]);
        if (v.comissarios[i] != 0 && !verificarCargoTripulacao(v.comissarios[i], "Comissario")) {
            printf("Código de comissário inválido! Ignorado.\n");
            v.comissarios[i] = 0; // Zerar código inválido
        }
    }

    // Validação da tarifa
    do {
        printf("Digite a tarifa (formato: 0.00): ");
        scanf("%f", &v.tarifa);
        if (v.tarifa < 0) {
            printf("Tarifa inválida! Deve ser um valor positivo.\n");
        }
    } while (v.tarifa < 0);

    // Determinar status do voo
    // Se piloto ou copiloto for inválido, o status será "Inativo"
    if (v.piloto != -1 && v.copiloto != -1) {
        v.status = 1; // Ativo se ambos forem válidos
    } else {
        v.status = 0; // Inativo se algum for inválido
    }

    fwrite(&v, sizeof(Voo), 1, file);
    fclose(file);
    printf("Voo cadastrado com sucesso! Status: %s\n", v.status ? "Ativo" : "Inativo");
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
        int statusOriginal = v.status;

        // Verificar piloto
        if (!verificarCargoTripulacao(v.piloto, "Piloto")) {
            printf("Aviso: Piloto com código %d não encontrado ou inválido. Voo marcado como inativo.\n", v.piloto);
            v.status = 0;
        }

        // Verificar copiloto
        if (!verificarCargoTripulacao(v.copiloto, "Copiloto")) {
            printf("Aviso: Copiloto com código %d não encontrado ou inválido. Voo marcado como inativo.\n", v.copiloto);
            v.status = 0;
        }

        // Verificar comissários
        for (int i = 0; i < MAX_COMISSARIOS; i++) {
            if (v.comissarios[i] != 0 && !verificarCargoTripulacao(v.comissarios[i], "Comissario")) {
                printf("Aviso: Comissário com código %d não encontrado ou inválido. Código ignorado.\n", v.comissarios[i]);
                v.comissarios[i] = 0; // Zerar código inválido
            }
        }

        // Exibir voo
        printf("Código: %d\n", v.codigo);
        printf("Data: %s\n", v.data);
        printf("Hora: %s\n", v.hora);
        printf("Origem: %s\n", v.origem);
        printf("Destino: %s\n", v.destino);
        printf("Avião: %s\n", v.aviao);
        printf("Piloto: %d\n", v.piloto);
        printf("Copiloto: %d\n", v.copiloto);
        printf("Tarifa: R$ %.2f\n", v.tarifa);
        printf("Comissários: ");
        for (int i = 0; i < MAX_COMISSARIOS; i++) {
            if (v.comissarios[i] != 0) {
                printf("%d ", v.comissarios[i]);
            }
        }
        printf("\nStatus: %s\n\n", v.status ? "Ativo" : "Inativo");

        // Restaurar status original após exibição (caso a validação seja temporária)
        v.status = statusOriginal;
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

        opcao = lerOpcao();

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