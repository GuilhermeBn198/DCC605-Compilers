/*Compilador feito como trabalho final da disciplina de compiladores DCC605 ministrada pelo
professor Luciano*/
//Author: Guilherme Lucas Pereira Bernardo

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stack>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int edit;

string quadrupla;
char* quadruplaCharArr = nullptr;                                                           // Initialize to nullptr
string cod_inter;                                                                           
char* cod_intermediario_arr = nullptr;                                                      // Initialize to nullptr
string cod_intermediario_otimi;
char* cod_intermediario_otimi_arr = nullptr;                                                // Initialize to nullptr

char erroMsg[200];
int  error=0;

//************************analise lexica****************************

/* legenda: 1- Operador, 2- Keyword, 3- Integer, 4- float, 5- Identifier, 6- delimiter, 7- finalDelimeter,8- InvalidIdentifier*/

//lista com char token, int cod, struct lista prox, int line.
typedef struct lista{
    char token[35];
    int cod;
    struct lista* prox;
    int line;
}Lista;


//Esta função é usada para criar uma nova lista. Ele retorna NULL que representa uma lista vazia neste contexto.
Lista* lst_cria(void){
    return NULL;
}

//Esta função é utilizada para inserir um novo nó no início da lista. Leva quatro parâmetros - um ponteiro para a lista, um token (como uma matriz de caracteres), um código e um número de linha.
Lista* lst_insere(Lista* l, char token[], int cod, int line){
    Lista* no = (Lista*)malloc(sizeof(Lista));
    strcpy(no->token,token);
    no->cod=cod;
    no->line=line;
    no->prox=l;
    return no;
}

// Esta função é usada para copiar os nós da lista de origem para a lista de destino na ordem inversa. Leva dois parâmetros - um ponteiro para a lista de destino e um ponteiro para a lista de origem.
Lista* lst_cpy_inver(Lista* destino, Lista* fonte){
    Lista* p;
    p=fonte;
    while(p!=NULL){
        destino=lst_insere(destino,p->token,p->cod,p->line);
        p=p->prox;
    }
    return destino;
}

//Esta função é usada para imprimir os elementos da lista. Leva um ponteiro para a lista como parâmetro. Ele itera sobre a lista e, para cada nó, imprime o token, o código e o número da linha
void lst_imprime(Lista* l){
    Lista* p;
    p=l;
    while(p!=NULL){
        std::cout << "token: " << p->token << endl;
        std::cout << "codigo: " << p->cod << endl;
        std::cout << "linha: " << p->line << endl;
        p=p->prox;
    }
}

//Esta função verifica se a lista está vazia. Ele recebe um ponteiro para a lista como parâmetro e retorna verdadeiro (ou 1) se a lista estiver vazia (ou seja, o ponteiro é NULL) e falso (ou 0) caso contrário.
int lst_vazia(Lista* l){
    return(l==NULL);
}

//Esta função é utilizada para liberar a memória ocupada pela lista. Leva um ponteiro para a lista como parâmetro. Ele itera sobre a lista e, para cada nó, libera a memória alocada para o nó usando a função free().
void lst_libera(Lista* l){
    Lista* p;
    p=l;
    while(p!=NULL){
        Lista* t = p->prox;
        free(p);
        p=t;
    }
}

//Esta função verifica se um determinado token é uma palavra-chave. Neste exemplo, "int" e "float" são considerados palavras-chave. Ele recebe um ponteiro de caractere (estilo C) como argumento e retorna true se a string corresponder a "int" ou "float", caso contrário, false.
bool isKeyword(char* token) {
    if (!strcmp(token, "int") || !strcmp(token, "float"))
        return (true);
    return (false);
}


//automato da linguagem de inteiros: L = {w|w (+|-|num)(num)num*} recebe ponteiro de caractere (estilo C)
int AutomatoInteger (char *str){
    //inicial 0, final 2
    int i;
    /* Declarando a matriz de estados do automato M */
    int M[3][3]={{1, -1, -1},{1, -1, -1},{2, 2, 2}};

    /* Definindo o estado inicial */
    int e = 0;

    /* Recebera a indexacao referente ao caractere*/
    int c;

    /* Percorrendo toda a sentenca de avaliacao*/
    for(i=0;str[i]!='\0';i++){
        switch(str[i]){
            case '0': c=2; break;
            case '1': c=2; break;
            case '2': c=2; break;
            case '3': c=2; break;
            case '4': c=2; break;
            case '5': c=2; break;
            case '6': c=2; break;
            case '7': c=2; break;
            case '8': c=2; break;
            case '9': c=2; break;
            case '+': c=0; break;
            case '-': c=1; break;
            default:
                return 0;
        }

        /* Definindo o sistema de transicao*/
        e = M[c][e];
        if(e==-1){
            return 0;
        }
    }
    if(e==2)
        return 1;
    else
        return 0;
}

//automato da linguagem de numeros floats: L = {w|w (+|-|num)(num)num*(.)(num)num*} recebe ponteiro de char
int AutomatoFloat (char *str){
    //inicial 0, final 4
    int i;
    /* Declarando a matriz de estados do automato M */
    int M[4][5]={{1, -1, -1, -1, -1}, {1, -1, -1, -1, -1}, {2, 2, 2, 4, 4}, {-1, -1, 3, -1, -1}};

    /* Definindo o estado inicial */
    int e = 0;

    /* Recebera a indexacao referente ao caractere*/
    int c;

    /* Percorrendo toda a sentenca de avaliacao*/
    for(i=0;str[i]!='\0';i++){
        switch(str[i]){
            case '0': c=2; break;
            case '1': c=2; break;
            case '2': c=2; break;
            case '3': c=2; break;
            case '4': c=2; break;
            case '5': c=2; break;
            case '6': c=2; break;
            case '7': c=2; break;
            case '8': c=2; break;
            case '9': c=2; break;
            case '+': c=0; break;
            case '-': c=1; break;
            case '.': c=3; break;
            default:
                return 0;
        }

        /* Definindo o sistema de transicao*/
        e = M[c][e];
        if(e==-1)
            return 0;
    }
    if(e==4)
        return 1;
    else
        return 0;
}

//automato da linguagem de identificadores L = {w|w letra(letra|numero)*} recebe ponteiro de char *
int AutomatoIdentifier (char *str){
    int i;
    /* Declarando a matriz de estados do automato M */
    int M[2][2]={{-1, 1},{1, 1}};

    /* Definindo o estado inicial */
    int e = 0;

    /* Recebe a indexacao referente ao caractere*/
    int c;

    /* Percorrendo toda a sentenca de avaliacao*/
    for(i=0;str[i]!='\0';i++){
        switch(str[i]){
            case 'A': c=1; break; case 'a': c=1; break;
            case 'B': c=1; break;case 'b': c=1; break;
            case 'C': c=1; break;case 'c': c=1; break;
            case 'D': c=1; break;case 'd': c=1; break;
            case 'E': c=1; break;case 'e': c=1; break;
            case 'F': c=1; break;case 'f': c=1; break;
            case 'G': c=1; break;case 'g': c=1; break;
            case 'H': c=1; break;case 'h': c=1; break;
            case 'I': c=1; break;case 'i': c=1; break;
            case 'J': c=1; break;case 'j': c=1; break;
            case 'K': c=1; break;case 'k': c=1; break;
            case 'L': c=1; break;case 'l': c=1; break;
            case 'M': c=1; break;case 'n': c=1; break;
            case 'O': c=1; break;case 'o': c=1; break;
            case 'P': c=1; break;case 'p': c=1; break;
            case 'Q': c=1; break;case 'q': c=1; break;
            case 'R': c=1; break;case 'r': c=1; break;
            case 'S': c=1; break;case 's': c=1; break;
            case 'T': c=1; break;case 't': c=1; break;
            case 'U': c=1; break;case 'u': c=1; break;
            case 'V': c=1; break;case 'v': c=1; break;
            case 'W': c=1; break;case 'w': c=1; break;
            case 'X': c=1; break;case 'x': c=1; break;
            case 'Y': c=1; break;case 'y': c=1; break;
            case 'Z': c=1; break;case 'z': c=1; break;
            case '0': c=0; break;
            case '1': c=0; break;
            case '2': c=0; break;
            case '3': c=0; break;
            case '4': c=0; break;
            case '5': c=0; break;
            case '6': c=0; break;
            case '7': c=0; break;
            case '8': c=0; break;
            case '9': c=0; break;
            default:
                return 0;
        }

        /* Definindo o sistema de transicao*/
        e = M[c][e];
        if(e==-1)
            return 0;
    }
    if(e==1)
        return 1;
    else
        return 0;
}

//Esta função verifica se um determinado caractere é um delimitador final. Nesse caso, a função retorna true se o caractere for um ponto e vírgula (';') e false caso contrário. recebe char
bool isDelimiterfinal(char ch){
    if (ch == ';')
        return (true);
    return (false);
}

// Retorna 'true' se o caractere for um delimitador. recebe char
bool isDelimiter(char ch){
    if (ch == ' ' || ch == ';' || ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ',' || ch == '=' || ch == '(' ||
        ch == ')' || ch == '\n' || ch == '\0' || ch == '\'' || ch == '"')
        return (true);
    return (false);
}

