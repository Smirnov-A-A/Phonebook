#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "phonebook.h"

#define N 101


//Структуры с индексами N-1 и N-2 являются служебными и не предназначены для хранения контактов
//поля N-2 всегда инициализировыны нулями для использования в функциях сортировки и удаления контактов
//N-1 используется для временного хранения нового контакта до выполнения функции сортировки

//Функция добавления новой записи
int new_rec(struct contact *pAbonent, int *pLast_rec) {

    //проверка свободного места в справочнике
    if (*pLast_rec == N-3) {
        printf("Список контактов заполнен, добавление новой записи невозможно!\n");
        return 1;
    }

    //добавление новой записи
    getchar();

    printf("\nВведите фамилию, имя, отчество: ");
    if (*pLast_rec == 0) {
        gets(pAbonent[0].name);
    }
    else gets(pAbonent[N-1].name);

    printf("Введите номер телефона: ");
    if (*pLast_rec == 0) {
        gets(pAbonent[0].phone_number);
    }
    else gets(pAbonent[N-1].phone_number);
    printf("\nНовый контакт добавлен!\n");

    //увеличиваем количество контактов на 1
    ++(*pLast_rec);

    return 0;
}


//Функция удаления записи
int del_rec(struct contact *pAbonent, int *pLast_rec) {

    int del_num = 0;
    int last_rec = *pLast_rec;
    do {
        printf("\nВыберите контакт, который хотите удалить (для возврата в меню нажмите ноль): ");
        scanf("%d", &del_num);
        if (del_num < 0 || del_num > last_rec) {
            printf("Такого контакта нет!");
        }
    }
    while (del_num < 0 || del_num > last_rec);

    if (del_num == 0)
        return 0;

    getchar();

    if (del_num == last_rec) {
        pAbonent[del_num-1] = pAbonent[N-2];
    }
    else {
        for (int i = del_num; i < last_rec; ++i) {
            pAbonent[i-1] = pAbonent[i];
        }
        pAbonent[last_rec-1] = pAbonent[N-2];
    }
    //уменьшаем количество контактов на 1
    --(*pLast_rec);

    printf("\nКонтакт удален!\n");

    return 0;
}


//Функция сортировки по алфавиту
int sort_contacts(struct contact *pAbonent, int last_rec) {

    if (last_rec == 0 || last_rec == 1)
        return 0;

    int j = 0;

    for ( ; j < last_rec; ++j) {
        int i = 0;
        char str1[100] = {0};
        char str2[100] = {0};

        strcpy(str1, pAbonent[N-1].name);
        strcpy(str2, pAbonent[j].name);

        while (tolower(str1[i]) >= tolower(str2[i]) && str1[i] != '\0' && str2[i] != '\0') {

            if (i == 0 && tolower(str1[i]) > tolower(str2[i]))
                break;
            if (i != 0 && tolower(str1[i]) > tolower(str2[i]))
                break;
            //printf("%c %c\n", pAbonent[N-1].name[i], pAbonent[j].name[i]);
            ++i;
        }

        if (tolower(str1[i]) < tolower(str2[i])) {
            break;
        }
    //printf("j = %d, last_rec = %d",  j,  last_rec);
    }

    if (j == last_rec) {
        pAbonent[j-1] = pAbonent[N-1];
    }
    else {
        //сдвигаем записи на 1 вниз, начиная с последней
        for (int i = last_rec + 1; i > j; --i) {
            //printf("i = %d\n", i);
            pAbonent[i] = pAbonent[i-1];
        }
        pAbonent[j] = pAbonent[N-1];
    }
    pAbonent[N-1] = pAbonent[N-2];

return 0;
}


//Функция записи в файл
int write_data(const char *filename, const struct contact *pAbonent, int *plast_rec) {

    if (*plast_rec == 0)
        return 0;

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Ошибка открытия файла contacts.txt для записи!\n");
        exit(1);
    }

    int size = sizeof(pAbonent[0]) * (*plast_rec);
    size_t written = fwrite(pAbonent, size, *plast_rec, fp);
    //printf("written_num = %d\n", written);

    fclose(fp);

    return 0;
}


