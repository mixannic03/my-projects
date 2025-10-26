#include <iostream>
#include <cstdlib>


#define N 7
#define K 4
#define NK (N-K)

unsigned int g[] = { 1,0,1,1 };
unsigned int syn[N][NK];
unsigned int V[N][N];
unsigned int G[K][N] = { {0,0,0,1,0,1,1},{0,0,1,0,1,1,0},{0,1,0,1,1,0,0},{1,0,1,1,0,0,0} }; // Порождающая матрица
unsigned int H[NK][N] = { {1,1,1,0,1,0,0},{0,1,1,1,0,1,0},{0,0,1,1,1,0,1} }; // Проверочная матрица
unsigned int syndrom[NK];
unsigned int mess[K] = {0,1,1,0}; // Сообщение содержащее информацию
unsigned int cod[N]; // Закодированное сообщение
unsigned int cod1[N] = {0,0,1,1,0,1,0}; // Пришедшее сообщение с ошибкой

void setArray(unsigned int* pArray, unsigned int size)
{
	memset(pArray, 0, size);
}


void showMass(unsigned int* pArray, unsigned int size)
{
	for (int i = 0; i < size; i++)
	{
		std::cout << pArray[i] << " ";
	}
}

void Encode(unsigned int* message)
{
	for (int j = 0; j < N; j++)
	{
		cod[j] = message[0] * G[0][j] ^ message[1] * G[1][j] ^ message[2] * G[2][j] ^ message[3] * G[3][j];
	}
}

void CreateV()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (j == 6 - i)
				V[i][j] = 1;
			else
				V[i][j] = 0;
		}
	}
}

void CreateS(unsigned int* Vec, unsigned int* Synd)
{
	for (int i = 0; i < NK; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Synd[i] ^= H[i][j] * Vec[j];
		}
	}
}

void Decode(unsigned int* dec, unsigned int* Synd)
{
	for (int i = 0; i < NK; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Synd[i] ^= H[i][j] * dec[j];
		}
	}
}


int main()
{
	int pos = 0;
	setlocale(LC_ALL, "Russian");
	std::cout << "Порождающая матрица G:\n";
	for (int i = 0; i < N; i++)
	{
		setArray(syn[i], NK);
	}
	for (int i = 0; i < K; i++)
	{
		showMass(G[i], N);
		std::cout << "\n";
	}
	std::cout << "Проверочная матрица H:\n";
	for (int i = 0; i < NK; i++)
	{
		showMass(H[i], N);
		std::cout << "\n";
	}
	std::cout << "Информационное сообщение:";
	showMass(mess, K); std::cout << "\n";
	Encode(mess);
	std::cout << "Закодированное сообщение:";
	showMass(cod, N); std::cout << "\n";
	CreateV();
	
	for (int i = 0; i < N; i++)
	{
		CreateS(V[i], syn[i]);
	}
	std::cout << "Таблица векторов и синдромов:\n";
	for (int i = 0; i < N; i++)
	{
		showMass(V[i], N);
		showMass(syn[i], NK);
		std::cout << "\n";
	}
	std::cout << "Сообщение с ошибкой:\n";
	showMass(cod1, N); std::cout << "\n";
	Decode(cod1, syndrom);

	for (int i = 0; i < N; i++)
	{
		int count = 0;
		for (int j = 0; j < NK; j++)
		{
			if (syndrom[j] == syn[i][j])
				count++;
		}
		if (count == 3)
		{
			pos = i;
			std::cout << "Ошибка на позиции:" << N - i << "\n";
		}
	}
	std::cout << "Исправленное сообщение:\n";
	for (int i = 0; i < N; i++)
	{
		cod[i] = V[pos][i] ^ cod1[i];
	}
	showMass(cod, N); std::cout << "\n";
	return 0;
}