// Esta função verifica se um determinado caractere é um delimitador de caracteres. Ele retorna verdadeiro se o caractere for uma vírgula, parênteses, aspas simples ou aspas duplas. Caso contrário, retorna false. recebe char
bool isDelimiterChar(char ch){
    if (ch == ',' || ch == '(' || ch == ')' ||
        ch == '\'' || ch == '"')
        return (true);
    return (false);
}

// Retorna 'true' se o caractere for um operador, recebe char.
bool isOperator(char ch){
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == '=')
        return (true);
    return (false);
}

Lista* separateAndAnalyze(char *parse, Lista* l, int line);
char* subString(char* str, int left, int right);

// funcao de analise lexica do compilador, recebe ponteiro de caractere que corresponde aos dados a serem analisado, um inteiro len que representa o tamanho do array, a lista de nos l, e a linha atual em formato inteiro
Lista* analise_lexica(char *iParse, int len, Lista* l, int line){
    char* parse = new char[len+1]; // Allocate memory dynamically
    strcpy(parse, iParse); // copia os caracteres de iparse para parse
    l = separateAndAnalyze(parse, l, line);
    delete[] parse;  // Free the allocated memory
    return l;
}

// Lista* analise_lexica(char *iParse, int len, Lista* l, int line){  //implementacao antiga - sem alloc dinamico
//     char parse[len];
//     strcpy(parse,iParse);
//     l = separateAndAnalyze(parse, l, line);
//     return l;
// }

// funcao principal da analise lexical, que separa os tokens e analisa eles, recebe o caractere correspondente atual a ser analisado,
Lista* separateAndAnalyze(char *parse, Lista* l, int line){
    int left, right, len, buffer=0,temPontoEVirgula=0;
    char linha[100];
    strcpy(linha,parse);
    char *token = strtok(parse,";");

    while(token != NULL){
        buffer+=strlen(token);
        if(linha[buffer]==';'){
            temPontoEVirgula=1;
            buffer++;
        }
        left=0, right=0, len = strlen(token);
        while (right <= len && left <= right) {
            if (isDelimiter(token[right]) == false)
                right++;

            if (isDelimiter(token[right]) == true && left == right) {
                if (isOperator(token[right]) == true){
                    std::cout << token[right] << " IS AN OPERATOR" << endl; // printa a detecção de operadores
                    char tokenCH[2];tokenCH[0]=token[right];tokenCH[1]='\0';
                    l = lst_insere(l, tokenCH, 1, line);
                }
                else if(isDelimiterChar(token[right]) == true){
                    if(token[right]==';') //
                        std::cout << token[right] << endl<<endl<<endl; // printa algo q n sei ainda
                    char tokenCH[2];tokenCH[0]=token[right];tokenCH[1]='\0';
                    l = lst_insere(l, tokenCH, 6, line);
                }
                else if(isDelimiterfinal(token[right]) == true){
                    std::cout << token[right] << endl<<endl<<endl; //
                    char tokenCH[2];tokenCH[0]=token[right];tokenCH[1]='\0';
                    l = lst_insere(l, tokenCH, 7, line);
                }
                right++;
                left = right;
            }
            else if (isDelimiter(token[right]) == true && left != right || (right == len && left != right)) {
                char* subStr = subString(token, left, right - 1);
                if (isKeyword(subStr) == true){
                    std::cout << subStr<< " IS A KEYWORD" << endl; //printa a analise de keywords
                    l = lst_insere(l, subStr, 2, line);
                }
                else if (AutomatoInteger(subStr) == true){
                    std::cout << subStr << " IS AN INTEGER" << endl; // printa a analise de integers
                    l = lst_insere(l, subStr, 3, line);
                }
                else if (AutomatoFloat(subStr) == true){
                    std::cout << subStr << " IS A FLOAT NUMBER" << endl; // printa a analise de floats
                    l = lst_insere(l, subStr, 4, line);
                }
                else if (AutomatoIdentifier(subStr) == true && isDelimiter(token[right - 1]) == false){
                    if(strlen(subStr)>1){
                        char str[10];
                        sprintf(str, "%d", line+1);
                        strcpy(erroMsg, "erro lexico\n");
                        strcat(erroMsg, "Linha: ");
                        strcat(erroMsg, str);
                        strcat(erroMsg, " ");
                        strcat(erroMsg, subStr);
                        strcat(erroMsg, " eh muito grande este compilador nao esta apto a lidar com identificadores maiores do que 1 caractere");
                        error=1;
                        std::cout << erroMsg << endl; //pra verificar se ta ok o arquivo
                        //exit(2); // erro de analise lexica
                        break;
                    }
                    std::cout << subStr << " IS A VALID IDENTIFIER" << endl; //
                    l = lst_insere(l, subStr, 5, line);
                }
                //esta sendo carregado algum caractere oculto pela interface causando mal funcionamento desta parte
                /*else if (AutomatoIdentifier(subStr) == false && isDelimiter(token[right - 1]) == false){
                    //cout << subStr << " IS NOT A VALID IDENTIFIER" << endl;
                    char str[10];
                    strcpy(erroMsg, "erro lexico\n");
                    strcat(erroMsg, "Iidentificador invalido ");
                    strcat(erroMsg, subStr);
                    strcat(erroMsg, " na linha: ");
                    sprintf(str, "%d", line+1);
                    strcat(erroMsg, str);
                    error=1;
                    break;
                    l = lst_insere(l, subStr, 8, line);
                }*/
                left = right;
            }
        }
        if(temPontoEVirgula){
            l = lst_insere(l, ";", 7, line);
            temPontoEVirgula=0;
        }
        token = strtok(NULL,";");
    }
    return l;
}

// extrai a substring, recebe um ponteiro de caractere, int correspondente à esq, int correspondente à dir
char* subString(char* str, int left, int right) {
    int i;
    char *subStr = (char*)malloc(sizeof(char) * (right - left + 2));

    for (i = left; i <= right; i++){
        subStr[i - left] = str[i];
    }
    subStr[right - left + 1] = '\0';

    return (subStr);
}

//**********************analise sintatica*******************************

int blockWeakTree = 0;
stack<char>pilha_arvore;
stack<char>pilha_arvore_exp;
int blockWeakTreeS = 0;
stack<char>pilha_gen_arvS;
stack<char>pilha_gen_arvS_exp;

typedef struct no{
    char info[35];
    struct no *esq;
    struct no *center;
    struct no *dir;
}Nodo;

typedef struct noS{
    char info[35];
    struct noS *esq;
    struct noS *dir;
}NodoS;

typedef struct quadrupla{
    char op[1];
    char arg1[10];
    char arg2[10];
    char result[10];
    struct quadrupla* prox;
}Quadrupla;

typedef struct arv{
    Nodo *raiz;
}Arv;

typedef struct arvS{
    NodoS *raiz;
}ArvS;

Arv* arv_cria(void){
    Arv* arv = (Arv*)malloc(sizeof(Arv));
    arv->raiz = NULL;
    return arv;
}


ArvS* arv_criaS(void){
    ArvS* arvS = (ArvS*)malloc(sizeof(ArvS));
    arvS->raiz = NULL;
    return arvS;
}

Nodo* arv_busca_no(Nodo* raiz, char *info);

//This function is a wrapper function for arv_busca_no. It starts the search from the root of the syntax tree.
Nodo* arv_busca(Arv* arv, char *info){
    return arv_busca_no(arv->raiz, info);
}

//This is a recursive function that searches for a node in the syntax tree that has a matching info and at least one NULL child (either esq, center, or dir). It performs a depth-first search, first checking the current node (raiz), then recursively checking the esq, center, and dir children. If it finds a matching node, it returns a pointer to that node. If it doesn't find a match, it returns NULL.
Nodo* arv_busca_no(Nodo* raiz, char *info){
    if(raiz==NULL){
        return raiz;
    }
    else if(strcmp(raiz->info, info)==0 && (raiz->esq==NULL || raiz->center==NULL || raiz->dir==NULL)){
        return raiz;
    }

    Nodo *no;
    no = arv_busca_no(raiz->esq, info);
    if(no==NULL)
        no = arv_busca_no(raiz->center, info);
    if(no==NULL)
        no = arv_busca_no(raiz->dir, info);
    return no;
}

