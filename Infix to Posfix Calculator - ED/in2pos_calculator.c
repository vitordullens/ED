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
Descricao: Calculadora infixo para posfixo */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

char pilha[1000];
int topo = -1;
int numeros[1000], pos = -1;
char expressao[1000];
char posfixo[1000];
double pilha_r[1000];
int topo_r = -1;

void push(char num){
    pilha[++topo] = num;
}

char pop(){
    if(topo == -1) return printf("ESTA VAZIA\n");
    return pilha[topo--];
}

char check(){
    return pilha[topo];
}

void esvazia_pilha(){
    topo = -1;
}

void push_r(double num){
    pilha_r[++topo_r] = num;
}

double pop_r(){
    if(topo_r == -1) return -1;
    return pilha_r[topo_r--];
}
double check_r(){
    return pilha_r[topo_r];
}


void ler(){
    expressao[0] = '(';
    scanf(" %[^\n]s", expressao+1);
    int N = strlen(expressao);
    expressao[N] = ')';
}


bool validar(){ /*((a+b)*c+(d+e)/(f+g)+h)/i*/
    int N = strlen(expressao);
    int i, L=0, R=0;
    for(i=1;i<N-1;i++){
      switch (expressao[i]) {
        case '(':
          i++;
          L++;
          while(expressao[i] == '(' && i != N-1) i++, L++;

          if(!isalnum(expressao[i])) return false;

          break;

        case ')':
          i++;
          R++;
          while(expressao[i] == ')' && i != N-1) i++, R++;

          if(expressao[i]!='+' && expressao[i]!='-' && expressao[i]!='/' && expressao[i]!= '*' && i != N-1) return false;

          break;

        case '+':
        case '-':
        case '/':
        case '*':
          if(expressao[i-1] != ')' && !isalnum(expressao[i-1])) return false;

          else if (expressao[i+1] != '(' && !isalnum(expressao[i+1])) return false;

          break;

        default: if(!isalnum(expressao[i])) return false;

      }
    }

    return L == R;

}

void to_numbers(char infixo[]){
    int i, k = -1;
    char aux[1000];
    for( i = 0 ; infixo[i] != '\0'; ++i){
        if(isdigit(infixo[i])){
            aux[++k] = infixo[i];
        }
        else if(k>= 0){
            aux[++k] = '\0';
            numeros[++pos] = atoi(aux);
            k = -1;
        }
    }

}

void in2pos(char infixo[]){
    int i, l = 0;
    char aux;
    for( i = 0 ; infixo[i] != '\0'; ++i){
        switch(infixo[i]){
            case '(':
                push(infixo[i]);
                break;
            case ')':
                while( (aux = check()) != '(' ){
                    printf("%c ", aux);
                    posfixo[l++] = aux;
                    pop();

                }
                pop();
                break;

            case '+':
            case '-':
                while( (aux = check()) != '(' ){
                    printf("%c ", aux);
                    posfixo[l++] = aux;
                    pop();
                }
                push(infixo[i]);
                break;

            case '*':
            case '/':
                while( (aux = check()) != '(' && aux != '+' && aux != '-'){
                    printf("%c ", aux);
                    posfixo[l++] = aux;
                    pop();
                }
                push(infixo[i]);
                break;

            default:{
                if(!isdigit(infixo[i])){
                    printf("%c ",infixo[i]);
                    posfixo[l++] = infixo[i];
                }
                else{
                    while(isdigit(infixo[i])){
                      printf("%c",infixo[i]);
                      posfixo[l++] = infixo[i];
                      i++;
                    }
                    printf(" ");
                    i--;
                }
            }
        }
    }
    printf("\n");
}


void resolve(){
    int i, k = 0;
    char aux[1000];
    bool ok = true;

    for(i = 0 ; posfixo[i] != '\0'; ++i){
        if(isdigit(posfixo[i])){
            push_r(numeros[k++]);
            sprintf(aux,"%d", numeros[k-1]);
            i += strlen(aux) -1;
        }
        else{
            double aux2 = pop_r();
            double aux3 = pop_r();
            switch(posfixo[i]){
                case '+':
                    aux3 += aux2;
                    push_r(aux3);
                    break;
                case '-':
                    aux3 -= aux2;
                    push_r(aux3);
                    break;
                case '*':
                    aux3 *= aux2;
                    push_r(aux3);
                    break;
                case '/':
                    if(aux2 == 0) ok = false;
                    aux3 /= aux2;
                    push_r(aux3);
                    break;
            }
        }
        if(!ok) break;
    }
    if(ok)
        printf("RESPOSTA:  %.2lf\n", check_r());
    else
        printf("\n--FUNCAO INVALIDA---x/0\n\n");

}
void print(char expressao[]){


    if(!validar()) printf("\n--FUNCAO INVALIDA--\n\n");
    else{
        int i;
        to_numbers(expressao);
        bool aux=false;
        if(pos != -1){
            printf("TEMOS OS NUMEROS:  ");
            aux = true;
            for(i = 0 ; i <= pos ; i++)
                printf("%d ",numeros[i]);
            printf("\n");
        }
        else printf("NAO TEMOS NUMEROS\n");
        printf("INFIXO:  ");
        for(i = 1 ; i < strlen(expressao)-1; i++)
            printf("%c", expressao[i]);
        printf("\nPOSFIXO:  ");
        in2pos(expressao);
        if(aux) resolve();
    }

}



int main(){
    printf("Digite uma espressao válida (nao pode possuir espaco):  -  i.e: (2+3)*5  -\n");
    ler();
    print(expressao);
    return 0;
}
