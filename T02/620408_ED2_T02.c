/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 02 - Árvore B
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
#define TAM_PRIMARY_KEY 11
#define TAM_NOME 51
#define TAM_MARCA 51
#define TAM_DATA 11
#define TAM_ANO 3
#define TAM_PRECO 8
#define TAM_DESCONTO 4
#define TAM_CATEGORIA 51
#define TAM_STRING_INDICE (TAM_MARCA + TAM_NOME)


#define TAM_REGISTRO 192
#define MAX_REGISTROS 1000
#define MAX_ORDEM 150
#define TAM_ARQUIVO (MAX_REGISTROS * TAM_REGISTRO + 1)

/* Saídas do usuário */
#define OPCAO_INVALIDA 				"Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE 		"Memoria insuficiente!\n"
#define REGISTRO_N_ENCONTRADO 		"Registro(s) nao encontrado!\n"
#define CAMPO_INVALIDO 				"Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA 			"ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO 				"Arquivo vazio!"
#define INICIO_BUSCA 				"********************************BUSCAR********************************\n"
#define INICIO_LISTAGEM				"********************************LISTAR********************************\n"
#define INICIO_ALTERACAO 			"********************************ALTERAR*******************************\n"
#define INICIO_ARQUIVO				"********************************ARQUIVO*******************************\n"
#define INICIO_INDICE_PRIMARIO      "***************************INDICE PRIMÁRIO****************************\n"
#define INICIO_INDICE_SECUNDARIO    "***************************INDICE SECUNDÁRIO**************************\n"
#define SUCESSO  				 	"OPERACAO REALIZADA COM SUCESSO!\n"
#define FALHA 					 	"FALHA AO REALIZAR OPERACAO!\n"
#define NOS_PERCORRIDOS_IP 			"Busca por %s. Nos percorridos:\n"
#define NOS_PERCORRIDOS_IS 			"Busca por %s.\nNos percorridos:\n"


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


/*Estrutura da chave de um nó do Índice Primário*/
typedef struct Chaveip
{
	char pk[TAM_PRIMARY_KEY];
	int rrn;
} Chave_ip;

/*Estrutura da chave de um  do Índice Secundário*/
typedef struct Chaveis
{
	char string[TAM_STRING_INDICE];
	char pk[TAM_PRIMARY_KEY];
} Chave_is;

/* Estrutura das Árvores-B */
typedef struct nodeip
{
	int num_chaves;		/* numero de chaves armazenadas*/
	Chave_ip *chave;		/* vetor das chaves e rrns [m-1]*/
	int *desc;	/* ponteiros para os descendentes, *desc[m]*/
	char folha;			/* flag folha da arvore*/
} node_Btree_ip;

typedef struct nodeis
{
	int num_chaves;		/* numero de chaves armazenadas*/
	Chave_is *chave;		/* vetor das chaves e rrns [m-1]*/
	int *desc;	/* ponteiros para os descendentes, *desc[m]*/
	char folha;			/* flag folha da arvore*/
} node_Btree_is;

typedef struct {
	int raiz;
} Indice;

/*
		Registro auxiliar para retorno das funções
*/
struct auxiliar_ip{
    Chave_ip *chave_promovida;
    node_Btree_ip *filho_direito;
};

struct auxiliar_is{
    Chave_is *chave_promovida;
    node_Btree_is *filho_direito;
};

/* Variáveis globais */
char ARQUIVO[MAX_REGISTROS * TAM_REGISTRO + 1];
char ARQUIVO_IP[2000*sizeof(Chave_ip)];
char ARQUIVO_IS[2000*sizeof(Chave_is)];
/* Ordem das arvores */
int ordem_ip;
int ordem_is;
int nregistros;
int nregistrosip; /*Número de nós presentes no ARQUIVO_IP*/
int nregistrosis; /*Número de nós presentes no ARQUIVO_IS*/
/*Quantidade de bytes que ocupa cada nó da árvore nos arquivos de índice:*/
int tamanho_registro_ip;
int tamanho_registro_is;
/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

 /********************FUNÇÕES DO MENU*********************/
 void cadastrar(Indice* iprimary, Indice* ibrand);

 int alterar(Indice iprimary);

 void buscar(Indice iprimary,Indice ibrand);

 void listar(Indice iprimary,Indice ibrand);

 /*******************************************************/

 /* Recebe do usuário uma string simulando o arquivo completo e retorna o número
  * de registros. */
 int carregar_arquivo();