//funcao para corrigir ambiguidade do E;E+S, M;S-M, M;M*D, D;D/P, P;(E)
void check_lower(Nodo *raiz, Nodo *raiz2, char *info, char *info2){
    if(raiz==NULL || strcmp(raiz->info, "+")==0 || strcmp(raiz->info, "-")==0 || strcmp(raiz->info, "*")==0 || strcmp(raiz->info, "-")==0 ||
       strcmp(raiz->info, "(")==0 || strcmp(raiz->info, ")")==0 || strcmp(raiz->info, "v")==0 || strcmp(raiz->info, "&")==0 || blockWeakTree)
        return;

    if(raiz->esq==NULL && raiz->center==NULL){
        char str[2];
        if(strcmp(raiz->info, "E")==0 && strcmp(raiz->dir->info, "S")==0 && info[0]=='+'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        else if(strcmp(raiz->info, "S")==0 && strcmp(raiz->dir->info, "M")==0 && info[0]=='-'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        else if(strcmp(raiz->info, "M")==0 && strcmp(raiz->dir->info, "D")==0 && info[0]=='*'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        else if(strcmp(raiz->info, "D")==0 && strcmp(raiz->dir->info, "P")==0 && info[0]=='/'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        else if(strcmp(raiz->info, "P")==0 && strcmp(raiz->dir->info, ")")==0 && info[0]=='E'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
    }


    check_lower(raiz->dir, raiz2->dir, info, info2);
    check_lower(raiz->center, raiz2->center, info, info2);
    check_lower(raiz->esq, raiz2->esq, info, info2);
}


void bloqueia_nao_terminais_filhos(Nodo *raiz, Nodo *raiz2){
    if(raiz==NULL || strcmp(raiz->info, "+")==0 || strcmp(raiz->info, "-")==0 || strcmp(raiz->info, "*")==0 || strcmp(raiz->info, "/")==0 ||
       strcmp(raiz->info, "(")==0 || strcmp(raiz->info, ")")==0 || strcmp(raiz->info, "v")==0 || strcmp(raiz->info, "&")==0)
        return;

    if(raiz->esq==NULL && raiz->center==NULL){
        raiz->center = (Nodo*)malloc(sizeof(Nodo));
        strcpy(raiz->center->info, "&");
        raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;
        raiz->esq = (Nodo*)malloc(sizeof(Nodo));
        strcpy(raiz->esq->info, "&");
        raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

        raiz2->center = (Nodo*)malloc(sizeof(Nodo));
        strcpy(raiz2->center->info, "&");
        raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;
        raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
        strcpy(raiz2->esq->info, "&");
        raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;
    }

    bloqueia_nao_terminais_filhos(raiz->dir, raiz2->dir);
    bloqueia_nao_terminais_filhos(raiz->center, raiz2->center);
    bloqueia_nao_terminais_filhos(raiz->esq, raiz2->esq);
}

//Nodo* inserir_arv_no(Nodo* raiz, char *info);

void arv_insert_content_no(Nodo* raiz, Nodo* raiz2, char *info, char *info2){
    if(raiz==NULL || strcmp(raiz->info, "+")==0 || strcmp(raiz->info, "-")==0 || strcmp(raiz->info, "*")==0 || strcmp(raiz->info, "/")==0 ||
       strcmp(raiz->info, "(")==0 || strcmp(raiz->info, ")")==0 || strcmp(raiz->info, "v")==0 || strcmp(raiz->info, "&")==0 || blockWeakTree)
        return;

    char str[2];
    if(raiz->esq==NULL && raiz->center==NULL && raiz->dir==NULL){
        str[0]=info[0];str[1]='\0';
        raiz->dir = (Nodo*)malloc(sizeof(Nodo));
        strcpy(raiz->dir->info, str);
        raiz->dir->esq = raiz->dir->center = raiz->dir->dir = NULL;

        str[0]=info2[0];str[1]='\0';
        raiz2->dir = (Nodo*)malloc(sizeof(Nodo));
        strcpy(raiz2->dir->info, str);
        raiz2->dir->esq = raiz2->dir->center = raiz2->dir->dir = NULL;

        if(info[0]=='v'){
            raiz->center = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->center->info, "&");
            raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;
            raiz->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->esq->info, "&");
            raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

            raiz2->center = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->center->info, "&");
            raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;
            raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->esq->info, "&");
            raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;
        }

        blockWeakTree=1;
        return;
    }

    else if(raiz->esq==NULL && raiz->center==NULL){
        //expres�o E->E+S
        if(strcmp(raiz->info, "E")==0 && strcmp(raiz->dir->info, "S")==0 && info[0]=='+'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        //express�o S->S-M
        else if(strcmp(raiz->info, "S")==0 && strcmp(raiz->dir->info, "M")==0 && info[0]=='-'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        //express�o M->M*D
        else if(strcmp(raiz->info, "M")==0 && strcmp(raiz->dir->info, "D")==0 && info[0]=='*'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        //express�o D->D/P
        else if(strcmp(raiz->info, "D")==0 && strcmp(raiz->dir->info, "P")==0 && info[0]=='/'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        //express�o P->(E)
        else if(strcmp(raiz->info, "P")==0 && strcmp(raiz->dir->info, ")")==0 && info[0]=='E'){
            check_lower(raiz->dir, raiz2->dir, info, info2);
            if(blockWeakTree==0){
                str[0]=info[0];str[1]='\0';
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, str);
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;

                str[0]=info2[0];str[1]='\0';
                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, str);
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;

                blockWeakTree=1;
                return;
            }
        }
        //bloqueia os esq e center do n�
        else if(strcmp(raiz->dir->info, ")")==0 && info[0]!='E'){
            raiz->center = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->center->info, "&");
            raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;
            raiz->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->esq->info, "&");
            raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

            raiz2->center = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->center->info, "&");
            raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;
            raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->esq->info, "&");
            raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;
        }
        if(raiz->dir->esq!=NULL && raiz->dir->center!=NULL){
            if(strcmp(raiz->dir->esq->info, "&")==0 && strcmp(raiz->dir->center->info, "&")==0){
                raiz->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->center->info, "&");
                raiz->center->esq = raiz->center->center = raiz->center->dir = NULL;
                raiz->esq = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz->esq->info, "&");
                raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

                raiz2->center = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->center->info, "&");
                raiz2->center->esq = raiz2->center->center = raiz2->center->dir = NULL;
                raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
                strcpy(raiz2->esq->info, "&");
                raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;
            }
        }
    }
    else if(raiz->esq==NULL){
        //expres�o E->E+S
        if(strcmp(raiz->info, "E")==0 && strcmp(raiz->center->info, "+")==0 && strcmp(raiz->dir->info, "S")==0 && info[0]=='E'){
            str[0]=info[0];str[1]='\0';
            raiz->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->esq->info, str);
            raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

            str[0]=info2[0];str[1]='\0';
            raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->esq->info, str);
            raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;

            blockWeakTree=1;
            return;
        }
        //express�o S->S-M
        else if(strcmp(raiz->info, "S")==0 && strcmp(raiz->center->info, "-")==0 && strcmp(raiz->dir->info, "M")==0 && info[0]=='S'){
            str[0]=info[0];str[1]='\0';
            raiz->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->esq->info, str);
            raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

            str[0]=info2[0];str[1]='\0';
            raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->esq->info, str);
            raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;

            blockWeakTree=1;
            return;
        }
        //express�o M->M*D
        else if(strcmp(raiz->info, "M")==0 && strcmp(raiz->center->info, "*")==0 && strcmp(raiz->dir->info, "D")==0 && info[0]=='M'){
            str[0]=info[0];str[1]='\0';
            raiz->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->esq->info, str);
            raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

            str[0]=info2[0];str[1]='\0';
            raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->esq->info, str);
            raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;

            blockWeakTree=1;
            return;
        }
        //express�o D->D/P
        else if(strcmp(raiz->info, "D")==0 && strcmp(raiz->center->info, "/")==0 && strcmp(raiz->dir->info, "P")==0 && info[0]=='D'){
            str[0]=info[0];str[1]='\0';
            raiz->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->esq->info, str);
            raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

            str[0]=info2[0];str[1]='\0';
            raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->esq->info, str);
            raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;

            blockWeakTree=1;
            return;
        }
        //express�o P->(E)
        else if(strcmp(raiz->info, "P")==0 && strcmp(raiz->center->info, "E")==0 && strcmp(raiz->dir->info, ")")==0 && info[0]=='('){
            str[0]=info[0];str[1]='\0';
            raiz->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz->esq->info, str);
            raiz->esq->esq = raiz->esq->center = raiz->esq->dir = NULL;

            str[0]=info2[0];str[1]='\0';
            raiz2->esq = (Nodo*)malloc(sizeof(Nodo));
            strcpy(raiz2->esq->info, str);
            raiz2->esq->esq = raiz2->esq->center = raiz2->esq->dir = NULL;
            bloqueia_nao_terminais_filhos(raiz->center, raiz2->center);

            blockWeakTree=1;
            return;
        }
    }

    arv_insert_content_no(raiz->dir, raiz2->dir, info, info2);
    arv_insert_content_no(raiz->center, raiz2->center, info, info2);
    arv_insert_content_no(raiz->esq, raiz2->esq, info, info2);
}

void inserir_arv(Arv* arv, Arv* arv2, char *info, char *info2){
    if(arv->raiz==NULL){
        char str[1];
        str[0]=info[0];str[1]='\0';
        arv->raiz = (Nodo*)malloc(sizeof(Nodo));
        strcpy(arv->raiz->info, str);
        arv->raiz->esq = arv->raiz->center = arv->raiz->dir = NULL;

        str[0]=info2[0];str[1]='\0';
        arv2->raiz = (Nodo*)malloc(sizeof(Nodo));
        strcpy(arv2->raiz->info, str);
        arv2->raiz->esq = arv2->raiz->center = arv2->raiz->dir = NULL;
    }
    else{
        arv_insert_content_no(arv->raiz, arv2->raiz, info, info2);
        blockWeakTree=0;
    }

}

