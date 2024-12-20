#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>

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
    if (telefone[0] == '0' && strlen(telefone) == 14) {
        for (int i = 0; i < 14; i++) {
            if (i == 0 || i == 1 || i == 2) {
                if (!isdigit(telefone[i])) {
                    return 0;
                }
            } else {
                if (!isdigit(telefone[i]) && telefone[i] != '-') {
                    return 0;
                }
            }
        }
        return 1;
    }
    return 0;
}

int validarCodigo(char *codigoStr) {
    for (int i = 0; codigoStr[i] != '\0'; i++) {
        if (!isdigit(codigoStr[i])) {
            return 0; // Retorna 0 se algum caractere não for número
        }
    }
    return 1; // Retorna 1 se todos os caracteres forem números
}

void cadastrarPassageiro() {
    FILE *file = fopen("passageiros.bin", "ab+");
    Passageiro p;
    char codigoStr[10];  // String temporária para capturar a entrada

    printf("Digite o código do passageiro: ");
    while (1) {
        scanf("%s", codigoStr);
        if (validarCodigo(codigoStr)) {
            p.codigo = atoi(codigoStr); // Converte a string para número inteiro
            break; // Sai do loop se o código for válido
        } else {
            printf("Código inválido! Digite apenas números.\n");
        }
    }

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

    // Removendo a parte de fidelidade, pois agora o passageiro sempre ganhará pontos com a reserva
    p.fidelidade = 0; // Inicializa como 0, mas os pontos serão atualizados quando ele reservar um assento
    p.pontos = 0; // Inicializa com 0 pontos, sendo atualizado quando a reserva for feita

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
        printf("Código: %d, Nome: %s, Pontos de Fidelidade: %d\n",
               p.codigo, p.nome, p.pontos);
    }

    fclose(file);
}



void cadastrarTripulacao() {
    FILE *file = fopen("tripulacao.bin", "ab+");
    if (!file) {
        printf("vErro ao abrir arquivo de tripulação.\n");
        return;
    }

    Tripulacao t;
    int codigoUnico = 1;
    char codigoStr[10];  // String temporária para capturar a entrada

    do {
        FILE *verificaFile = fopen("tripulacao.bin", "rb");
        if (!verificaFile) {
            printf("Erro ao abrir o arquivo de verificação.\n");
            fclose(file);
            return;
        }

        printf("Digite o código da tripulação: ");
        while (1) {
            fgets(codigoStr, sizeof(codigoStr), stdin); // Usando fgets para capturar a entrada
            // Remover o '\n' que é adicionado pelo fgets
            codigoStr[strcspn(codigoStr, "\n")] = '\0';

            if (validarCodigo(codigoStr)) {
                t.codigo = atoi(codigoStr); // Converte a string para número inteiro
                break; // Sai do loop se o código for válido
            } else {
                printf("Código inválido! Digite apenas números.\n");
            }
        }

        Tripulacao temp;
        codigoUnico = 1; // Resetando a flag de código único
        while (fread(&temp, sizeof(Tripulacao), 1, verificaFile)) {
            if (temp.codigo == t.codigo) {
                printf("Código de tripulante já existente! Por favor, insira outro código.\n");
                codigoUnico = 0; // Código não é único, solicita novo código
                break;
            }
        }
        fclose(verificaFile);
    } while (!codigoUnico);

    // Coleta os dados do tripulante
    printf("Digite o nome: ");
    scanf(" %[^\n]", t.nome);

    do {
        printf("Digite o telefone (formato: 0XX-XXXXXXXX): ");
        scanf(" %[^\n]", t.telefone);
        if (!validarTelefone(t.telefone)) {
            printf("Telefone inválido! Certifique-se de seguir o formato 0XX-XXXXXXXX.\n");
        }
    } while (!validarTelefone(t.telefone));

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

int validarData(char* data) {
    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0; // Data inválida
    }

    // Validar mês
    if (mes < 1 || mes > 12) {
        return 0;
    }

    // Validar dias de cada mês
    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2) { // Verificar ano bissexto para fevereiro
        if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
            diasNoMes[1] = 29;
        }
    }

    // Verificar se o dia é válido para o mês
    if (dia < 1 || dia > diasNoMes[mes - 1]) {
        return 0;
    }

    return 1; // Data válida
}




