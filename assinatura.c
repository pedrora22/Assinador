#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

/*
    Feito por Pedro Rabelo
    Trabalho de LP1 do curso de SI da UNEB
    12/2024

*/

typedef struct
{
    char nome[50];
    char CPF[12];
    char email[40];
    char senha[40];

} Conta;

typedef struct 
{
    char CPF[12];
    char senha[40];
} Login;

Conta logado;

void cifra(char *senha, int chave, int modo) { //A cifra de cesar movimenta os caracteres na tabela ASCII de acordo com a chave

    int tam = strlen(senha);

    int chaveMod;

    if(chave == 0) {
        chaveMod = chave + 1;
    }
    else {
        chaveMod = chave;
    }

    for (int i = 0; i < tam; i++) {

        if(modo == 0) { //criptografar
            senha[i] = senha[i] + chaveMod;
        }
        else { //descriptografar
            senha[i] = senha[i] - chaveMod;
        }
    }
}

int verificaSenha(char senha[]) {
    //Definindo os grupos de caracteres
    char *maiusculas = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *minusculas = "abcdefghijklmnopqrstuvwxyz";
    char *digitos = "0123456789";
    char *especiais = "!#$%%&()*+,-./:;<=>?@[\\]^_`{|}~";

    //Flags para verificar se encontramos pelo menos um caractere de cada categoria
    int temMaiuscula = 0;
    int temMinuscula = 0;
    int temDigito = 0;
    int temEspecial = 0;

    //Verificando se a senha contém ao menos um caractere de cada categoria
    for (int i = 0; senha[i] != '\0'; i++) {
        if (strchr(maiusculas, senha[i]) != NULL) {
            temMaiuscula = 1;
        }
        if (strchr(minusculas, senha[i]) != NULL) {
            temMinuscula = 1;
        }
        if (strchr(digitos, senha[i]) != NULL) {
            temDigito = 1;
        }
        if (strchr(especiais, senha[i]) != NULL) {
            temEspecial = 1;
        }
    }

    //Verificando se a senha contém ao menos um caractere de cada categoria
    if (temMaiuscula && temMinuscula && temDigito && temEspecial) {
        return 1; //A senha contém pelo menos um caractere de cada categoria
    } else {
        if(temMaiuscula == 0) {
            printf("Falta pelo menos uma letra maiuscula!\n");
        }
        if(temMinuscula == 0) {
            printf("Falta pelo menos uma letra minuscula!\n");
        }
        if(temDigito == 0) {
            printf("Falta pelo menos um numero!\n");
        }
        if(temEspecial == 0) {
            printf("Falta pelo menos um digito especial!\n");
        }

        return 0; //A senha não contém pelo menos um caractere de cada categoria
    }
}

int verificaToken() {

    srand(time(NULL));

    int token = rand() % 10000; //numero randomico de 1000 a 9999

    int tokenInserido = 0;

    printf("digite o token: %d\n", token);
    scanf("%d", &tokenInserido);

    if(tokenInserido == token) {
        printf("acertou!\n");
        return 1;
    }
    else {
        printf("errou! Tente novamente.\n");
        return 0;
    }

}


int signin() {
    FILE *users = fopen("usuarios.txt", "r"); // Modo de leitura para arquivos de texto

    if (users == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;  // Retorna 0 se o arquivo não puder ser aberto
    }

    Login login;
    Conta conta;
    char linha[256];

    printf("Digite seu CPF (sem pontos e traços ex: 12345678910):\n");
    scanf("%s", login.CPF);

    printf("Digite sua senha:\n");
    scanf("%s", login.senha);

    // 0 - conta não existe (CPF não encontrado), 1 - conta existe, mas senha incorreta, 2 - tudo certo
    while (fscanf(users, "Nome: %s\n", conta.nome) == 1) { 
        fscanf(users, "CPF: %s\n", conta.CPF);     
        fscanf(users, "Email: %s\n", conta.email);   
        fscanf(users, "Senha: %s\n\n", conta.senha); 

        if (strcmp(conta.CPF, login.CPF) == 0) {  // Verifica se o CPF coincide
            int chave = conta.CPF[0] - '0';
            cifra(conta.senha, chave, 1);  // Cifra a senha 

            if (strcmp(conta.senha, login.senha) == 0) {  // Verifica se as senhas coincidem
                logado = conta; 
                fclose(users);
                return 2;  // Tudo certo, login bem-sucedido
            }
            fclose(users);
            return 1;  // Senha incorreta
        }
    }

    fclose(users);
    return 0;  // CPF não encontrado
}