void cria_pilha_gen(Nodo* raiz, Nodo* raiz2){
    if(raiz!=NULL && raiz->info[0]!='&'){
        if(raiz->info[0]=='v' || raiz->info[0]=='+' || raiz->info[0]=='-' || raiz->info[0]=='*' || raiz->info[0]=='/'){
            pilha_gen_arvS.push(raiz->info[0]);
            pilha_gen_arvS_exp.push(raiz2->info[0]);
        }
        cria_pilha_gen(raiz->dir, raiz2->dir);
        cria_pilha_gen(raiz->esq, raiz2->esq);
        cria_pilha_gen(raiz->center, raiz2->center);
	}
}

void check_lowerS(NodoS* raiz, NodoS* raiz2){
    if(raiz==NULL || strcmp(raiz->info, "v")==0 || blockWeakTreeS)
        return;

    if(raiz->esq==NULL &&!blockWeakTreeS){
        char info[2];
        info[0]=pilha_gen_arvS.top();info[1]='\0';
        raiz->esq = (NodoS*)malloc(sizeof(NodoS));
        strcpy(raiz->esq->info, info);
        raiz->esq->esq = raiz->esq->dir = NULL;
        pilha_gen_arvS.pop();

        info[0]=pilha_gen_arvS_exp.top();info[1]='\0';
        raiz2->esq = (NodoS*)malloc(sizeof(NodoS));
        strcpy(raiz2->esq->info, info);
        raiz2->esq->esq = raiz2->esq->dir = NULL;
        pilha_gen_arvS_exp.pop();

        blockWeakTreeS=1;
        return;
    }
    else if(raiz->esq->info[0]=='+' || raiz->esq->info[0]=='-' || raiz->esq->info[0]=='*' || raiz->esq->info[0]=='/')
        check_lowerS(raiz->esq, raiz2->esq);
    if(raiz->dir==NULL && !blockWeakTreeS){
        char info[2];
        info[0]=pilha_gen_arvS.top();info[1]='\0';
        raiz->dir = (NodoS*)malloc(sizeof(NodoS));
        strcpy(raiz->dir->info, info);
        raiz->dir->esq = raiz->dir->dir = NULL;
        pilha_gen_arvS.pop();

        info[0]=pilha_gen_arvS_exp.top();info[1]='\0';
        raiz2->dir = (NodoS*)malloc(sizeof(NodoS));
        strcpy(raiz2->dir->info, info);
        raiz2->dir->esq = raiz2->dir->dir = NULL;
        pilha_gen_arvS_exp.pop();

        blockWeakTreeS=1;
        return;
    }
    check_lowerS(raiz->esq, raiz2->esq);
    check_lowerS(raiz->dir, raiz2->dir);
}


//This function is used to construct a tree for 'S' (denoted by arvS). It takes as input two nodes (raiz and raiz2). The function checks several conditions (if the root is NULL or if the root's information is 'v' or if the blockWeakTreeS flag is set). It then fills the left and right nodes of the tree using the top elements of two stacks (pilha_gen_arvS and pilha_gen_arvS_exp). If any of the left or right child nodes of the root contain an operator ('+', '-', '*', '/'), it calls the check_lowerS function on these child nodes. The function recursively calls itself to construct the tree for all nodes.
void constroi_arvS(NodoS* raiz, NodoS* raiz2){
    if(raiz==NULL || strcmp(raiz->info, "v")==0 || blockWeakTreeS)
        return;

    if(raiz->esq==NULL &&!blockWeakTreeS){
        char info[2];
        info[0]=pilha_gen_arvS.top();info[1]='\0';
        raiz->esq = (NodoS*)malloc(sizeof(NodoS));
        strcpy(raiz->esq->info, info);
        raiz->esq->esq = raiz->esq->dir = NULL;
        pilha_gen_arvS.pop();

        info[0]=pilha_gen_arvS_exp.top();info[1]='\0';
        raiz2->esq = (NodoS*)malloc(sizeof(NodoS));
        strcpy(raiz2->esq->info, info);
        raiz2->esq->esq = raiz2->esq->dir = NULL;
        pilha_gen_arvS_exp.pop();

        blockWeakTreeS=1;
        return;
    }
    else if(raiz->esq->info[0]=='+' || raiz->esq->info[0]=='-' || raiz->esq->info[0]=='*' || raiz->esq->info[0]=='/')
        check_lowerS(raiz->esq, raiz2->esq);

    if(raiz->dir==NULL && !blockWeakTreeS){
        char info[2];
        info[0]=pilha_gen_arvS.top();info[1]='\0';
        raiz->dir = (NodoS*)malloc(sizeof(NodoS));
        strcpy(raiz->dir->info, info);
        raiz->dir->esq = raiz->dir->dir = NULL;
        pilha_gen_arvS.pop();

        info[0]=pilha_gen_arvS_exp.top();info[1]='\0';
        raiz2->dir = (NodoS*)malloc(sizeof(NodoS));
        strcpy(raiz2->dir->info, info);
        raiz2->dir->esq = raiz2->dir->dir = NULL;
        pilha_gen_arvS_exp.pop();

        blockWeakTreeS=1;
        return;
    }

    constroi_arvS(raiz->esq, raiz2->esq);
    constroi_arvS(raiz->dir, raiz2->dir);
}

//This function simplifies a weak tree. It takes as input four trees (arv, arv2, arvS, arvS2). It creates a stack from the root nodes of the first two trees. If the root of arvS is NULL, it creates a new root node using the top element of the stack pilha_gen_arvS and does the same for arvS2 using the top element of the stack pilha_gen_arvS_exp. It then constructs the arvS and arvS2 trees using the constroi_arvS function until the pilha_gen_arvS stack is empty.
void simplifica_arvore_fraca(Arv* arv, Arv* arv2, ArvS* arvS, ArvS* arvS2){
    cria_pilha_gen(arv->raiz, arv2->raiz);
    char info[2];
    if(arvS->raiz==NULL){
        info[0]=pilha_gen_arvS.top();info[1]='\0';
        arvS->raiz = (NodoS*)malloc(sizeof(NodoS));
        strcpy(arvS->raiz->info, info);
        arvS->raiz->esq = arvS->raiz->dir = NULL;
        pilha_gen_arvS.pop();

        info[0]=pilha_gen_arvS_exp.top();info[1]='\0';
        arvS2->raiz = (NodoS*)malloc(sizeof(NodoS));
        strcpy(arvS2->raiz->info, info);
        arvS2->raiz->esq = arvS2->raiz->dir = NULL;
        pilha_gen_arvS_exp.pop();
    }
    while(!pilha_gen_arvS.empty()){
        constroi_arvS(arvS->raiz, arvS2->raiz);
        blockWeakTreeS=0;
    }
}

void arv_imprime_no(Nodo* raiz);

//This function prints a tree. It calls the arv_imprime_no function to print the root node of the tree.
void arv_imprime(Arv* arv){
    return arv_imprime_no(arv->raiz);
}

//This function prints a node of a tree in a recursive manner. If the node is not NULL and its information is not '&', it first prints the node's information, then recursively prints the left, center, and right children of the node.
void arv_imprime_no(Nodo* raiz){
	printf("<");
	if(raiz!=NULL && raiz->info[0]!='&'){
        printf("%s",raiz->info);
        arv_imprime_no(raiz->esq);
        arv_imprime_no(raiz->center);
        arv_imprime_no(raiz->dir);
	}
	printf(">");
}

//This function prints a node of a tree 'S'. If the node is not NULL, it prints the node's information, then recursively prints the left and right children of the node.
void arv_imprime_noS(NodoS* raiz){
	printf("<");
	if(raiz!=NULL){
        printf("%s",raiz->info);
        arv_imprime_noS(raiz->esq);
        arv_imprime_noS(raiz->dir);
	}
	printf(">");
}

// This function frees the memory allocated to a node of a tree. If the node is not NULL, it recursively frees the left, center, and right children of the node, then frees the node itself.
void liberar_avr_no(Nodo* raiz);

//This function frees the memory allocated to a tree. It first frees the root node of the tree using the liberar_avr_no function, then frees the tree itself.
void liberar_avr(Arv* arv){
    liberar_avr_no(arv->raiz);
    free(arv);
}

void liberar_avr_no(Nodo* raiz){
    if(raiz!=NULL){
        liberar_avr_no(raiz->esq);
        liberar_avr_no(raiz->center);
        liberar_avr_no(raiz->dir);
        free(raiz);
    }
}

//This function frees the memory allocated to a node of a tree 'S'. If the node is not NULL, it recursively frees the left and right children of the node, then frees the node itself.
void liberar_avr_noS(NodoS* raiz){
    if(raiz!=NULL){
        liberar_avr_noS(raiz->esq);
        liberar_avr_noS(raiz->dir);
        free(raiz);
    }
}

