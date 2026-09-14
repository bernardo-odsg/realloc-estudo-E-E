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

//Respostas

// 1) O ponteiro dados armazenará o novo endereço de memória do bloco no Heap.

// 2) Se, na chamada da função processa_dados na main, temp == NULL, o comando tentará acessar o endereço E + 2*sizeof(int), sendo E o endereço armazenado pelo ponteiro *dados após a execução do malloc, o que configura um Undefined Behavior. Caso temp != NULL, o comando acessará o endereço E' + 2*sizeof(int), sendo E' o endereço do bloco de memória armazenado pelo ponteiro temp, e imprimirá o que está armazenado nesse endereço, que no caso é o número inteiro 99.

// 3) Não existe vazamento de memória no código, pois a memória alocada dinamicamente na HEAP é liberada no fim do programa com o comando free(dados).

// 4) Não há necessidade de correção no código apresentado.