#include "long_number.hpp"
#include <string>
#include <cstring> 

using lae::LongNumber;
		
LongNumber::LongNumber() {
	sign = 1;
	length = 1;
	numbers = new int[1];
	numbers[0] = 0;
}

LongNumber::LongNumber(const int a) {
	sign = 1;
	if (a < 0) {
		sign = -1;
	}
	length = 1;
	numbers = new int[1];
	numbers[0] = x;
}

LongNumber::LongNumber(int length, int sign) {
    this->sign = sign;
    this->length = length;
    numbers = new int[length];
    for (int i = 0; i < length; ++i) {
        numbers[i] = 0;
    }
}

LongNumber::LongNumber(const char* const str) {
    int start_index;
    if (str[0] == '-') {
        start_index = 1;
        sign = -1;
    }
    else {
        sign = 1;
        start_index = 0;
    }
    
    length = strlen(str) - start_index;
    numbers = new int[length];
    for (int i = 0; i < length; ++i) {
        numbers[i] = str[start_index + i] - '0'; 
    }
}

LongNumber::LongNumber(const LongNumber& x) {
	length = x.length;
	sign = x.sign;

	numbers = new int[length];
	for (int i = 0; i < length; i++) {
		numbers[i] = x.numbers[i];
	}
}

LongNumber::LongNumber(LongNumber&& x) {
	length = x.length;
	sign = x.sign;

	numbers = new int[length];
	for (int i = 0; i < length; i++) {
		numbers[i] = x.numbers[i];
	}
	x.length = 0;
	x.sign = 1;
	x.numbers = nullptr;
}

LongNumber::~LongNumber() {
	if (numbers != nullptr) {
		delete[] numbers;
		numbers = nullptr;
	}
}

LongNumber& LongNumber::operator = (const char* const str) {
    delete[] numbers;
    
    int start_index;
    if (str[0] == '-') {
        start_index = 1;
        sign = -1;
    }
    else {
        sign = 1;
        start_index = 0;
    }
    
    length = strlen(str) - start_index;
    numbers = new int[length];
    for (int i = 0; i < length; ++i) {
        numbers[i] = str[start_index + i] - '0';
    }
    
    return *this;
}


LongNumber& LongNumber::operator = (const LongNumber& x) {
	if (this == &x) return *this;
	
	length = x.length;
	sign = x.sign;
	
	delete [] numbers;
	numbers = new int[length];
	for (int i = 0; i  < length; i++) {
		numbers[i] = x.numbers[i];
	}
	
	return *this;
}

LongNumber& LongNumber::operator=(LongNumber&& x) {
    if (this != &x) {
        delete[] numbers;
        
        numbers = x.numbers;
        length = x.length;
        sign = x.sign;
        
        x.numbers = nullptr;
        x.length = 0;
        x.sign = 1;
    }
    return *this;
}

bool LongNumber::operator == (const LongNumber& x) const {
	if (length != x.length) {
        return false;
    }
    
    if (sign != x.sign) {
        return false;
    }
    
    for (int i = 0; i < length; i++) {
        if (numbers[i] != x.numbers[i]) {
            return false;
        }
    }
    return true;
}

bool LongNumber::operator != (const LongNumber& x) const {
	return !(*this == x);
}

