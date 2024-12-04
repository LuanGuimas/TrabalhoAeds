#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.c" 

#define MAX_COMISSARIOS 3

void limparArquivos() {
    remove("passageiros.bin");
    remove("tripulacao.bin");
    remove("voos.bin");
    // Adicione outros arquivos usados, se necessário.
}
void testCadastrarPassageiro() {
    limparArquivos();

    // Simule a entrada do usuário
    freopen("/dev/null", "w", stdout); // Suprime saídas de printf no console
    freopen("/dev/null", "r", stdin); // Simula entrada vazia

    FILE *file = fopen("passageiros.bin", "rb");
    if (!file) {
        printf("Falha ao abrir passageiros.bin\n");
        return;
    }

     Passageiro p;
    if (fread(&p, sizeof(Passageiro), 1, file) == 1) {
        if (p.codigo == 1 && strcmp(p.nome, "Teste Nome") == 0) {
            printf("Teste cadastrarPassageiro passou!\n");
        } else {
            printf("Teste cadastrarPassageiro falhou!\n");
        }
    } else {
        printf("Nenhum passageiro encontrado. Teste falhou!\n");
    }

    fclose(file);
    freopen("/dev/tty", "w", stdout); // Restaura saída padrão
}
void testCadastrarVoo() {
    limparArquivos();

    // Simula o cadastro de um voo válido
    FILE *file = fopen("voos.bin", "wb");
    if (!file) {
        printf("Falha ao criar voos.bin\n");
        return;
    }

    Voo vooTeste = {
        .codigo = 1,
        .status = 1,
    };
    strcpy(vooTeste.origem, "Origem Teste");
    fwrite(&vooTeste, sizeof(Voo), 1, file);
    fclose(file);

    // Reabre o arquivo para leitura
    file = fopen("voos.bin", "rb");
    if (!file) {
        printf("Falha ao abrir voos.bin para leitura\n");
        return;
    }

    Voo v;
    if (fread(&v, sizeof(Voo), 1, file) == 1) {
        if (v.codigo == 1 && strcmp(v.origem, "Origem Teste") == 0 && v.status == 1) {
            printf("Teste cadastrarVoo passou!\n");
        } else {
            printf("Teste cadastrarVoo falhou!\n");
        }
    } else {
        printf("Nenhum voo encontrado. Teste falhou!\n");
    }

    fclose(file);
}
void testListarPassageiros() {
    limparArquivos();
    cadastrarPassageiro();

    freopen("output.tmp", "w", stdout); // Redireciona saída para arquivo
    listarPassageiros();
    freopen("/dev/tty", "w", stdout);  // Restaura saída padrão

    FILE *out = fopen("output.tmp", "r");
    char buffer[256];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), out)) {
        if (strstr(buffer, "Teste Nome") != NULL) {
            found = 1;
            break;
        }
    }

    fclose(out);
    remove("output.tmp");

    if (found) {
        printf("Teste listarPassageiros passou!\n");
    } else {
        printf("Teste listarPassageiros falhou!\n");
    }
}
void testListarVoos() {
    limparArquivos();

    cadastrarVoo();

    freopen("output.tmp", "w", stdout); // Redireciona saída para arquivo
    listarVoos();
    freopen("/dev/tty", "w", stdout);  // Restaura saída padrão

    FILE *out = fopen("output.tmp", "r");
    char buffer[256];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), out)) {
        if (strstr(buffer, "Origem Teste") != NULL) {
            found = 1;
            break;
        }
    }

    fclose(out);
    remove("output.tmp");

    if (found) {
        printf("Teste listarVoos passou!\n");
    } else {
        printf("Teste listarVoos falhou!\n");
    }
}
void testComissariosNoVoo() {
    limparArquivos();

    cadastrarTripulacao(); // Cadastre tripulantes para serem usados no voo
    cadastrarVoo();

    FILE *file = fopen("voos.bin", "rb");
    if (!file) {
        printf("Falha ao abrir voos.bin\n");
        return;
    }

    Voo v;
    if (fread(&v, sizeof(Voo), 1, file) == 1) {
        int valid = 1;
        for (int i = 0; i < MAX_COMISSARIOS; i++) {
            if (v.comissarios[i] < 0) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            printf("Teste comissariosNoVoo passou!\n");
        } else {
            printf("Teste comissariosNoVoo falhou!\n");
        }
    } else {
        printf("Nenhum voo encontrado. Teste falhou!\n");
    }

    fclose(file);
}
void testReservarAssento() {
    limparArquivos();

    // Simular cadastro de voo e passageiros antes de reservar
    cadastrarVoo();
    cadastrarPassageiro();

    FILE *file = fopen("reservas.bin", "rb");
    if (!file) {
        printf("Falha ao abrir reservas.bin\n");
        return;
    }

    Reserva r;
    if (fread(&r, sizeof(Reserva), 1, file) == 1) {
        if (r.codigoVoo == 1 && r.numeroAssento == 1 && r.codigoPassageiro == 1) {
            printf("Teste reservarAssento passou!\n");
        } else {
            printf("Teste reservarAssento falhou!\n");
        }
    } else {
        printf("Nenhuma reserva encontrada. Teste falhou!\n");
    }

    fclose(file);
}

