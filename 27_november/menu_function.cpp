#include "menu_function.hpp"
#include "main.hpp"
#include <iostream>

using namespace LAlex;

const MenuItem* LAlex::show_menu(const MenuItem* current) {
    std::cout << current->title << ":\n";
    for (int i = 0; i < current->children_count; ++i) {
        std::cout << current->children[i]->title << "\n";
    }
    
    int choice;
    std::cin >> choice;
    
    if (choice >= 0 && choice < current->children_count) {
        return current->children[choice];
    }
    
    return current;
}

const MenuItem* LAlex::exit(const MenuItem* current) {
    return nullptr;
}

const MenuItem* LAlex::study_matanaliz_func(const MenuItem* current) {
    std::cout << current->title << ":\n";
    for (int i = 0; i < current->children_count; ++i) {
        std::cout << current->children[i]->title << "\n";
    }

    int choice;
    std::cin >> choice;

    if (choice >= 0 && choice < current->children_count) {
        return current->children[choice];
    }

    return current;

}

const MenuItem* LAlex::study_algebra_func(const MenuItem* current) {
    std::cout << current->title << ":\n";
    for (int i = 0; i < current->children_count; ++i) {
        std::cout << current->children[i]->title << "\n";
    }

    int choice;
    std::cin >> choice;

    if (choice >= 0 && choice < current->children_count) {
        return current->children[choice];
    }

    return current;

}

const MenuItem* LAlex::algebra_first_func(const MenuItem* current) {
    std::cout << current->title << std::endl;
    return current->parent;

}

const MenuItem* LAlex::algebra_second_func(const MenuItem* current) {
    std::cout << current->title << std::endl;
    return current->parent;

}

const MenuItem* LAlex::matanaliz_first_func(const MenuItem* current) {
    std::cout << current->title << std::endl;
    return current->parent;

}

const MenuItem* LAlex::matanaliz_second_func(const MenuItem* current) {
    std::cout << current->title << std::endl;
    return current->parent;

}


const MenuItem* LAlex::study_go_back(const MenuItem* current) {
    return current->parent->parent;
}