bool LongNumber::operator > (const LongNumber& x) const {
    if (sign > x.sign) {return true; }
    if (sign < x.sign) {return false;}
    
    if (sign == 1) {
        if (length > x.length) {return true;}
        if (length < x.length) {return false;}
        
        for (int i = 0; i < length; i++) {
            if (numbers[i] > x.numbers[i]) {return true;}
		if (numbers[i] < x.numbers[i]) {return false;}
        }
    } else {
        if (length > x.length) {return false;
		if (length < x.length) {return true;}
        
        for (int i = 0; i < length; i++) {
            if (numbers[i] > x.numbers[i]) {return false;}
            if (numbers[i] < x.numbers[i]) {return true;}
        }
    }
    
    return false;
}

bool LongNumber::operator < (const LongNumber& x) const {
		return !(*this > x) && !(*this == x);
}

LongNumber LongNumber::operator + (const LongNumber& x) const {
    if (sign != x.sign) {
        if (sign == -1) {
            LongNumber temp = *this;
            temp.sign = 1;
            return x - temp;
        } else {
            LongNumber temp = x;
            temp.sign = 1;
            return *this - temp;
        }
    }
    
    int max_len = (length > x.length ? length : x.length) + 1;
    LongNumber result(max_len, sign);
    
    for (int i = 0; i < max_len; ++i) {
        result.numbers[i] = 0;
    }
    
    int i = length - 1;
    int j = x.length - 1;
    int k = max_len - 1;
    int carry = 0;
    
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) {
            sum += numbers[i];
            --i;
        }
        if (j >= 0) {
            sum += x.numbers[j];
            --j;
        }
        result.numbers[k] = sum % 10;
        carry = sum / 10;
        --k;
    }
    
    int start = 0;
    while (start < max_len - 1 && result.numbers[start] == 0) {
        ++start;
    }
    
    if (start > 0) {
        int new_len = max_len - start;
        LongNumber trimmed(new_len, sign);
        for (int idx = 0; idx < new_len; ++idx) {
            trimmed.numbers[idx] = result.numbers[start + idx];
        }
        return trimmed;
    }
    
    return result;
}
LongNumber LongNumber::operator-(const LongNumber& x) const {
    if (sign == -1 && x.sign == 1) {
        LongNumber a_abs = *this;
        a_abs.sign = 1;
        LongNumber result = a_abs + x;
        result.sign = -1;
        return result;
    }
    
    if (sign == 1 && x.sign == -1) {
        LongNumber b_abs = x;
        b_abs.sign = 1;
        return *this + b_abs;
    }
    
    if (sign == -1 && x.sign == -1) {
        LongNumber a_abs = *this, b_abs = x;
        a_abs.sign = 1;
        b_abs.sign = 1;
        return b_abs - a_abs;
    }
    
    if (sign == 1 && x.sign == 1) {
        if (*this == x) {
            return LongNumber("0");
        }
        
        if (*this < x) {
            LongNumber result = x - *this;
            result.sign = -1;
            return result;
        }
        
        int max_len = length + 1;
        LongNumber result(max_len, 1);
        
        for (int i = 0; i < length; ++i) {
            result.numbers[max_len - 1 - i] = numbers[length - 1 - i];
        }
        
        int idx_result = max_len - 1;
        int idx_x = x.length - 1;
        int borrow = 0;
        

        while (idx_x >= 0 && idx_result >= 0) {
            int digit_result = result.numbers[idx_result];
            int digit_x = x.numbers[idx_x];
            
            int diff = digit_result - digit_x - borrow;
            
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            
            result.numbers[idx_result] = diff;
            --idx_result;
            --idx_x;
        }
        
        while (borrow && idx_result >= 0) {
            int diff = result.numbers[idx_result] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.numbers[idx_result] = diff;
            --idx_result;
        }
        
        int first_non_zero = 0;
        while (first_non_zero < max_len - 1 && result.numbers[first_non_zero] == 0) {
            ++first_non_zero;
        }
        
        int new_len = max_len - first_non_zero;
        LongNumber final_result(new_len, 1);
        for (int i = 0; i < new_len; ++i) {
            final_result.numbers[i] = result.numbers[first_non_zero + i];
        }
        
        return final_result;
    }

    return LongNumber("0");
}

