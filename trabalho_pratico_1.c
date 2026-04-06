/*
----- TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS II + ALGORITMOS -------
NOME: Caue Sordi Paulino
NUSP: 14564520
NOME: Santhiago Aguiar Afonso da Rosa 
NUSP: 14607274



*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
//struct de 17 bytes fixos
typedef struct {
        //byte status
        char status;
        // 4 bytes para o topo offset do primeiro registro removido
        int32_t topo;
        // 4 bytes proxRRN 
        int32_t proxRRN;
        //4 BYTES numero_estacoes 
        int32_t nroEstacoes;
        //numero_pares_estacoes para quantidade de pares unicos entre as estacoes
        int32_t nroParesEstacao;
    } Cabecalho;

    typedef struct {
    char    removido;              /**< '0'=ativo, '1'=removido */
    int32_t proximo;               /**< RRN do próximo removido, ou -1 */
    int32_t tamNomeEstacao;        /**< tamanho da string nomeEstacao */
    int32_t codEstacao;            /**< código da estação (-1 = nulo) */
    int32_t codLinha;              /**< código da linha */
    int32_t codProxEstacao;        /**< código da próxima estação */
    int32_t distProxEstacao;       /**< distância para a próxima estação */
    int32_t codLinhaIntegra;       /**< código da linha de integração */
    int32_t codEstIntegra;         /**< código da estação de integração */
    int32_t tamNomeLinha;          /**< tamanho da string nomeLinha */
    char    nomeEstacao[43];       /**< nome da estação (com '\0' interno) */
    char    nomeLinha[43];         /**< nome da linha (com '\0' interno) */
} RegistroEstacao;





void ler_linha_csv(char *linha, RegistroEstacao *registro) 
{
    /* Inicializa tudo com nulos por padrao */
    memset(registro, 0, sizeof(RegistroEstacao));
    registro->removido          = '0';   /* ativo */
    registro->proximo           = -1;    // sem próximo removido
    registro->codEstacao        = -1;    //nulo por padrão
    registro->codLinha          = -1;
    registro->codProxEstacao    = -1;
    registro->distProxEstacao   = -1;
    registro->codLinhaIntegra   = -1;
    registro->codEstIntegra     = -1;
    registro->tamNomeEstacao    = 0;
    registro->tamNomeLinha      = 0;

    char copia[512]; // buffer temporário para manipulação da linha
    strncpy(copia, linha, sizeof(copia) - 1);  /* copia segura */
    copia[sizeof(copia) - 1] = '\0';

    /* Remove '\n' e '\r' do final */
    copia[strcspn(copia, "\n\r")] = '\0';

    char *token;

    // dfine o delimitador e verifica se o campo existe e não é vazio antes de atribuir
    token = strtok(copia, ",");
    if (token != NULL && strlen(token) > 0 && strcmp(token, " ") != 0) {
        registro->codEstacao = atoi(token);
    }
    /* se vazio, mantém -1 (nulo) */

    // dfine o delimitador e verifica se o campo existe e não é vazio antes de atribuir
    token = strtok(NULL, ",");
    if (token != NULL && strlen(token) > 0) {
        strncpy(registro->nomeEstacao, token, sizeof(registro->nomeEstacao) - 1);
        // define o último byte como '\0' para garantir a terminação da string
        registro->nomeEstacao[sizeof(registro->nomeEstacao) - 1] = '\0';
        registro->tamNomeEstacao = strlen(registro->nomeEstacao);
    }
    /* se vazio, tamNomeEstacao fica 0 e nomeEstacao fica "" */

    
    token = strtok(NULL, ",");
    if (token != NULL && strlen(token) > 0) {
        registro->codLinha = atoi(token);
    }

    
    token = strtok(NULL, ",");
    if (token != NULL && strlen(token) > 0) {
        strncpy(registro->nomeLinha, token, sizeof(registro->nomeLinha) - 1);
        registro->nomeLinha[sizeof(registro->nomeLinha) - 1] = '\0';
        registro->tamNomeLinha = strlen(registro->nomeLinha);
    }

    
    token = strtok(NULL, ",");
    if (token != NULL && strlen(token) > 0) {
        registro->codProxEstacao = atoi(token);
    }

    
    token = strtok(NULL, ",");
    if (token != NULL && strlen(token) > 0) {
        registro->distProxEstacao = atoi(token);
    }

    
    token = strtok(NULL, ",");
    if (token != NULL && strlen(token) > 0) {
        registro->codLinhaIntegra = atoi(token);
    }

    
    token = strtok(NULL, ",");
    if (token != NULL && strlen(token) > 0) {
        registro->codEstIntegra = atoi(token);
    }
}



