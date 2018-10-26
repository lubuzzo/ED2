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
void inserirProduto(int *num, Ip *indice);

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
void ordernar_iprimary(Ip *indice_primario, int* nregistros);

/*
		Função para comparar o indice primario com uma string
*/
int comparar_iprimary_str(const void *a, const void *b);

/*
		Função para busca binária no indice primario
*/
int bb_primaria(char *chave, Ip *indice, int *nregistros);


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
	Is *ibrand = (Is *) malloc (MAX_REGISTROS * sizeof(Is));
	Ir *icategory = (Ir *) malloc (sizeof(Ir));
	Isf *iprice = (Isf *) malloc (MAX_REGISTROS * sizeof(Isf));

	/*
		Debug to 9
		int count = 0;
	*/
	/* Execução do programa */
	int opcao = 0;
	while(1)
	{
		scanf("%d%*c", &opcao);
		switch(opcao)
		{
			case 1:
				inserirProduto(&nregistros, iprimary);
			break;
			case 2:
				/*alterar desconto*/
				printf(INICIO_ALTERACAO);
				/*
				if(alterar([args]))
					printf(SUCESSO);
				else
					printf(FALHA);
				*/
			break;
			case 3:
				/*excluir produto*/
				printf(INICIO_EXCLUSAO);
				/*
				if(remover([args]))
					printf(SUCESSO);
				else
					printf(FALHA);
				*/
			break;
			case 4:
				/*busca*/
				printf(INICIO_BUSCA);
			break;
			case 5:
				/*listagens*/
				printf(INICIO_LISTAGEM);
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
	while (count < strlen(prod->data) && count < 5) {
		if (strcmp(prod->data, "/") != 0)
			chave_temp[4+count] = prod->data[count];
		count++;
	}

	strcat(chave, chave_temp);

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

void inserirProduto(int *num, Ip *indice) {
	Produto *prod = (Produto *) malloc(sizeof(Produto));

	strcpy(prod->nome, lerNomeProduto());
	strcpy(prod->marca, lerMarcaProduto());
	strcpy(prod->data, lerDataProduto());
	strcpy(prod->ano, lerAnoProduto());
	strcpy(prod->preco, lerPrecoProduto());
	strcpy(prod->desconto, lerDescontoProduto());
	strcpy(prod->categoria, lerCategoriaProduto());

	gerarChave(prod);


	if (bb_primaria(prod->pk, indice, num) == 1)
		printf(ERRO_PK_REPETIDA, prod->pk);
	else {
		gravarNoArquivo(prod, indice, num);

		//Gravar no final do iprimary
		Ip *novoIndice = (Ip *) malloc(sizeof(Ip));
		strcpy(novoIndice->pk, prod->pk);
		//printf("%d\n", *num);
		novoIndice->rrn = (*num);
		indice[*num] = *novoIndice;
		*num = *num + 1;

		//Ordena o iprimary
		ordernar_iprimary(indice, num);
	}
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
		ordernar_iprimary(indice_primario, nregistros);

		/*
			Debug do iprimary
			for (count = 0; count < *nregistros; count++) {
				printf("%s -> %d\n", indice_primario[count].pk, indice_primario[count].rrn);
			}
	*/
	}
}

int bb_primaria(char *chave, Ip *indice, int *nregistros) {
	Ip *resultado = (Ip *) bsearch(chave, indice, *nregistros, sizeof(indice[0]), comparar_iprimary_str);
	if (resultado != NULL)
		return 1;
	return 0;
}

void ordernar_iprimary(Ip *indice_primario, int* nregistros) {
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