int validarTarifa(float tarifa) {
    return tarifa >= 0;
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
            return 1;
        }
    }

    fclose(file);
    return 0;
}


// Função para validar a hora (hh:mm)
int validarHora(const char* hora) {
    int hora_int, minuto_int;
    // Verificar se a hora segue o formato hh:mm
    if (sscanf(hora, "%d:%d", &hora_int, &minuto_int) != 2) {
        return 0; // Formato inválido
    }

    // Verificar se a hora e o minuto estão no intervalo válido
    if (hora_int < 0 || hora_int > 23 || minuto_int < 0 || minuto_int > 59) {
        return 0; // Hora ou minuto fora do intervalo válido
    }

    return 1; // Hora válida
}

// Função para validar o formato e intervalo de uma data
int validarDatas(const char *data) {
    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0;  // Formato inválido
    }
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900) {
        return 0;  // Fora dos limites
    }
    // Validação de dias por mês (anos bissextos não considerados neste exemplo)
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;
    if (mes == 2 && dia > 29) return 0;  // Simplificado para aceitar 29 (bissexto seria tratado separadamente)
    return 1;  // Data válida
}

// Função para validar o formato e intervalo de uma hora
int validarHoras(const char *hora) {
    int hh, mm;
    if (sscanf(hora, "%d:%d", &hh, &mm) != 2) {
        return 0;  // Formato inválido
    }
    if (hh < 0 || hh > 23 || mm < 0 || mm > 59) {
        return 0;  // Fora dos limites
    }
    return 1;  // Hora válida
}