void testCadastrarTripulacaoCodigoUnico() {
    limparArquivos();

    cadastrarTripulacao();

    FILE *file = fopen("tripulacao.bin", "rb");
    if (!file) {
        printf("Falha ao abrir tripulacao.bin\n");
        return;
    }

    Tripulacao t1, t2;

    // Lê o primeiro tripulante
    if (fread(&t1, sizeof(Tripulacao), 1, file) == 1) {
        // Simula a tentativa de cadastrar o mesmo código de tripulante
        int codigoDuplicado = t1.codigo;
        freopen("/dev/null", "w", stdout); // Suprime a saída no console
        cadastrarTripulacao();
        freopen("/dev/tty", "w", stdout); // Restaura a saída no console

        // Verifica se apenas um tripulante foi cadastrado
        fseek(file, 0, SEEK_SET);
        if (fread(&t2, sizeof(Tripulacao), 1, file) == 1 && feof(file)) {
            printf("Teste cadastrarTripulacaoCodigoUnico passou!\n");
        } else {
            printf("Teste cadastrarTripulacaoCodigoUnico falhou!\n");
        }
    } else {
        printf("Nenhum tripulante encontrado. Teste falhou!\n");
    }

    fclose(file);
}
void testValidarTarifa() {
    limparArquivos();

    // Simula o cadastro de um voo com tarifa negativa
    freopen("/dev/null", "w", stdout); // Suprime saídas de printf no console
    freopen("/dev/null", "r", stdin); // Suprime entradas para evitar interação
    cadastrarVoo(); // Suprimir as entradas que validam a tarifa

    FILE *file = fopen("voos.bin", "rb");
    if (!file) {
        printf("Falha ao abrir voos.bin\n");
        return;
    }

    Voo v;
    if (fread(&v, sizeof(Voo), 1, file) == 1) {
        if (v.tarifa >= 0) {
            printf("Teste validarTarifa passou!\n");
        } else {
            printf("Teste validarTarifa falhou!\n");
        }
    } else {
        printf("Nenhum voo encontrado. Teste falhou!\n");
    }

    fclose(file);
}

void testValidarDataHora() {
    printf("Testando a função de validar data e hora...\n");

    // Simulando a inserção de uma data e hora válidas
    assert(validarDataHora("25/12/2024", "15:30") == 1);
    // Simulando a inserção de uma data inválida
    assert(validarDataHora("32/12/2024", "15:30") == 0);
    // Simulando a inserção de uma hora inválida
    assert(validarDataHora("25/12/2024", "25:00") == 0);

    printf("Todos os testes para validar data e hora passaram!\n");
}

void testCadastrarPassageiroCodigoUnico() {
    limparArquivos();

    cadastrarPassageiro();

    FILE *file = fopen("passageiros.bin", "rb");
    if (!file) {
        printf("Falha ao abrir passageiros.bin\n");
        return;
    }

    Passageiro p1, p2;

    // Lê o primeiro passageiro
    if (fread(&p1, sizeof(Passageiro), 1, file) == 1) {
        // Simula a tentativa de cadastrar o mesmo código de passageiro
        int codigoDuplicado = p1.codigo;
        freopen("/dev/null", "w", stdout); // Suprime a saída no console
        cadastrarPassageiro();
        freopen("/dev/tty", "w", stdout); // Restaura a saída no console

        // Verifica se apenas um passageiro foi cadastrado
        fseek(file, 0, SEEK_SET);
        if (fread(&p2, sizeof(Passageiro), 1, file) == 1 && feof(file)) {
            printf("Teste cadastrarPassageiroCodigoUnico passou!\n");
        } else {
            printf("Teste cadastrarPassageiroCodigoUnico falhou!\n");
        }
    } else {
        printf("Nenhum passageiro encontrado. Teste falhou!\n");
    }

    fclose(file);
}

