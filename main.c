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
<<<<<<< HEAD
    int status;      
=======
    int status;     
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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
<<<<<<< HEAD
   
    if (telefone[0] == '0' && strlen(telefone) == 14) {
        
        for (int i = 0; i < 14; i++) {
            if (i == 0 || i == 1 || i == 2) {
                if (!isdigit(telefone[i])) {
                    return 0; 
                }
            } else {
                if (!isdigit(telefone[i]) && telefone[i] != '-') {
                    return 0; 
=======
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
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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

<<<<<<< HEAD
    
=======
    // Validação do telefone
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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

<<<<<<< HEAD
    
=======
    // Garantir código único para o tripulante
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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

<<<<<<< HEAD
    
    printf("Digite o nome: ");
    scanf(" %[^\n]", t.nome);

    
=======
    // Informações do tripulante
    printf("Digite o nome: ");
    scanf(" %[^\n]", t.nome);

    // Validação do telefone
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
    do {
        printf("Digite o telefone (formato: 0XX-XXXXXXXX): ");
        scanf(" %[^\n]", t.telefone);
        if (!validarTelefone(t.telefone)) {
            printf("Telefone inválido! Certifique-se de seguir o formato 0XX-XXXXXXXX.\n");
        }
    } while (!validarTelefone(t.telefone));

<<<<<<< HEAD
    
=======
    // Validação do cargo
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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
<<<<<<< HEAD
    return tarifa >= 0; 
=======
    return tarifa >= 0; // Tarifa não pode ser negativa
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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
<<<<<<< HEAD
            return 1; 
=======
            return 1; // Código encontrado com o cargo correto
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
        }
    }

    fclose(file);
<<<<<<< HEAD
    return 0; 
=======
    return 0; // Código não encontrado ou cargo incorreto
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
}
void cadastrarVoo() {
    FILE *file = fopen("voos.bin", "ab+");
    if (!file) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Voo v;
    int codigoUnico;
<<<<<<< HEAD

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

=======
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f

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

<<<<<<< HEAD
    
=======
    // Verificar o código do piloto (não é necessário ser único)
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
    printf("Digite o código do piloto: ");
    scanf("%d", &v.piloto);
    if (!verificarCargoTripulacao(v.piloto, "Piloto")) {
        printf("Código inválido ou não pertence a um Piloto! O voo será registrado como INATIVO.\n");
<<<<<<< HEAD
        v.piloto = -1; 
    }

    
=======
        v.piloto = -1; // Marcando como inválido
    }

    // Verificar o código do copiloto
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
    printf("Digite o código do copiloto: ");
    scanf("%d", &v.copiloto);
    if (!verificarCargoTripulacao(v.copiloto, "Copiloto")) {
        printf("Código inválido ou não pertence a um Copiloto! O voo será registrado como INATIVO.\n");
<<<<<<< HEAD
        v.copiloto = -1; 
    }

    
=======
        v.copiloto = -1; // Marcando como inválido
    }

    // Verificar e adicionar comissários
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
    for (int i = 0; i < MAX_COMISSARIOS; i++) {
        printf("Digite o código do comissário %d (0 para nenhum): ", i + 1);
        scanf("%d", &v.comissarios[i]);
        if (v.comissarios[i] != 0 && !verificarCargoTripulacao(v.comissarios[i], "Comissario")) {
            printf("Código de comissário inválido! Ignorado.\n");
<<<<<<< HEAD
            v.comissarios[i] = 0; 
        }
    }

    
    int tarifaValida = 0;
    while (!tarifaValida) {
        printf("Digite a tarifa (formato: 0.00): ");
        if (scanf("%f", &v.tarifa) != 1) {
            printf("Erro: A tarifa deve ser um número válido. Tente novamente.\n");
            while(getchar() != '\n'); 
        } else if (v.tarifa < 0) {
            printf("Tarifa inválida! Deve ser um valor positivo.\n");
        } else {
            tarifaValida = 1;
        }
    }

   
    if (v.piloto != -1 && v.copiloto != -1) {
        v.status = 1; 
    } else {
        v.status = 0; 
=======
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
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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
    novoAssento.status = 1; // Inicialmente livre

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

    // Abrir outros arquivos
    FILE *fileVoo = fopen("voos.bin", "rb");
    FILE *fileAssento = fopen("assentos.bin", "r+b");
    FILE *filePassageiro = fopen("passageiros.bin", "rb");

    if (!fileVoo || !fileAssento || !filePassageiro) {
        printf("Erro ao abrir os arquivos necessários.\n");
        if (fileVoo) fclose(fileVoo);
        if (fileAssento) fclose(fileAssento);
        if (filePassageiro) fclose(filePassageiro);
        fclose(fileReserva); // Fechar arquivo de reservas
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
    rewind(fileReserva); // Voltar ao início do arquivo de reservas
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

<<<<<<< HEAD
    // Verificar se o assento está disponível (status 1)
    Assento assento;
    int assentoLivre = 0;
    while (fread(&assento, sizeof(Assento), 1, fileAssento)) {
        if (assento.codigoVoo == codigoVoo && assento.numero == numeroAssento) {
            if (assento.status == 1) {  // Assento disponível
=======
    // Verificar se o assento está disponível (status 1) ou foi dado baixa (liberado)
    Assento assento;
    int assentoLivre = 0;
    rewind(fileAssento); // Voltar ao início do arquivo de assentos
    while (fread(&assento, sizeof(Assento), 1, fileAssento)) {
        if (assento.codigoVoo == codigoVoo && assento.numero == numeroAssento) {
            if (assento.status == 1) {  // Assento disponível ou liberado
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
                assentoLivre = 1;
                break;
            } else {
                // Se o assento foi dado baixa, ele será liberado e poderá ser reservado novamente
                printf("Assento %d foi liberado anteriormente e agora pode ser reservado.\n", numeroAssento);
                assento.status = 1;  // Liberando o assento para nova reserva
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
    assento.status = 0; // Ocupar assento
    fwrite(&assento, sizeof(Assento), 1, fileAssento);

    printf("Reserva realizada com sucesso!\n");

    fclose(fileVoo);
    fclose(fileAssento);
    fclose(fileReserva);
    fclose(filePassageiro);
}
<<<<<<< HEAD
=======

>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
void darBaixaReserva() {
    int codigoVoo, numeroAssento, codigoPassageiro;

    printf("Digite o código do voo: ");
    scanf("%d", &codigoVoo);
    printf("Digite o número do assento: ");
    scanf("%d", &numeroAssento);
    printf("Digite o código do passageiro: ");
    scanf("%d", &codigoPassageiro);

    // Abrir arquivos
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

    // Percorrer o arquivo de reservas para encontrar a reserva do passageiro
    while (fread(&r, sizeof(Reserva), 1, fileReservas)) {
        if (r.codigoVoo == codigoVoo && r.numeroAssento == numeroAssento && r.codigoPassageiro == codigoPassageiro) {
            reservaEncontrada = 1;
            break;  // Encontramos a reserva
        }
    }

    if (!reservaEncontrada) {
        printf("Reserva não encontrada para o passageiro %d no voo %d, assento %d.\n", codigoPassageiro, codigoVoo, numeroAssento);
        fclose(fileAssentos);
        fclose(fileReservas);
        return;
    }

    // Percorrer os assentos para encontrar o assento a ser liberado
    while (fread(&a, sizeof(Assento), 1, fileAssentos)) {
        if (a.codigoVoo == codigoVoo && a.numero == numeroAssento) {
            if (a.status == 0) {  // Se o assento estiver ocupado
                a.status = 1;  // Liberar o assento (disponível)
                fseek(fileAssentos, -sizeof(Assento), SEEK_CUR);  // Voltar à posição do assento
                fwrite(&a, sizeof(Assento), 1, fileAssentos);  // Escrever o novo status
                assentoEncontrado = 1;
                break;
            }
        }
    }

    if (assentoEncontrado) {
        // Apagar a reserva do assento
        fseek(fileReservas, -sizeof(Reserva), SEEK_CUR);
        // Atualizar o arquivo de reservas removendo a reserva
        printf("Assento %d no voo %d liberado com sucesso!\n", numeroAssento, codigoVoo);
    } else {
        printf("Assento não encontrado ou já está disponível.\n");
    }

    fclose(fileAssentos);
    fclose(fileReservas);
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

<<<<<<< HEAD
  
    while (fread(&r, sizeof(Reserva), 1, fileReservas)) {
        if (r.codigoPassageiro == codigoPassageiro) {
            
            rewind(fileVoos);  
            while (fread(&voo, sizeof(Voo), 1, fileVoos)) {
                if (r.codigoVoo == voo.codigo) {
                    
                    rewind(fileAssentos);  
                    while (fread(&a, sizeof(Assento), 1, fileAssentos)) {
                        if (a.codigoVoo == r.codigoVoo && a.numero == r.numeroAssento) {
                            
                            if (a.status == 1) {
                               
                                printf("Voo %d (Assento %d) - Reserva concluída (Assento disponível)\n", r.codigoVoo, r.numeroAssento);
                            } else {
                                
                                printf("Voo %d (Assento %d) - Reservado\n", r.codigoVoo, r.numeroAssento);
                            }
                            encontrado = 1;
                            break;  
                        }
                    }
                    break;  
=======
    // Lê todas as reservas do passageiro
    while (fread(&r, sizeof(Reserva), 1, fileReservas)) {
        if (r.codigoPassageiro == codigoPassageiro) {
            // Busca o voo associado à reserva
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
                            break;  // Saímos do loop ao encontrar o assento
                        }
                    }
                    break;  // Saímos do loop ao encontrar o voo
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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
    int codigoPassageiro;
    printf("Digite o código do passageiro: ");
    scanf("%d", &codigoPassageiro);

    FILE *file = fopen("passageiros.bin", "r+b");
    Passageiro p;
    while (fread(&p, sizeof(Passageiro), 1, file)) {
        if (p.codigo == codigoPassageiro) {
            printf("Passageiro encontrado: %s\n", p.nome);
            printf("Pontos atuais: %d\n", p.pontos);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("Passageiro não encontrado!\n");
}
void pesquisarTripulante() {
<<<<<<< HEAD
    int codigoTripulante;
    printf("Digite o código do tripulante: ");
    scanf("%d", &codigoTripulante);

    FILE *file = fopen("tripulantes.bin", "r+b");
    Tripulacao t;
    while (fread(&t, sizeof(Tripulacao), 1, file)) {
        if (t.codigo == codigoTripulante) {
            printf("Tripulante encontrado: %s\n", t.nome);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("Tripulante não encontrado!\n");
=======
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
>>>>>>> 750aba68e77fbdcdf2727d26b85e772dabeb299f
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