// Função para validar se a data e hora fornecidas estão no futuro
int validarDataHoraFuturas(const char *data, const char *hora) {
    struct tm tm_voo = {0};
    time_t agora = time(NULL);  // Obter o tempo atual
    struct tm *tempoAtual = localtime(&agora);

    // Converter a data e hora fornecidas para struct tm
    if (sscanf(data, "%d/%d/%d", &tm_voo.tm_mday, &tm_voo.tm_mon, &tm_voo.tm_year) != 3) {
        return 0;  // Formato inválido
    }
    if (sscanf(hora, "%d:%d", &tm_voo.tm_hour, &tm_voo.tm_min) != 2) {
        return 0;  // Formato inválido
    }

    // Ajustar valores para struct tm (mês começa em 0 e ano começa em 1900)
    tm_voo.tm_mon -= 1;
    tm_voo.tm_year -= 1900;

    // Converter struct tm do voo para time_t
    time_t tempo_voo = mktime(&tm_voo);

    // Comparar o tempo do voo com o tempo atual
    if (tempo_voo <= agora) {
        return 0;  // Data/hora já passou
    }

    return 1;  // Data/hora futura válida
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
    int dataHoraValida = 0;
    do {
        printf("Digite a data do voo (dd/mm/aaaa): ");
        scanf(" %[^\n]", v.data);
        printf("Digite a hora do voo (hh:mm): ");
        scanf(" %[^\n]", v.hora);

        if (!validarData(v.data)) {
            printf("Data inválida! Por favor, insira uma data válida.\n");
        } else if (!validarHora(v.hora)) {
            printf("Hora inválida! A hora deve estar no formato hh:mm e dentro do intervalo 00:00 a 23:59.\n");
        } else if (!validarDataHoraFuturas(v.data, v.hora)) {
            printf("Erro: Não é possível cadastrar voos em datas ou horários passados.\n");
        } else {
            dataHoraValida = 1;
        }
    } while (!dataHoraValida);

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

    // Tratamento de entrada para a tarifa (apenas float)
    int tarifaValida = 0;
    while (!tarifaValida) {
        printf("Digite a tarifa (formato: 0.00): ");
        if (scanf("%f", &v.tarifa) != 1) {
            printf("Erro: A tarifa deve ser um número válido. Tente novamente.\n");
            while(getchar() != '\n'); // Limpa o buffer de entrada
        } else if (v.tarifa < 0) {
            printf("Tarifa inválida! Deve ser um valor positivo.\n");
        } else {
            tarifaValida = 1;
        }
    }

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


        v.status = statusOriginal;
    }


    fclose(file);
}
void cadastrarAssento() {
    // Abrir arquivo de voos para leitura
    FILE *fileVoo = fopen("voos.bin", "rb");
    if (!fileVoo) {
        printf("Erro: O arquivo 'voos.bin' não foi encontrado.\n");
        return;
    }

    // Abrir ou criar arquivo de assentos para leitura e gravação
    FILE *fileAssento = fopen("assentos.bin", "rb+");
    if (!fileAssento) {
        // Arquivo não existe; criar novo
        fileAssento = fopen("assentos.bin", "wb+");
        if (!fileAssento) {
            printf("Erro ao criar o arquivo 'assentos.bin'.\n");
            fclose(fileVoo);
            return;
        }
    }

    int codigoVoo, numeroAssento;

    // Solicitar o código do voo
    printf("Digite o código do voo: ");
    scanf("%d", &codigoVoo);

    Voo voo;
    int vooExistente = 0;

    // Verificar se o voo existe e está ativo
    while (fread(&voo, sizeof(Voo), 1, fileVoo)) {
        if (voo.codigo == codigoVoo) {
            vooExistente = 1;
            break;
        }
    }

    if (!vooExistente) {
        printf("Voo não encontrado.\n");
        fclose(fileVoo);
        fclose(fileAssento);
        return;
    }

    if (voo.status == 0) { // 0 representa voo inativo
        printf("Não é possível cadastrar assentos em voos inativos.\n");
        fclose(fileVoo);
        fclose(fileAssento);
        return;
    }

    // Solicitar o número do assento
    printf("Digite o número do assento: ");
    scanf("%d", &numeroAssento);

    Assento assento;
    int assentoDuplicado = 0;

    // Verificar se o assento já existe no voo
    rewind(fileAssento); // Voltar ao início do arquivo de assentos
    while (fread(&assento, sizeof(Assento), 1, fileAssento)) {
        if (assento.codigoVoo == codigoVoo && assento.numero == numeroAssento) {
            assentoDuplicado = 1;
            break;
        }
    }

    if (assentoDuplicado) {
        printf("O assento já está cadastrado neste voo.\n");
        fclose(fileVoo);
        fclose(fileAssento);
        return;
    }

    // Cadastrar o novo assento
    Assento novoAssento;
    novoAssento.codigoVoo = codigoVoo;
    novoAssento.numero = numeroAssento;
    novoAssento.status = 1;

    fseek(fileAssento, 0, SEEK_END);
    fwrite(&novoAssento, sizeof(Assento), 1, fileAssento);

    printf("Assento cadastrado com sucesso!\n");

    fclose(fileVoo);
    fclose(fileAssento);
}
void cadastrarReserva() {
    // Verificar e criar arquivo de reservas caso não exista
    FILE *fileReserva = fopen("reservas.bin", "rb+");
    if (!fileReserva) {
        fileReserva = fopen("reservas.bin", "wb+");
        if (!fileReserva) {
            printf("Erro ao criar o arquivo 'reservas.bin'.\n");
            return;
        }
    }

    FILE *fileVoo = fopen("voos.bin", "rb");
    FILE *fileAssento = fopen("assentos.bin", "r+b");
    FILE *filePassageiro = fopen("passageiros.bin", "r+b");  // Agora com permissão de leitura e escrita

    if (!fileVoo || !fileAssento || !filePassageiro) {
        printf("Erro ao abrir os arquivos necessários.\n");
        if (fileVoo) fclose(fileVoo);
        if (fileAssento) fclose(fileAssento);
        if (filePassageiro) fclose(filePassageiro);
        fclose(fileReserva);
        return;
    }

    int codigoVoo, numeroAssento, codigoPassageiro;

    // Solicitar código do voo
    printf("Digite o código do voo: ");
    scanf("%d", &codigoVoo);

    Voo voo;
    int vooAtivo = 0;

    // Verificar se o voo está ativo
    while (fread(&voo, sizeof(Voo), 1, fileVoo)) {
        if (voo.codigo == codigoVoo && voo.status == 1) {
            vooAtivo = 1;
            break;
        }
    }

    if (!vooAtivo) {
        printf("Voo não encontrado ou não está ativo.\n");
        fclose(fileVoo);
        fclose(fileAssento);
        fclose(fileReserva);
        fclose(filePassageiro);
        return;
    }

    // Solicitar número do assento
    printf("Digite o número do assento: ");
    scanf("%d", &numeroAssento);

    Reserva reserva;
    int assentoReservado = 0;

    // Verificar se o assento já está reservado
    rewind(fileReserva);
    while (fread(&reserva, sizeof(Reserva), 1, fileReserva)) {
        if (reserva.codigoVoo == codigoVoo && reserva.numeroAssento == numeroAssento) {
            assentoReservado = 1;
            break;
        }
    }

    if (assentoReservado) {
        printf("Assento já reservado.\n");
        fclose(fileVoo);
        fclose(fileAssento);
        fclose(fileReserva);
        fclose(filePassageiro);
        return;
    }

    // Solicitar código do passageiro
    printf("Digite o código do passageiro: ");
    scanf("%d", &codigoPassageiro);

    Passageiro p;
    int passageiroExistente = 0;

    // Verificar se o passageiro existe
    while (fread(&p, sizeof(Passageiro), 1, filePassageiro)) {
        if (p.codigo == codigoPassageiro) {
            passageiroExistente = 1;
            break;
        }
    }

    if (!passageiroExistente) {
        printf("Passageiro não encontrado.\n");
        fclose(fileVoo);
        fclose(fileAssento);
        fclose(fileReserva);
        fclose(filePassageiro);
        return;
    }

    // Verificar se o passageiro já possui uma reserva neste voo
    rewind(fileReserva);
    int reservaExistente = 0;
    while (fread(&reserva, sizeof(Reserva), 1, fileReserva)) {
        if (reserva.codigoVoo == codigoVoo && reserva.codigoPassageiro == codigoPassageiro) {
            reservaExistente = 1;
            break;
        }
    }

    if (reservaExistente) {
        printf("Passageiro já possui uma reserva neste voo.\n");
        fclose(fileVoo);
        fclose(fileAssento);
        fclose(fileReserva);
        fclose(filePassageiro);
        return;
    }

    // Verificar se o assento está disponível (status 1)
    Assento assento;
    int assentoLivre = 0;
    rewind(fileAssento);  // Voltar ao início do arquivo de assentos
    while (fread(&assento, sizeof(Assento), 1, fileAssento)) {
        if (assento.codigoVoo == codigoVoo && assento.numero == numeroAssento) {
            if (assento.status == 1) {  // Assento disponível
                assentoLivre = 1;
                break;
            }
        }
    }

    if (!assentoLivre) {
        printf("Assento não disponível ou não encontrado.\n");
        fclose(fileVoo);
        fclose(fileAssento);
        fclose(fileReserva);
        fclose(filePassageiro);
        return;
    }

    // Criar nova reserva
    Reserva r;
    r.codigoVoo = codigoVoo;
    r.numeroAssento = numeroAssento;
    r.codigoPassageiro = codigoPassageiro;

    fseek(fileReserva, 0, SEEK_END);
    fwrite(&r, sizeof(Reserva), 1, fileReserva);

    // Atualizar status do assento
    fseek(fileAssento, -sizeof(Assento), SEEK_CUR);
    assento.status = 0;  // Ocupar assento
    fwrite(&assento, sizeof(Assento), 1, fileAssento);

    printf("Reserva realizada com sucesso!\n");

    // * Atualizar pontos de fidelidade *
    rewind(filePassageiro);  // Voltar ao início do arquivo de passageiros
    while (fread(&p, sizeof(Passageiro), 1, filePassageiro)) {
        if (p.codigo == codigoPassageiro) {
            p.pontos += 10;  // Adicionar 10 pontos

            fseek(filePassageiro, -sizeof(Passageiro), SEEK_CUR);
            fwrite(&p, sizeof(Passageiro), 1, filePassageiro);
            printf("Pontos de fidelidade atualizados: %d\n", p.pontos);
            break;
        }
    }

    fclose(fileVoo);
    fclose(fileAssento);
    fclose(fileReserva);
    fclose(filePassageiro);
}
void reservarAssento() {
    int codigoVoo, numeroAssento, codigoPassageiro;

    printf("Digite o código do voo: ");
    scanf("%d", &codigoVoo);
    printf("Digite o número do assento: ");
    scanf("%d", &numeroAssento);
    printf("Digite o código do passageiro: ");
    scanf("%d", &codigoPassageiro);

    // Abrir os arquivos de assentos e reservas
    FILE *fileAssentos = fopen("assentos.bin", "r+b");
    FILE *fileReservas = fopen("reservas.bin", "r+b");
    Assento a;
    Reserva r;
    int assentoDisponivel = 0;

    if (!fileAssentos || !fileReservas) {
        printf("Erro ao abrir os arquivos necessários.\n");
        return;
    }

    // Verificar se o assento está disponível
    fseek(fileAssentos, 0, SEEK_SET);
    while (fread(&a, sizeof(Assento), 1, fileAssentos)) {
        if (a.codigoVoo == codigoVoo && a.numero == numeroAssento) {
            if (a.status == 0) {  // O assento está reservado
                printf("Assento %d no voo %d já está reservado.\n", numeroAssento, codigoVoo);
                fclose(fileAssentos);
                fclose(fileReservas);
                return;
            } else {
                assentoDisponivel = 1;
                break;
            }
        }
    }

    if (assentoDisponivel) {
        // Criar uma nova reserva para o assento
        fseek(fileReservas, 0, SEEK_END);
        r.codigoVoo = codigoVoo;
        r.numeroAssento = numeroAssento;
        r.codigoPassageiro = codigoPassageiro;

        fwrite(&r, sizeof(Reserva), 1, fileReservas);

        // Alterar o status do assento para reservado
        fseek(fileAssentos, -sizeof(Assento), SEEK_CUR);
        a.status = 0; // Marcar como reservado
        fwrite(&a, sizeof(Assento), 1, fileAssentos);

        printf("Assento %d no voo %d reservado com sucesso!\n", numeroAssento, codigoVoo);
    } else {
        printf("Assento %d no voo %d não está disponível para reserva.\n", numeroAssento, codigoVoo);
    }

    fclose(fileAssentos);
    fclose(fileReservas);
}

