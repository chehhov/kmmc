#include "../src/long_number.hpp"
#include <iostream>
#include <cassert>

using namespace lae;


void test_addition() {
    
    // Тест 1: Сложение двух положительных чисел
    LongNumber num1("123");
    LongNumber num2("456");
    LongNumber result = num1 + num2;
    LongNumber expected1("579");
    assert(result == expected1);
    
    // Тест 2: Сложение с переходом через разряд
    LongNumber num3("999");
    LongNumber num4("1");
    result = num3 + num4;
    LongNumber expected2("1000");
    assert(result == expected2);
    
    // Тест 3: Сложение чисел разной длины
    LongNumber num5("12345");
    LongNumber num6("678");
    result = num5 + num6;
    LongNumber expected3("13023");
    assert(result == expected3);
    
    // Тест 4: Сложение с нулём
    LongNumber num7("987654321");
    LongNumber num8("0");
    result = num7 + num8;
    LongNumber expected4("987654321");
    assert(result == expected4);
    
    // Тест 5: Сложение двух больших чисел
    LongNumber num9("99999999999999999999");
    LongNumber num10("11111111111111111111");
    result = num9 + num10;
    LongNumber expected5("111111111111111111110");
    assert(result == expected5);
    
    // Тест 6: Сложение отрицательного и положительного
    LongNumber num11("-123");
    LongNumber num12("456");
    result = num11 + num12;
    LongNumber expected6("333");
    assert(result == expected6);
    
    // Тест 7: Сложение двух отрицательных чисел
    LongNumber num13("-123");
    LongNumber num14("-456");
    result = num13 + num14;
    LongNumber expected7("-579");
    assert(result == expected7);
    
    // Тест 8: Сложение с обратным элементом
    LongNumber num15("100");
    LongNumber num16("-100");
    result = num15 + num16;
    LongNumber expected8("0");
    assert(result == expected8);
    
    std::cout << "Все тесты сложения пройдены" << std::endl;
}


void test_subtraction() {

    // Тест 1: Вычитание положительных чисел
    LongNumber num1("500");
    LongNumber num2("200");
    LongNumber result = num1 - num2;
    LongNumber expected1("300");
    assert(result == expected1);
    
    // Тест 2: Вычитание с заёмом
    LongNumber num3("1000");
    LongNumber num4("1");
    result = num3 - num4;
    LongNumber expected2("999");
    assert(result == expected2);
    
    // Тест 3: Результат вычитания отрицательный
    LongNumber num5("100");
    LongNumber num6("200");
    result = num5 - num6;
    LongNumber expected3("-100");
    assert(result == expected3);
    
    // Тест 4: Вычитание равных чисел
    LongNumber num7("12345");
    LongNumber num8("12345");
    result = num7 - num8;
    LongNumber expected4("0");
    assert(result == expected4);
    
    // Тест 5: Вычитание отрицательных чисел
    LongNumber num9("-100");
    LongNumber num10("-50");
    result = num9 - num10;
    LongNumber expected5("-50");
    assert(result == expected5);
    
    // Тест 6: Вычитание: минус на плюс
    LongNumber num11("-100");
    LongNumber num12("50");
    result = num11 - num12;
    LongNumber expected6("-150");
    assert(result == expected6);
    
    // Тест 7: Вычитание: плюс на минус
    LongNumber num13("100");
    LongNumber num14("-50");
    result = num13 - num14;
    LongNumber expected7("150");
    assert(result == expected7);
    
    // Тест 8: Вычитание с нулём
    LongNumber num15("777");
    LongNumber num16("0");
    result = num15 - num16;
    LongNumber expected8("777");
    assert(result == expected8);
	
      std::cout << "Все тесты вычитания пройдены" << std::endl;
}

void test_multiplication() {
    
    // Тест 1: Умножение двух положительных чисел
    LongNumber num1("12");
    LongNumber num2("34");
    LongNumber result = num1 * num2;
    LongNumber expected1("408");
    assert(result == expected1);
    
    // Тест 2: Умножение на ноль
    LongNumber num3("12345");
    LongNumber num4("0");
    result = num3 * num4;
    LongNumber expected2("0");
    assert(result == expected2);
    
    // Тест 3: Умножение на единицу
    LongNumber num5("98765");
    LongNumber num6("1");
    result = num5 * num6;
    LongNumber expected3("98765");
    assert(result == expected3);
    
    // Тест 4: Умножение больших чисел
    LongNumber num7("123456789");
    LongNumber num8("987654321");
    result = num7 * num8;
    LongNumber expected4("121932631112635269");
    assert(result == expected4);
    
    // Тест 5: Умножение с переносом разряда
    LongNumber num9("999");
    LongNumber num10("999");
    result = num9 * num10;
    LongNumber expected5("998001");
    assert(result == expected5);
    
    // Тест 6: Умножение с разными знаками (+ * -)
    LongNumber num11("10");
    LongNumber num12("-5");
    result = num11 * num12;
    LongNumber expected6("-50");
    assert(result == expected6);
    
    // Тест 7: Умножение двух отрицательных чисел
    LongNumber num13("-10");
    LongNumber num14("-5");
    result = num13 * num14;
    LongNumber expected7("50");
    assert(result == expected7);
    
    // Тест 8: Коммутативность умножения
    LongNumber a("123");
    LongNumber b("456");
    LongNumber r1 = a * b;
    LongNumber r2 = b * a;
    assert(r1 == r2);
    
	  std::cout << "Все тесты умножения пройдены" << std::endl;
}

