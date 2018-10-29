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

/*
		Gerar a chave primária do registro
*/
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
int criar_secondary_cat(Ir **secondary, int *nregistros, int ncat);

/*
		Função para ordenar icategory
*/
void ordenar_categoria(Ir *secondary, int *ncats);

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


/*
		Função para excluir definitivamente (hahahaha) os produtos
*/
void liberarEspaco(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, Is *iproduct, int *nregistros, int ncat);
/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main(){
  /* Arquivo */
	int carregarArquivo = 0, nregistros = 0, ncat = 0;
	scanf("%d%*c", &carregarArquivo); /* 1 (sim) | 0 (nao) */
	if (carregarArquivo)
		nregistros = carregar_arquivo();

	/* Índice primário */
	Ip *iprimary;
	iprimary = (Ip *) calloc (MAX_REGISTROS * sizeof(Ip),1);

	if (!iprimary) {
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iprimary(iprimary, &nregistros);

	/*Alocar e criar índices secundários*/

	Is *iproduct;
	iproduct = (Is *) calloc(1,MAX_REGISTROS * sizeof(Is));
	if (carregarArquivo)
		criar_secondary(iproduct, &nregistros, 0);

	Is *ibrand;
	ibrand = (Is *) calloc(1,MAX_REGISTROS * sizeof(Is));
	if (carregarArquivo)
		criar_secondary(ibrand, &nregistros, 1);

	Ir *icategory;
	icategory = (Ir *) calloc (MAX_CATEGORIAS * sizeof(Ir), 1);


	if (carregarArquivo)
		ncat = criar_secondary_cat(&icategory, &nregistros, ncat);

	Isf *iprice = (Isf *) calloc(1,MAX_REGISTROS * sizeof(Isf));
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
				if(resultado == 1) {
					printf(SUCESSO);
				}
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
				liberarEspaco(iprimary, icategory, ibrand, iprice, iproduct, &nregistros, ncat);
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
      		/*Liberar memória e finalizar o programa */
					free(iproduct);
					free(ibrand);
					free(icategory);
					free(iprice);
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
	int i = 0;
	float temp;
	printf(INICIO_ARQUIVO_SECUNDARIO);
	scanf("%d", &opPrint);
	if(!nregistros) {
		printf(ARQUIVO_VAZIO);
		printf("\n");
	}
	switch (opPrint) {
		case 1:
			for(i = 0; i < nregistros; i++){
				printf("%s %s\n",iproduct[i].pk, iproduct[i].string);
			}
		break;
		case 2:
			for(i = 0; i < nregistros; i++){
				printf("%s %s\n",ibrand[i].pk, ibrand[i].string);
			}
		break;
		case 3:
			for(i = 0; i < ncat; i++){
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
		for(i = 0; i < nregistros; i++){
			temp = 100*iprice[i].price;
			temp = (int)temp / (float)100.0;
			printf("%s %.2f\n",iprice[i].pk, temp);
		}
		break;
	}
}

void gerarChave(Produto *prod) {
	char *chave_temp = (char *) calloc(1,TAM_PRIMARY_KEY * sizeof(char));
	char *chave = (char *) calloc(1,TAM_PRIMARY_KEY * sizeof(char));
	int count = 0;

	strncpy(chave_temp, prod->nome, 2);
	strcat(chave, chave_temp);

	strncpy(chave_temp, prod->marca, 2);
	strcat(chave, chave_temp);

	chave[4] = prod->data[0];
	chave[5] = prod->data[1];
	chave[6] = prod->data[3];
	chave[7] = prod->data[4];

	strncpy(chave_temp, prod->ano, 2);
	strcat(chave, chave_temp);

	for (count = 0; count < strlen(chave_temp); count++)
		chave_temp[count] = toupper(chave_temp[count]);

	strcpy(prod->pk, chave);

}

float arredondamento(float preco, float desconto) {
  preco = preco *  (100-desconto);
  preco = ((int) preco)/ (float) 100;

  return preco;
}

int inserirProduto(int *num, Ip *indice, Is *iproduct, Is *ibrand, Isf *iprice, Ir *icategory, int numCategorias) {
	Produto *prod = (Produto *) calloc(1,sizeof(Produto));

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

		ordenar_iprimary(indice, num);

		ordenar_secondary(iproduct, num, 0);
		ordenar_secondary(ibrand, num, 1);
		ordenar_secondary_price(iprice, num);

		return totalCategorias;
	}
	return numCategorias;
}

void addIprimary(Produto *prod, Ip *indice, int num) {
	Ip *novoIndice = (Ip *) calloc(1,sizeof(Ip));
	strcpy(novoIndice->pk, prod->pk);
	novoIndice->rrn = (num);
	indice[num] = *novoIndice;
}

void addSecondary(Produto *prod, Is *indice, int caso, int num) {
	Is *novoIndice = (Is *) calloc(1,sizeof(Is));
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
	char *nome = (char *) calloc(1,TAM_NOME * sizeof(char));

	char *ch = (char *) calloc(1,sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(nome, ch);
	}


	return nome;
}

char *lerMarcaProduto() {
	char *marca = (char *) calloc(1,TAM_MARCA * sizeof(char));

	char *ch = (char *) calloc(1,sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(marca, ch);
	}



	return marca;
}

char *lerDataProduto() {
	char *data = (char *) calloc(1,TAM_DATA * sizeof(char));

	char *ch = (char *) calloc(1,sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(data, ch);
	}


	return data;
}

char *lerAnoProduto() {
	char *ano = (char *) calloc(1,TAM_ANO * sizeof(char));

	char *ch = (char *) calloc(1,sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(ano, ch);
	}


	return ano;
}

char *lerPrecoProduto() {
	char *preco = (char *) calloc(1,TAM_PRECO * sizeof(char));

	char *ch = (char *) calloc(1,sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(preco, ch);
	}

	return preco;
}

char *lerDescontoProduto() {
	char *desconto = (char *) calloc(1,TAM_DESCONTO * sizeof(char));

	char *ch = (char *) calloc(1,sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(desconto, ch);
	}


	return desconto;
}

char *lerCategoriaProduto() {
	char *categoria = (char *) calloc(1,TAM_CATEGORIA * sizeof(char));

	char *ch = (char *) calloc(1,sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(categoria, ch);
	}


	return categoria;
}

void criar_iprimary(Ip *indice_primario, int* nregistros) {
	if (*nregistros > 0) {
		int count = 0;
		Produto temp;
		for (; count < *nregistros; count++) {
			if (strlen(ARQUIVO) > (count * 192)) {
				temp = recuperar_registro(count);

				Ip *novoIndice = (Ip *) calloc(1,sizeof(Ip));
				strcpy(novoIndice->pk, temp.pk);
				novoIndice->rrn = (count);

				indice_primario[count] = *novoIndice;
			} else {
				break;
			}
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
	return strncmp(ia->pk, ib->pk, TAM_PRIMARY_KEY);
}

int comparar_iprimary_str(const void *a, const void *b) {
	const char *ia = (char *)a;
	const Ip *ib = (Ip *)b;


	return strncmp(ia, ib->pk, TAM_PRIMARY_KEY);
}

void ordenar_secondary(Is *secondary, int* nregistros, int caso) {
	if (*nregistros > 0)
		qsort(secondary, *nregistros, sizeof(Is), comparar_secondary);
}

int comparar_secondary(const void *a, const void *b) {
	const Is *ia = (Is *)a;
	const Is *ib = (Is *)b;
	int retorno = strncmp(ia->string, ib->string, TAM_NOME);

	if (retorno == 0)
		return strncmp(ia->pk, ib->pk, TAM_PRIMARY_KEY);
	return retorno;
}

int comparar_secondary_str(const void *a, const void *b) {
	const char *ia = (char *)a;
	const Is *ib = (Is *)b;
	int retorno = strncmp(ia, ib->string, TAM_NOME);
	return retorno;
}

void criar_secondary(Is *secondary, int *nregistros, int caso) {
	if (*nregistros > 0) {
		int count = 0;
		Produto temp;

		if (caso == 0) {


			for (; count < *nregistros; count++) {
				if (strlen(ARQUIVO) > (count * 192)) {
					temp = recuperar_registro(count);

					Is *novoIndice = (Is *) calloc(1,sizeof(Is));
					strcpy(novoIndice->string, temp.nome);
					strcpy(novoIndice->pk, temp.pk);
					secondary[count] = *novoIndice;
				} else {
					break;
				}
			}
			ordenar_secondary(secondary, nregistros, 0);
		} else if (caso == 1) {
			for (; count < *nregistros; count++) {
				if (strlen(ARQUIVO) > (count * 192)) {
					temp = recuperar_registro(count);

					Is *novoIndice = (Is *) calloc(1,sizeof(Is));
					strcpy(novoIndice->string, temp.marca);
					strcpy(novoIndice->pk, temp.pk);
					secondary[count] = *novoIndice;
				} else {
					break;
				}
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
			if (strlen(ARQUIVO) > (count * 192)) {
				temp = recuperar_registro(count);

				Isf *novoIndice = (Isf *) calloc(1,sizeof(Isf));
				novoIndice->price = (arredondamento((float) strtof(temp.preco, NULL), (float) strtof(temp.desconto, NULL)));
				strcpy(novoIndice->pk, temp.pk);
				secondary[count] = *novoIndice;
			} else {
				break;
			}
		}
		ordenar_secondary_price(secondary, nregistros);
	}
}

void addSecondary_price(Produto *prod, Isf *indice, int num) {
	Isf *novoIndice = (Isf *) calloc(1,sizeof(Isf));
	novoIndice->price = (arredondamento((float) strtof(prod->preco, NULL), (float) strtof(prod->desconto, NULL)));
	strcpy(novoIndice->pk, prod->pk);

	indice[num] = *novoIndice;
}

int addSecondary_cat(Produto *prod, Ir *indice, int numCategorias) {
	Produto temp;
	temp = *prod;

	int numProdutos = 0;

	char categoria_produto[TAM_CATEGORIA];
	char *token;

	int localNumCategorias = numCategorias;

	Ir *nova_categoria = NULL;

	strcpy(categoria_produto, temp.categoria);

	token = strtok(categoria_produto, "|");

	ll *lista_aux;

	while (token != NULL) {
		nova_categoria = bb_categoria(token, indice, &localNumCategorias);
		if (nova_categoria == NULL) {
			Ir *novoIndice = (Ir *) calloc(1,sizeof(Ir));
			strcpy(novoIndice->cat, token);

			novoIndice->lista = (ll *) calloc(1,sizeof(ll));
			strcpy(novoIndice->lista->pk, temp.pk);
			novoIndice->lista->prox = NULL;

			indice[localNumCategorias] = *novoIndice;
			localNumCategorias++;
		} else {
			numProdutos = 0;

			lista_aux = nova_categoria->lista;

			if (lista_aux == NULL) {
				lista_aux = (ll *) calloc(sizeof(ll), 1);
			}

			if (lista_aux != NULL) {
				while (lista_aux->prox != NULL) {
					lista_aux = lista_aux->prox;
					numProdutos++;
				}
			}

			ll *novo_no = (ll *) calloc(sizeof(ll), 1);
			strcpy(novo_no->pk, temp.pk);
			novo_no->prox = NULL;

			lista_aux->prox = novo_no;
			if (nova_categoria->lista != NULL)
				MergeSort(&(nova_categoria->lista));

		}
		ordenar_categoria(indice, &localNumCategorias);
		token = strtok(NULL, "|");
	}
	return localNumCategorias;
}

int criar_secondary_cat(Ir **secondary, int *nregistros, int ncat) {

	int count = 0;
	ll *auxiliar, *aux;

	if (ncat > 0) {
		for (count = 0; count < ncat; count++) {
			aux = (*secondary)[count].lista;
			while (aux) {
				auxiliar = aux->prox;
				free(aux);
				aux = auxiliar;
			}

			(*secondary)[count].lista = NULL;

		}
		free(*secondary);

		*secondary = (Ir *) calloc(1,sizeof(Ir) * MAX_CATEGORIAS);
	}

	if (*nregistros > 0) {
		Produto temp;

		int numCategorias = 0;

		char categoria_produto[TAM_CATEGORIA];
		char *token;

		Ir *nova_categoria = NULL;

		for (count = 0; count < *nregistros; count++) {
			if (strlen(ARQUIVO) > (count * 192) && ARQUIVO[count*192]!='*' && ARQUIVO[count*192 +1] != '|') {
				temp = recuperar_registro(count);

				strncpy(categoria_produto, temp.categoria, TAM_CATEGORIA);

				token = strtok(categoria_produto, "|");
				while (token != NULL) {
					nova_categoria = bb_categoria(token, *secondary, &numCategorias);
					if (nova_categoria == NULL) {
						Ir *novoIndice = (Ir *) calloc(1,sizeof(Ir));
						strcpy(novoIndice->cat, token);

						novoIndice->lista = (ll *) calloc(1,sizeof(ll));
						strcpy(novoIndice->lista->pk, temp.pk);
						novoIndice->lista->prox = NULL;


						(*secondary)[numCategorias] = *novoIndice;
						numCategorias++;
					} else {

						ll *lista_aux = nova_categoria->lista;

						while (lista_aux->prox != NULL) {
							lista_aux = lista_aux->prox;
						}

						ll *novo_no = (ll *) calloc(1,sizeof(ll));
						strcpy(novo_no->pk, temp.pk);
						novo_no->prox = NULL;

						lista_aux->prox = novo_no;

						MergeSort(&(nova_categoria->lista));

					}
					ordenar_categoria(*secondary, &numCategorias);
					token = strtok(NULL, "|");
				}
			}
		}
		return numCategorias;
	}
	return 0;
}

Ir *bb_categoria(char *chave, Ir *indice, int *ncat) {
	return (Ir *) bsearch(chave, indice, *ncat, sizeof(Ir), comparar_categoria_str);
}

int comparar_categoria_str(const void *a, const void *b) {
	const char *ia = (char *)a;
	const Ir *ib = (Ir *)b;

	return strcmp(ia, ib->cat);
}

void ordenar_categoria(Ir *secondary, int *ncats) {
	if (*ncats > 2)
		qsort(secondary, *ncats, sizeof(Ir), comparar_icategory);
}

int comparar_icategory(const void *a, const void *b) {
	const Ir *ia = (Ir *)a;
	const Ir *ib = (Ir *)b;
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
	char *chavinha = (char *) calloc(1,TAM_PRIMARY_KEY * sizeof(char));

	char *descontinho = (char *) calloc(1,TAM_DESCONTO * sizeof(char));

	char *ch = (char *) calloc(1,sizeof(char));

	while ((*ch = getchar()) != '\n') {
		strcat(chavinha, ch);
	}

	Ip * indice = bb_primaria(chavinha, iprimary, &nregistros);

	if (indice == NULL) {
		printf(REGISTRO_N_ENCONTRADO);
		return -1;
	}
	int rrn = indice->rrn;
	/*
	esse recuperar_registro vc quem fez, ou lhe deram assim?
	me deram esse negócio feio sem ponteiro, e quando ele nao acha, oq ele faz?

	*/
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

	int movimentacao = ((192 * rrn) + strlen(prod.nome) + strlen(prod.marca) + strlen(prod.data) + strlen(prod.ano) + strlen(prod.preco) + 5);
	char *p = ARQUIVO + movimentacao;
	sprintf(p, "%s", descontinho);
	ARQUIVO[movimentacao+3] = '@';
	criar_secondary_price(iprice, &nregistros);
	return 1;
}

int remover(Ip *iprimary, int *nregistros) {
	char *chavinha = (char *) malloc(TAM_PRIMARY_KEY + 2);

  fgets(chavinha, TAM_PRIMARY_KEY+1, stdin);
  chavinha[strlen(chavinha)-1]  = '\0';

	Ip * indice = bb_primaria(chavinha, iprimary, nregistros);

	if (indice == NULL) {
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}
	int rrn = indice->rrn;

	int movimentacao = (192 * rrn);
	char *p = ARQUIVO + movimentacao;

	strncpy(p, "*|", 2);

	strncpy(indice->pk, "", TAM_PRIMARY_KEY);
	strcpy(indice->pk, "-1");

	ordenar_iprimary(iprimary, nregistros);

	return 1;
}

void buscar(Ip *iprimary, Is *iproduct, Is *ibrand, Ir *icategory, int *nregistros, int ncat) {
	int opPrint = 0;
	ll *aux;
	char codProduto[TAM_PRIMARY_KEY];

	char nomeProduto[TAM_NOME];

	char marcaProduto[TAM_MARCA];

	char categoriaProduto[TAM_CATEGORIA];

	scanf("%d%*c", &opPrint);

	int count = 0, count2 = 0;
	Ip *busca;

	Ir *buscaParcialMarca;

	Produto resultados[MAX_REGISTROS];
	int numResultados = 0;

	switch (opPrint) {
		case 1:

		  fgets(codProduto, TAM_PRIMARY_KEY+1, stdin);
		  codProduto[strlen(codProduto)-1]  = '\0';

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

			fgets(nomeProduto, TAM_NOME+1, stdin);
			nomeProduto[strlen(nomeProduto)-1]  = '\0';

			for (count = 0; count < *nregistros; count++) {
				if (strlen(ARQUIVO) > (count * 192)) {
					if (strcmp(nomeProduto, iproduct[count].string) == 0) {
						busca = bb_primaria(iproduct[count].pk, iprimary, nregistros);
						if (!(busca == NULL || (strcmp(busca->pk, "-1") == 0))) {

							Produto prod = recuperar_registro(busca->rrn);

							printf("%s\n", prod.pk);
							printf("%s\n", prod.nome);
							printf("%s\n", prod.marca);
							printf("%s\n", prod.data);
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
							printf("\n");

						}
					} else if (strcmp(nomeProduto, iproduct[count].string) < 0)
						return;
				} else {
					break;
				}
			}
			break;
		case 3:

			fgets(marcaProduto, TAM_MARCA+1, stdin);
			marcaProduto[strlen(marcaProduto)-1]  = '\0';

			fgets(categoriaProduto, TAM_CATEGORIA+1, stdin);
			categoriaProduto[strlen(categoriaProduto)-1]  = '\0';

			for (count = 0; count < *nregistros; count++) {
				if (strcmp(ibrand[count].string, marcaProduto) == 0) {
					busca = bb_primaria(ibrand[count].pk, iprimary, nregistros);
					if (busca != NULL) {
						if (strcmp(busca->pk, "-1") != 0) {
							resultados[numResultados] = recuperar_registro(busca->rrn);
							numResultados++;
						}
					}
				}
			}
			if (numResultados == 0) {
				printf(REGISTRO_N_ENCONTRADO);
				return;
			} else {

				buscaParcialMarca = bb_categoria(categoriaProduto, icategory, &ncat);
				if (buscaParcialMarca == NULL) {
					printf(REGISTRO_N_ENCONTRADO);
					return;
				} else {

					aux =  buscaParcialMarca->lista;
					while(aux != NULL){
						for (count = 0; count < numResultados; count++) {
							if (strcmp(resultados[count].pk, aux->pk) == 0) {
								printf("%s\n", resultados[count].pk);
								printf("%s\n", resultados[count].nome);
								printf("%s\n", resultados[count].marca);
								printf("%s\n", resultados[count].data);
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

	char *ch = (char *) calloc(sizeof(char),1);

	int opPrint = 0;
	int count = 0, count2 = 0;
	int numProdutos = 0;
	Produto prod;

	char categoriaProduto[TAM_CATEGORIA];
	char tempCategoria[TAM_CATEGORIA];

	Ir *buscaParcialCategoria;
	ll *aux;

	Ip *primaria;
	Produto resultado;

	scanf("%d%*c", &opPrint);
	switch (opPrint) {
		case 1:
			for (count = 0; count < *nregistros; count++) {
				if (strcmp(iprimary[count].pk, "-1") != 0) {
					numProdutos++;
					prod = recuperar_registro(iprimary[count].rrn);
					printf("%s\n", prod.pk);
					printf("%s\n", prod.nome);
					printf("%s\n", prod.marca);
					printf("%s\n", prod.data);
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
					printf("\n");
					if (count < (*nregistros - 1))
						printf("\n");
				}
			}
			if (numProdutos == 0) {
				printf(REGISTRO_N_ENCONTRADO);
				return;
			}

			break;
		case 2:
			numProdutos = 0;
			strcpy(tempCategoria, "");
			while ((*ch = getchar()) != '\n') {
				strcat(tempCategoria, ch);
			}

			strcpy(categoriaProduto, tempCategoria);

			buscaParcialCategoria = bb_categoria(categoriaProduto, icategory, &ncat);
			if (buscaParcialCategoria == NULL) {
				printf(REGISTRO_N_ENCONTRADO);
				return;
			} else {

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
						printf("\n");
						if (aux->prox != NULL)
							printf("\n");
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

			for (count = 0; count < *nregistros; count++) {

				primaria = bb_primaria(ibrand[count].pk, iprimary, nregistros);

					if (primaria != NULL) {
						resultado = recuperar_registro(primaria->rrn);

						printf("%s\n", resultado.pk);
						printf("%s\n", resultado.nome);
						printf("%s\n", resultado.marca);
						printf("%s\n", resultado.data);
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
						printf("\n");
						if (count < (*nregistros - 1))
							printf("\n");
				}
			}
			break;

		case 4:

			for (count = 0; count < *nregistros; count++) {



				primaria = bb_primaria(iprice[count].pk, iprimary, nregistros);
				if (primaria != NULL) {
					if (strcmp(primaria->pk, "-1") != 0) {
						prod = recuperar_registro(primaria->rrn);

						printf("%s\n", prod.pk);
						printf("%s\n", prod.nome);
						printf("%s\n", prod.marca);
						printf("%s\n", prod.data);

						printf("%07.2f\n", iprice[count].price);

						char categoria2[TAM_CATEGORIA];
						strcpy(categoria2, prod.categoria);

						for (count2 = 0; count2 < strlen(prod.categoria); count2++) {
							if (prod.categoria[count2] == '|')
								printf(", ");
							else
								printf("%c", prod.categoria[count2]);
						}
						printf("\n");
						if (count < (*nregistros - 1))
							printf("\n");

					}
				}
			}

			break;
	}
}

void liberarEspaco(Ip *iprimary, Ir *icategory, Is *ibrand, Isf *iprice, Is *iproduct, int *nregistros, int ncat) {

  int contador_arquivo, contador_swap;
    if(*nregistros <= 0)
      return;
    char *swap = (char *) malloc ( (*nregistros) * 192  + 1);
    swap[(*nregistros) * 192 ] = '\0';
    for(contador_arquivo = 0, contador_swap = 0; contador_arquivo < *nregistros; contador_arquivo++) {
      if( strncmp(ARQUIVO + contador_arquivo * 192, "*|", 2) ) {
        strncpy(swap + contador_swap * 192, ARQUIVO + contador_arquivo*192, 192);
      contador_swap++;
    }
  }
  strncpy(ARQUIVO, swap, 192*contador_swap);
  free(swap);
  *nregistros = contador_swap;
  ARQUIVO[192 * contador_swap] = '\0';

  criar_iprimary(iprimary, nregistros);
  criar_secondary(iproduct, nregistros, 0);
  criar_secondary(ibrand, nregistros, 1);
  criar_secondary_price(iprice, nregistros);
  criar_secondary_cat(&icategory, nregistros, ncat);
}
