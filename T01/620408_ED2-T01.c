/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 01
 *
 * RA: 620408
 * Aluno: Lucas T. de O. Buzzo
 * ========================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY	11
#define TAM_NOME 		51
#define TAM_MARCA 		51
#define TAM_DATA 		11
#define TAM_ANO 		3
#define TAM_PRECO 		8
#define TAM_DESCONTO 	4
#define TAM_CATEGORIA 	51


#define TAM_REGISTRO 	192
#define MAX_REGISTROS 	1000
#define MAX_CATEGORIAS 	30
#define TAM_ARQUIVO (MAX_REGISTROS * TAM_REGISTRO + 1)


/* Saídas para o usuario */
#define OPCAO_INVALIDA						"Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE			"Memoria insuficiente!"
#define REGISTRO_N_ENCONTRADO			"Registro(s) nao encontrado!\n"
#define CAMPO_INVALIDO						"Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA					"ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO							"Arquivo vazio!"
#define INICIO_BUSCA							"**********************BUSCAR**********************\n"
#define INICIO_LISTAGEM						"**********************LISTAR**********************\n"
#define INICIO_ALTERACAO					"**********************ALTERAR*********************\n"
#define INICIO_EXCLUSAO						"**********************EXCLUIR*********************\n"
#define INICIO_ARQUIVO						"**********************ARQUIVO*********************\n"
#define INICIO_ARQUIVO_SECUNDARIO	"*****************ARQUIVO SECUNDARIO****************\n"
#define SUCESSO										"OPERACAO REALIZADA COM SUCESSO!\n"
#define FALHA											"FALHA AO REALIZAR OPERACAO!\n"



/* Registro do Produto */
typedef struct {
	char pk[TAM_PRIMARY_KEY];
	char nome[TAM_NOME];
	char marca[TAM_MARCA];
	char data[TAM_DATA];	/* DD/MM/AAAA */
	char ano[TAM_ANO];
	char preco[TAM_PRECO];
	char desconto[TAM_DESCONTO];
	char categoria[TAM_CATEGORIA];
} Produto;


/*----- Registros dos Índices -----*/

/* Struct para índice Primário */
typedef struct primary_index{
  char pk[TAM_PRIMARY_KEY];
  int rrn;
} Ip;

/* Struct para índice secundário */
typedef struct secundary_index{
  char pk[TAM_PRIMARY_KEY];
  char string[TAM_NOME];
} Is;

/* Struct para índice secundário de preços */
typedef struct secundary_index_of_final_price{
  float price;
  char pk[TAM_PRIMARY_KEY];
} Isf;

/* Lista ligada para o Índice abaixo*/
typedef struct linked_list{
  char pk[TAM_PRIMARY_KEY];
  struct linked_list *prox;
} ll;

/* Struct para lista invertida */
typedef struct reverse_index{
  char cat[TAM_CATEGORIA];
  ll* lista;
} Ir;

/*----- GLOBAL -----*/
char ARQUIVO[TAM_ARQUIVO];

/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo();

/* Exibe o Produto */
int exibir_registro(int rrn, char com_desconto);

/* Recupera do arquivo o registro com o rrn informado
 *  e retorna os dados na struct Produto */
Produto recuperar_registro(int rrn);

/* (Re)faz o índice respectivo */
void criar_iprimary(Ip *indice_primario, int* nregistros);

/* Realiza os scanfs na struct Produto */
void ler_entrada(char* registro, Produto *novo);

/* Rotina para impressao de indice secundario */
void imprimirSecundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat);

//Gerar a chave primária do registro
void gerarChave(Produto *prod);

/*
		Função para calcular o novo preço, depois de aplicar desconto
		Retorna o preço (float) com o valor arredodado (apos aplicar o desconto)
*/
float arredondamento(float preco, float desconto);

/*
		Função para inserir um novo Produto
*/
int inserirProduto(int *num, Ip *indice, Is *iproduct, Is *ibrand, Isf *iprice, Ir *icategory, int numCategorias);

/*
		Função para contar quantos registros há no ARQUIVO
		Retorna a quantidade (int) de produtos no ARQUIVO
*/
int quantidadeRegistros();

/*
		Função para ler o nome do produto (do standart input para ram)
		Retorna o nome do produto lido
*/
char *lerNomeProduto();

/*
		Função para ler a marca do produto (do standart input para ram)
		Retorna a marca do produto lido
*/
char *lerMarcaProduto();

/*
		Função para ler a data de registro do produto (do standart input para ram)
		Retorna a data de registro do produto lido
*/
char *lerDataProduto();

/*
		Função para ler o ano de lançamento do produto (do standart input para ram)
		Retorna o ano de lançamento do produto lido
*/
char *lerAnoProduto();

/*
		Função para ler o preço do produto (do standart input para ram)
		Retorna o preço do produto lido
*/
char *lerPrecoProduto();

/*
		Função para ler o desconto no preço do produto (do standart input para ram)
		Retorna o desconto no preço do produto lido
*/
char *lerDescontoProduto();

/*
		Função para ler a(s) categoria(s) do produto (do standart input para ram)
		Retorna a(s) categoria(s) do produto lido
*/
char *lerCategoriaProduto();

/*
		Recebe um Produto como parâmetro e grava no arquivo
*/
void gravarNoArquivo(Produto * prod, Ip *indice, int *num);

/*
		Função para comparação de dois valores da Struct
*/
int comparar_iprimary(const void *a, const void *b);

/*
		Função para ordernar o iprimary
*/
void ordenar_iprimary(Ip *indice_primario, int* nregistros);

/*
		Função para comparar o indice primario com uma string
*/
int comparar_iprimary_str(const void *a, const void *b);

/*
		Função para busca binária no indice primario
*/
Ip *bb_primaria(char *chave, Ip *indice, int *nregistros);