void darBaixaReserva() {
    int codigoVoo, numeroAssento, codigoPassageiro;

    printf("Digite o código do voo: ");
    scanf("%d", &codigoVoo);
    printf("Digite o número do assento: ");
    scanf("%d", &numeroAssento);
    printf("Digite o código do passageiro: ");
    scanf("%d", &codigoPassageiro);

    // Abrir os arquivos de assentos e reservas
    FILE *fileAssentos = fopen("assentos.bin", "r+b");
    FILE *fileReservas = fopen("reservas.bin", "r+b");
    Assento a;
    Reserva r;
    int assentoEncontrado = 0;
    int reservaEncontrada = 0;

    if (!fileAssentos || !fileReservas) {
        printf("Erro ao abrir os arquivos necessários.\n");
        return;
    }

    // Verificar se existe a reserva para o assento e passageiro
    while (fread(&r, sizeof(Reserva), 1, fileReservas)) {
        if (r.codigoVoo == codigoVoo && r.numeroAssento == numeroAssento && r.codigoPassageiro == codigoPassageiro) {
            reservaEncontrada = 1;
            break;  // Reserva encontrada
        }
    }

    if (!reservaEncontrada) {
        printf("Reserva não encontrada para o passageiro %d no voo %d, assento %d.\n", codigoPassageiro, codigoVoo, numeroAssento);
        fclose(fileAssentos);
        fclose(fileReservas);
        return;
    }

    // Buscar o assento no arquivo de assentos
    fseek(fileAssentos, 0, SEEK_SET);
    while (fread(&a, sizeof(Assento), 1, fileAssentos)) {
        if (a.codigoVoo == codigoVoo && a.numero == numeroAssento) {
            if (a.status == 0) {  // Assento já estava reservado
                // Liberar o assento (alterar status para disponível)
                a.status = 1;
                fseek(fileAssentos, -sizeof(Assento), SEEK_CUR);  // Voltar para o local correto do assento
                fwrite(&a, sizeof(Assento), 1, fileAssentos);  // Salvar a mudança no arquivo de assentos
                assentoEncontrado = 1;
                break;
            }
        }
    }

    if (assentoEncontrado) {
        // Criar um arquivo temporário para registrar as reservas restantes
        FILE *tempFile = fopen("temp_reservas.bin", "wb+");
        if (!tempFile) {
            printf("Erro ao criar arquivo temporário.\n");
            fclose(fileAssentos);
            fclose(fileReservas);
            return;
        }

        // Copiar as reservas que não são a do assento liberado
        rewind(fileReservas);
        while (fread(&r, sizeof(Reserva), 1, fileReservas)) {
            if (r.codigoVoo != codigoVoo || r.numeroAssento != numeroAssento || r.codigoPassageiro != codigoPassageiro) {
                fwrite(&r, sizeof(Reserva), 1, tempFile);
            }
        }

        fclose(fileReservas);
        fclose(tempFile);

        // Substituir o arquivo de reservas pelo temporário
        remove("reservas.bin");
        rename("temp_reservas.bin", "reservas.bin");

        printf("Reserva de assento %d no voo %d foi liberada com sucesso!\n", numeroAssento, codigoVoo);
    } else {
        printf("Assento não encontrado ou já está disponível.\n");
    }

    fclose(fileAssentos);
}