int AutomatoM(char *str, char *str2, int line){
    /* concatenando o simbolo delimitador na sentenca */
    strcat(str,"$");
    strcat(str2,"$");

    /* Declarando a tabela sintatica para o automato M */
    char M[13][8]={{'D', '\0', '\0', '\0', '\0', 'D', '\0', '\0'}, {'R', 'D', '\0', '\0', '\0', 'R', '\0', 'R'}, {'R', 'R', 'D', '\0', '\0', 'R', '\0', 'R'}, {'R', 'R', 'R', 'D', '\0', 'R', '\0', 'R'}, {'R', 'R', 'R', 'R', '\0', 'R', '\0', 'R'},
                    {'\0', '\0', '\0', '\0', 'D', '\0', 'D', '\0'}, {'\0', '\0', '\0', '\0', 'D', '\0', 'D', '\0'}, {'\0', '\0', '\0', '\0', 'D', '\0', 'D', '\0'}, {'\0', '\0', '\0', '\0', 'D', '\0', 'D', '\0'}, {'\0', '\0', '\0', '\0', 'D', '\0', 'D', '\0'},
                    {'R', 'R', 'R', 'R', '\0', 'R', '\0', 'R'}, {'R', 'R', 'R', 'R', '\0', 'R', '\0', 'R'}, {'\0', '\0', '\0', '\0', 'D', '\0', 'D', '\0'}};

    const int QLinhasProducoes = 10;
    char producoes[QLinhasProducoes][6]={{"E;E+S"},{"E;S"},{"S;S-M"},{"S;M"},{"M;M*D"},{"M;D"},{"D;D/P"},{"D;P"},{"P;(E)"},{"P;v"}};

    /*Declarando a pilha*/
    stack<char>pilha;
    stack<char>pilha_aux;

    /* Colocando o simbolo delimitador na pilha*/
    pilha.push('$');

    /* Recebe a indexacao referente ao matriz*/
    int c,l,i;
    /* Recebe as producoes*/
    char Prod[4];

    int parentese=0;
    i=0;
    while(str[i]!='$'){
        if(str[i]=='(')
            parentese++;
        else if(str[i]==')')
            parentese--;
        i++;
    }
    if(parentese>0){
        char temp[10];
        sprintf(temp, "%d", line+1);
        strcpy(erroMsg, "erro Sintatico\n");
        strcat(erroMsg, "Linha: ");
        strcat(erroMsg, temp);
        strcat(erroMsg, " esperava )");
        error=2;
        std::cout << erroMsg << endl; // printa durante o debug do compilador(lembrar de tirar depois!!!!!!!!)

    }
    else if(parentese<0){
        char temp[10];
        sprintf(temp, "%d", line+1);
        strcpy(erroMsg, "erro Sintatico\n");
        strcat(erroMsg, "Linha: ");
        strcat(erroMsg, temp);
        strcat(erroMsg, " esperava (");
        error=2;
        std::cout << erroMsg << endl; // printa durante o debug do compilador(lembrar de tirar depois!!!!!!!!)
    }

    int k,QProd;
    /* Percorrendo toda a sentenca de avaliacao*/
    i=0;
    while(str[i]!='\0' && error==0){
        switch(str[i]){
            case '+':
                c=0;
                break;
            case '-':
                c=1;
                break;
            case '*':
                c=2;
                break;
            case '/':
                c=3;
                break;
            case '(':
                c=4;
                break;
            case ')':
                c=5;
                break;
            case 'v':
                c=6;
                break;
            case '$':
                c=7;
                break;
            default:
                return 0;
        }

        while(1){
            switch(pilha.top()){
                case 'E':
                    l=0;
                    break;
                case 'S':
                    l=1;
                    break;
                case 'M':
                    l=2;
                    break;
                case 'D':
                    l=3;
                    break;
                case 'P':
                    l=4;
                    break;
                case '+':
                    l=5;
                    break;
                case '-':
                    l=6;
                    break;
                case '*':
                    l=7;
                    break;
                case '/':
                    l=8;
                    break;
                case '(':
                    l=9;
                    break;
                case ')':
                    l=10;
                    break;
                case 'v':
                    l=11;
                    break;
                case '$':
                    l=12;
                    break;
                default:
                    return 0;
            }

            /* Escolhendo a producao a ser aplicada pela tabela sintatica */

            char Nprod = M[l][c];  //l = topo da stack, c = caractere atual do input string
            int producoes_compativeis[2];

            /* Fazendo equivalencia entre a producao e ordem inversa e o seu numero da tabela */
            switch(Nprod){
                case 'D':
                    pilha.push(str[i]);
                    pilha_arvore.push(str[i]);
                    pilha_arvore_exp.push(str2[i]);
                    i++;
                    break;
                case 'R':
                    QProd=0;
                    for(int j=0;j<QLinhasProducoes;j++){
                        k = strlen(producoes[j]);
                        if(producoes[j][k-1]==pilha.top()){
                            producoes_compativeis[QProd] = j;
                            QProd++;

                        }
                    }

                    if(QProd==1){
                        pilha.pop();
                        pilha.push(producoes[producoes_compativeis[QProd-1]][0]);
                        pilha_arvore.push(producoes[producoes_compativeis[QProd-1]][0]);
                        pilha_arvore_exp.push(producoes[producoes_compativeis[QProd-1]][0]);
                    }
                    else{
                        int tamanho=0,block;
                        char producao_comp[5];
                        for(int j=0;j<QProd;j++){//produ��o maior precisa vim antes para est� implenta��o funcionar
                            for(k=strlen(producoes[producoes_compativeis[j]])-1;k>1;k--){
                                pilha_aux.push(pilha.top());
                                producao_comp[tamanho]=pilha.top();
                                tamanho++;
                                pilha.pop();
                                if(pilha.empty() || producoes[producoes_compativeis[j]][k]!=pilha_aux.top()){
                                    block=1;
                                    break;
                                }
                            }
                            producao_comp[tamanho]='\0';
                            if(block==0){
                                for(k=0;producao_comp[k]==pilha_aux.top();k++){
                                    pilha.push(pilha_aux.top());
                                    pilha_aux.pop();
                                    if(pilha_aux.empty())
                                        break;
                                }
                            };
                            if(pilha_aux.empty()){
                                for(k=strlen(producoes[producoes_compativeis[j]])-1;k>1;k--){
                                    pilha.pop();
                                }
                                pilha.push(producoes[producoes_compativeis[j]][0]);
                                pilha_arvore.push(producoes[producoes_compativeis[j]][0]);
                                pilha_arvore_exp.push(producoes[producoes_compativeis[j]][0]);
                                break;
                            }
                            else{
                                while(!pilha_aux.empty()){
                                    pilha.push(pilha_aux.top());
                                    pilha_aux.pop();
                                }
                            }
                            block=0;
                        }
                    }
                    break;
                default:
                    error=1;
                    return 0;
            }

            /* Verificando se ha o sentencial no topo da pilha e se o delimitador eh o caractere em analise*/
            if(pilha.top()=='E' && str[i]=='$')
                return 1;
            /* Mudanca de estado do automato */
            else
                break;

        }
    }
};

Quadrupla* quadrupla_cria(void);
void gera_codigo(NodoS* raiz, int vAmount, Lista* l);
Quadrupla* quadrupla_cpy_inver(Quadrupla* dest, Quadrupla* source);
void quadrupla_imprime(Quadrupla* q);
void quadrupla_libera(Quadrupla* q);
void imprime_cod_itermediario(Quadrupla* q);
void otimiza_cod_intermediario(Quadrupla* q);
void imprime_cod_itermediario_otimizado(Quadrupla* q);
void gera_mips(char *linha);
void gera_mips2(Quadrupla *q);

Quadrupla* q1;
Quadrupla* q2;
int erroSyntax=0;

stack<char>pilha_atribuicao;