/*
		Função para comparar os indices secundarios
*/
int comparar_secondary(const void *a, const void *b);

/*
		Função para ordernar o iproduct
*/
void ordenar_secondary(Is *secondary, int *nregistros, int caso);

/*
		Função para criar indice secundário
*/
void criar_secondary(Is *secondary, int *nregistros, int caso);

/*
		Função para adicionar produto ao iprimary
*/
void addIprimary(Produto *prod, Ip *indice, int num);

/*
		Função para adicionar produto ao isecondary
*/
void addSecondary(Produto *prod, Is *indice, int caso, int num);

/*
		Função para comparar os indices secundarios (iprice)
*/
int comparar_secondary_price(const void *a, const void *b);

/*
		Função para ordernar o iproduct
*/
void ordenar_secondary_price(Isf *secondary, int *nregistros);

/*
		Função para criar indice secundário (iprice)
*/
void criar_secondary_price(Isf *secondary, int *nregistros);

/*
		Função para adicionar produto ao isecondary (iprice)
*/
void addSecondary_price(Produto *prod, Isf *indice, int num);

/*
		Função para busca binária por categoria
*/
Ir *bb_categoria(char *chave, Ir *indice, int *nregistros);

/*
		Função para comparar categoria com string
*/
int comparar_categoria_str(const void *a, const void *b);

/*
		Função para criar o icategory
		retorna o número de categorias
*/
int criar_secondary_cat(Ir *secondary, int *nregistros);

/*
		Função para ordenar icategory
*/
void ordenar_categoria(Ir *secondary, int *nregistros);

/*
		Função para comparar o icategories
*/
int comparar_icategory(const void *a, const void *b);

/*
		Conjunto de funções para executar o merge sort (log.n) na orderação das PKs na categoria
		Base: https://www.geeksforgeeks.org/merge-sort-for-linked-list/
*/
void MergeSort(ll** head);
void FrontBackSplit(ll* source, ll** frontRef, ll** backRef);
ll* SortedMerge(ll* a, ll* b);

/*
		Função para adicionar o produto ao icategory
*/
int addSecondary_cat(Produto *prod, Ir *indice, int numCategorias);

/*
		Função para alterar um produto
*/
int alterar(Ip *iprimary, int nregistros, Isf *iprice);

/*
		Função para remover um produto
*/
int remover(Ip *iprimary, int *nregistros);

/*
		Função para comparar um indice secundário com string
*/
int comparar_secondary_str(const void *a, const void *b);

/*
		Função para buscar nos indices secundários
*/
Is *bb_secundario(char *chave, Is *indice, int *nregistros);

/*
		Função para buscar um produto
*/
void buscar(Ip *iprimary, Is *iproduct, Is *ibrand, Ir *icategory, int *nregistros, int numcat);

/*
		Função para listagem de produtos
*/
void listagem(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, int *nregistros, int ncat);

