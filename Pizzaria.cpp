#include "Pizzaria.h"

void exibirMenu()
{
	cout << "\nSistema de Controle\n"
		<< "===================\n"
		<< "(P)edir\n"
		<< "(A)dicionar\n"
		<< "(E)xcluir\n"
		<< "(L)istar\n"
		<< "(S)air\n"
		<< "===================\n"
		<< "Op��o: [ ]\b\b";
}

void pedir(Produto* p, int* ptam)
{
	ifstream pedido;
	char arquivo[20];

	cout << "\nPedir\n-----\n";
	cout << "Arquivo: ";
	cin >> arquivo;

	pedido.open(arquivo);

	if (pedido.is_open())
	{
		char nome[20];
		int i{}, qtd, tam{};

		// l� quantos produtos tem no arquivo
		while (pedido.getline(nome, 20)) tam++;

		// cria um vetor de pedidos, fecha o arquivo e reabre para guardar as informa��es no vetor
		Produto* pedidos = new Produto[tam -= 2];
		pedido.close();
		pedido.open(arquivo);
		pedido.getline(nome, 20);
		pedido.getline(nome, 20);

		while (pedido >> nome && pedido >> qtd)
		{
			// define o primeiro caractere do nome para mai�sculo
			nome[0] = toupper(nome[0]);

			// define os demais caracteres para min�sculo
			for (int j = 1; nome[j]; j++)
				nome[j] = tolower(nome[j]);

			// armazena as informa��es j� formatadas no vetor de pedidos
			strcpy(pedidos[i].nome, nome);
			pedidos[i++].quantidade = qtd;
		}
		pedido.close();

		// passa o vetor para uma fun��o que verifica se h� produtos repetidos e organiza o vetor
		organizarPedidos(pedidos, &tam);

		// verifica se os produtos no estoque atendem os pedidos, a fun��o retorna true se o pedido falhar
		bool falha = analizarPedidos(pedidos, tam, p, ptam);

		// grava as informa��es do vetor de pedidos no arquivo de recibo se n�o houver falhas de pedido
		if (!falha)
		{
			ofstream recibo;
			char nomeRecibo[20]{};

			// copia o nome do arquivo de pedido at� o ponto e adiciona .nfc no final
			for (int i = 0; arquivo[i] != '.'; i++)
				nomeRecibo[i] = arquivo[i];
			strcat(nomeRecibo, ".nfc");

			// gera o arquivo de recibo
			recibo.open(nomeRecibo);
			recibo.precision(2);
			recibo << fixed << "Pizzaria Mamute\n--------------------------------------------------\n";

			float compra{}, desconto{}, total{};
			int j{ pedidos[0].iEstoque };
			for (i = 0; i < tam; i++, j = pedidos[i].iEstoque)
			{
				compra += pedidos[i].quantidade * p[j].preco;
				recibo.width(11); recibo << left << pedidos[i].nome;
				recibo.width(2); recibo << right << pedidos[i].quantidade << "kg    a    "
					<< p[j].preco; recibo << "/kg" << (p[j].preco < 10.0f ? "  " : " ") << "   =    R$"
					<< pedidos[i].quantidade * p[j].preco << endl;
			}
			recibo << "--------------------------------------------------\n";
			recibo.width(32); recibo << right << "Compra" << "    =    R$" << compra << endl;
			recibo.width(32); recibo << "Desconto" << "    =    R$" << (desconto = (compra >= 1000 ? 0.1f * compra : 0.0f)) << endl;
			recibo.width(32); recibo << "Total" << "    =    R$" << compra - desconto;
			recibo.close();

			// chama a fun��o para atualizar o estoque
			atualizarEstoque(pedidos, tam, p, ptam);

			cout << "Opera��o realizada com sucesso.\n";
		}
	}
	else
		cout << "\nO arquivo de pedido \"" << arquivo << "\" n�o p�de ser aberto.\n";
	cout << endl;
}

void adicionar(Produto* p, int* qtd)
{
	char nome[20];
	float preco;
	int quantidade;

	cout << "\nAdicionar\n---------\n";
	cout << "Produto: ";    cin >> nome;
	cout << "Pre�o: ";      cin >> preco;
	cout << "Quantidade: "; cin >> quantidade;

	nome[0] = toupper(nome[0]);
	for (int i = 1; nome[i]; i++)
		nome[i] = tolower(nome[i]);

	// verifica se o produto a adicionar j� existe no estoque, se existir, s� atualiza os dados
	bool existe = false;
	for (int i = 0; i < (*qtd); i++)
	{
		if (!strcmp(p[i].nome, nome))
		{
			existe = true;
			p[i].preco = preco;
			p[i].quantidade += quantidade;
		}
	}

	// caso n�o exista, adiciona o produto ao final do vetor de estoque
	if (!existe)
	{
		strcpy(p[*qtd].nome, nome);
		p[*qtd].preco = preco;
		p[*qtd].quantidade = quantidade;
		(*qtd)++;
	}

	cout << "\nProduto adicionado ao estoque com sucesso.\n" << endl;
}