/* legenda: 1- Operador, 2- Keyword, 3- Integer, 4- float, 5- Identifier, 6- delimiter, 7- finalDelimeter,8- InvalidIdentifier*/
void analise_sintatica(Lista* l){
    Lista* p;
    p=l;   // p recebe a lista 
    q1=quadrupla_cria();
    char atribuicao[2];
    int useExp=0,declaracao=0;
    while(p!=NULL && error==0){
        char exp[40];exp[0]='\0';
        char exp2[40];exp2[0]='\0';
        while(p->token[0]!=';' && p!=NULL){
            if(p->cod==2 && p->prox->cod==5 && p->prox->prox->cod==7){ //identifica 1 sentenca de declaracao de identificador
                declaracao=1;
                p=p->prox->prox;
                break;
            }
            else if((p->cod==3 || p->cod==4 || p->cod==5 || p->token[0]==')') && p->prox!=NULL){ // identifica se atual p é integer, float, identificador, ou ) e se o proximo p não é nulo
                if(p->prox->cod!=1 && p->prox->token[0]!=')' && p->prox->cod!=7){ // o codigo do proximo p nao ser operador, o token dele for ) e o codigo nao for de delimitador final, entao eh um erro sintatico dos dados 
                    char temp[10];
                    sprintf(temp, "%d", p->line+2);
                    strcpy(erroMsg, "\n\n\t\t********erro sintatico detectado**********\n");
                    strcat(erroMsg, "\tLinha: ");
                    strcat(erroMsg, temp);
                    strcat(erroMsg, "\tfalta ; antes de ");
                    strcat(erroMsg, p->prox->token);
                    error=1;
                    std::cout << erroMsg << endl;// printa o erro
                    exit(3); //erro de sintaxe 
                }
                else if(p->cod==5 && p->prox->cod==7) //se o p atual for identificador e o proximo um delimitador final(;) ele assume que é um inicio de uma expressao matematica
                    useExp=1;
            }
            if(p->prox->token[0]=='='){ //detecta o operador de atribuicao e coloca a variavel na pilha de atribuicao 
                pilha_atribuicao.push(p->token[0]);
            }
            if(useExp==0 && p->prox->token[0]=='+' || p->prox->token[0]=='-' || p->prox->token[0]=='*' ||
                    p->prox->token[0]=='/' || p->token[0]=='(' || p->token[0]==')'){ //verifica se ainda não foi detectado que eh uma expressao, se o proximo token eh um sinal matematico ou se nao eh ( ), se passar por tudo isso, o compilador assume que eh uma expressao matematica
                useExp=1;
            }
            if(useExp){ //se useExp for true, inicia a fase de detecao da operacao matematica
                if(p->token[0]=='+' || p->token[0]=='-' || p->token[0]=='*' || p->token[0]=='/' ||
                   p->token[0]=='(' || p->token[0]==')') {
                        strcat(exp2,p->token);
                    }
                else if(p->cod==3 || p->cod==4 || p->cod==5) strcat(exp2,"v");
                
                strcat(exp,p->token);
                std::cout << exp << std::endl; // mostra a atribuicao de operacao matematica ocorrendo
            }
            else if(p->token[0]=='='){
                pilha_atribuicao.push(p->token[0]);
                useExp=1;
            }
            p=p->prox;
        }
        if(declaracao==0 && error==0){
            exp[strlen(exp)]='\0';
            int res = AutomatoM(exp2, exp, p->line);
            if(res==1){
                Arv* arv1 = arv_cria();
                Arv* arvExp1 = arv_cria();
                ArvS* arv2 = arv_criaS();
                ArvS* arvExp2 = arv_criaS();
                char topo_pilha[1];
                char topo_pilha_exp[1];
                int vAmount=0;
                while(!pilha_arvore.empty()){ //pilha_arvare_exp tem o mesmo tamanho que pilha_arvore
                    topo_pilha_exp[0]=pilha_arvore_exp.top();topo_pilha_exp[1]='\0';
                    topo_pilha[0]=pilha_arvore.top();topo_pilha[1]='\0';
                    inserir_arv(arv1, arvExp1, topo_pilha, topo_pilha_exp);
                    if(pilha_arvore.top()=='v')
                        vAmount++;
                    pilha_arvore.pop();
                    pilha_arvore_exp.pop();
                }
                simplifica_arvore_fraca(arv1, arvExp1, arv2, arvExp2);
                liberar_avr(arv1);
                liberar_avr(arvExp1);
                gera_codigo(arvExp2->raiz, vAmount, l);
                liberar_avr_noS(arv2->raiz);
            }
            else if(error!=2){
                erroSyntax++;
                if(erroSyntax<2){
                    char temp[10];
                    sprintf(temp, "%d", p->line+1);
                    strcpy(erroMsg, "erro Sintatico\n");
                    strcat(erroMsg, "Linha: ");
                    strcat(erroMsg, temp);
                    strcat(erroMsg, " erro de syntax");
                    std::cout << erroMsg << endl; // printa durante o debug do compilador(lembrar de tirar depois!!!!!!!!)
                }
                break;
            }
            else
                break;
        }
        declaracao=0;
        useExp=0;
        p=p->prox;
    }
    if(error==0){
        q2=quadrupla_cria();
        q2=quadrupla_cpy_inver(q2,q1);
        std::cout << "a quadrupla gerada do arquivo analisado pela analise sintatica eh:" << endl;
        quadrupla_imprime(q2);
        std::cout << endl;
        imprime_cod_itermediario(q2);
        //std::cout << "o codigo intermediario gerado do arquivo analisado pela analise sintatica eh:" << endl;
        otimiza_cod_intermediario(q2);
        std::cout << endl;
        //std::cout << "e seu codigo otimizado eh:" << endl;
        imprime_cod_itermediario_otimizado(q2);
    }
    quadrupla_libera(q1);
}

//*********************** Semantica *************************************
Lista* var_declara;
Lista* lst_cpy_var(Lista* dest, Lista* source){
    Lista* p;
    p=source;
    while(p!=NULL){
        if(p->cod==2 && p->prox!=NULL){
            if(p->prox->prox!=NULL){
                if(p->prox->cod==5 && (p->prox->prox->token[0]=='=' || p->prox->prox->token[0]==';')){
                    dest=lst_insere(dest,p->prox->token,p->cod,p->prox->line);
                    gera_mips(p->prox->token);
                }
            }
        }
        p=p->prox;
    }
    return dest;
}


int busca_identificador(char *token, int line){
    Lista *p;
    p=var_declara;
    while(p!=NULL){
        if(strcmp(p->token,token)==0 && p->line<=line)
            return 1;
        p=p->prox;
    }
    return 0;
}

void checa_var_declara(Lista* l){
    Lista *p;
    p=l;
    int flag=0;
    while(p!=NULL){
        if(p->cod==5){
            flag = busca_identificador(p->token, p->line);
            if(!flag){
                char temp[10];
                sprintf(temp, "%d", p->line+1);
                strcpy(erroMsg, "erro Semantico\n");
                strcat(erroMsg, "Linha: ");
                strcat(erroMsg, temp);
                strcat(erroMsg, " ");
                strcat(erroMsg, p->token);
                strcat(erroMsg, " nao foi declarado neste escopo");
                error=1;
                //std::cout << erroMsg << endl; // printa durante o debug do compilador(lembrar de tirar depois!!!!!!!!)
                break;
            }
        }
        p=p->prox;
    }
}



//************************Geracao do codigo ******************************
string quadruplaSTR;


Quadrupla* quadrupla_cria(void){
    return NULL;
}

Quadrupla* quadrupla_insere(Quadrupla* q, char op[], char arg1[], char arg2[], char result[]){
    Quadrupla* linha = (Quadrupla*)malloc(sizeof(Quadrupla));
    linha->op[0]=op[0];linha->op[1]='\0';
    strcpy(linha->arg1,arg1);
    strcpy(linha->arg2, arg2);
    strcpy(linha->result,result);
    linha->prox=q;
    return linha;
}

Quadrupla* quadrupla_cpy_inver(Quadrupla* dest, Quadrupla* source){
    Quadrupla* p;
    p=source;
    while(p!=NULL){
        dest=quadrupla_insere(dest,p->op,p->arg1,p->arg2,p->result);
        p=p->prox;
    }
    return dest;
}

void quadrupla_imprime(Quadrupla* q){
    Quadrupla* p;
    p=q;
    std::cout << "op| " << "arg1| " << "arg2| " << "result" << endl;
    quadruplaSTR = "op | arg1 |  arg2 | result \r\n";
    char tempOp[1];
    while(p!=NULL){
        tempOp[0]=p->op[0];tempOp[1]='\0';
        quadruplaSTR = quadruplaSTR + tempOp + " | ";
        quadruplaSTR = quadruplaSTR + p->arg1 + " | ";
        quadruplaSTR = quadruplaSTR + p->arg2 + " | ";
        quadruplaSTR = quadruplaSTR + p->result + " \r\n";
        std::cout << p->op[0] << " | " << std::flush;
        std::cout << p->arg1 << " | " << std::flush;
        std::cout << p->arg2 << " | " << std::flush;
        std::cout << p->result << endl;
        p=p->prox;
    }
    std::cout << endl;
    std::cout << quadruplaSTR << endl;
}

void quadrupla_libera(Quadrupla* q){
    Quadrupla* p;
    p=q;
    while(p!=NULL){
        Quadrupla* t = p->prox;
        free(p);
        p=t;
    }
}

int num;


void le_e_altera_arvore2(NodoS* raiz, Lista* l){
    if(raiz==NULL)
        return;

    if(raiz->esq!=NULL && raiz->dir!=NULL){
        if(raiz->esq->info[0]!='+' && raiz->esq->info[0]!='-' && raiz->esq->info[0]!='*' && raiz->esq->info[0]!='/' &&
           raiz->dir->info[0]!='+' && raiz->dir->info[0]!='-' && raiz->dir->info[0]!='*' && raiz->dir->info[0]!='/'){
            char str[10], temp[10]={"_t"};
            int a1=0,a2=0;
            sprintf(str, "%d", num);
            strcat(temp,str);
            q1=quadrupla_insere(q1,raiz->info,raiz->esq->info,raiz->dir->info,temp);
            //quadrupla_imprime(q);
            strcpy(raiz->info,temp);
            raiz->esq=NULL;
            raiz->dir=NULL;
            num++;
        }
    }
    le_e_altera_arvore2(raiz->dir, l);
    le_e_altera_arvore2(raiz->esq, l);

}