/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main(){
  /* Arquivo */
	int carregarArquivo = 0, nregistros = 0, ncat = 0;
	scanf("%d%*c", &carregarArquivo); /* 1 (sim) | 0 (nao) */
	if (carregarArquivo)
		nregistros = carregar_arquivo();
	//printf("Num = %d\n", nregistros);
	/* Índice primário */
	Ip *iprimary = (Ip *) malloc (MAX_REGISTROS * sizeof(Ip));

	if (!iprimary) {
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iprimary(iprimary, &nregistros);

	/*Alocar e criar índices secundários*/

	Is *iproduct = (Is *) malloc (MAX_REGISTROS * sizeof(Is));
	if (carregarArquivo)
		criar_secondary(iproduct, &nregistros, 0);

	Is *ibrand = (Is *) malloc (MAX_REGISTROS * sizeof(Is));
	if (carregarArquivo)
		criar_secondary(ibrand, &nregistros, 1);

	Ir *icategory = (Ir *) malloc (MAX_REGISTROS * sizeof(Ir));
	if (carregarArquivo)
		ncat = criar_secondary_cat(icategory, &nregistros);

	Isf *iprice = (Isf *) malloc (MAX_REGISTROS * sizeof(Isf));
	if (carregarArquivo)
		criar_secondary_price(iprice, &nregistros);

	/*
		Debug to 9
		int count = 0;
	*/
	/* Execução do programa */
	int opcao = 0;
	int resultado = 0;
	while(1)
	{
		scanf("%d%*c", &opcao);
		switch(opcao)
		{
			case 1:
				ncat = inserirProduto(&nregistros, iprimary, iproduct, ibrand, iprice, icategory, ncat);
			break;
			case 2:
				/*alterar desconto*/
				printf(INICIO_ALTERACAO);
				resultado = (alterar(iprimary, nregistros, iprice) == 1);
				if (resultado == 1)
					printf(SUCESSO);
				else if (resultado == 0)
					printf(FALHA);
			break;
			case 3:
				/*excluir produto*/
				printf(INICIO_EXCLUSAO);
				resultado = remover(iprimary, &nregistros);
				if(resultado == 1)
					printf(SUCESSO);
				else if (resultado == 0)
					printf(FALHA);
			break;
			case 4:
				/*busca*/
				printf(INICIO_BUSCA);
				buscar(iprimary, iproduct, ibrand, icategory, &nregistros, ncat);
			break;
			case 5:
				/*listagens*/
				printf(INICIO_LISTAGEM);
				listagem(iprimary, icategory, ibrand, iprice, &nregistros, ncat);
			break;
			case 6:
				/*libera espaço*/
			break;
			case 7:
				/*imprime o arquivo de dados*/
				printf(INICIO_ARQUIVO);
				printf("%s\n", ARQUIVO);
			break;
			case 8:
				/*imprime os índices secundários*/
				imprimirSecundario(iproduct, ibrand, icategory, iprice, nregistros, ncat);
			break;
			case 9:
				/*
					Debug 9
					for (count = 0; count < nregistros; count++) {
						printf("%s -> %d\n", iprimary[count].pk, iprimary[count].rrn);
					}
				*/
	      		/*Liberar memória e finalizar o programa */
				return 0;
			break;
			default:
				printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}


/* Exibe o Produto */
int exibir_registro(int rrn, char com_desconto) {
	if(rrn<0)
		return 0;
	float preco;
	int desconto;
	Produto j = recuperar_registro(rrn);
  	char *cat, categorias[TAM_CATEGORIA];
	printf("%s\n", j.pk);
	printf("%s\n", j.nome);
	printf("%s\n", j.marca);
	printf("%s\n", j.data);
	if(!com_desconto)
	{
		printf("%s\n", j.preco);
		printf("%s\n", j.desconto);
	}
	else
	{
		sscanf(j.desconto,"%d",&desconto);
		sscanf(j.preco,"%f",&preco);
		preco = preco *  (100-desconto);
		preco = ((int) preco)/ (float) 100 ;
		printf("%07.2f\n",  preco);

	}
	strcpy(categorias, j.categoria);

	cat = strtok (categorias, "|");

	while(cat != NULL){
		printf("%s", cat);
		cat = strtok (NULL, "|");
		if(cat != NULL){
			printf(", ");
		}
	}

	printf("\n");

	return 1;
}


int quantidadeRegistros() {
	return strlen(ARQUIVO) / TAM_REGISTRO;
}

int carregar_arquivo() {
	scanf("%[^\n]\n", ARQUIVO);
	return quantidadeRegistros();
}


/* Recupera do arquivo o registro com o rrn
 * informado e retorna os dados na struct Produto */
Produto recuperar_registro(int rrn) {
	char temp[193], *p;
	strncpy(temp, ARQUIVO + ((rrn)*192), 192);
	temp[192] = '\0';
	Produto j;
	p = strtok(temp,"@");
	strcpy(j.nome,p);
	p = strtok(NULL,"@");
	strcpy(j.marca,p);
	p = strtok(NULL,"@");
	strcpy(j.data,p);
	p = strtok(NULL,"@");
	strcpy(j.ano,p);
	p = strtok(NULL,"@");
	strcpy(j.preco,p);
	p = strtok(NULL,"@");
	strcpy(j.desconto,p);
	p = strtok(NULL,"@");
	strcpy(j.categoria,p);
	gerarChave(&j);
	return j;
}


/* Imprimir indices secundarios */
void imprimirSecundario(Is* iproduct, Is* ibrand, Ir* icategory, Isf *iprice, int nregistros, int ncat){
	int opPrint = 0;
	ll *aux;
	printf(INICIO_ARQUIVO_SECUNDARIO);
	scanf("%d", &opPrint);
	if(!nregistros)
		printf(ARQUIVO_VAZIO);
	switch (opPrint) {
		case 1:
			for(int i = 0; i < nregistros; i++){
				printf("%s %s\n",iproduct[i].pk, iproduct[i].string);
			}
		break;
		case 2:
			for(int i = 0; i < nregistros; i++){
				printf("%s %s\n",ibrand[i].pk, ibrand[i].string);
			}
		break;
		case 3:
			for(int i = 0; i < ncat; i++){
				printf("%s", icategory[i].cat);
				aux =  icategory[i].lista;
				while(aux != NULL){
					printf(" %s", aux->pk);
					aux = aux->prox;
				}
				printf("\n");
			}
		break;

		case 4:
		for(int i = 0; i < nregistros; i++){
			printf("%s %.2f\n",iprice[i].pk, iprice[i].price);
		}
		break;
	}
}

//Gera a chave primária
void gerarChave(Produto *prod) {
	char *chave_temp = (char *) malloc(TAM_PRIMARY_KEY * sizeof(char));
	char *chave = (char *) malloc(TAM_PRIMARY_KEY * sizeof(char));
	int count = 0;

	//GE
	strncpy(chave_temp, prod->nome, 2);
	strcat(chave, chave_temp);

	//GEMS
	strncpy(chave_temp, prod->marca, 2);
	strcat(chave, chave_temp);

	//GEMS1109
	chave[4] = prod->data[0];
	chave[5] = prod->data[1];
	chave[6] = prod->data[3];
	chave[7] = prod->data[4];

	//GEMS110917
	strncpy(chave_temp, prod->ano, 2);
	strcat(chave, chave_temp);

	//Tornar canônica
	for (count = 0; count < strlen(chave_temp); count++)
		chave_temp[count] = toupper(chave_temp[count]);

	strcpy(prod->pk, chave);

}

//Retorna o preço com o valor arredodado (após aplicar o desconto)
float arredondamento(float preco, float desconto) {
  preco = preco *  (100-desconto);
  preco = ((int) preco)/ (float) 100;

  return preco;
}

int inserirProduto(int *num, Ip *indice, Is *iproduct, Is *ibrand, Isf *iprice, Ir *icategory, int numCategorias) {
	Produto *prod = (Produto *) malloc(sizeof(Produto));

	strcpy(prod->nome, lerNomeProduto());
	strcpy(prod->marca, lerMarcaProduto());
	strcpy(prod->data, lerDataProduto());
	strcpy(prod->ano, lerAnoProduto());
	strcpy(prod->preco, lerPrecoProduto());
	strcpy(prod->desconto, lerDescontoProduto());
	strcpy(prod->categoria, lerCategoriaProduto());

	gerarChave(prod);


	if (bb_primaria(prod->pk, indice, num) != NULL)
		printf(ERRO_PK_REPETIDA, prod->pk);
	else {
		gravarNoArquivo(prod, indice, num);

		addIprimary(prod, indice, *num);
		addSecondary(prod, iproduct, 0, *num);
		addSecondary(prod, ibrand, 1, *num);
		addSecondary_price(prod, iprice, *num);
		int totalCategorias = addSecondary_cat(prod, icategory, numCategorias);
		*num = *num + 1;

		//Ordena o iprimary
		ordenar_iprimary(indice, num);

		ordenar_secondary(iproduct, num, 0);
		ordenar_secondary(ibrand, num, 1);
		ordenar_secondary_price(iprice, num);

		return totalCategorias;
	}
	return numCategorias;
}

void addIprimary(Produto *prod, Ip *indice, int num) {
	//Gravar no final do iprimary
	Ip *novoIndice = (Ip *) malloc(sizeof(Ip));
	strcpy(novoIndice->pk, prod->pk);
	//printf("%d\n", *num);
	novoIndice->rrn = (num);
	indice[num] = *novoIndice;
}

void addSecondary(Produto *prod, Is *indice, int caso, int num) {
	//Gravar no final do isecondary
	Is *novoIndice = (Is *) malloc(sizeof(Is));
	if (caso == 0)
		strcpy(novoIndice->string, prod->nome);
	else if (caso == 1)
		strcpy(novoIndice->string, prod->marca);

	strcpy(novoIndice->pk, prod->pk);

	indice[num] = *novoIndice;
}

void gravarNoArquivo(Produto * prod, Ip *indice, int *num) {
	int tamanho = 0;

	tamanho+=strlen(prod->nome);
	sprintf(ARQUIVO+strlen(ARQUIVO), "%s", prod->nome);

	tamanho+=(strlen(prod->marca)+1);
	sprintf(ARQUIVO+strlen(ARQUIVO), "@%s", prod->marca);

	tamanho+=(strlen(prod->data)+1);
	sprintf(ARQUIVO+strlen(ARQUIVO), "@%s", prod->data);

	tamanho+=(strlen(prod->ano)+1);
	sprintf(ARQUIVO+strlen(ARQUIVO), "@%s", prod->ano);

	tamanho+=(strlen(prod->preco)+1);
	sprintf(ARQUIVO+strlen(ARQUIVO), "@%s", prod->preco);

	tamanho+=(strlen(prod->desconto)+1);
	sprintf(ARQUIVO+strlen(ARQUIVO), "@%s", prod->desconto);

	tamanho+=(strlen(prod->categoria)+1);
	sprintf(ARQUIVO+strlen(ARQUIVO), "@%s@", prod->categoria);

	while (tamanho < 191) {
		sprintf(ARQUIVO+strlen(ARQUIVO), "#");
		tamanho++;
	}
}

char *lerNomeProduto() {
	char *nome = (char *) malloc(TAM_NOME * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(nome, ch);
	}

	//nome = getline(stdin);

	return nome;
}

char *lerMarcaProduto() {
	char *marca = (char *) malloc(TAM_MARCA * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(marca, ch);
	}

	//marca = getline(stdin);

	return marca;
}

char *lerDataProduto() {
	char *data = (char *) malloc(TAM_DATA * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(data, ch);
	}

	//data = getline(stdin);

	return data;
}

char *lerAnoProduto() {
	char *ano = (char *) malloc(TAM_ANO * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(ano, ch);
	}

	//ano = getline(stdin);

	return ano;
}

char *lerPrecoProduto() {
	char *preco = (char *) malloc(TAM_PRECO * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(preco, ch);
	}

	//preco = getline(stdin);

	return preco;
}

char *lerDescontoProduto() {
	char *desconto = (char *) malloc(TAM_DESCONTO * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(desconto, ch);
	}

	//desconto = getline(stdin);

	return desconto;
}

char *lerCategoriaProduto() {
	char *categoria = (char *) malloc(TAM_CATEGORIA * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(categoria, ch);
	}

	//categoria = getline(stdin);

	return categoria;
}

void criar_iprimary(Ip *indice_primario, int* nregistros) {
	//printf("Num = %d\n", *nregistros);
	if (*nregistros > 0) {
		int count = 0;
		Produto temp;
		for (; count < *nregistros; count++) {
			temp = recuperar_registro(count);

			//Gravar no final do iprimary
			Ip *novoIndice = (Ip *) malloc(sizeof(Ip));
			strcpy(novoIndice->pk, temp.pk);
			novoIndice->rrn = (count);
			//printf("%s\n", temp.pk);
			indice_primario[count] = *novoIndice;
		}
		ordenar_iprimary(indice_primario, nregistros);

		/*
			Debug do iprimary
			for (count = 0; count < *nregistros; count++) {
				printf("%s -> %d\n", indice_primario[count].pk, indice_primario[count].rrn);
			}
	*/
	}
}

Is *bb_secundario(char *chave, Is *indice, int *nregistros) {
	return (Is *) bsearch(chave, indice, *nregistros, sizeof(indice[0]), comparar_secondary_str);
}

Ip *bb_primaria(char *chave, Ip *indice, int *nregistros) {
	return (Ip *) bsearch(chave, indice, *nregistros, sizeof(indice[0]), comparar_iprimary_str);
}

void ordenar_iprimary(Ip *indice_primario, int* nregistros) {
	if (*nregistros > 0)
		qsort(indice_primario, *nregistros, sizeof(Ip), comparar_iprimary);
}

int comparar_iprimary(const void *a, const void *b) {
	const Ip *ia = (Ip *)a;
	const Ip *ib = (Ip *)b;
	//printf("%s vs %s == %d\n", ia->pk, ib->pk, strcmp(ia->pk, ib->pk));
	return strncmp(ia->pk, ib->pk, TAM_PRIMARY_KEY);
}

int comparar_iprimary_str(const void *a, const void *b) {
	const char *ia = (char *)a;
	const Ip *ib = (Ip *)b;

	//printf("%s vs %s == %d\n", ia, ib->pk, strcmp(ia, ib->pk));

	return strncmp(ia, ib->pk, TAM_PRIMARY_KEY);
}

void ordenar_secondary(Is *secondary, int* nregistros, int caso) {
	if (*nregistros > 0)
		qsort(secondary, *nregistros, sizeof(Is), comparar_secondary);
}

int comparar_secondary(const void *a, const void *b) {
	const Is *ia = (Is *)a;
	const Is *ib = (Is *)b;
	//printf("%s vs %s == %d\n", ia->pk, ib->pk, strcmp(ia->pk, ib->pk));
	int retorno = strncmp(ia->string, ib->string, TAM_NOME);

	if (retorno == 0)
		return strncmp(ia->pk, ib->pk, TAM_PRIMARY_KEY);
	return retorno;
}

int comparar_secondary_str(const void *a, const void *b) {
	const char *ia = (char *)a;
	const Is *ib = (Is *)b;
	//printf("%s vs %s == %d\n", ia->pk, ib->pk, strcmp(ia->pk, ib->pk));
	int retorno = strncmp(ia, ib->string, TAM_NOME);
}

void criar_secondary(Is *secondary, int *nregistros, int caso) {
	if (*nregistros > 0) {
		int count = 0;
		Produto temp;

		if (caso == 0) {
		//iproduct
			for (; count < *nregistros; count++) {
				temp = recuperar_registro(count);

				//Gravar no final do iproduct
				Is *novoIndice = (Is *) malloc(sizeof(Is));
				strcpy(novoIndice->string, temp.nome);
				strcpy(novoIndice->pk, temp.pk);
				//printf("%s\n", temp.pk);
				secondary[count] = *novoIndice;
			}
			ordenar_secondary(secondary, nregistros, 0);
		} else if (caso == 1) {
			//ibrand
			for (; count < *nregistros; count++) {
				temp = recuperar_registro(count);

				//Gravar no final do ibrand
				Is *novoIndice = (Is *) malloc(sizeof(Is));
				strcpy(novoIndice->string, temp.marca);
				strcpy(novoIndice->pk, temp.pk);
				//printf("%s\n", temp.pk);
				secondary[count] = *novoIndice;
			}
			ordenar_secondary(secondary, nregistros, 1);
		}
	}
}

int comparar_secondary_price(const void *a, const void *b) {
	const Isf *ia = (Isf *)a;
	const Isf *ib = (Isf *)b;

	return (ia->price - ib->price);
}

void ordenar_secondary_price(Isf *secondary, int *nregistros) {
	if (*nregistros > 0)
		qsort(secondary, *nregistros, sizeof(Isf), comparar_secondary_price);
}

void criar_secondary_price(Isf *secondary, int *nregistros){
	if (*nregistros > 0) {
		int count = 0;
		Produto temp;

		for (; count < *nregistros; count++) {
			temp = recuperar_registro(count);

			//Gravar no final do iprice
			Isf *novoIndice = (Isf *) malloc(sizeof(Isf));
			novoIndice->price = (arredondamento((float) strtof(temp.preco, NULL), (float) strtof(temp.desconto, NULL)));
			strcpy(novoIndice->pk, temp.pk);
			//printf("%s\n", temp.pk);
			secondary[count] = *novoIndice;
		}
		ordenar_secondary_price(secondary, nregistros);
	}
}

void addSecondary_price(Produto *prod, Isf *indice, int num) {
	//Gravar no final do iprice
	Isf *novoIndice = (Isf *) malloc(sizeof(Isf));
	novoIndice->price = (arredondamento((float) strtof(prod->preco, NULL), (float) strtof(prod->desconto, NULL)));
	strcpy(novoIndice->pk, prod->pk);

	indice[num] = *novoIndice;
}

int addSecondary_cat(Produto *prod, Ir *indice, int numCategorias) {
	int count = 0;
	Produto temp;
	temp = *prod;

	int numProdutos = 0;

	char categoria_produto[TAM_CATEGORIA];
	char *token;

	int localNumCategorias = numCategorias;

	Ir *nova_categoria = NULL;

	strcpy(categoria_produto, temp.categoria);

	token = strtok(categoria_produto, "|");

	while (token != NULL) {
		nova_categoria = bb_categoria(token, indice, &localNumCategorias);
		if (nova_categoria == NULL) {
			//Gravar no final do icategory
			Ir *novoIndice = (Ir *) malloc(sizeof(Ir));
			strcpy(novoIndice->cat, token);

			//Criar a lista
			novoIndice->lista = (ll *) malloc(sizeof(ll));
			strcpy(novoIndice->lista->pk, temp.pk);
			novoIndice->lista->prox = NULL;

			indice[localNumCategorias] = *novoIndice;
			localNumCategorias++;
		} else {
			numProdutos = 0;
			ll *lista_aux = nova_categoria->lista;

			while (lista_aux->prox != NULL) {
				lista_aux = lista_aux->prox;
				numProdutos++;
			}

			ll *novo_no = (ll *) malloc(sizeof(ll));
			strcpy(novo_no->pk, temp.pk);
			novo_no->prox = NULL;

			lista_aux->prox = novo_no;

			MergeSort(&(nova_categoria->lista));

		}
		ordenar_categoria(indice, &localNumCategorias);
		token = strtok(NULL, "|");
	}
	return localNumCategorias;
}

int criar_secondary_cat(Ir *secondary, int *nregistros) {
	if (*nregistros > 0) {
		int count = 0;
		Produto temp;

		int numCategorias = 0, numProdutos = 0;

		char categoria_produto[TAM_CATEGORIA];
		char *token;

		Ir *nova_categoria = NULL;

		for (; count < *nregistros; count++) {
			temp = recuperar_registro(count);

			strcpy(categoria_produto, temp.categoria);

			token = strtok(categoria_produto, "|");

			while (token != NULL) {
				nova_categoria = bb_categoria(token, secondary, &numCategorias);
				if (nova_categoria == NULL) {
					//Gravar no final do icategory
					Ir *novoIndice = (Ir *) malloc(sizeof(Ir));
					strcpy(novoIndice->cat, token);

					//Criar a lista
					novoIndice->lista = (ll *) malloc(sizeof(ll));
					strcpy(novoIndice->lista->pk, temp.pk);
					novoIndice->lista->prox = NULL;

					secondary[numCategorias] = *novoIndice;
					numCategorias++;
				} else {
					numProdutos = 0;
					ll *lista_aux = nova_categoria->lista;

					while (lista_aux->prox != NULL) {
						lista_aux = lista_aux->prox;
						numProdutos++;
					}

					ll *novo_no = (ll *) malloc(sizeof(ll));
					strcpy(novo_no->pk, temp.pk);
					novo_no->prox = NULL;

					lista_aux->prox = novo_no;

					MergeSort(&(nova_categoria->lista));

				}
				ordenar_categoria(secondary, &numCategorias);
				token = strtok(NULL, "|");
			}
		}
		return numCategorias;
	}
	return 0;
}

Ir *bb_categoria(char *chave, Ir *indice, int *nregistros) {
	return (Ir *) bsearch(chave, indice, *nregistros, sizeof(indice[0]), comparar_categoria_str);
}

int comparar_categoria_str(const void *a, const void *b) {
	const char *ia = (char *)a;
	const Ir *ib = (Ir *)b;

	return strcmp(ia, ib->cat);
}

void ordenar_categoria(Ir *secondary, int *nregistros) {
	if (*nregistros > 1)
		qsort(secondary, *nregistros, sizeof(Ir), comparar_icategory);
}

int comparar_icategory(const void *a, const void *b) {
	const Ir *ia = (Ir *)a;
	const Ir *ib = (Ir *)b;
	//printf("%s vs %s == %d\n", ia->pk, ib->pk, strcmp(ia->pk, ib->pk));
	return strcmp(ia->cat, ib->cat);
}

ll* SortedMerge(ll* a, ll* b) {
	if (a == NULL)
		return b;

	else if (b == NULL)
		return a;

	ll* result = NULL;

	if (strcmp(a->pk, b->pk) < 0) {
		result = a;
		result->prox = SortedMerge(a->prox, b);
	}
	else {
		result = b;
		result->prox = SortedMerge(a, b->prox);
	}

	return result;
}

void FrontBackSplit(ll* source, ll** frontRef, ll** backRef) {
	// if length is less than 2, handle separately
	if (source == NULL || source->prox == NULL) {
		*frontRef = source;
		*backRef = NULL;
		return;
	}

	ll* slow = source;
	ll* fast = source->prox;

	while (fast != NULL) {
		fast = fast->prox;
		if (fast != NULL) {
			slow = slow->prox;
			fast = fast->prox;
		}
	}

	*frontRef = source;
	*backRef = slow->prox;
	slow->prox = NULL;
}

void MergeSort(ll** head) {
	if (*head == NULL || (*head)->prox == NULL)
		return;

	ll* a;
	ll* b;

	FrontBackSplit(*head, &a, &b);

	MergeSort(&a);
	MergeSort(&b);

	*head = SortedMerge(a, b);
}


int alterar(Ip *iprimary, int nregistros, Isf *iprice) {
	char *chavinha = (char *) malloc(TAM_PRIMARY_KEY * sizeof(char));

	char *descontinho = (char *) malloc(TAM_DESCONTO * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(chavinha, ch);
	}

	Ip * indice = bb_primaria(chavinha, iprimary, &nregistros);

	if (indice == NULL) {
		printf(REGISTRO_N_ENCONTRADO);
		return -1;
	}
	int rrn = indice->rrn;
	Produto prod = recuperar_registro(rrn);

	do {
		strcpy(descontinho, "");
		while ((*ch = getchar()) != '\n') {
			strcat(descontinho, ch);
		}
		if (strlen(descontinho) < 3 || (strtol(descontinho, NULL, 10) > 100) || (strtol(descontinho, NULL, 10) < 0)) {
			printf(CAMPO_INVALIDO);
			strcpy(descontinho, "");
		}
	} while (strlen(descontinho) < 3 || (strtol(descontinho, NULL, 10) > 100) || (strtol(descontinho, NULL, 10) < 0)) ;

	if (&prod != NULL) {
		int movimentacao = ((192 * rrn) + strlen(prod.nome) + strlen(prod.marca) + strlen(prod.data) + strlen(prod.ano) + strlen(prod.preco) + 5);
		char *p = ARQUIVO + movimentacao;
		sprintf(p, "%s", descontinho);
		ARQUIVO[movimentacao+3] = '@';
		criar_secondary_price(iprice, &nregistros);
		return 1;
	}
	return 0;
}

int remover(Ip *iprimary, int *nregistros) {
	char *chavinha = (char *) malloc(TAM_PRIMARY_KEY * sizeof(char));

	char *ch = (char *) malloc(sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(chavinha, ch);
	}

	Ip * indice = bb_primaria(chavinha, iprimary, nregistros);

	if (indice == NULL) {
		printf(REGISTRO_N_ENCONTRADO);
		return -1;
	}
	int rrn = indice->rrn;
	Produto prod = recuperar_registro(rrn);

	if (&prod != NULL) {
		int movimentacao = (192 * rrn);
		char *p = ARQUIVO + movimentacao;
		sprintf(p, "*|");
		ARQUIVO[movimentacao+2] = '@';

		strncpy(indice->pk, "", TAM_PRIMARY_KEY);
		strcpy(indice->pk, "-1");
		*nregistros--;
		//TODO: Recriar todos os indices

		ordenar_iprimary(iprimary, nregistros);

		return 1;
	}
	return 0;
}

void buscar(Ip *iprimary, Is *iproduct, Is *ibrand, Ir *icategory, int *nregistros, int ncat) {
	int opPrint = 0;
	ll *aux;
	char codProduto[TAM_PRIMARY_KEY];
	char tempCodigo[TAM_PRIMARY_KEY];

	char nomeProduto[TAM_NOME];
	char tempNome[TAM_NOME];

	char marcaProduto[TAM_MARCA];
	char tempMarca[TAM_MARCA];

	char categoriaProduto[TAM_CATEGORIA];
	char tempCategoria[TAM_CATEGORIA];

	char *ch = (char *) malloc(sizeof(char));

	scanf("%d%*c", &opPrint);

	int count = 0, count2 = 0;
	Ip *busca;
	Is *buscaParcial;

	Ir *buscaParcialMarca;

	Produto resultados[MAX_REGISTROS];
	int numResultados = 0;

	switch (opPrint) {
		//TODO: não imprimir os removidos (pk == 1)
		case 1:

			while ((*ch = getchar()) != '\n') {
				strcat(tempCodigo, ch);
			}

			strcpy(codProduto, tempCodigo);

			busca = bb_primaria(codProduto, iprimary, nregistros);

			if (busca == NULL || (strcmp(busca->pk, "-1") == 0)) {
				printf(REGISTRO_N_ENCONTRADO);
				return;
			} else {
				Produto prod = recuperar_registro(busca->rrn);

				printf("%s\n", prod.pk);
				printf("%s\n", prod.nome);
				printf("%s\n", prod.marca);
				printf("%s\n", prod.data);
				//printf("%s\n", prod.ano);
				printf("%s\n", prod.preco);
				printf("%s\n", prod.desconto);

				char categoria[TAM_CATEGORIA];
				strcpy(categoria, prod.categoria);

				for (count = 0; count < strlen(prod.categoria); count++) {
					if (prod.categoria[count] == '|')
						printf(", ");
					else
						printf("%c", prod.categoria[count]);
				}
				printf("\n");

			}
			return;

			break;
		case 2:
			//TODO: quando há mais que um produto com o mesmo nome
			strcpy(tempNome, "");
			while ((*ch = getchar()) != '\n') {
				strcat(tempNome, ch);
			}

			strcpy(nomeProduto, tempNome);

			buscaParcial = bb_secundario(nomeProduto, iproduct, nregistros);
			if (buscaParcial == NULL)	{
				printf(REGISTRO_N_ENCONTRADO);
				return;
			} else {
				busca = bb_primaria(buscaParcial->pk, iprimary, nregistros);
				if (busca == NULL || (strcmp(busca->pk, "-1") == 0)) {
					printf(REGISTRO_N_ENCONTRADO);
					return;
				} else {
					Produto prod = recuperar_registro(busca->rrn);

					printf("%s\n", prod.pk);
					printf("%s\n", prod.nome);
					printf("%s\n", prod.marca);
					printf("%s\n", prod.data);
					//printf("%s\n", prod.ano);
					printf("%s\n", prod.preco);
					printf("%s\n", prod.desconto);

					char categoria[TAM_CATEGORIA];
					strcpy(categoria, prod.categoria);

					for (count = 0; count < strlen(prod.categoria); count++) {
						if (prod.categoria[count] == '|')
							printf(", ");
						else
							printf("%c", prod.categoria[count]);
					}
					printf("\n");
				}
				return;
			}

			break;
		case 3:
			strcpy(tempMarca, "");
			while ((*ch = getchar()) != '\n') {
				strcat(tempMarca, ch);
			}

			strcpy(marcaProduto, tempMarca);

			//categoria
			strcpy(tempCategoria, "");
			while ((*ch = getchar()) != '\n') {
				strcat(tempCategoria, ch);
			}

			strcpy(categoriaProduto, tempCategoria);

			//Busca pela marca
			for (count = 0; count < *nregistros; count++) {
				if (strcmp(ibrand[count].string, marcaProduto) == 0) {
					busca = bb_primaria(ibrand[count].pk, iprimary, nregistros);
					if (busca != NULL) {
						if (strcmp(busca->pk, "-1") != 0) {
							resultados[numResultados] = recuperar_registro(busca->rrn);
							numResultados++;
						}
					}
					//printf("achei\n");
				}
			}

			if (numResultados == 0) {
				printf(REGISTRO_N_ENCONTRADO);
				return;
			} else {

				//Busca binária para achar a categoria
				buscaParcialMarca = bb_categoria(tempCategoria, icategory, &ncat);
				if (buscaParcialMarca == NULL) {
					printf(REGISTRO_N_ENCONTRADO);
					return;
				} else {
					//printf("%s", buscaParcialMarca->cat);

					aux =  buscaParcialMarca->lista;
					while(aux != NULL){
						for (count = 0; count < numResultados; count++) {
							if (strcmp(resultados[count].pk, aux->pk) == 0) {
								printf("%s\n", resultados[count].pk);
								printf("%s\n", resultados[count].nome);
								printf("%s\n", resultados[count].marca);
								printf("%s\n", resultados[count].data);
								//printf("%s\n", prod.ano);
								printf("%s\n", resultados[count].preco);
								printf("%s\n", resultados[count].desconto);

								char categoria2[TAM_CATEGORIA];
								strcpy(categoria2, resultados[count].categoria);

								for (count2 = 0; count2 < strlen(resultados[count].categoria); count2++) {
									if (resultados[count].categoria[count2] == '|')
										printf(", ");
									else
										printf("%c", resultados[count].categoria[count2]);
								}
								printf("\n");
							}
						}
						aux = aux->prox;
					}
				}

			}
			return;

			break;
	}
}

void listagem(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, int *nregistros, int ncat) {

	char *ch = (char *) malloc(sizeof(char));

	int opPrint = 0;
	int count = 0, count2 = 0;
	int numProdutos = 0;
	Produto prod;

	char categoriaProduto[TAM_CATEGORIA];
	char tempCategoria[TAM_CATEGORIA];

	Ir *buscaParcialCategoria;
	ll *aux;

	Ip *primaria;
	Produto resultados[MAX_REGISTROS];
	Produto resultado;
	int numResultados = 0;

	char marcaProduto[TAM_MARCA];
	char tempMarca[TAM_MARCA];

	Isf *auxiliarPreco;

	scanf("%d%*c", &opPrint);
	switch (opPrint) {
		case 1:
			for (count = 0; count < *nregistros; count++) {
				//printf("%s\n", iprimary[count].pk);
				if (strcmp(iprimary[count].pk, "-1") != 0) {
					numProdutos++;
					prod = recuperar_registro(iprimary[count].rrn);
					printf("%s\n", prod.pk);
					printf("%s\n", prod.nome);
					printf("%s\n", prod.marca);
					printf("%s\n", prod.data);
					//printf("%s\n", prod.ano);
					printf("%s\n", prod.preco);
					printf("%s\n", prod.desconto);

					char categoria[TAM_CATEGORIA];
					strcpy(categoria, prod.categoria);

					for (count2 = 0; count2 < strlen(prod.categoria); count2++) {
						if (prod.categoria[count2] == '|')
							printf(", ");
						else
							printf("%c", prod.categoria[count2]);
					}
					printf("\n\n");
				}
			}
			if (numProdutos == 0) {
				printf(REGISTRO_N_ENCONTRADO);
				return;
			}

			break;
		case 2:
			numProdutos = 0;
			//categoria
			strcpy(tempCategoria, "");
			while ((*ch = getchar()) != '\n') {
				strcat(tempCategoria, ch);
			}

			strcpy(categoriaProduto, tempCategoria);

			//Busca binária para achar a categoria
			buscaParcialCategoria = bb_categoria(categoriaProduto, icategory, &ncat);
			if (buscaParcialCategoria == NULL) {
				printf(REGISTRO_N_ENCONTRADO);
				return;
			} else {
				//printf("%s", buscaParcialCategoria->cat);

				aux =  buscaParcialCategoria->lista;
				while(aux != NULL){
					primaria = bb_primaria(aux->pk, iprimary, nregistros);
					if (primaria != NULL && strcmp(primaria->pk, "-1") != 0) {
						numProdutos++;
						resultado = recuperar_registro(primaria->rrn);

						printf("%s\n", resultado.pk);
						printf("%s\n", resultado.nome);
						printf("%s\n", resultado.marca);
						printf("%s\n", resultado.data);
						//printf("%s\n", prod.ano);
						printf("%s\n", resultado.preco);
						printf("%s\n", resultado.desconto);

						char categoria2[TAM_CATEGORIA];
						strcpy(categoria2, resultado.categoria);

						for (count2 = 0; count2 < strlen(resultado.categoria); count2++) {
							if (resultado.categoria[count2] == '|')
								printf(", ");
							else
								printf("%c", resultado.categoria[count2]);
						}
						printf("\n\n");
					}
					aux = aux->prox;
				}
				if (numProdutos == 0) {
					printf(REGISTRO_N_ENCONTRADO);
					return;
				}
			}
			break;

		case 3:
			numResultados = 0;
			strcpy(tempMarca, "");
			while ((*ch = getchar()) != '\n') {
				strcat(tempMarca, ch);
			}

			strcpy(marcaProduto, tempMarca);

			//Busca pela marca
			for (count = 0; count < *nregistros; count++) {
				if (strcmp(ibrand[count].string, marcaProduto) == 0) {
					primaria = bb_primaria(ibrand[count].pk, iprimary, nregistros);
					if (primaria != NULL) {
						if (strcmp(primaria->pk, "-1") != 0) {
							resultados[numResultados] = recuperar_registro(primaria->rrn);
							numResultados++;
						}
					}
				}
			}
			if (numResultados > 0) {
				for (count = 0; count < numResultados; count++) {
					printf("%s\n", resultados[count].pk);
					printf("%s\n", resultados[count].nome);
					printf("%s\n", resultados[count].marca);
					printf("%s\n", resultados[count].data);
					//printf("%s\n", prod.ano);
					printf("%s\n", resultados[count].preco);
					printf("%s\n", resultados[count].desconto);

					char categoria2[TAM_CATEGORIA];
					strcpy(categoria2, resultados[count].categoria);

					for (count2 = 0; count2 < strlen(resultados[count].categoria); count2++) {
						if (resultados[count].categoria[count2] == '|')
							printf(", ");
						else
							printf("%c", resultados[count].categoria[count2]);
					}
					printf("\n\n");
				}
			} else {
				printf(REGISTRO_N_ENCONTRADO);
				return;
			}

			break;

		case 4:

			//Buscando os preços
			for (count = 0; count < *nregistros; count++) {

				//auxiliarPreco = iprice[count];


				primaria = bb_primaria(iprice[count].pk, iprimary, nregistros);
				if (primaria != NULL) {
					if (strcmp(primaria->pk, "-1") != 0) {
						prod = recuperar_registro(primaria->rrn);

						printf("%s\n", prod.pk);
						printf("%s\n", prod.nome);
						printf("%s\n", prod.marca);
						printf("%s\n", prod.data);
						//printf("%s\n", prod.ano);
						printf("%s\n", prod.preco);
						printf("%s\n", prod.desconto);

						char categoria2[TAM_CATEGORIA];
						strcpy(categoria2, prod.categoria);

						for (count2 = 0; count2 < strlen(prod.categoria); count2++) {
							if (prod.categoria[count2] == '|')
								printf(", ");
							else
								printf("%c", prod.categoria[count2]);
						}
						printf("\n\n");

					}
				}
			}

			break;
	}
}
