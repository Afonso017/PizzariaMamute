#include "Pizzaria.h"

int main()
{
	system("chcp 1252 > nul");

	// ponteiro para o vetor dinâmico de produtos
	Produto* produtos;
	ifstream estoque;

	estoque.open("estoque.bin", ios_base::in | ios_base::binary);

	// inicializa as variáveis de tamanho, e quantidade de elementos
	int tam{ 1 }, qtd{};

	// verifica se o arquivo de estoque abriu normalmente e lê os produtos do estoque para o vetor
	if (estoque.is_open())
	{
		estoque.read((char*)&qtd, sizeof(int));
		produtos = new Produto[qtd];

		estoque.read((char*)produtos, sizeof(Produto) * qtd);
		tam = qtd;
	}
	else
	{
		cout << "O arquivo de estoque ainda não existe, ao finalizar o programa o arquivo será criado.\n";
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
				// função para crescer o vetor
				crescerVetor(&produtos, &tam);
			}
			adicionar(produtos, &qtd);
			system("pause");
			system("cls");
			break;

		case 'E':
		case 'e':

			// exclui algum produto do estoque se não estiver vazio
			if (qtd > 0)
				excluir(produtos, &qtd);
			else
				cout << "\nO estoque está vazio, não é possível excluir nenhum item.\n";

			system("pause");
			system("cls");
			break;

		case 'L':
		case 'l':

			// lista se houver ao menos um produto no estoque
			if (qtd > 0)
				listar(produtos, qtd);
			else
				cout << "\nNão há produtos no estoque, adicione produtos primeiro.\n";

			system("pause");
			system("cls");
			break;

		case 'S':
		case 's':

			// escreve os produtos do vetor dinâmico no arquivo de estoque e finaliza o programa
			finalizarPrograma(produtos, qtd);

			delete[] produtos;

			cout << "\nPrograma encerrado.\n" << endl;
			system("pause");
			return 0;

		default:

			system("cls");
			cout << "\nOpção inválida, tente novamente\n";
			break;
		}
	}
}