void listarVoosPassageiro() {
    int codigoPassageiro;
    printf("Digite o código do passageiro: ");
    scanf("%d", &codigoPassageiro);

    FILE *fileReservas = fopen("reservas.bin", "rb");
    Reserva r;
    int encontrado = 0;

    FILE *fileAssentos = fopen("assentos.bin", "rb");
    Assento a;

    FILE *fileVoos = fopen("voos.bin", "rb");
    Voo voo;

    if (!fileReservas || !fileAssentos || !fileVoos) {
        printf("Erro ao abrir os arquivos necessários.\n");
        return;
    }

    printf("\n--- Histórico de Voos do Passageiro %d ---\n", codigoPassageiro);

    // Lê todas as reservas do passageiro
    while (fread(&r, sizeof(Reserva), 1, fileReservas)) {
        if (r.codigoPassageiro == codigoPassageiro) {
            // Busca o voo
            rewind(fileVoos);  // Volta ao início do arquivo de voos
            while (fread(&voo, sizeof(Voo), 1, fileVoos)) {
                if (r.codigoVoo == voo.codigo) {
                    // Busca o assento associado à reserva
                    rewind(fileAssentos);  // Volta ao início do arquivo de assentos
                    while (fread(&a, sizeof(Assento), 1, fileAssentos)) {
                        if (a.codigoVoo == r.codigoVoo && a.numero == r.numeroAssento) {
                            // Mostrar todos os voos, mesmo com a reserva dada baixa
                            if (a.status == 1) {
                                // Assento disponível
                                printf("Voo %d (Assento %d) - Reserva concluída (Assento disponível)\n", r.codigoVoo, r.numeroAssento);
                            } else {
                                // Assento ocupado (reserva dada baixa)
                                printf("Voo %d (Assento %d) - Reservado\n", r.codigoVoo, r.numeroAssento);
                            }
                            encontrado = 1;
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    fclose(fileReservas);
    fclose(fileAssentos);
    fclose(fileVoos);

    if (!encontrado) {
        printf("Nenhum voo encontrado para este passageiro.\n");
    }
}

void pesquisarPassageiro() {
    int opcao;
    printf("Deseja pesquisar por:\n1. Código do passageiro\n2. Nome do passageiro\nEscolha: ");
    scanf("%d", &opcao);

    FILE *file = fopen("passageiros.bin", "rb");  // Abrindo o arquivo com os passageiros
    if (!file) {
        printf("Erro ao abrir o arquivo de passageiros.\n");
        return;
    }

    Passageiro p;
    int encontrado = 0;

    if (opcao == 1) {
        int codigoBusca;
        printf("Digite o código do passageiro: ");
        scanf("%d", &codigoBusca);

        while (fread(&p, sizeof(Passageiro), 1, file)) {
            if (p.codigo == codigoBusca) {
                printf("Passageiro encontrado:\n");
                printf("Código: %d\nNome: %s\nEndereço: %s\nTelefone: %s\nFidelidade: %d\nPontos: %d\n",
                        p.codigo, p.nome, p.endereco, p.telefone, p.fidelidade, p.pontos);
                encontrado = 1;
                break;
            }
        }

    } else if (opcao == 2) {
        char nomeBusca[50];
        printf("Digite o nome do passageiro: ");
        getchar(); // Para limpar o buffer do stdin
        fgets(nomeBusca, sizeof(nomeBusca), stdin);
        nomeBusca[strcspn(nomeBusca, "\n")] = 0;  // Remover a nova linha gerada pelo fgets

        while (fread(&p, sizeof(Passageiro), 1, file)) {
            if (strstr(p.nome, nomeBusca) != NULL) {  // Pesquisa pelo nome
                printf("Passageiro encontrado:\n");
                printf("Código: %d\nNome: %s\nEndereço: %s\nTelefone: %s\nFidelidade: %d\nPontos: %d\n",
                        p.codigo, p.nome, p.endereco, p.telefone, p.fidelidade, p.pontos);
                encontrado = 1;
            }
        }
    } else {
        printf("Opção inválida.\n");
        fclose(file);
        return;
    }

    if (!encontrado) {
        printf("Passageiro não encontrado.\n");
    }

    fclose(file);
}

void pesquisarTripulante() {
    int opcao;
    printf("Deseja pesquisar por:\n1. Código do tripulante\n2. Nome do tripulante\nEscolha: ");
    scanf("%d", &opcao);

    FILE *file = fopen("tripulacao.bin", "rb");  // Abrindo o arquivo com os tripulantes
    if (!file) {
        printf("Erro ao abrir o arquivo de tripulação.\n");
        return;
    }

    Tripulacao t;
    int encontrado = 0;

    if (opcao == 1) {  // Pesquisa por código
        int codigoTripulante;
        printf("Digite o código do tripulante: ");
        scanf("%d", &codigoTripulante);

        while (fread(&t, sizeof(Tripulacao), 1, file)) {
            if (t.codigo == codigoTripulante) {
                printf("Tripulante encontrado: %s\nCargo: %s\n", t.nome, t.cargo);
                encontrado = 1;
                break;
            }
        }
    } else if (opcao == 2) {  // Pesquisa por nome
        char nomeTripulante[100];
        printf("Digite o nome do tripulante: ");
        scanf(" %[^\n]", nomeTripulante);  // Lê o nome com espaços

        while (fread(&t, sizeof(Tripulacao), 1, file)) {
            if (strstr(t.nome, nomeTripulante) != NULL) {  // Verifica se o nome contém o texto informado
                printf("Tripulante encontrado: %s\nCargo: %s\n", t.nome, t.cargo);
                encontrado = 1;
            }
        }
    } else {
        printf("Opção inválida!\n");
    }

    if (!encontrado) {
        printf("Tripulante não encontrado!\n");
    }

    fclose(file);
}

void programaFidelidade() {
    int escolha;

    printf("Escolha uma opção:\n");
    printf("1 - Pesquisar Passageiro\n");
    printf("2 - Pesquisar Tripulante\n");
    printf("Digite sua escolha: ");
    scanf("%d", &escolha);

    if (escolha == 1) {
        pesquisarPassageiro();
    } else if (escolha == 2) {
        pesquisarTripulante();
    } else {
        printf("Opção inválida!\n");
    }
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
        printf("7. Cadastrar Assento\n");
        printf("8. Reservar Assento\n");
        printf("9. Dar baixa em reserva\n");
        printf("10. Listar Voos de um Passageiro\n");
        printf("11. Buscar Passageiro\n");
        printf("12. Buscar Tripulante\n");
        printf("13. Programa de Fidelidade\n");
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
            case 7:
                cadastrarAssento();
                break;
            case 8:
                cadastrarReserva();
                break;
            case 9:
                darBaixaReserva();
                break;
            case 10:
                listarVoosPassageiro();
                break;
            case 11:
                pesquisarPassageiro();
                break;
            case 12:
                pesquisarTripulante();
                break;
            case 13:
                programaFidelidade();
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