/* (Re)faz o Cria iprimary*/
void criar_iprimary(Indice *iprimary);

/* (Re)faz o índice de produtos  */
void criar_ibrand(Indice *ibrand) ;

/* Aloca um nó de árvore para ser utilizado em memória primária, o primeiro parametro serve para informar que árvore se trata
* É conveniente que essa função também inicialize os campos necessários com valores nulos*/
void *criar_no(char ip);

/*Libera todos os campos dinâmicos do nó, inclusive ele mesmo*/
void libera_no(void *node, char ip);

/* Escreve um nó da árvore no arquivo de índice */
void write_btree_ip(node_Btree_ip *salvar, int rrn);
void write_btree_is(node_Btree_is *salvar, int rrn);

/* Lê um nó do arquivo de índice e retorna na estrutura */
node_Btree_ip *read_btree_ip(int rrn);
node_Btree_is *read_btree_is(int rrn);

/* Atualiza os dois índices com o novo registro inserido */
void inserir_registro_indices(Indice *iprimary, Indice *ibrand, Produto p);

/* Exibe o jogo */
int exibir_registro(int rrn);

/*
		Função para ler o nome do produto (do standart input para ram)
		Retorna o nome do produto lido
*/
void lerNomeProduto(Produto *prod);

/*
		Função para ler a marca do produto (do standart input para ram)
		Retorna a marca do produto lido
*/
void lerMarcaProduto(Produto *prod);

/*
		Função para ler a data de registro do produto (do standart input para ram)
		Retorna a data de registro do produto lido
*/
void lerDataProduto(Produto *prod);

/*
		Função para ler o ano de lançamento do produto (do standart input para ram)
		Retorna o ano de lançamento do produto lido
*/
void lerAnoProduto(Produto *prod);

/*
		Função para ler o preço do produto (do standart input para ram)
		Retorna o preço do produto lido
*/
void lerPrecoProduto(Produto *prod);

/*
		Função para ler o desconto no preço do produto (do standart input para ram)
		Retorna o desconto no preço do produto lido
*/
void lerDescontoProduto(Produto *prod);

/*
		Função para ler a(s) categoria(s) do produto (do standart input para ram)
		Retorna a(s) categoria(s) do produto lido
*/
void lerCategoriaProduto(Produto *prod);

/*
		Gerar a chave primária do registro
*/
void gerarCodigo(Produto *prod);

/*
		Recuperar registro
*/
Produto recuperar_registro(int rrn);

/*
		Gravar produto no arquivo
*/
void gravarNoArquivo(Produto *prod, Indice *iprimary, Indice *ibrand);

/*
		Inicializa a árvore
*/
void *inicializar_arvore(Indice *arvore, char ip);

/*
		Retorna o menor valor
*/
int min(int x, int y) {
	if (x > y)
		return y;
	return x;
}

/*
		insere uma chave no iprimary
*/
void insere_ip(Indice *indice, Chave_ip *ip);


