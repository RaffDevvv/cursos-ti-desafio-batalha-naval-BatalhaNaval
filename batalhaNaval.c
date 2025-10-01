#include <stdio.h>

#define N 10      /* tamanho do tabuleiro */
#define MS 5      /* tamanho das matrizes de habilidade (5x5) */

/* ------------------------- Utilidades de tabuleiro ------------------------- */

void zera_tab(int B[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            B[i][j] = 0;
}

void imprime_tab(const int B[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%d ", B[i][j]);
        printf("\n");
    }
}

/* Posiciona alguns navios exemplo (valor 3) — só para visualização */
void posiciona_navios_demo(int B[N][N]) {
    /* Navio vertical (tamanho 3) iniciando em (1,2) */
    for (int k = 0; k < 3; k++)
        B[1 + k][2] = 3;

    /* Navio horizontal (tamanho 4) iniciando em (7,6) */
    for (int k = 0; k < 4; k++)
        B[7][6 - k] = 3;
}

/* Aplica uma matriz de habilidade MSxMS no tabuleiro, marcando com 'mark'.
   origin_r/origin_c = ponto de origem no TABULEIRO.
   anchor_r/anchor_c = célula da MATRIZ que deve coincidir com a origem.
   (Ex.: para CRUZ e OCTAEDRO, âncora = centro (2,2); para CONE, âncora = ápice (0,2))
*/
void aplica_mascara(int B[N][N],
                    const int M[MS][MS],
                    int origin_r, int origin_c,
                    int anchor_r, int anchor_c,
                    int mark)
{
    for (int mi = 0; mi < MS; mi++) {
        for (int mj = 0; mj < MS; mj++) {
            if (M[mi][mj] == 0) continue;

            /* Converte coordenada da matriz para o tabuleiro usando a âncora */
            int r = origin_r + (mi - anchor_r);
            int c = origin_c + (mj - anchor_c);

            if (r >= 0 && r < N && c >= 0 && c < N) {
                /* Marcamos 5 para área afetada, conforme enunciado.
                   Obs.: isso sobrescreve 0 (água) e 3 (navio) por 5 apenas na visualização. */
                B[r][c] = mark;
            }
        }
    }
}

/* --------------------------- Matrizes de habilidade ------------------------- */

/* CONE 5x5 (ápice no topo, coluna central = 2; expande para baixo)
   Exemplo de saída (1 = afetado):
     0 0 1 0 0
     0 1 1 1 0
     1 1 1 1 1
     0 0 0 0 0
     0 0 0 0 0
*/
void constroi_cone(int M[MS][MS]) {
    int apex_col = 2;  /* coluna do ápice */
    for (int i = 0; i < MS; i++) {
        for (int j = 0; j < MS; j++) {
            /* Condição: largura cresce 1 p/ cada linha que desce */
            int largura = i; /* em i=0 pega só o centro; i=1 pega centro±1; i=2 pega centro±2 */
            M[i][j] = ( (j >= apex_col - largura) && (j <= apex_col + largura) && (i <= 2) )
                      ? 1 : 0;
        }
    }
}

/* CRUZ 5x5 (centro (2,2)) — linha e coluna centrais */
void constroi_cruz(int M[MS][MS]) {
    for (int i = 0; i < MS; i++) {
        for (int j = 0; j < MS; j++) {
            M[i][j] = (i == 2 || j == 2) ? 1 : 0;
        }
    }
}

/* OCTAEDRO (losango) 5x5 (centro (2,2)), raio = 2 -> |i-2| + |j-2| <= 2 */
void constroi_octaedro(int M[MS][MS]) {
    int cx = 2, cy = 2, r = 2;
    for (int i = 0; i < MS; i++) {
        for (int j = 0; j < MS; j++) {
            int di = (i > cx) ? (i - cx) : (cx - i);
            int dj = (j > cy) ? (j - cy) : (cy - j);
            M[i][j] = (di + dj <= r) ? 1 : 0;
        }
    }
}

/* ------------------------------- Programa ----------------------------------- */

int main(void) {
    int board[N][N];
    int cone[MS][MS], cruz[MS][MS], oct[MS][MS];

    /* 1) Tabuleiro base 10x10 */
    zera_tab(board);
    posiciona_navios_demo(board); /* marca alguns navios (valor 3) */

    /* 2) Construir matrizes de habilidade (0/1) via loops + condicionais */
    constroi_cone(cone);
    constroi_cruz(cruz);
    constroi_octaedro(oct);

    /* 3) Integrar habilidades ao tabuleiro (sobreposição), marcando com 5 */
    /*    - CONE: âncora = ápice da matriz (0,2) → origem no tabuleiro (linha,coluna) */
    aplica_mascara(board, cone,      /* tabuleiro + máscara */
                   1, 5,             /* origem do cone no tabuleiro (ápice em (1,5)) */
                   0, 2,             /* âncora na matriz (0,2) */
                   5);               /* marca */

    /*    - CRUZ: âncora = centro (2,2) */
    aplica_mascara(board, cruz,
                   5, 2,             /* origem = centro da cruz no tabuleiro */
                   2, 2,             /* âncora centro da máscara */
                   5);

    /*    - OCTAEDRO: âncora = centro (2,2) */
    aplica_mascara(board, oct,
                   3, 7,             /* origem = centro do losango no tabuleiro */
                   2, 2,             /* âncora centro */
                   5);

    /* 4) Exibir tabuleiro final: 0=água, 3=navio, 5=área afetada */
    puts("Tabuleiro 10x10 (0=agua, 3=navio, 5=area de habilidade):");
    imprime_tab(board);

    return 0;
}