LongNumber LongNumber::operator * (const LongNumber& x) const {
    int length_a = length;
    int length_b = x.length;
    int length_c = length_a + length_b + 1;
    
    LongNumber result(length_c, 1); 
    
    if (sign != x.sign) {
        result.sign = -1;
    }
    
    for (int i = 0; i < length_c; i++) {
        result.numbers[i] = 0;
    }
    
    for (int i = 0; i < length_a; i++) {
        for (int j = 0; j < length_b; j++) {
            int pos = length_c - 1 - (i + j);
            result.numbers[pos] += numbers[length_a - 1 - i] * x.numbers[length_b - 1 - j];
        }
    }
    
    for (int i = length_c - 1; i > 0; i--) {
        if (result.numbers[i] >= 10) {
            result.numbers[i - 1] += result.numbers[i] / 10;
            result.numbers[i] = result.numbers[i] % 10;
        }
    }
    
    int first_digit = 0;
    while (first_digit < length_c - 1 && result.numbers[first_digit] == 0) {
        first_digit++;
    }
    
    if (first_digit > 0) {
        int new_length = length_c - first_digit;
        LongNumber temp(new_length, result.sign);
        for (int i = 0; i < new_length; i++) {
            temp.numbers[i] = result.numbers[first_digit + i];
        }
        return temp;
    }
    
    return result;
}
LongNumber LongNumber::operator / (const LongNumber& x) const {
    if (x.length == 1 && x.numbers[0] == 0) {
        std::cout << "Ошибка!" << std::endl;
    }
    
    int result_sign = (sign == x.sign) ? 1 : -1;
    
    LongNumber dividend = *this;
    LongNumber divisor = x;
    dividend.sign = 1;
    divisor.sign = 1;

    if (dividend < divisor) {
        return 0;
    }
    
    if (dividend == divisor) {
        LongNumber result("1");
        result.sign = result_sign;
        return result;
    }
    
    LongNumber remainder("0"); 
    LongNumber quotient("0");
    
    for (int i = 0; i < dividend.length; ++i) {
        LongNumber temp(remainder.length + 1, 1);
        for (int j = 0; j < remainder.length; ++j) {
            temp.numbers[j] = remainder.numbers[j];
        }
        temp.numbers[remainder.length] = dividend.numbers[i];
        
        int start = 0;
        while (start < temp.length - 1 && temp.numbers[start] == 0) {
            ++start;
        }
        if (start > 0) {
            LongNumber trimmed(temp.length - start, 1);
            for (int j = 0; j < trimmed.length; ++j) {
                trimmed.numbers[j] = temp.numbers[start + j];
            }
            remainder = trimmed;
        } else {
            remainder = temp;
        }
        
        int digit = 0;
        for (int d = 9; d >= 0; --d) {
            LongNumber product = divisor * d;
            if (product < remainder || product == remainder) {
                digit = d;
                remainder = remainder - product;
                break;
            }
        }

        LongNumber temp_quotient(quotient.length + 1, 1);
        for (int j = 0; j < quotient.length; ++j) {
            temp_quotient.numbers[j] = quotient.numbers[j];
        }
        temp_quotient.numbers[quotient.length] = digit;
        
        start = 0;
        while (start < temp_quotient.length - 1 && temp_quotient.numbers[start] == 0) {
            ++start;
        }
        if (start > 0) {
            LongNumber trimmed(temp_quotient.length - start, 1);
            for (int j = 0; j < trimmed.length; ++j) {
                trimmed.numbers[j] = temp_quotient.numbers[start + j];
            }
            quotient = trimmed;
        } else {
            quotient = temp_quotient;
        }
    }
    
    quotient.sign = result_sign;

    if (sign == -1 && !(remainder.length == 1 && remainder.numbers[0] == 0)) {
        if (x.sign == -1) {
            quotient = quotient + LongNumber("1");
        } else {
            quotient = quotient - LongNumber("1");
        }
    }

    if (quotient.length == 1 && quotient.numbers[0] == 0) {
        quotient.sign = 1;
    }
    
    return quotient;
}

LongNumber LongNumber::operator % (const LongNumber& x) const {
    return *this - *this / x * x;
}


bool LongNumber::is_negative() const noexcept {
    return sign == -1;
}

// ----------------------------------------------------------
// PRIVATE
// ----------------------------------------------------------
int LongNumber::get_length(const char* const str) const noexcept {
	int len = 0;
    int i = 0;
    
    if (str[0] == '-') {
        i = 1;
    }
    
    while (str[i] != '\0') {
        len++;
        i++;
    }
    
    return len;
}

// ----------------------------------------------------------
// FRIENDLY
// ----------------------------------------------------------
namespace lae {
	std::ostream& operator << (std::ostream &os, const LongNumber& x) {
		 if (x.sign == -1) {
            os << '-';
        }
        for (int i = 0; i < x.length; i++) {
            os << x.numbers[i];
        }
        return os;
	}
}
