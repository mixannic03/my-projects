#include <iostream>
#include <cstdlib>
#pragma warning(disable : 4996)

char emess[15];
char syndata[4];

void Encode(char* input)
{
    emess[0] = input[0] ^ input[1] ^ input[3] ^ input[4] ^ input[6] ^ input[8] ^ input[10];
    emess[1] = input[0] ^ input[1] ^ input[3] ^ input[5] ^ input[6] ^ input[9] ^ input[10];
    emess[3] = input[1] ^ input[2] ^ input[3] ^ input[7] ^ input[8] ^ input[9] ^ input[10];
    emess[7] = input[4] ^ input[5] ^ input[6] ^ input[7] ^ input[8] ^ input[9] ^ input[10];

    emess[2] = input[0];
    emess[4] = input[1];
    emess[5] = input[2];
    emess[6] = input[3];
    emess[8] = input[4];
    emess[9] = input[5];
    emess[10] = input[6];
    emess[11] = input[7];
    emess[12] = input[8];
    emess[13] = input[9];
    emess[14] = input[10];
}

int Decode(char* dimess)
{
    syndata[0] = dimess[0] ^ dimess[2] ^ dimess[4] ^ dimess[6] ^ dimess[8] ^ dimess[10] ^ dimess[12] ^ dimess[14];
    syndata[1] = dimess[1] ^ dimess[2] ^ dimess[5] ^ dimess[6] ^ dimess[9] ^ dimess[10] ^ dimess[13] ^ dimess[14];
    syndata[2] = dimess[3] ^ dimess[4] ^ dimess[5] ^ dimess[6] ^ dimess[11] ^ dimess[12] ^ dimess[13] ^ dimess[14];
    syndata[3] = dimess[7] ^ dimess[8] ^ dimess[9] ^ dimess[10] ^ dimess[11] ^ dimess[12] ^ dimess[13] ^ dimess[14];
    if (!(syndata[0] || syndata[1] || syndata[2] || syndata[3]))
        return 0;
    else
    {
        char pos = 0;
        if (syndata[3])
        {
            pos |= 1;
            pos = pos << 1;
        }
        if (syndata[2])
        {
            pos |= 1;
            pos = pos << 1;
        }
        if (syndata[1])
        {
            pos |= 1;
            pos = pos << 1;
        }
        if (syndata[0])
        {
            pos |= 1;
        }
        return pos;
    }
}


int main()
{
    setlocale(LC_ALL, "Russian");
    char input[12];
    char revinput[12];
    char dmess[16];
    char inp[12];
    char simb[2];
    int posi, in, in1;
    int k = 0, count = 0;
    int n = 0;
    int j = 0;

    std::cout << "Введите кодируемое число, меньше 200\n";
    while (!k) {
        std::cin >> in;

        if (in > 200)
        {
            std::cout << "Введенное вами число больше 200\n";
            k = 0;
        }
        else
        {
            k = 1;
        }
    }
    k = 0;
    while (in > 0)
    {
        itoa(in % 2, simb, 10);
        revinput[k] = simb[0];
        in = in / 2;
        k++;
    }

    for (int j = k - 1; j >= 0; j--)
    {
        input[n] = revinput[j];
        n++;
    }

    k = 0;
    n = 0;
    for (int i = 0; input[i] != -52; i++)
    {
        k += 1;
    }
    n = k;
    for (int i = 0; 11 - k > 0; i++, k++)
    {
        inp[i] = '0';
    }
    int nuli = 11 - n;
    for (nuli; nuli < 11; nuli++)
    {
        inp[nuli] = input[j];
        j++;
    }
    std::cout << "Кодируемое число в двоичном виде ";
    for (int i = 0; inp[i] != -52; i++)
    {
        std::cout << inp[i];
    }
    std::cout << "\n";
    Encode(inp);
    std::cout << "Закодированное число:" << emess << "\n";
    std::cout << "Введите декодируемое двоичное число\n";
    std::cin >> dmess;
    for (int i = 0; i < 11; i++)
    {
        if (emess[i] != dmess[i])
            count += 1;
    }
    if (count > 1)
    {
        std::cout << "Невозможно корректно декодировать: в сообщении более одной ошибки\n";
        return 0;
    }
    posi = Decode(dmess);
    std::cout << "Ошибка была на позиции:" << posi << "\n";
    return 0;
}