/* -------------------------------------------------------
 Escreve o cabeçalho de 17 bytes no arquivo binário.
 
  @param fp   arquivo binário aberto para escrita
  @param cab  cabeçalho a gravar
  ------------------------------------------------------- */
void escrever_cabecalho(FILE *fp, Cabecalho cab) {
    char buffer[17];
    memset(buffer, '$', 17);

    /* [0] status */
    buffer[0] = cab.status;

    /* [1..4] topo */
    memcpy(buffer + 1,  &cab.topo,                  sizeof(int32_t));

    /* [5..8] proxRRN */
    memcpy(buffer + 5,  &cab.proxRRN,               sizeof(int32_t));

    /* [9..12] numero_estacoes */
    memcpy(buffer + 9,  &cab.nroEstacoes,        sizeof(int32_t));

    /* [13..16] numero_pares_estacoes */
    memcpy(buffer + 13, &cab.nroParesEstacao,  sizeof(int32_t));

    fwrite(buffer, 1, 17, fp);
}

void escrever_registro(FILE *fp, RegistroEstacao reg) {
    char buffer[80];
    memset(buffer, '$', 80);  /* preenche tudo com lixo */

    int pos = 0;

    /* [0] removido */
    buffer[pos] = reg.removido;
    pos += 1;                   /* pos = 1 */

    /* [1..4] proximo */
    memcpy(buffer + pos, &reg.proximo, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 5 */

    /* [5..8] tamNomeEstacao */
    memcpy(buffer + pos, &reg.tamNomeEstacao, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 9 */

    /* [9..12] codEstacao */
    memcpy(buffer + pos, &reg.codEstacao, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 13 */

    /* [13..16] codLinha */
    memcpy(buffer + pos, &reg.codLinha, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 17 */

    /* [17..20] codProxEstacao */
    memcpy(buffer + pos, &reg.codProxEstacao, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 21 */

    /* [21..24] distProxEstacao */
    memcpy(buffer + pos, &reg.distProxEstacao, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 25 */

    /* [25..28] codLinhaIntegra */
    memcpy(buffer + pos, &reg.codLinhaIntegra, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 29 */

    /* [29..32] codEstIntegra */
    memcpy(buffer + pos, &reg.codEstIntegra, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 33 */

    /* [33..36] tamNomeLinha */
    memcpy(buffer + pos, &reg.tamNomeLinha, sizeof(int32_t));
    pos += sizeof(int32_t);     /* pos = 37 */

    /* [37 .. 37+tamNomeEstacao-1] nomeEstacao (sem '\0') */
    if (reg.tamNomeEstacao > 0) {
        memcpy(buffer + pos, reg.nomeEstacao, reg.tamNomeEstacao);
        pos += reg.tamNomeEstacao;
    }

    /* [pos .. pos+tamNomeLinha-1] nomeLinha (sem '\0') */
    if (reg.tamNomeLinha > 0) {
        memcpy(buffer + pos, reg.nomeLinha, reg.tamNomeLinha);
        pos += reg.tamNomeLinha;
    }
// explicando o que agente fez aqui:
    /* O registro tem campos fixos (removido, proximo, tamNomeEstacao, 
    codEstacao, codLinha, codProxEstacao, distProxEstacao, codLinhaIntegra,
     codEstIntegra, tamNomeLinha) que ocupam os primeiros 37 bytes. 
     Depois disso, temos os campos variáveis nomeEstacao e nomeLinha, que podem ocupar até 43 bytes cada, 
     mas só usamos o que for necessário. O restante do espaço até 80 bytes é preenchido com '$' para garantir 
     que o registro tenha sempre o mesmo tamanho. A variável 'pos' é usada para controlar a posição atual 
     no buffer onde os dados estão sendo escritos. 
     Depois de escrever os campos fixos, 'pos' aponta para o início dos campos, usando ponteiros para os campos
      variáveis, e à medida que escrevemos nomeEstacao e nomeLinha, 'pos' é 
      atualizado para refletir a posição atual. No final, 'pos' pode ser menor que 80,
       mas isso não é um problema, pois o restante do buffer já está preenchido com '$'.
        /* pos nunca ultrapassa 80 pois o CSV garante isso */

    fwrite(buffer, 1, 80, fp); // escreve os 80 bytes do registro no arquivo binário
}

/* 
 
       1. chama binarioNaTela antes de abrir o arquivo
       2. abre o .bin para escrita
       3. grava cabeçalho com status inconsistente ('0')
       4. lê cada linha do CSV e grava o registro
        5. volta ao início e reescreve cabeçalho consistente ('1')
      6. fecha o arquivo
       7. chama binarioNaTela depois de fechar
 */


int ler_registro(FILE *fp, RegistroEstacao *reg)
{
    char buffer[80]; // tenta ler os 80 bytes do registro

    if (fread(buffer, 1, 80, fp) != 80)
    {
        return 0; //erro na leitura ou fim
        
    }
    int pos = 0; //onde buffer esta
    reg->removido = buffer[pos];
    pos += 1;  

    memcpy(&reg->proximo, buffer + pos, sizeof(int32_t)); // de [1..4]
    pos += sizeof(int32_t);// acrecimo de 4 bytes para o próximo campo (tamNomeEstacao)

    memcpy(&reg->tamNomeEstacao, buffer + pos, sizeof(int32_t)); // de [5..8]
    pos += sizeof(int32_t);

    memcpy(&reg->codEstacao, buffer + pos, sizeof(int32_t)); // de [9..12]
    pos += sizeof(int32_t);

    memcpy(&reg->codLinha, buffer + pos, sizeof(int32_t)); // de [13..16]
    pos += sizeof(int32_t);

    memcpy(&reg->codProxEstacao, buffer + pos, sizeof(int32_t)); // de [17..20]
    pos += sizeof(int32_t);

    memcpy(&reg->distProxEstacao, buffer + pos, sizeof(int32_t)); // de [21..24]
    pos += sizeof(int32_t);

    memcpy(&reg->codLinhaIntegra, buffer + pos, sizeof(int32_t)); // de [25..28]
    pos += sizeof(int32_t);

    memcpy(&reg->codEstIntegra, buffer + pos, sizeof(int32_t)); // de [29..32]
    pos += sizeof(int32_t);

    memcpy(&reg->tamNomeLinha, buffer + pos, sizeof(int32_t)); // de [33..36]
    pos += sizeof(int32_t);

    // Agora pos = 37, onde comeca nomeEstacao
    if (reg ->tamNomeEstacao > 0 && reg-> tamNomeEstacao < 43 ) //verificacao de buffer
    {
        memcpy( reg->nomeEstacao, buffer + pos, reg->tamNomeEstacao);
        reg->nomeEstacao[reg->tamNomeEstacao] = '\0'; // para terminar a string
        pos += reg->tamNomeEstacao; // acrecimo do tamanho do nomeEstacao

    } else {
        reg->nomeEstacao[0] = '\0'; // nome vazio
    }

    if (reg->tamNomeLinha > 0 && reg->tamNomeLinha < 43) {
        memcpy(reg->nomeLinha, buffer + pos, reg->tamNomeLinha);
        reg->nomeLinha[reg->tamNomeLinha] = '\0';
    } else {
        reg->nomeLinha[0] = '\0';
    }
    if (reg->tamNomeLinha > 0 && reg->tamNomeLinha < 43) {
        memcpy(reg->nomeLinha, buffer + pos, reg->tamNomeLinha);
        reg->nomeLinha[reg->tamNomeLinha] = '\0';
    } else {
        reg->nomeLinha[0] = '\0';
    }

    return 1; // leitura bem sucedida
}

void ler_cabecalho(FILE *fp, Cabecalho *cab) {

    /*cab->status = '1'; 
    cab->topo = -1;
    cab->proxRRN = 0;
    cab->nroEstacoes = 0;
    cab->nroParesEstacao = 0;*/

    fread(&cab->status, sizeof(char), 1, fp);
    fread(&cab->topo, sizeof(int32_t), 1, fp);
    fread(&cab->proxRRN, sizeof(int32_t), 1, fp);
    fread(&cab->nroEstacoes, sizeof(int32_t), 1, fp);
    fread(&cab->nroParesEstacao, sizeof(int32_t), 1, fp);
}




void exibir_registro(RegistroEstacao reg) 
{

    //verifica se o atributo esta nulo ou esta  aplicado corretamente

    if (reg.codEstacao == -1)
        printf("NULO ");
    else
        printf("%d ", reg.codEstacao);

    if (reg.tamNomeEstacao == 0)
        printf("NULO ");
    else
        printf("%s ", reg.nomeEstacao);

    if (reg.codLinha == -1)
        printf("NULO ");
    else
        printf("%d ", reg.codLinha);

    if (reg.tamNomeLinha == 0)
        printf("NULO ");
    else
        printf("%s ", reg.nomeLinha);

    if (reg.codProxEstacao == -1)
        printf("NULO ");
    else
        printf("%d ", reg.codProxEstacao);

    if (reg.distProxEstacao == -1)
        printf("NULO ");
    else
        printf("%d ", reg.distProxEstacao);

    if (reg.codLinhaIntegra == -1)
        printf("NULO ");
    else
        printf("%d ", reg.codLinhaIntegra);

    if (reg.codEstIntegra == -1)
        printf("NULO\n");
    else
        printf("%d\n", reg.codEstIntegra);
}


void funcionalidade1(const char *nome_csv, const char *nome_bin) {

    /* chama binarioNaTela antes de abrir para escrita */
    /* binarioNaTela(nome_bin); */

    FILE *fp = fopen(nome_bin, "wb");
    if (!fp) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    /* Cabeçalho inicial com status inconsistente */
    Cabecalho cab;
    cab.status                = '0';   /* inconsistente */
    cab.topo                  = -1;    /* sem removidos */
    cab.proxRRN               = 0;     /* primeiro RRN */
    cab.nroEstacoes       = 0;
    cab.nroParesEstacao = 0;

    escrever_cabecalho(fp, cab);

    /* Abre o CSV */
    FILE *csv = fopen(nome_csv, "r");
    if (!csv) {
        printf("Falha no processamento do arquivo.\n");
        fclose(fp);
        return;
    }

    char linha[512];

    /* Descarta a primeira linha (cabeçalho do CSV) */
    fgets(linha, sizeof(linha), csv);

    int qtd = 0;
    while (fgets(linha, sizeof(linha), csv)) {
        /* Ignora linhas em branco */
        if (linha[0] == '\n' || linha[0] == '\r') continue;

        RegistroEstacao reg;
        ler_linha_csv(linha, &reg);
        escrever_registro(fp, reg);
        qtd++;
    }

    fclose(csv);

    /* Reescreve o cabeçalho com status consistente */
    cab.status                = '1';
    cab.proxRRN               = qtd;
    cab.nroEstacoes       = qtd;   /* ajuste conforme regra real */
    cab.nroParesEstacao = qtd;   /* ajuste conforme regra real */

    rewind(fp);
    escrever_cabecalho(fp, cab);

    fclose(fp);

    /* chama binarioNaTela depois de fechar */
    /* binarioNaTela(nome_bin); */
}










void funcionalidade3(const char *nome_bin,
                     char campos[][50],
                     char valores[][50],
                     int m) {

    FILE *fp = fopen(nome_bin, "rb");
    if (!fp) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cab;
    if (!ler_cabecalho(fp, &cab) || cab.status != '1') {
        printf("Falha no processamento do arquivo.\n");
        fclose(fp);
        return;
    }

    int encontrou = 0;
    RegistroEstacao reg;

    while (ler_registro(fp, &reg)) { 
        
        if (reg.removido == '1') continue;

        // depois de ignorar registros removidos, verifica se o registro bate com os critérios de busca
        for (int i = 0; i < m; i++) {
            if (campo_bate(reg, campos[i], valores[i])) {
                exibir_registro(reg);
                encontrou = 1;
                break; /* evita exibir o mesmo registro duas vezes */
            }
        }
    }

    if (!encontrou) { //se nenhum registro bateu com os critérios, exibe mensagem
        printf("Registro inexistente.\n");
    }

    fclose(fp);
}




void funcionalidade4(const char *nome_bin, int rrn) {

    FILE *fp = fopen(nome_bin, "rb");
    if (!fp) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cab;
    if (!ler_cabecalho(fp, &cab) || cab.status != '1') {
        printf("Falha no processamento do arquivo.\n");
        fclose(fp);
        return;
    }

    /* calcula o offset: pula cabeçalho + RRN registros de 80 bytes */
    long offset = 17 + (long)rrn * 80;
    fseek(fp, offset, SEEK_SET);

    RegistroEstacao reg;
    if (!ler_registro(fp, &reg) || reg.removido == '1') {
        printf("Registro inexistente.\n");
        fclose(fp);
        return;
    }

    exibir_registro(reg);
    fclose(fp);
}
