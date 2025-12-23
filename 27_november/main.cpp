#include <iostream>
#include "main.hpp"
#include "menu_function.hpp"

using namespace LAlex;

int main() {
    // Пункты меню матанализа
    MenuItem matanaliz_first = {"1 - Первый билет по матанализу", matanaliz_first_func, nullptr, nullptr, 0};
    MenuItem matanaliz_second = {"2 - Второй билет по матанализу", matanaliz_second_func, nullptr, nullptr, 0};

    // Пункты меню алгебры
    MenuItem algebra_first = {"1 - Первый билет по алгебре", algebra_first_func, nullptr, nullptr, 0};  // Исправлено название предмета
    MenuItem algebra_second = {"2 - Второй билет по алгебре", algebra_second_func, nullptr, nullptr, 0}; // Исправлено название предмета

    
    MenuItem* matanaliz_children[] = {&matanaliz_first, &matanaliz_second};
    MenuItem* algebra_children[] = {&algebra_first, &algebra_second};

    // Элементы меню среднего уровня
    MenuItem study_go_back_item = {"0 - Выйти в главное меню", study_go_back, nullptr, nullptr, 0};
    
    // Элементы с привязкой дочерних массивов
    MenuItem study_matanaliz = {"1 - Хочу научиться матанализу", show_menu, nullptr, matanaliz_children, 2}; // Добавлены дети
    MenuItem study_algebra = {"2 - Хочу научиться алгебре", show_menu, nullptr, algebra_children, 2};         // Добавлены дети

    // Массив для раздела
    MenuItem* study_children[] = {
        &study_go_back_item,
        &study_matanaliz,
        &study_algebra
    };

    // Главное меню 
    MenuItem exit_item = {"0 - Лучше пойду полежу", exit, nullptr, nullptr, 0};
    MenuItem study_item = {"1 - Хочу учиться математике", show_menu, nullptr, study_children, 3}; // Добавлены дети

    MenuItem* main_children[] = {&exit_item, &study_item};
    MenuItem main_item = {"Главное меню", show_menu, nullptr, main_children, 2};

    // Настраиваем связи родитель-потомок
    // Для главного меню
    exit_item.parent = &main_item;
    study_item.parent = &main_item;

    // Для раздела обучения
    study_go_back_item.parent = &study_item;
    study_matanaliz.parent = &study_item;
    study_algebra.parent = &study_item;

    // Для подразделов
    matanaliz_first.parent = &study_matanaliz;
    matanaliz_second.parent = &study_matanaliz;
    algebra_first.parent = &study_algebra;
    algebra_second.parent = &study_algebra;

    // Запускаем меню
    const MenuItem* current = &main_item;
    while (current != nullptr) {
        current = current->func(current);
    }

    return 0;
}