int main()
{
	char *p; /* # */
	Indice iprimary;
	Indice ibrand;
	int opcao;
  /* Arquivo */
	int carregarArquivo = 0; nregistros = 0, nregistrosip = 0, nregistrosis = 0;
	scanf("%d\n", &carregarArquivo); /* 1 (sim) | 0 (nao) */
	if (carregarArquivo)
		nregistros = carregar_arquivo();

	scanf("%d %d%*c", &ordem_ip, &ordem_is);

	tamanho_registro_ip = ordem_ip*3 + 4 + (-1 + ordem_ip)*14;
	tamanho_registro_is = ordem_is*3 + 4 + (-1 + ordem_is)* (	TAM_STRING_INDICE +9);

	/* Índice primário */
	criar_iprimary(&iprimary);

	/* Índice secundário de nomes dos Produtos */
	/*criar_ibrand(&ibrand);*/

	/* Execução do programa */
	opcao = 0;
	while(1)
	{
		scanf("%d%*c", &opcao);
		switch(opcao) {
		case 1: /* Cadastrar um novo Produto */
			cadastrar(&iprimary, &ibrand);
			break;
		case 2: /* Alterar o desconto de um Produto */
			printf(INICIO_ALTERACAO);
			/*if (alterar(iprimary))
				printf(SUCESSO);
			else
				printf(FALHA);*/
			break;
		case 3: /* Buscar um Produto */
			printf(INICIO_BUSCA);
			/*buscar(iprimary, ibrand);*/
			break;
		case 4: /* Listar todos os Produtos */
			printf(INICIO_LISTAGEM);
			/*listar(iprimary, ibrand);*/
			break;
		case 5: /* Imprimir o arquivo de dados */
			printf(INICIO_ARQUIVO);
			printf("%s\n", (*ARQUIVO!='\0') ? ARQUIVO : ARQUIVO_VAZIO);
			break;
		case 6: /* Imprime o Arquivo de Índice Primário*/
			printf(INICIO_INDICE_PRIMARIO);
			if(!*ARQUIVO_IP)
				puts(ARQUIVO_VAZIO);
			else
				for(p = ARQUIVO_IP; *p!='\0'; p+=tamanho_registro_ip)
				{
					fwrite( p , 1 ,tamanho_registro_ip,stdout);
					puts("");
				}
			break;
		case 7: /* Imprime o Arquivo de Índice Secundário*/
			printf(INICIO_INDICE_SECUNDARIO);
			if(!*ARQUIVO_IS)
				puts(ARQUIVO_VAZIO);
			else
				for(p = ARQUIVO_IS; *p!='\0'; p+=tamanho_registro_is)
				{
					fwrite( p , 1 ,tamanho_registro_is,stdout);
					puts("");

				}
			break;
		case 8: /*Libera toda memória alocada dinâmicamente (se ainda houver) e encerra*/
			return 0;
		default: /* exibe mensagem de erro */
			printf(OPCAO_INVALIDA);
			break;
		}
	}
	return -1;
}

/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo()
{
	scanf("%[^\n]\n", ARQUIVO);
	return strlen(ARQUIVO) / TAM_REGISTRO;
}


/* Exibe o Produto */
int exibir_registro(int rrn)
{
	float preco;
	int desconto;
	Produto j;
	char *cat, categorias[TAM_CATEGORIA];
	if(rrn < 0)
		return 0;

	j = recuperar_registro(rrn);

	printf("%s\n", j.pk);
	printf("%s\n", j.nome);
	printf("%s\n", j.marca);
	printf("%s\n", j.data);
	printf("%s\n", j.ano);
	sscanf(j.desconto,"%d",&desconto);
	sscanf(j.preco,"%f",&preco);
	preco = preco *  (100-desconto);
	preco = ((int) preco)/ (float) 100 ;
	printf("%07.2f\n",  preco);
	strcpy(categorias, j.categoria);

	for (cat = strtok (categorias, "|"); cat != NULL; cat = strtok (NULL, "|"))
    	printf("%s ", cat);
	printf("\n");

	return 1;
}

void cadastrar(Indice* iprimary, Indice* ibrand) {
	Produto *prod = (Produto *) calloc(1,sizeof(Produto));
	Chave_ip *chave_ip;

	lerNomeProduto(prod);
	lerMarcaProduto(prod);
	lerDataProduto(prod);
	lerAnoProduto(prod);
	lerPrecoProduto(prod);
	lerDescontoProduto(prod);
	lerCategoriaProduto(prod);

	gerarCodigo(prod);

	gravarNoArquivo(prod, iprimary, ibrand);

	chave_ip = (Chave_ip *) calloc(1, sizeof(Chave_ip));
	strcpy(chave_ip->pk, prod->pk);
	chave_ip->rrn = nregistros;

	insere_ip(iprimary, chave_ip);
	nregistros++;

	free(prod);
	free(chave_ip);
}

void gravarNoArquivo(Produto *prod, Indice *iprimary, Indice *ibrand) {
	int tamanho = 0;

	tamanho+=strlen(prod->pk);
	sprintf(ARQUIVO+strlen(ARQUIVO), "%s", prod->pk);

	tamanho+=(strlen(prod->nome)+1);
	sprintf(ARQUIVO+strlen(ARQUIVO), "@%s", prod->nome);

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
	sprintf(ARQUIVO+strlen(ARQUIVO), "@%s", prod->categoria);

	while (tamanho < 192) {
		sprintf(ARQUIVO+strlen(ARQUIVO), "#");
		tamanho++;
	}
}