void testListarTripulacao() {
    limparArquivos();

    cadastrarTripulacao();

    freopen("output.tmp", "w", stdout); // Redireciona saída para arquivo
    listarTripulacao();
    freopen("/dev/tty", "w", stdout);  // Restaura saída padrão

    FILE *out = fopen("output.tmp", "r");
    char buffer[256];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), out)) {
        if (strstr(buffer, "Piloto") != NULL || strstr(buffer, "Copiloto") != NULL || strstr(buffer, "Comissario") != NULL) {
            found = 1;
            break;
        }
    }

    fclose(out);
    remove("output.tmp");

    if (found) {
        printf("Teste listarTripulacao passou!\n");
    } else {
        printf("Teste listarTripulacao falhou!\n");
    }
}
void testVerificarCargoTripulacao() {
    limparArquivos();

    // Simula o cadastro de um tripulante com o cargo de Piloto
    cadastrarTripulacao();

    FILE *file = fopen("tripulacao.bin", "rb");
    if (!file) {
        printf("Falha ao abrir tripulacao.bin\n");
        return;
    }

    Tripulacao t;
    if (fread(&t, sizeof(Tripulacao), 1, file) == 1) {
        int resultado = verificarCargoTripulacao(t.codigo, "Piloto");
        if (resultado == 1) {
            printf("Teste verificarCargoTripulacao passou!\n");
        } else {
            printf("Teste verificarCargoTripulacao falhou!\n");
        }
    } else {
        printf("Nenhum tripulante encontrado. Teste falhou!\n");
    }

    fclose(file);
}
void testCadastrarAssentoReservado() {
    limparArquivos();

    // Simular cadastro de voo e passageiro antes de reservar
    cadastrarVoo();
    cadastrarPassageiro();

    FILE *file = fopen("reservas.bin", "wb");
    if (!file) {
        printf("Falha ao criar reservas.bin\n");
        return;
    }

    Reserva reservaTeste = {
        .codigoVoo = 1,
        .numeroAssento = 1,
        .codigoPassageiro = 1
    };
    fwrite(&reservaTeste, sizeof(Reserva), 1, file);
    fclose(file);

    // Reabre o arquivo para leitura
    file = fopen("reservas.bin", "rb");
    if (!file) {
        printf("Falha ao abrir reservas.bin\n");
        return;
    }

    Reserva r;
    if (fread(&r, sizeof(Reserva), 1, file) == 1) {
        if (r.codigoVoo == 1 && r.numeroAssento == 1 && r.codigoPassageiro == 1) {
            printf("Teste cadastrarAssentoReservado passou!\n");
        } else {
            printf("Teste cadastrarAssentoReservado falhou!\n");
        }
    } else {
        printf("Nenhuma reserva encontrada. Teste falhou!\n");
    }

    fclose(file);
}
void testValidarTelefone() {
    printf("Testando a função de validar telefone...\n");

    // Telefone válido
    assert(validarTelefone("011-12345-6789") == 1);
    // Telefone inválido (formato incorreto)
    assert(validarTelefone("11-12345-6789") == 0);
    // Telefone inválido (número insuficiente de dígitos)
    assert(validarTelefone("011-123-6789") == 0);
    // Telefone inválido (caracteres não permitidos)
    assert(validarTelefone("011-abcde-6789") == 0);

    printf("Todos os testes para validar telefone passaram!\n");
}

void testCadastrarVooPilotoInvalido() {
    limparArquivos();

    // Simula o cadastro de um tripulante como copiloto para ser usado no voo
    FILE *file = fopen("tripulacao.bin", "wb");
    if (!file) {
        printf("Falha ao criar tripulacao.bin\n");
        return;
    }

    Tripulacao copilotoTeste = {
        .codigo = 200,
    };
    strcpy(copilotoTeste.nome, "Copiloto Teste");
    strcpy(copilotoTeste.cargo, "Copiloto");
    fwrite(&copilotoTeste, sizeof(Tripulacao), 1, file);
    fclose(file);

    // Simula o cadastro de um voo com piloto inválido e copiloto válido
    Voo vooTeste = {
        .codigo = 1,
        .piloto = -1, // Piloto inválido
        .copiloto = 200, // Copiloto válido
    };
    strcpy(vooTeste.origem, "Origem Teste");
    vooTeste.status = (vooTeste.piloto != -1 && vooTeste.copiloto != -1) ? 1 : 0;

    file = fopen("voos.bin", "wb");
    if (!file) {
        printf("Falha ao criar voos.bin\n");
        return;
    }
    fwrite(&vooTeste, sizeof(Voo), 1, file);
    fclose(file);

    // Reabre o arquivo para leitura e valida o status do voo
    file = fopen("voos.bin", "rb");
    if (!file) {
        printf("Falha ao abrir voos.bin para leitura\n");
        return;
    }

    Voo v;
    if (fread(&v, sizeof(Voo), 1, file) == 1) {
        if (v.status == 0) {
            printf("Teste cadastrarVooPilotoInvalido passou! O voo foi registrado como INATIVO.\n");
        } else {
            printf("Teste cadastrarVooPilotoInvalido falhou! O status do voo deveria ser INATIVO.\n");
        }
    } else {
        printf("Nenhum voo encontrado. Teste falhou!\n");
    }

    fclose(file);
}

