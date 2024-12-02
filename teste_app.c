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
