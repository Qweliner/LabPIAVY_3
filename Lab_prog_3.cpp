﻿// Lab_prog_3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Lab_prog_header_3.h"

int main()
{
    // Устанавливаем кодировку консоли для корректного отображения кириллицы
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Устанавливаем изначальный размер окна консольного приложения
    system("mode con cols=120 lines=30");

    // Вызываем главную функцию меню
    menu();

    return 0;
}

//
//  TO DO
//
// Интерфейс
// Придумать алгоритм при котором можно работать с бесконечным файлом - РЕАЛИЗОВАНО ЧЕРЕЗ ПОСТРОЧНОЕ ЧТЕНИЕ
// Сделать защиту от дурака - ЧАСТИЧНО РЕАЛИЗОВАНО
// Проверка правильности входа данных - ЧАСТИЧНО РЕАЛИЗОВАНО (ПРОВЕРКА ДАТЫ)
// Сортировка методом выбора - РЕАЛИЗОВАНО
// При сортировке и находятся пустые поля в сортируемом "столбце"- отдельно внизу их вывести - РЕАЛИЗОВАНО
// Сортировка от а до я, от 1 до 999, от 1.1.1 до 31.12.2025, направление выбирает пользователь - РЕАЛИЗОВАНО
// Результат сортировки каждый раз должен выводится в новый текстовый файл, желательно автоматически
// названный, желательно чтобы в названии файла отображалась информация какая сортировка идет,
// в самом файле надо вывести сортировка по такому - то полю в таком - то направлении - РЕАЛИЗОВАНО
//
// Если текущий путь не существует или путь еще не введен то на экране главного меню писать что путь не выбран - РЕАЛИЗОВАНО
// Без дублированного кода(минимально дублированного кода) - ЧАСТИЧНО РЕАЛИЗОВАНО
// Функции должны по хорошему умещаться на экране - ПО ВОЗМОЖНОСТИ
// Вторичное упорядочивание не требуется, мы ограничимся одним полем - РЕАЛИЗОВАНО
// При повторной сортировке того что уже отсортировано спрашивать надо ли делать уже сделанное - ТРЕБУЕТСЯ РЕАЛИЗАЦИЯ
// Сделать интерфейс красивым - ПО ВОЗМОЖНОСТИ
//
//  DONE
//
//