//функция чтения из файла
int read_data(const char *filename, struct contact *pAbonent, int *plast_rec) {

    if (*plast_rec == 0) {
        return 0;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Ошибка открытия файла contacts.txt!\n");
        exit(1);
    }

    int size = sizeof(pAbonent[0]) * (*plast_rec);
    //printf("size_func = %d\n", size);

    size_t read = fread(pAbonent, size, *plast_rec, fp);
    //printf("num_of_struct_read = %d\n", read);

    fclose(fp);

    return 0;
}


//Функция вывода контактов на экран
void show_contacts(const struct contact *pAbonent, int last_rec) {

    if (last_rec == 0) {
        printf("Список контактов пуст!\n");
        return;
    }
    printf("\nСписок контактов:\n");
    for (int i = 0; i < last_rec; ++i) {
        printf("%d\t %-50s\t %s\n", i+1, pAbonent[i].name, pAbonent[i].phone_number);
    }
}


//Функция записи количества контактов в файл
void wr_num_c(const char *filename2, int *pLast_rec) {

    FILE *fp = fopen(filename2, "w");
    if (fp == NULL) {
        perror("Ошибка открытия файла num_of_contacts.txt для записи!\n");
        exit(1);
    }

    fprintf(fp, "%d", *pLast_rec);
    fclose(fp);
}


//Функция чтения количества контактов из файла
void read_num_c(const char *filename2, int *pLast_rec) {

    FILE *fp = fopen(filename2, "r");
    if (fp == NULL) {
        perror("Ошибка открытия файла num_of_contacts!\n");
        exit(1);
    }

    fscanf(fp, "%d", pLast_rec);
    fclose(fp);
}


//Функция меню
void menu() {

    printf("Меню телефонного справочника:\n");
    printf("1 - Вывести на экран список контактов\n");
    printf("2 - Добавить новый контакт\n");
    printf("3 - Удалить контакт\n");
    printf("4 - Найти контакт\n");
    printf("5 - Завершить работу\n");
}


int main() {

    int op_num = 0;                             //номер операции

    int last_rec = 0;                           //номер последней записи в справочнике
    int *pLast_rec = &last_rec;

    char *filename = "contacts.txt";            //файл для хранения контактов
    char *filename2 = "num_of_contacts.txt";    //файл для хранения количества контактов

    struct contact abonent[N] = {{"0", "0"}};   //массив структур
    struct contact *pAbonent = abonent;         //указатель на массив структур, но не на структуру!!!

    read_num_c(filename2, pLast_rec);
    read_data(filename, pAbonent, pLast_rec);

    menu();
    do {
        do {
            printf("\nВыберите пункт меню от 1 до 5: ");
            scanf("%d", &op_num);
            if (op_num < 1 || op_num > 5) {
                printf("Такой операции нет!");
            }
        }
        while (op_num < 1 || op_num > 5);

        switch (op_num) {

            case 1:
                show_contacts(pAbonent, last_rec);
                break;

            case 2:
                if (new_rec(pAbonent, pLast_rec) == 1) {
                    break;
                }
                else {
                    sort_contacts(pAbonent, last_rec);
                    write_data(filename, pAbonent, pLast_rec);
                }
                break;

            case 3:
                show_contacts(pAbonent, last_rec);
                del_rec(pAbonent, pLast_rec);
                write_data(filename, pAbonent, pLast_rec);
                break;

            case 4:
                printf("\nФункция поиска в стадии разработки (: \n");
                break;

            case 5:
                printf("\nРабота со справочником завершена.\n");
                break;
        }
    }

    while (op_num != 5);

    wr_num_c(filename2, pLast_rec);

    return 0;
}