void excluir(Produto* p, int* qtd)
{
	cout << "\nExcluir\n-------\n";
	for (int i = 0; i < *qtd; i++)
		cout << i + 1 << ") " << p[i].nome << endl;

	int	num;
	char ch;
	cout << "\nN�mero do produto: "; cin >> num;
	cout << "Deseja excluir \"" << p[num - 1].nome << "\" (S/N)? "; cin >> ch;

	if (ch == 'S' || ch == 's')
	{
		reordenar(p, qtd, num); // fun��o que exclui o produto do vetor e organiza as posi��es dos demais produtos
		cout << "\nProduto exclu�do com sucesso\n";
	}
	else
		cout << "\nOpera��o cancelada.\n";
	cout << endl;
}

void listar(const Produto* p, int qtd)
{
	cout.precision(2);
	cout << fixed << "\nListagem\n--------\n";

	// ajusteNome e ajustePreco para controlar a disposi��o dos itens na exibi��o
	int ajusteNome{}, ajustePreco{ int(p[0].preco) }, tamanho;
	for (int i = 0; i < qtd; i++)
	{
		// quantos caracteres tem cada nome de produto, verifica qual � o nome com mais caracteres
		tamanho = int(strlen(p[i].nome));
		if (ajusteNome <= tamanho) ajusteNome = tamanho + 1;

		// verifica qual o maior pre�o
		if (i < qtd - 1 && ajustePreco < p[i + 1].preco) ajustePreco = int(p[i + 1].preco);
	}

	// exibe as informa��es formatadas na tela
	for (int i = 0; i < qtd; i++)
	{
		cout.width(ajusteNome); cout << left << p[i].nome << "- ";
		cout << "R$"; cout.width((ajustePreco < 10 ? 5 : ajustePreco < 100 ? 6 : 7)); // verifica quantos d�gitos tem o pre�o
		cout << left << p[i].preco << "- ";
		cout << p[i].quantidade << "kg\n";
	}
	cout << endl;
}

void crescerVetor(Produto** p, int* tam)
{
	static int n; // vari�vel est�tica que guarda quantas vezes o vetor foi expandido
	// tamanho para o novo vetor
	int novoTam = (*tam) + int(pow(2, n++));

	// aloca uma nova mem�ria com o novo tamanho apropriado
	Produto* novo = new Produto[novoTam]{};

	// faz a c�pia dos valores da mem�ria do vetor principal para a nova mem�ria
	for (int i = 0; i < (*tam); i++)
	{
		strcpy(novo[i].nome, (*p)[i].nome);
		novo[i].preco = (*p)[i].preco;
		novo[i].quantidade = (*p)[i].quantidade;
	}

	// libera a mem�ria do vetor principal
	delete[](*p);
	*p = novo; // agora p aponta para a nova mem�ria alocada

	(*tam) = novoTam; // atualiza o tamanho do vetor
}

void reordenar(Produto* p, int* qtd, int pos)
{
	// verifica se o produto a ser escolhido � o �ltimo produto do vetor
	if (pos != (*qtd))
		for (int i = pos - 1; i < (*qtd) - 1; i++)
			p[i] = p[i + 1];

	// atribui valores nulos para o �ltimo produto "excluindo" ele e diminui a quantidade de produtos
	p[(*qtd) - 1] = {};
	(*qtd)--;
}

void organizarPedidos(Produto* p, int* tam)
{
	for (int i = 0; i < (*tam) - 1; i++)
		for (int j = i + 1; j < (*tam); j++)
			if (!strcmp(p[i].nome, p[j].nome))
			{
				p[i].quantidade += p[j].quantidade;
				reordenar(p, tam, j + 1);
			}
}

bool analizarPedidos(Produto* pedido, int qtd, Produto* estoque, int* tam)
{
	bool falha = false;
	for (int i = 0; i < qtd; i++)
	{
		bool disp = false; // vari�vel que verifica se um pedido est� dispon�vel no estoque
		for (int j = 0; j < (*tam); j++)
		{
			if (!strcmp(pedido[i].nome, estoque[j].nome))
			{
				if (estoque[j].quantidade < pedido[i].quantidade)
				{
					if (!falha)
					{
						cout << "\nPedido falhou!\n";
						falha = true;
					}
					cout << pedido[i].nome << ": Solicitado = " << pedido[i].quantidade
						<< "kg / Em estoque = " << estoque[j].quantidade << "kg" << endl;
					falha = true;
				}
				pedido[i].iEstoque = j; // �ndice correspondente do produto de pedido no estoque
				disp = true;
			}
		}
		if (!disp)
		{
			if (!falha)
			{
				cout << "\nPedido falhou!\n";
				falha = true;
			}
			cout << "Produto " << pedido[i].nome << " n�o dispon�vel no estoque.\n";
		}
	}
	return falha;
}

void atualizarEstoque(Produto* pedido, int qtd, Produto* estoque, int* tam)
{
	for (int i = 0; i < qtd; i++)
		for (int j = 0; j < (*tam); j++)
			if (!strcmp(pedido[i].nome, estoque[j].nome))
			{
				estoque[j].quantidade -= pedido[i].quantidade;
				if (estoque[j].quantidade == 0) // se ap�s atualizar o estoque o produto se esgotar, exclui ele do vetor
					reordenar(estoque, tam, j + 1);
			}
}

void finalizarPrograma(Produto* p, int qtd)
{
	ofstream estoque;
	estoque.open("estoque.bin", ios_base::out | ios_base::binary);

	// escreve o tamanho e o vetor de produtos no arquivo de estoque
	estoque.write((char*)&qtd, sizeof(int));
	estoque.write((char*)p, sizeof(Produto) * qtd);

	estoque.close();
}