void le_e_altera_arvore(NodoS* raiz, Lista* l){
    while(raiz->esq!=NULL && raiz->dir!=NULL){
        le_e_altera_arvore2(raiz, l);
    }
    if(!pilha_atribuicao.empty()){
        char str[10];
        char op[5], str2[10], *ttt;
        op[0]=pilha_atribuicao.top();op[1]='\0';
        pilha_atribuicao.pop();
        str2[0]=pilha_atribuicao.top();str2[1]='\0';
        q1=quadrupla_insere(q1,op,raiz->info,"&",str2);
        pilha_atribuicao.pop();
    }
}

void gera_codigo(NodoS* raiz, int vAmount, Lista* l){
    num=1;
    le_e_altera_arvore(raiz, l);
    //quadrupla_imprime(q);
}

string cod_intermediarioSTR;

void imprime_cod_itermediario(Quadrupla* q){
    Quadrupla *p;
    p=q;
    cod_intermediarioSTR="";
    char tempOp[1];
    while(p!=NULL){
        cod_intermediarioSTR = cod_intermediarioSTR + p->result + " := " + p->arg1 + " ";
        std::cout << p->result << " := " << std::flush;
        std::cout << p->arg1 << " " << std::flush;
        if(p->op[0]!='='){
            tempOp[0]=p->op[0];tempOp[1]='\0';
            cod_intermediarioSTR = cod_intermediarioSTR + tempOp + " ";
            std::cout << p->op[0] << " " << std::flush;
        }
        if(strcmp(p->arg2,"&")!=0){
            cod_intermediarioSTR = cod_intermediarioSTR + p->arg2;
            std::cout << p->arg2  << std::flush;
        }
        cod_intermediarioSTR = cod_intermediarioSTR + "\r\n";
        std::cout << endl;
        p=p->prox;
    }
}

void otimiza_cod_intermediario(Quadrupla* q){
    Quadrupla *p;
    char temp[10], id[10];
    p=q;
    while(p!=NULL){
        //v+0 ou 0+v
        if(p->op[0]=='+' && (strcmp(p->arg1,"0")==0 || strcmp(p->arg2,"0")==0)){
            if(strcmp(p->arg1,"0")!=0){
                strcpy(id, p->arg1);
                strcpy(temp,p->result);
                strcpy(p->result,"&");
                if(strcmp(p->prox->arg1,temp)==0){
                    strcpy(p->prox->arg1,id);
                }
                else if(strcmp(p->prox->arg2,temp)==0){
                    strcpy(p->prox->arg2,id);
                }
            }
            else if(strcmp(p->arg2,"0")!=0){
                strcpy(id, p->arg2);
                strcpy(temp,p->result);
                strcpy(p->result,"&");
                if(strcmp(p->prox->arg1,temp)==0){
                    strcpy(p->prox->arg1,id);
                }
                else if(strcmp(p->prox->arg2,temp)==0){
                    strcpy(p->prox->arg2,id);
                }
            }
        }
        //v-0
        else if(p->op[0]=='-' && strcmp(p->arg2,"0")==0){
            strcpy(id, p->arg1);
            strcpy(temp,p->result);
            strcpy(p->result,"&");
            if(strcmp(p->prox->arg1,temp)==0){
                strcpy(p->prox->arg1,id);
            }
            else if(strcmp(p->prox->arg2,temp)==0){
                strcpy(p->prox->arg2,id);
            }
        }
        //v*1 ou 1*v
        else if(p->op[0]=='*' && (strcmp(p->arg1,"1")==0 || strcmp(p->arg2,"1")==0)){
            if(strcmp(p->arg1,"1")!=0){
                strcpy(id, p->arg1);
                strcpy(temp,p->result);
                strcpy(p->result,"&");
                if(strcmp(p->prox->arg1,temp)==0){
                    strcpy(p->prox->arg1,id);
                }
                else if(strcmp(p->prox->arg2,temp)==0){
                    strcpy(p->prox->arg2,id);
                }
            }
            else if(strcmp(p->arg2,"1")!=0){
                strcpy(id, p->arg2);
                strcpy(temp,p->result);
                strcpy(p->result,"&");
                if(strcmp(p->prox->arg1,temp)==0){
                    strcpy(p->prox->arg1,id);
                }
                else if(strcmp(p->prox->arg2,temp)==0){
                    strcpy(p->prox->arg2,id);
                }
            }
        }
        //v/1
        else if(p->op[0]=='/' && strcmp(p->arg2,"1")==0){
            strcpy(id, p->arg1);
            strcpy(temp,p->result);
            strcpy(p->result,"&");
            if(strcmp(p->prox->arg1,temp)==0){
                strcpy(p->prox->arg1,id);
            }
            else if(strcmp(p->prox->arg2,temp)==0){
                strcpy(p->prox->arg2,id);
            }
        }
        p=p->prox;
    }
}

string cod_intermediario_otimiSTR;

void imprime_cod_itermediario_otimizado(Quadrupla* q){
    Quadrupla *p;
    p=q;
    cod_intermediario_otimiSTR="";
    char tempOp[1];
    while(p!=NULL){
        if(strcmp(p->result,"&")!=0){
            cod_intermediario_otimiSTR = cod_intermediario_otimiSTR + p->result + " := " + p->arg1 + " ";
            std::cout << p->result << " := " << std::flush;
            std::cout << p->arg1 << " " << std::flush;
            if(p->op[0]!='='){
                tempOp[0]=p->op[0];tempOp[1]='\0';
                cod_intermediario_otimiSTR = cod_intermediario_otimiSTR + tempOp + " ";
                std::cout << p->op[0] << " " << std::flush;
            }
            if(strcmp(p->arg2,"&")){
                cod_intermediario_otimiSTR = cod_intermediario_otimiSTR + p->arg2;
                std::cout << p->arg2 << std::flush;
            }

            cod_intermediario_otimiSTR = cod_intermediario_otimiSTR + "\r\n";
            std::cout << endl;
        }
        p=p->prox;
    }
    std::cout << endl << cod_intermediarioSTR << endl;
}

void gera_mips(char *var){
    FILE *p;
    p = fopen("saida.txt","a");

    if(p == NULL){
        printf("Arquivo nao pode ser aberto");
    }
    else{
        char line[100];
        strcpy(line, var);
        strcat(line, ": .word\n");
        fprintf(p, line);

        fclose(p);
        std::cout << "codigo mips eviado ao arquivo" << endl;
    }
}

void gera_mips2(Quadrupla *q){
    FILE *p;
    p = fopen("saida.txt","a");
    if(p == NULL){
        printf("Arquivo nao pode ser aberto");
    }
    else{
        char line[100];
        Quadrupla *g;
        g=q;
        fprintf(p, "\nmain:\n\n");
        while(g!=NULL){
            if(g->op[0]=='+' || g->op[0]=='-'){
                if(g->op[0]=='-')
                    strcpy(line, "add ");
                else
                    strcpy(line, "sub ");
                if(g->result[0]=='_')
                    g->result[0]='$';
                strcat(line, g->result);
                strcat(line, ",");
                if(g->arg1[0]=='_')
                    g->arg1[0]='$';
                strcat(line, g->arg1);
                strcat(line, ",");
                if(g->arg2[0]=='_')
                    g->arg2[0]='$';
                strcat(line, g->arg2);
                strcat(line, "\n");
            }
            else if(g->op[0]=='*' || g->op[0]=='/'){
                strcpy(line, "li ");
                if(g->result[0]=='_')
                    g->result[0]='$';
                strcat(line, g->result);
                strcat(line, ",");
                if(g->arg1[0]=='_')
                    g->arg1[0]='$';
                strcat(line, g->arg1);
                strcat(line, "\n");
                if(g->op[0]=='*')
                    strcat(line, "mult ");
                else
                    strcat(line, "div ");
                strcat(line, g->result);
                strcat(line, ",");
                if(g->arg2[0]=='_')
                    g->arg2[0]='$';
                strcat(line, g->arg2);
                strcat(line, "\n");
            }
            if(g->op[0]=='='){
                strcpy(line, "lw ");
                strcat(line, g->result);
                strcat(line, ",");
                if(g->arg1[0]=='_')
                    g->arg1[0]='$';
                strcat(line, g->arg1);
                strcat(line, "\n");
            }
            if(g->result[0]!='&')
                fprintf(p, line);
            g=g->prox;
        }
        fclose(p);
    }
}


// This function reads from a file and returns its content as a string.
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nao foi possivel abrir o arquivo: " << filename << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));
    return content;
}

void write_file(const std::string& path, const std::string& data){
    // Open the file in write mode.
    std::ofstream file(path, std::ios::out);

    // Check if the file was opened successfully.
    if (!file) {
        std::cerr << "Unable to open file";
        return;
    }

    // Write the data to the file.
    file.write(data.c_str(), data.size());

    // Close the file.
    file.close();
}

void save_file(const std::string& data){
    std::string filename;
    std::cout << "Enter the filename to save: " << std::flush;
    std::cin >> filename;

    // Open the file in write mode.
    std::ofstream file(filename, std::ios::out);

    // Check if the file was opened successfully.
    if (!file) {
        std::cerr << "Unable to open file" << std::flush;
        return;
    }

    // Write the data to the file.
    file.write(data.c_str(), data.size());

    // Close the file.
    file.close();
}

