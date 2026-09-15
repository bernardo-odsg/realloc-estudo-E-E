#include <stdio.h>
#include <stdlib.h>

void processa_dados(int *vetor, int *tamanho) {
    int novo_tamanho = *tamanho * 2;
    int *temp = realloc(vetor, novo_tamanho * sizeof(int));
    
    if (temp != NULL) {
        vetor = temp;
        vetor[*tamanho] = 99;
        *tamanho = novo_tamanho;
    }
}

int main(void) {
    int tamanho = 2;
    int *dados = malloc(tamanho * sizeof(int));

    if (dados == NULL) return 1;

    dados[0] = 10;
    dados[1] = 20;

    processa_dados(dados, &tamanho);

    printf("Tamanho atual: %d\n", tamanho);
    printf("Primeiro elemento: %d\n", dados[0]);
    printf("Novo elemento: %d\n", dados[2]);

    free(dados);
    return 0;
}

//Exercícios

// 1) O que acontece com o ponteiro dados no Stack Frame da main após a execução da função processa_dados caso o realloc precise mover o bloco de memória no Heap para outro endereço (cenário out-of-place)?

// 2) O que o comando printf("Novo elemento: %d\n", dados[2]); tentará acessar? Isso é um comportamento definido ou um Undefined Behavior? Por quê?

// 3) Existe algum cenário de vazamento de memória nesse código? Onde ele ocorre?

// 4) Como a assinatura da função processa_dados e a chamada dentro da main devem ser corrigidas para que a modificação do vetor no Heap reflita com segurança na main?

//Respostas corrigidas

// 1. Seja E o endereço do bloco de memória no HEAP, armazenado pelo ponteiro dados. Ao executar a função processa_dados, se não houver espaço contíguo de 4 * sizeof(int) bytes, a função realloc irá copiar os números 10 e 20, alocar um novo bloco de memória de 4 * sizeof(int) bytes, de endereço E', e inserir os números 10 e 20 nas duas primeiras posições, respectivamente. O bloco de endereço E será liberado pelo sistema operacional, e o endereço E' será armazenado na variável temp. Em seguida, E' será armazenado na variável local vetor. Logo, o ponteiro dados estará armazenando E, endereço de um bloco de memória no HEAP que já foi liberado pelo sistema operacional. 

// 2. Seja E o endereço do bloco de memória no HEAP, armazenado pelo ponteiro dados. Ao executar a função processa_dados, se houver espaço contíguo de 4 * sizeof(int) bytes, a função realloc reservará esse espaço e devolverá ao ponteiro temp o endereço E. Em seguida, a variável local vetor receberá E, acessará o endereço E + 2 * sizeof(int) e armazenará nele o número 99. Logo, nesse caso, o comando printf acessará esse último endereço e imprimirá o número 99. Por outro lado, se não houver espaço contíguo de 4 * sizeof(int) bytes, sabemos, pela questão 1, que o bloco em questão será liberado pelo sistema operacional. Logo, nesse caso, concluímos que o comando printf estará realizando um Undefined Behavior, por tentar acessar um local que está em função do endereço de um bloco no HEAP que já foi liberado pelo sistema operacional.

// 3. Ao executar a função processa_dados, se não houver espaço contíguo de memória, sabemos, pelo item 1, que será alocado um novo bloco de memória de endereço E', e o bloco de endereço E será liberado pelo sistema operacional. No entanto, como o ponteiro dados na main continua apontando para E, o bloco de endereço E' não é liberado pelo sistema operacional, o que constitui um memory leak. Além disso, ao executar o comando free(dados), ocorre a tentativa de liberar um bloco que já foi liberado pelo SO, o que constitui um double free.

// 4.

// --------------------------------Código corrigido----------------------------------

#include <stdio.h>
#include <stdlib.h>

void processa_dados(int **vetor, int *tamanho) {
    int novo_tamanho = *tamanho * 2;
    int *temp = realloc(*vetor, novo_tamanho * sizeof(int));

    if (temp != NULL) {
        *vetor = temp;
        *(*vetor + *tamanho) = 99;
        *tamanho = novo_tamanho;          
    }
}

int main(void) {
    int tamanho = 2;
    int *dados = malloc(tamanho * sizeof(int));

    if (dados == NULL){
        return 1;
    }

    dados[0] = 10;
    dados[1] = 20;

    processa_dados(&dados, &tamanho);

    printf("Tamanho atual: %d\n", tamanho);
    printf("Primeiro elemento: %d\n", dados[0]);
    printf("Novo elemento: %d\n", dados[2]);

    free(dados);
    return 0;
}

// -----------------------------Explicação-------------------------------------------

// Ao executar a função processa_dados, passamos ao ponteiro local vetor o endereço do ponteiro dados da main. Ao desreferenciar o ponteiro vetor, é acessado o conteúdo armazenado pelo ponteiro dados, ou seja, o endereço E. Caso o realloc não retorne NULL, o ponteiro dados continuará armazenando E, no caso de haver espaço contíguo na execução do realloc, ou, caso contrário, receberá o endereço E', onde os números 10 e 20 serão armazenados nos endereços (E ou E') e (E ou E') + sizeof(int), respectivamente. (Lembrando que neste último caso, o bloco de endereço E será liberado pelo SO). Sendo assim, ao fazer *(*vetor + *tamanho) = 99, será acessado o endereço (E ou E') + 2 * sizeof(int) e armazenado o número 99 nesse endereço. Dessa forma, garantimos que as impressões serão executadas como o esperado. Por fim, liberamos o bloco da HEAP, garantindo efetivamente o gerenciamento seguro de memória ao longo de toda a execução do programa.
