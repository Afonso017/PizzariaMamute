#include "Pizzaria.h"

int main()
{
	system("chcp 1252 > nul");

	// ponteiro para o vetor din�mico de produtos
	Produto* produtos;
	ifstream estoque;

	estoque.open("estoque.bin", ios_base::in | ios_base::binary);

	// inicializa as vari�veis de tamanho, e quantidade de elementos
	int tam{ 1 }, qtd{};

	// verifica se o arquivo de estoque abriu normalmente e l� os produtos do estoque para o vetor
	if (estoque.is_open())
	{
		estoque.read((char*)&qtd, sizeof(int));
		produtos = new Produto[qtd];

		estoque.read((char*)produtos, sizeof(Produto) * qtd);
		tam = qtd;
	}
	else
	{
		cout << "O arquivo de estoque ainda n�o existe, ao finalizar o programa o arquivo ser� criado.\n";
		produtos = new Produto[tam]{};
	}
	estoque.close();

	char opc;
	while (true)
	{
		exibirMenu();
		cin >> opc;
		switch (opc)
		{
		case 'P':
		case 'p':

			pedir(produtos, &qtd);
			system("pause");
			system("cls");
			break;

		case 'A':
		case 'a':

			if (tam <= qtd)
			{
				// fun��o para crescer o vetor
				crescerVetor(&produtos, &tam);
			}
			adicionar(produtos, &qtd);
			system("pause");
			system("cls");
			break;

		case 'E':
		case 'e':

			// exclui algum produto do estoque se n�o estiver vazio
			if (qtd > 0)
				excluir(produtos, &qtd);
			else
				cout << "\nO estoque est� vazio, n�o � poss�vel excluir nenhum item.\n";

			system("pause");
			system("cls");
			break;

		case 'L':
		case 'l':

			// lista se houver ao menos um produto no estoque
			if (qtd > 0)
				listar(produtos, qtd);
			else
				cout << "\nN�o h� produtos no estoque, adicione produtos primeiro.\n";

			system("pause");
			system("cls");
			break;

		case 'S':
		case 's':

			// escreve os produtos do vetor din�mico no arquivo de estoque e finaliza o programa
			finalizarPrograma(produtos, qtd);

			delete[] produtos;

			cout << "\nPrograma encerrado.\n" << endl;
			system("pause");
			return 0;

		default:

			system("cls");
			cout << "\nOp��o inv�lida, tente novamente\n";
			break;
		}
	}
}