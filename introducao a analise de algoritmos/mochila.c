#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    double capacidade,
    *peso,
    *valor;
    int quantidade;
} MOCHILA;

/* Se houver memória disponível, devolve o valor de uma solução ótima para a instância do PMB armazenada em p; caso contrário, devolve -1 */
double pmbPD(MOCHILA p) {
    int i, j;
    double *x, *ant, *aux;
    x = malloc(sizeof(double) * (p.capacidade + 1));
    ant = calloc(p.capacidade + 1, sizeof(double));
    if (x == NULL || ant == NULL) {
        return -1;
    }
    for (j = 0; j < p.quantidade; j++) {
        for (i = 0; i <= p.capacidade; i++) {
            if (p.peso[j] <= i && p.valor[j] + ant[i - (int) p.peso[j]] > ant[i]) {
                x[i] = p.valor[j] + ant[i - (int) p.peso[j]];
            } else {
                x[i] = ant[i];
            }
        }
        aux = ant;
        ant = x;
        x = aux;
    }
    return ant[(int) p.capacidade];
}
/* fim pmbPD */

/* se a solução codificada pelos bits i é viável, devolve o valor dessa solução; caso contrário, devolve -1 */
double valorMochila(MOCHILA p, int i) {
    int j;
    double valor = 0, peso = 0;
    for (j = 0; j < p.quantidade; j++) {
        if (i % 2) {
            if (p.peso[j] + peso > p.capacidade) {
                return -1;
            }
            valor += p.valor[j];
            peso += p.peso[j];
        }
        i = i / 2;
    }
    return valor;
}
/* fim valorMochila */

/* devolve o valor de uma solução ótima para a instância do PMB armazenada em p */
double pmbEE(MOCHILA p) {
    int i, n = 2 << p.quantidade;
    double valor, max = 0;
    for (i = 1; i < n; i++) {
        valor = valorMochila(p, i);
        if (valor > max) {
            max = valor;
        }
    }
    return max;
}
/* fim pmbEE */

int main() {
    int i;
    long tempo;
    MOCHILA p;
    printf("Capacidade da mochila: ");
    scanf("%lf", &p.capacidade);
    printf("Quantidade de itens: ");
    scanf("%d", &p.quantidade);
    p.peso = malloc(sizeof(double) * p.quantidade);
    p.valor = malloc(sizeof(double) * p.quantidade);
    if (p.peso == NULL || p.valor == NULL) {
        printf("Deu pau! Falta de memória\n");
        return 1;
    }
    for (i = 0; i < p.quantidade; i++) {
        printf("Peso do item %d: ", i + 1);
        scanf("%lf", &p.peso[i]);
        printf("Valor do item %d", i + 1);
        scanf("%lf", &p.valor[i]);
    }
    tempo = clock();
    printf("\nSolução pmbPD: %lf (%lf ms)", pmbPD(p), (double) (clock() - tempo) / CLOCKS_PER_SEC * 1000);
    printf("\nSolução pmbEE: %lf (%lf ms)\n", pmbEE(p), (double) (clock() - tempo) / CLOCKS_PER_SEC * 1000);
    return 0;
}
/* fim main */