void test_division() {
    
    // Тест 1: Деление нацело
    LongNumber num1("100");
    LongNumber num2("5");
    LongNumber result = num1 / num2;
    LongNumber expected1("20");
    assert(result == expected1);
    
    // Тест 2: Деление с остатком (целая часть)
    LongNumber num3("10");
    LongNumber num4("3");
    result = num3 / num4;
    LongNumber expected2("3");
    assert(result == expected2);
    
    // Тест 3: Деление меньшего на большее
    LongNumber num5("5");
    LongNumber num6("10");
    result = num5 / num6;
    LongNumber expected3("0");
    assert(result == expected3);
    
    // Тест 4: Деление на единицу
    LongNumber num7("123456789");
    LongNumber num8("1");
    result = num7 / num8;
    LongNumber expected4("123456789");
    assert(result == expected4);
    
    // Тест 5: Деление больших чисел
    LongNumber num9("121932631112635269");
    LongNumber num10("123456789");
    result = num9 / num10;
    LongNumber expected5("987654321");
    assert(result == expected5);
    
    // Тест 6: Деление с разными знаками (+ / -)
    LongNumber num11("20");
    LongNumber num12("-4");
    result = num11 / num12;
    LongNumber expected6("-5");
    assert(result == expected6);
    
    // Тест 7: Деление двух отрицательных чисел
    LongNumber num13("-20");
    LongNumber num14("-4");
    result = num13 / num14;
    LongNumber expected7("5");
    assert(result == expected7);
    
    // Тест 8: Деление числа на само себя
    LongNumber num15("555");
    LongNumber num16("555");
    result = num15 / num16;
    LongNumber expected8("1");
    assert(result == expected8);
    
    // Тест 9: Деление нуля на число
    LongNumber num17("0");
    LongNumber num18("123");
    result = num17 / num18;
    LongNumber expected9("0");
    assert(result == expected9);
    
	  std::cout << "Все тесты деления пройдены" << std::endl;
}

void test_modulo() {
    
    // Тест 1: Базовый остаток
    LongNumber num1("10");
    LongNumber num2("3");
    LongNumber result = num1 % num2;
    LongNumber expected1("1");
    assert(result == expected1);
    
    // Тест 2: Остаток при делении с другим значением
    LongNumber num3("17");
    LongNumber num4("5");
    result = num3 % num4;
    LongNumber expected2("2");
    assert(result == expected2);
    
    // Тест 3: Деление нацело — остаток 0
    LongNumber num5("100");
    LongNumber num6("5");
    result = num5 % num6;
    LongNumber expected3("0");
    assert(result == expected3);
    
    // Тест 4: Остаток при делении меньшего на большее
    LongNumber num7("5");
    LongNumber num8("10");
    result = num7 % num8;
    LongNumber expected4("5");
    assert(result == expected4);
    
    // Тест 5: Остаток от деления нуля
    LongNumber num9("0");
    LongNumber num10("123");
    result = num9 % num10;
    LongNumber expected5("0");
    assert(result == expected5);
    
    // Тест 6: Остаток с отрицательным делимым
    LongNumber num11("-10");
    LongNumber num12("3");
    result = num11 % num12;
    LongNumber expected6("-1");
    assert(result == expected6);
    
    // Тест 7: Проверка тождества: (a / b) * b + (a % b) == a
    LongNumber a("12345");
    LongNumber b("67");
    LongNumber q = a / b;
    LongNumber r = a % b;
    LongNumber check = q * b + r;
	assert(check == a);
	
	  std::cout << "Все тесты остатка от деления пройдены" << std::endl;
}

int main() {
    test_addition();
    test_subtraction();
    test_multiplication();
    test_division();
    test_modulo();
	 std::cout << "ВСЕ ТЕСТЫ ПРОЙДЕНЫ" << std::endl;
}