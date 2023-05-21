#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
using namespace std;

struct Produto
{
	char nome[20];
	float preco;
	int quantidade, iEstoque;
};

void exibirMenu();

void pedir(Produto*, int*);

void adicionar(Produto*, int*);

void excluir(Produto*, int*);

void listar(const Produto*, int);

void crescerVetor(Produto**, int*);

void reordenar(Produto*, int*, int);

void organizarPedidos(Produto*, int*);

bool analizarPedidos(Produto*, int, Produto*, int*);

void atualizarEstoque(Produto*, int, Produto*, int*);

void finalizarPrograma(Produto*, int);