void lerNomeProduto(Produto *prod) {
	fgets(prod->nome, TAM_NOME+1, stdin);
	prod->nome[strlen(prod->nome)-1]  = '\0';
}

void lerMarcaProduto(Produto *prod) {
	fgets(prod->marca, TAM_MARCA+1, stdin);
	prod->marca[strlen(prod->marca)-1]  = '\0';
}

void lerDataProduto(Produto *prod) {
	fgets(prod->data, TAM_DATA+1, stdin);
	prod->data[strlen(prod->data)-1]  = '\0';
}

void lerAnoProduto(Produto *prod) {
	fgets(prod->ano, TAM_ANO+1, stdin);
	prod->ano[strlen(prod->ano)-1]  = '\0';
}

void lerPrecoProduto(Produto *prod) {
	fgets(prod->preco, TAM_PRECO+1, stdin);
	prod->preco[strlen(prod->preco)-1]  = '\0';
}

void lerDescontoProduto(Produto *prod) {
	fgets(prod->desconto, TAM_DESCONTO+1, stdin);
	prod->desconto[strlen(prod->desconto)-1]  = '\0';
}

void lerCategoriaProduto(Produto *prod) {
	fgets(prod->categoria, TAM_CATEGORIA+1, stdin);
	prod->categoria[strlen(prod->categoria)-1]  = '\0';
}

void gerarCodigo(Produto *prod) {
	char *codigo_temp = (char *) calloc(1,TAM_PRIMARY_KEY * sizeof(char));
	char *codigo = (char *) calloc(1,TAM_PRIMARY_KEY * sizeof(char));
	int count = 0;

	strncpy(codigo_temp, prod->nome, 2);
	strcat(codigo, codigo_temp);

	strncpy(codigo_temp, prod->marca, 2);
	strcat(codigo, codigo_temp);

	codigo[4] = prod->data[0];
	codigo[5] = prod->data[1];
	codigo[6] = prod->data[3];
	codigo[7] = prod->data[4];

	strncpy(codigo_temp, prod->ano, 2);
	strcat(codigo, codigo_temp);

	for (count = 0; count < strlen(codigo_temp); count++)
		codigo_temp[count] = toupper(codigo_temp[count]);

	strcpy(prod->pk, codigo);

	free(codigo_temp);
	free(codigo);
}

Produto recuperar_registro(int rrn) {
	/*printf("%s\n", ARQUIVO);*/
	Produto j;
	char temp[193], *p;
	strncpy(temp, ARQUIVO + ((rrn)*192), 192);
	temp[192] = '\0';
	p = strtok(temp,"@");
	strcpy(j.pk,p);
	p = strtok(NULL,"@");
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
	strncpy(j.categoria,p, TAM_CATEGORIA);
	/*gerarCodigo(&j);*/
	return j;
}

void write_btree_ip(node_Btree_ip *salvar, int rrn) {
	int tamanho; /* Quantidade escrita */
	int size; /* Tamanho do indice */
	int count;
	size = ( 3 + (ordem_ip * 14) + 1 + (ordem_ip * 3) );
	tamanho = 0;

	tamanho+=3;
	sprintf(ARQUIVO_IP+(rrn*size), "%.3d", salvar->num_chaves);

	for(count = 0; count < salvar->num_chaves; count++) {
		tamanho+=TAM_PRIMARY_KEY;
		sprintf(ARQUIVO_IP+strlen(ARQUIVO_IP), "%s", salvar->chave[count].pk);

		tamanho+=4;
		sprintf(ARQUIVO_IP+strlen(ARQUIVO_IP), "%.4d", salvar->chave[count].rrn);
	}

	if (salvar->folha == 'F') {
		/*É folha, pode, garantidamente, preecher com '#'*/
		for (count = tamanho; count < ((ordem_ip - 1) * 14); count++) {
			tamanho+=1;
			sprintf(ARQUIVO_IP+strlen(ARQUIVO_IP), "%s", "#");
		}
	}

	tamanho+=1;
	sprintf(ARQUIVO_IP+strlen(ARQUIVO_IP), "%c", salvar->folha);

	if (salvar->folha == 'F') {
		/*É folha, pode, garantidamente, preecher com '*'*/
		for (count = 0; count < (ordem_ip * 3); count++) {
			tamanho+=1;
			sprintf(ARQUIVO_IP+strlen(ARQUIVO_IP), "%s", "*");
		}
	}
}