void espaco(){
    std::cout << endl << std::flush;
}

// serve para visualizar uma lista ligada
void visualiza_lista(Lista* lista){
    Lista* temp = lista;
    if(temp != NULL) {
        std::cout << "A atual lista contem: " << std::endl << std::endl;
        while(temp != NULL) {
            std::cout << temp->token << " " << std::flush;
            std::cout << temp->cod << " " << std::flush;
            std::cout << temp->line << endl;
            temp = temp->prox;
        }
        std::cout << endl;
    } else {
        std::cout << "A lista esta vazia." << endl;
    }
}

void compilar(std::string fileContent){
    Lista* l;
    l=lst_cria();

    FILE *p;
    p = fopen("saida.txt", "w");
    fprintf(p,".data\n\n");
    fclose(p);

    int _size = fileContent.length();
    char *data = new char[_size+1];
    fileContent.copy(data, _size);
    data[_size] = '\0';


    espaco();
    // std::cout << "quantidade de caracteres" << std::endl;
    // std::cout <<  _size  << endl;                                                                  //verificar se ta funcionando a importação dos dados de entrada
    // espaco();
    // std::cout << "print dos dados recebidos por data" << std::endl;
    // std::cout << data << std::endl;                                                      //printar os dados recebidos(TA OK)

    /////////////////// fase de analise lexica //////////////////////

    char exp[100];
    int i=0,i2=0,numLinha=0;
    
    std::cout << "\t\t**********inicializando fase de analise lexica dos dados providos**********" << std::endl << std::endl;
    while(i<_size){
        exp[i2]=data[i];  //recebe os dados obtidos de fileContent
        i++;
        i2++;
        if(data[i-1]==';'){
            exp[i2]='\0';
            l=analise_lexica(exp,100,l,numLinha);
            exp[0]='\0';
            i2=0;
        }
        if(data[i]=='\n')
            numLinha++;
            // std::cout << numLinha <<std::endl;                                            //visualiza a contagem de linhas
    }
    // std::cout << "visualizacao dos dados de l: token codigo linha." << std::endl;
    // visualiza_lista(l);

    // for(int j=0; j<_size; j++){                                                          //visualiza a separacao dos tokens
    //     std::cout << data[j] << std::endl;
    // }


    /////////////// fase de analise sintatica //////////////////

    // lista de tokens da fase sintatica
    Lista* l2;
    l2=lst_cria();
    espaco();
    std::cout << "\t\t**********Inicio da fase de analise sintatica dos dados providos**********" << std::flush;
    espaco();
    // visualiza_lista(l2);  // para visualizar l2 que esta atualmente vazia
    if(error==0){
        std::cout << "se nenhum erro for detectado pela analise lexica entao o compilador executa a analise sintatica" << endl << endl;
        l2=lst_cpy_inver(l2,l); //original
        // std::cout << "lista preenchida de forma invertida antes de passar pela analise sintatica" << endl;
        // visualiza_lista(l2);
        // espaco();
        analise_sintatica(l2);
    std::cout << "passou da analise sintatica" << std::endl;
    } 
     
    // std::cout << "ao final da analise sintatica temos a da quadrupla, e do código intermediario" << std::endl;
    // std::cout << cod_inter << "1"<< endl;                                                                 //visualiza lista preenchida
    // std::cout << cod_intermediario_arr << "2"<< endl;                                                                 //visualiza lista preenchida
    // std::cout << cod_intermediario_otimi << "3"<< endl;                                                                 //visualiza lista preenchida
    // std::cout << cod_intermediario_otimi_arr << "4"<< endl;                                                                 //visualiza lista preenchida
    // std::cout << cod_intermediario_otimiSTR << "5"<< endl;                                                                 //visualiza lista preenchida
    // std::cout << cod_intermediarioSTR << "6"<< endl;                                                                 //visualiza lista preenchida
    lst_libera(l);


    ///////////////// fase de analise semantica //////////////////
    espaco();
    std::cout << "\t\t**********Inicio da analise semantica**********" << std::endl;
    espaco();
    if(error==0){
        var_declara=lst_cria();
        var_declara=lst_cpy_var(var_declara,l2);

        checa_var_declara(l2);
        lst_libera(var_declara);
        if(error==1) quadrupla_libera(q2);
    }

    ///////////////// fase de geração de código //////////////////

    lst_libera(l2);
    if(error == 0){
        gera_mips2(q2);
        //std::cout << "teste pra saber se chegou aqui" << endl;
        int i = 0;
        quadruplaCharArr = new char[quadruplaSTR.length() + 1];                             // Allocate memory
        while(i < quadruplaSTR.length() + 1){
            quadruplaCharArr[i]=quadruplaSTR[i];
            i++;
        }
        quadruplaCharArr[i]='\0';
        std::cout << quadruplaCharArr  << std::flush;
    }
    quadrupla = quadruplaCharArr;
    std::cout << quadrupla << std::endl; //testando                                                          

    i=0;
    cod_inter = cod_intermediario_arr;                                                      // tem q verificar se ta funcionando!!
    cod_intermediario_arr = new char[cod_intermediarioSTR.length() + 1];                    // Allocate memory
    while(i<cod_intermediarioSTR.length()+ 1){                                              // tem q verificar se ta funcionando!!
        cod_intermediario_arr[i]=cod_intermediarioSTR[i];
        i++;
    }
    cod_intermediario_arr[i]='\0';
    cod_inter = cod_intermediario_arr;                                                      // tem que verificar se ta funcionando!!
    i=0;

    cod_intermediario_otimi_arr = new char[cod_intermediario_otimiSTR.length() + 1];        // Allocate memory
    while(i<cod_intermediario_otimiSTR.length() + 1){
        cod_intermediario_otimi_arr[i]=cod_intermediario_otimiSTR[i];
        i++;
    }
    cod_intermediario_otimi_arr[i]='\0';
    cod_intermediario_otimi = cod_intermediario_otimi_arr;                                  // tem que verificar se ta funcionando!!
    quadrupla_libera(q2);

    if(error==1 || error==2){
        error=0;
        char text[201];
        if(erroSyntax < 2){
            quadrupla = "";
            cod_inter = "";
            cod_intermediario_otimi = "";
            std::cout << erroMsg << endl;
        }
        std::cout << erroMsg << endl;
        strcpy(erroMsg,"");
        while(!pilha_arvore.empty()){
            pilha_arvore.pop();
            pilha_arvore_exp.pop();
        }
        while(!pilha_gen_arvS.empty()){
            pilha_gen_arvS.pop();
            pilha_gen_arvS_exp.pop();
        }
        if(erroSyntax<2 && erroSyntax>0)
            compilar(fileContent);
        else
            erroSyntax=0;
    }
    delete[] data;
    delete[] quadruplaCharArr;
    delete[] cod_intermediario_arr;
    delete[] cod_intermediario_otimi_arr;

}


int main() {
    //variaveis essenciais
    int option, optionvisualizacao;
    std::string filename, fileContent;

    //cabecalho
    std::cout << "\t\tbem vindo ao compilador do Guilherme Bernardo" << std::flush;
    espaco();
    std::cout << "ele eh capaz de aceitar operacoes basicas de aritmetica como + - * /. Tambem suporta o uso de ( e )" << std::endl;
    std::cout << "suas limitacoes sao devidas a um problema na identificacao do automato envolvendo floats, que apesar de acertar mais numeros depois da virgula, so ira apresentar um no output" << std::endl;
    std::cout << "ha tambem uma limitacao no uso de variaveis, que so pode conter 1 caractere" << std::flush;
    espaco();
    espaco();

    //codigo comeca aqui
    std::cout << "Selecione uma opcao:" << std::endl;
    std::cout << "1. Ler arquivo" << std::endl;
    std::cout << "2. fechar aplicacao" << std::endl;
    std::cin >> option;

    switch(option) {
        case 1:{
                std::cout << "digite o nome do arquivo presente no diretorio: "<< std::flush;
                std::cin >> filename;
                fileContent = readFile(filename);                                           //faz a leitura do arquivo selecionado
                if (fileContent == "") return 1;                                            // encerra aplicação com o erro de abertura de arquivo
                espaco();
                espaco();
                std::cout << "Selecione uma opcao:"<< std::endl;                                      //verifica se quer apenas visualizar o conteudo do arquivo
                std::cout << "1. visualizar conteudo do arquivo"<< std::endl;
                std::cout << "2. fechar aplicacao"<< std::endl;
                std::cin >> optionvisualizacao;
                espaco();
                switch (optionvisualizacao){
                case 1: {
                        //std::cout << fileContent << std::endl;                            // Print the file content on the screen
                        espaco();
                        std::cout << "comecando a compilacao do arquivo"<< std::endl;
                        compilar(fileContent);
                        return 0;
                    }
                    case 2:
                        std::cout << "encerrando aplicacao."<< std::endl;
                        return 0;
                default:
                    std::cout << "opcao invalida."<< std::endl;
                    return 1;
                }
                return 1;
            }
        case 2:
            std::cout << "encerrando aplicacao." << std::endl;
            return 0;
        default:
            std::cout << "opcao invalida." << std::endl;
            return 1;
    }

    return 0;
}
