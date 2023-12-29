#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>

#define INF 9999999

void print_msm(int len, int** G) {
    printf("матрица смежностей: \n");
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++)
        {
            printf("%d", G[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int** create_msm(int len) {
    int** m = (int**)malloc(len * sizeof(int*));
    for (int i = 0; i < len; i++) {
        m[i] = (int*)malloc(len * sizeof(int));
    }
    /*
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++)
        {
            if (i == j) {
                m[i][j] = 0;
            }
        }
    }
    */
    return m;
}

int** read_file(int** read_G) {
    FILE* read;
    read = fopen("input.txt", "r");
    if (getc(read) == EOF) { printf("Файл пустой"); exit(0); };
    if (read == NULL) { printf("Файла с таким названием не существует\n"); exit(0); }

    char* mas = (char*)malloc(10 * sizeof(char));
    int len = 0;

    while (!feof(read)) {
        fscanf(read, "%c", &mas[len]);
        len++;
        if (mas[len - 1] == ('\n')) {
            break;
        }
    }
    len = len / 2;
    fclose(read);
    read = fopen("input.txt", "r");

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            fscanf(read, "%d", &read_G[i][j]); 
        }
    }

    fclose(read);
    return read_G;
    
}

void save_in_file(int** G, int len, int* otvet) {

    int** mass = create_msm(len);
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            mass[i][j] = 0;
        }
    }

    int l = 1, i = 0, j = 0, n = 0;
    printf("%d", otvet[0]);
    while (l < otvet[0] - 1) {
        i = l;
        j = l + 1;
        n = l + 2;
        mass[otvet[i]][otvet[j]] = otvet[n];
        mass[otvet[j]][otvet[i]] = otvet[n];
        l = l + 3;
    }

    FILE* Save;
    Save = fopen("results.txt", "w");


    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            int ch = G[i][j];
            fprintf(Save, "%d", ch);
        }
        fprintf(Save, "\n");
    }

    fprintf(Save, "\n");
/*
    int ch = 3;
    for (int i = 1; i < otvet[0]; i++) {
        if (i == ch) {
            fprintf(Save, ":");
            ch += 3;
        }
        else if (i == ch - 1) {
            fprintf(Save, "-");
        }
        fprintf(Save, "%d", otvet[i]);

        if ( i % 3 == 0) {
            fprintf(Save, "\n");
        }
    };
    */
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            fprintf(Save,"%d",mass[i][j]);
        }
        fprintf(Save, "\n");
    }
    fclose(Save);
}

int** manual_input(int** G, int len) {
    int ch;
    int lastj = 0;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++)
        {
            if (i == j) {
                G[i][j] = 0;
            }
            else if (j >= lastj) {
                printf("Введите размер пути из %d в %d: ", i, j);
                if (scanf("%d", &ch) != 1) { printf("Вы ввели не число"); break; };


                G[i][j] = ch; 
                G[j][i] = ch; 
                lastj = i + 1;
            }
            if (j == len - 1) {
                printf("Массив: \n");
                for (int l = 0; l < i + 1; l++) {
                    for (int n = 0; n < j + 1; n++) {
                        printf("%d", G[l][n]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
        }
    }
    return G;
}

int* alg_prima(int len, int** G) {

    int** mass = create_msm(len);
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            mass[i][j] = 0;
        }
    }

    int* otvet = (int*)malloc(len * sizeof(int));

    int no_edge;           
    
    bool* selected = (bool*)malloc(len * sizeof(bool));

    
    memset(selected, false, sizeof(selected));
    
    no_edge = 0;

    selected[0] = true;

    int x;            
    int y;            
    int l = 1;


   // printf("связи минимального остового дерева:\n");
    //printf("грань : вес\n");
    while (no_edge < len - 1) {

        int min = INF;
        x = 0;
        y = 0;

        for (int i = 0; i < len; i++) {
            if (selected[i]) {
                for (int j = 0; j < len; j++) {
                    if (!selected[j] && G[i][j]) {
                        if (min > G[i][j]) {
                            min = G[i][j];
                            x = i;
                            y = j;
                        }

                    }
                }
            }
        }
        //printf("%d - %d : %d\n", x, y, G[x][y]);

        otvet[l] = x;
        l++;
        otvet[l] = y;
        l++;
        otvet[l] = G[x][y];
        l++;

        mass[x][y] = G[x][y];
        mass[y][x] = G[x][y];

        selected[y] = true;
        no_edge++;
    }

    int Flag = 0;
    int minimum = 9999999;
    for (int i = 0; i < len; i++) {
        Flag = 0;
        for (int j = 0; j < len; j++) {
            if (mass[i][j] != 0) {
                Flag = 1;
            }
        }
        if (Flag == 0) {
            for (int l = 0; l < len; l++) {
                if (mass[i][l] < minimum && mass[i][l] != 0) {
                    minimum = mass[i][l];
                    x = i;
                    y = l;
                }
            }
        }
    }

    printf("матрица смежностей минимального остового графа:\n");
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            printf("%d",mass[i][j]);

        }
        printf("\n");
    }
    printf("\n");
    otvet[0] = l;
    return otvet;
}

int** generate_msm(int len, int** m) {
    int lastj = 0;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++)
        {
            if (i == j){
                m[i][j] = 0; 
            }
            else if (j >= lastj) {
                int s = rand() % 10;
                m[i][j] = s; // заполняем массив случайными числами
                m[j][i] = s; // заполняем массив случайными числами
                int lastj = j;
            }
            
        }
    }
    return m;
}

int* menu(int len, int** G) {
    int ch;

    printf("Меню\n");
    printf("1-ввести матрица смежностей самостоятельно\n");
    printf("2-создать матрица смежностей автоматически\n");
    printf("3-скопировать матрив смежностей из файла\n");
    if (scanf("%d", &ch) != 1) { printf("Вы ввели не число"); };
    system("cls");

    switch (ch) {
    case 1:
        manual_input(G, len);
        break;
    case 2:
        generate_msm(len, G);
        print_msm(len, G);
        break;
    case 3:
        read_file(G);
        print_msm(len, G);
        break;
    default: printf("Вы ввели не число"); exit(0);
    }
    int* otvet = alg_prima(len, G);
    return otvet;
}

int main() {
    setlocale(LC_ALL, "Rus");

    int len;
    int ch;
    printf("\n\t\t\tКурсовая работа\nПо дисциплине Логика и основы алгоритмизации в инжинерных задачах\nНа тему : Реализация алгоритма Прима\"\n\nВыполнил студент группы 22ВВП1: Перфилов Александр\nПринял: к.э.н. доцент Акифьев И.В.\n");
    getchar();
    system("cls");

    while (true) {
        printf("Введите размер стороны матрицы смежностей: ");
        if (scanf("%d", &len) != 1) { printf("Вы ввели не число"); break; };
        system("cls");

        int** G = create_msm(len);

        int* otvet = menu(len, G);

        printf("1 - Сохранить результаты в файл\n0 - ввести новые данные\n");
        scanf("%d", &ch);

        switch (ch) {
        case 1:
            save_in_file(G, len, otvet);
            break;
        case 0:
            break;
        }
        system("cls");
    }
}