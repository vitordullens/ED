/* Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Estrutura de Dados – 1/2017
Alunos(as): Thiago Veras Machado
       ---- Vitor Fernandes Dullens
Matricula: 160146682
      ---- 160148260
Turma: A
Versão do compilador: gcc version 5.4.0
Descricao: Jogo mancala */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

typedef struct gametree{
  int dado;
  struct gametree *galhos[6];
}gametree;

int tabuleiro[14], dif;
int mm = 86;
void table(){
  system(CLEAR);
  printf("          | %2d |   | %2d |   | %2d |   | %2d |   | %2d |   | %2d |  Player 2\n", tabuleiro[12], tabuleiro[11], tabuleiro[10], tabuleiro[9], tabuleiro[8], tabuleiro[7]);
	printf(" ------   ------   ------   ------   ------   ------   ------  ------\n");
	printf("|      |     |        |        |        |        |        |   |      |\n");
	printf("|  %2d  |     1        2        3        4        5        6   |  %2d  |\n", tabuleiro[13], tabuleiro[6]);
	printf("|      |     |        |        |        |        |        |   |      |\n");
	printf(" ------   ------   ------   ------   ------   ------   ------  ------\n");
	printf(" Player 1 | %2d |   | %2d |   | %2d |   | %2d |   | %2d |   | %2d |\n", tabuleiro[0], tabuleiro[1], tabuleiro[2], tabuleiro[3], tabuleiro[4], tabuleiro[5]);
	printf("          ------   ------   ------   ------   ------   ------\n");
}

int play(int casa, int kalah, int begin){
  int aux, prox;
  if(tabuleiro[casa]==0) return 1;
  aux = tabuleiro[casa];
  tabuleiro[casa] = 0;
  prox = casa+1;
  while(aux > 1){
    if(prox == 14) prox = 0;
    else{
      tabuleiro[prox]++;
      prox++;
      aux--;
    }
  }
  if(prox==14) prox = 0;
  if(tabuleiro[prox]==0 && prox >= begin && prox < kalah){
    if(tabuleiro[12-prox] > 0){
      tabuleiro[kalah] += tabuleiro[12 - prox]+1;
			tabuleiro[12 - prox] = 0;
			return 0;
    }
  }
  tabuleiro[prox]++;
  if(prox == kalah) return 1;
  else return 0;
}

int ai(int qnt, int turno){

  int i, pts;
  int temp[14];
  int pos, aux, nzero, player;
  nzero = 0;
  if(qnt == dif) return tabuleiro[13] - tabuleiro[6]; // profundidade da árvore = nivel de dificuldade
  if(turno == 1){
    pts = -mm;; //seta como menor valor para ir pegando o max

    for(i=7;i<13;i++){
      if(tabuleiro[i]!=0){
        memcpy(temp, tabuleiro, sizeof(tabuleiro)); // se ultima peça caiu na minha kahala, ai o jogador pode jogar novamete
       player = play(i, 13, 7);

        if (player) aux = ai(qnt+1, turno); // nova jogada aprofundada na árvore
        else aux = ai(qnt+1, !turno);

        if(aux > pts){ //caso a nova jogada seja melhor, atualiza e salva a pos da kahala
          pts = aux;
          pos = i;
        }
        memcpy(tabuleiro, temp, sizeof(temp));
      }
      else nzero++; //aumenta a quantidade de kahalas vazias
    }
    if(qnt==0) return pos; // desceu em toda recursividade a voltou tudo, agora deve voltar a posição da melhor jogada
    if(nzero==6){ // acabou o jogo
      int resto = 0;
      for(i=0;i<6;i++) resto += tabuleiro[i];
      return tabuleiro[13] - (tabuleiro[6] + resto);
    }
  return pts; // caso nao esteja na ultima ou primeira recurssao, retorna a melhor qnt de pontos de uma jogada
  }
  pts = mm;
  for(i=0;i<6;i++){ // mesma coisa que encima só que ele pega o menor valor da jogada do adversário
    if(tabuleiro[i]!=0){
        memcpy(temp, tabuleiro, sizeof(tabuleiro));
        player = play(i, 6, 0);

        if (player) aux = ai(qnt+1, turno);
        else aux = ai(qnt+1, !turno);

        if(aux < pts) pts = aux;
        memcpy(tabuleiro, temp, sizeof(temp));
      }
      else nzero++;
  }
  if(nzero==6){
    int resto = 0;
    for(i=7;i<13;i++) resto+=tabuleiro[i];
    return (tabuleiro[13] + resto) - tabuleiro[6] ;
  }
  return pts;
}

int pick(int turno){
  int casa;
  scanf("%d", &casa);
  getchar();
  if(casa < 1 || casa > 6) return 1;
  if(!turno) return play(casa-1, 6, 0);
  return play(13-casa, 13, 7);
}

void resultado(){ // finaliza de verdade, na ia ele apenas simula o fim (caso seja a ultima jogada)
	int i;
	char ok;
	for(i = 0; i < 6; i++){ /*adicionar peças que faltam para a kalah*/
		tabuleiro[6] += tabuleiro[i];
		tabuleiro[i] = 0;
	}
	for(i = 7; i < 13; i++){ /*adicionar peças que faltaram para a kalah*/
		tabuleiro[13] += tabuleiro[i];
		tabuleiro[i] = 0;
	}
	table();

	if(tabuleiro[6] > tabuleiro[13]) printf("Player 1 venceu!\n");
	else if(tabuleiro[6] < tabuleiro[13]) printf("Player 2 venceu!\n");
	else printf("Empate.\n");

	printf("\tDigite qualquer tecla para voltar ao menu.   :)");
	getchar();


}

void initGame(){
  tabuleiro[0] = tabuleiro[1] = tabuleiro[2] = tabuleiro[3] = tabuleiro[4] = tabuleiro[5] = tabuleiro[7] = tabuleiro[8] = tabuleiro[9] = tabuleiro[10] = tabuleiro[11] = tabuleiro[12] = 4; /*começam com 4*/
	tabuleiro[6] = tabuleiro[13] = 0; /*kalahs vazias*/
}

void game(int modo){
  int player = 0;
  int aux;
  initGame();
  while((tabuleiro[0] > 0 || tabuleiro[1] > 0  || tabuleiro[2]  > 0 || tabuleiro[3] > 0 || tabuleiro[4] > 0 || tabuleiro[5] > 0 || player == 1) && ( tabuleiro[7] > 0 || tabuleiro[8] > 0 || tabuleiro[9] > 0 || tabuleiro[10] > 0 || tabuleiro[11] > 0 || tabuleiro[12] > 0 || player == 0)){
    table();
    printf("Turno - Player %d: ", player+1);
    if(modo == 1) aux = pick(player);
    else{
      if(player == 1){
        aux = ai(0, player);
        printf("%d   -----   casa escolhida pela I.A\n", 13-aux);
        getchar();
        aux = play(aux, 13, 7);
      }
      else aux = pick(player);
    }
    if(!aux) player = !player;
  }
  resultado();
}

void start(){
  int modo = 1;
  while (modo!=0){
    system(CLEAR);
    printf("\tMANCALA\n");
    printf("0 - Sair\n1 - Multijogador\n2 - Muito Facil\n3 - Facil\n4 - Medio\n5 - Dificil\n");
    scanf("%d", &modo);
    getchar();
    if(modo!=0){
        switch(modo){
          case 2:
            dif = 5;
            break;
          case 3:
            dif = 7;
            break;
          case 4:
            dif = 9;
            break;
          case 5:
            dif = 11;
            break;
        }
        game(modo);
    }
  }
}

int main(){
  start();
  return 0;
}