void signup() {
    FILE *users = fopen("usuarios.txt", "a"); // Modo de append para arquivos de texto

    if (users == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Conta conta;

    printf("Digite seu nome:\n");
    scanf("%s", conta.nome);

    printf("Digite seu CPF (sem pontos e traços ex: 12345678910):\n");
    scanf("%s", conta.CPF);

    printf("Digite seu email:\n");
    scanf("%s", conta.email);

    while(1) {
        printf("Digite sua senha: (ela deve conter letras minúsculas, maiúsculas, dígitos e caracteres especiais):\n");
        scanf("%s", conta.senha);

        if(verificaSenha(conta.senha)) { //Verifica se a senha atende os requisitos
            int chave = conta.CPF[0] - '0'; //Transforma o primeiro char do cpf em int
            cifra(conta.senha, chave, 0); //Cifra de Cesar de acordo com a chave obtida

            // Salva os dados no arquivo de texto
            fprintf(users, "Nome: %s\nCPF: %s\nEmail: %s\nSenha: %s\n\n", conta.nome, conta.CPF, conta.email, conta.senha);
            break;
        }
    }

    fclose(users);
}

void assinarDoc() {

    if (verificaToken() == 0) {
        printf("Token incorreto. Acesso Negado.\n");
        return;
    }

    char nomeDoc[50];
    int tipoDoc = 0;
    char nomeOriginal[50];
    
    printf("por favor, coloque o arquivo que gostaria de assinar na pasta arquivos.\n");

    printf("Digite o nome do arquivo: (sem a extensao, ex: inves de doc.txt, deve escrever apenas doc.\n");
    scanf("%s", nomeDoc);
    printf("digite 1 caso o arquivo é um txt, digite 2 caso o arquivo seja um bin.\n");
    scanf("%d", &tipoDoc);

    strcpy(nomeOriginal, nomeDoc);

    time_t t = time(NULL); //Inicialização do tempo
    struct tm *data_atual = localtime(&t);

    switch (tipoDoc)
    {
    case 1:
        char ch;

        // Adiciona "\arquivos\" antes do nome do arquivo original
        char nomeDocCompleto[256] = "arquivos\\";
        strcat(nomeDocCompleto, nomeDoc);
        strcat(nomeDocCompleto, ".txt");

        FILE *fileOrigem = fopen(nomeDocCompleto, "r");

        if (fileOrigem == NULL) {
        printf("Erro ao abrir arquivo de origem\n");
        return;
        }

        // Adiciona "\arquivos\" antes do nome do arquivo de destino
        char nomeOriginalCompleto[256] = "arquivos\\";
        strcat(nomeOriginalCompleto, nomeOriginal);
        strcat(nomeOriginalCompleto, "_assinado.txt");

        FILE *fileDestino = fopen(nomeOriginalCompleto, "w");

        if (fileDestino == NULL) {
        printf("Erro ao criar arquivo de destino\n");
        fclose(fileOrigem);
        return;
        }

        while((ch = fgetc(fileOrigem)) != EOF) {
            fputc(ch, fileDestino); // Transfere os dados do arquivo origem para o arquivo destino
        }

        fclose(fileOrigem);
        fclose(fileDestino);

        FILE *docNovo = fopen(nomeOriginalCompleto, "a");

        char data_str[100];
        strftime(data_str, sizeof(data_str), "%d/%m/%Y", data_atual);
        fprintf(docNovo, "\nAssinatura LP1, %s, %s, %s", logado.nome, logado.CPF, data_str);   
        fclose(docNovo);         
                    
        break;

    case 2:
        char chb;

        // Adiciona "\arquivos\" antes do nome do arquivo original
        char nomeDocCompletobin[256] = "arquivos\\";
        strcat(nomeDocCompletobin, nomeDoc);
        strcat(nomeDocCompletobin, ".bin");

        FILE *fileOrigembin = fopen(nomeDocCompletobin, "rb");

        if (fileOrigembin == NULL) {
        printf("Erro ao abrir arquivo de origem\n");
        return;
        }

        // Adiciona "\arquivos\" antes do nome do arquivo de destino
        char nomeOriginalCompletobin[256] = "arquivos\\";
        strcat(nomeOriginalCompletobin, nomeOriginal);
        strcat(nomeOriginalCompletobin, "_assinado.bin");

        FILE *fileDestinobin = fopen(nomeOriginalCompletobin, "wb");

        if (fileDestinobin == NULL) {
        printf("Erro ao criar arquivo de destino\n");
        fclose(fileOrigembin);
        return;
        }

        while((ch = fgetc(fileOrigembin)) != EOF) {
            fputc(ch, fileDestinobin); // Transfere os dados do arquivo origem para o arquivo destino
        }

        fclose(fileOrigembin);
        fclose(fileDestinobin);

        FILE *docNovobin = fopen(nomeOriginalCompletobin, "ab");

        char data_strbin[11];
        strftime(data_strbin, sizeof(data_strbin), "%d/%m/%Y", data_atual);
        fprintf(docNovobin, "\nAssinatura LP1, %s, %s, %s", logado.nome, logado.CPF, data_str);

        char assinaturaTexto[200];
        sprintf(assinaturaTexto, "Assinatura LP1, %s, %s, %s", //Forma uma string a ser adiciona ao arquivo bin
            logado.nome, 
            logado.CPF, 
            data_str);

        fwrite(assinaturaTexto, strlen(assinaturaTexto), 1, docNovobin);  
        fclose(docNovobin);         
                    
        break;
    
    default:
        break;
    }

}

int main() {

    int menuInicial = 1;
    int menuLogado = 0;

    while(1) {

        while(menuInicial) {
            int escolhaInicial = 0;

            printf("------------------------------------------------------------------\n");
            printf("                      Bem-vindo ao Assinaqui!                     \n");
            printf("------------------------------------------------------------------\n");
            printf("\n");
            printf("MENU INICIAL:\n");
            printf("|-------------------------|\n");
            printf("| 1 - Login               |\n");
            printf("| 2 - Criar conta         |\n");
            printf("| 3 - Sair                |\n");
            printf("|-------------------------|\n");
            scanf("%d", &escolhaInicial);

            switch (escolhaInicial)
            {
            case 1:
                    int estadoSignin = 3;

                    while(estadoSignin != 2) {
                        estadoSignin = signin();

                        if(estadoSignin == 0) {
                            printf("conta nao existe com esse CPF, tente novamente.\n");
                        }
                        else if(estadoSignin == 1) {
                            printf("senha incorreta, tente novamente.\n");
                        }
                        else if(estadoSignin == 2) {
                            printf("Logado! Redirecionando...\n");
                            menuLogado = 1;
                            menuInicial = 0;
                            break;
                        }
                        else {
                            printf("erro no signin.");
                        }
                    }
                break;

            case 2:
                signup();
                break;
            case 3:
                printf("saindo...\n");
                return 0;
            
            default:
                printf("input incorreto.");
                break;
            }
        }

        while(menuLogado) {

        int escolhaLogado = 0;

        printf("\n");
        printf("Logado como: %s\n", logado.nome);

        printf("MENU DE ASSINATURA:\n");
        printf("|-------------------------|\n");
        printf("| 1 - Assinar             |\n");
        printf("| 2 - Deslogar            |\n");
        printf("| 3 - Sair                |\n");
        printf("|-------------------------|\n");
        scanf("%d", &escolhaLogado);

        switch (escolhaLogado)
        {
        case 1:
                assinarDoc();
            break;

        case 2:
            menuInicial = 1;
            menuLogado = 0;
            break;

        case 3:
            printf("Saindo...\n");
            return 0;
        
        default:
            printf("input incorreto.\n");
            break;
        }
        }
    }

    return 0;
}