node_Btree_ip *read_btree_ip(int rrn) {
	node_Btree_ip *no;
	int tamanho;
	char num_chaves[4];
	char rrn_produto[5];
	int count;
	char *temp;

	tamanho = ( 3 + (ordem_ip * 14) + 1 + (ordem_ip * 3) );

	temp = (char *) calloc(1, tamanho);

	strncpy(temp, ARQUIVO_IP + ((rrn)*tamanho), tamanho);

	no = calloc(1, sizeof(node_Btree_ip));

	strncpy(num_chaves, temp, 3);
	num_chaves[3] = '\0';
	no->num_chaves = atoi(num_chaves);

	no->chave = (Chave_ip *) calloc(ordem_ip - 1, sizeof(Chave_ip));
	for (count = 0; count < min(ordem_ip, no->num_chaves); count++) {
		strncpy(no->chave[count].pk, temp+3+(count*14), 10);
		strncpy(rrn_produto, temp+13+(count*14), 4);
		no->chave[count].rrn = atoi(rrn_produto);
	}
	return no;
}

void *criar_no(char ip) {
		if (ip == 'p') {
			node_Btree_ip *arvore = malloc(sizeof(node_Btree_ip));

			arvore->num_chaves = 0;
			arvore->folha = 'F';
			arvore->chave = calloc((ordem_ip-1), sizeof(Chave_ip));
			arvore->desc = calloc((ordem_ip), sizeof(int));

			return arvore;
		} else if (ip == 's') {
			node_Btree_is *arvore = malloc(sizeof(node_Btree_is));

			arvore->num_chaves = 0;
			arvore->folha = 'F';
			arvore->chave = calloc((ordem_is-1), sizeof(Chave_is));
			arvore->desc = calloc((ordem_is), sizeof(int));

			return arvore;
		}
		return NULL;
}

void *inicializar_arvore(Indice *arvore, char ip) {
	arvore->raiz = -1;
	if (ip == 'p') {
		return calloc(1, sizeof(node_Btree_ip));
	} else {
		return calloc(1, sizeof(node_Btree_is));
	}
	return NULL;
}

void insere_ip(Indice *indice, Chave_ip *ip) {
	int count = 0;
	node_Btree_ip *aux;
	/*struct auxiliar_ip *ret_funcoes;*/

	if( indice->raiz == -1 ) {
		aux = criar_no('p');
		aux->folha = 'F';
		aux->num_chaves = 1;
		aux->chave[0] = *ip;

		/*Vai estar na primeira posição*/
		indice->raiz = 0;

		/*
			TODO: mover para uma função específica
		*/

		nregistrosip = 1;
		write_btree_ip(aux, 0);
	} else {

		aux = read_btree_ip(indice->raiz);
		/*
			Tem espaço na raíz
		*/
		if (aux->num_chaves < ordem_ip-1) {
			for (count = aux->num_chaves; count >= 0; count--) {
				if (strcmp(aux->chave[count-1].pk, ip->pk) > 0) {
					aux->chave[count] = aux->chave[count-1];
				} else {
					aux->chave[count] = *ip;
					aux->folha = 'F';
					aux->num_chaves+=1;
					nregistrosip++;
					break;
				}
			}
		}
		write_btree_ip(aux, 0);
	}
}

void criar_iprimary(Indice *iprimary) {
	Produto p;
	int count;
	Chave_ip ip;
	iprimary->raiz = -1;
	/*node_Btree_ip *iprimary_real = inicializar_arvore(iprimary, 'p');*/

	for (count = 0; count < nregistros; count++) {
		p = recuperar_registro(count);
		strcpy(ip.pk, p.pk);
		/*
			printf("Produto nome = %s\n", p.nome);
			printf("Produto marca = %s\n", p.marca);
			printf("%s\n", ip.pk);
		*/
		ip.rrn = count;
		insere_ip(iprimary, &ip);
	}
}

void criar_ibrand(Indice *ibrand) {
	/*
		TODO:
			- desalocar arvore existente
			- criar nova árvore lendo o arquivo
	*/

	/*node_Btree_is arvore = criar_arvore_is(ordem_is); */
}
