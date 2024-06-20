#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>

#include "s21_string.h"

#define MAX_FORMAT_LEN 1024

static char* convert_integer_to_string(long number, int base, bool negative);
static char* convert_unsigned_to_string(unsigned long number, int base);
static char* convert_double_to_string(double number, int precision);
static void reverse_string_in_place(char* str);
static void add_padding(char** out_pos, int* out_len, int width, char pad_char);
static void add_alternate_form(char** out_pos, int* out_len, char specifier);
static void copy_formatted_value(char** out_pos, int* out_len, const char* fmt_buf);

int s21_sprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char fmt_buf[MAX_FORMAT_LEN];
    char out_buf[MAX_FORMAT_LEN];

    char* out_pos = out_buf;
    int out_len = 0;

    while (*format != '\0') {
        if (*format == '%') {
            ++format;

            // Обработка флагов
            bool zero_padding = false;
            bool alt_form = false;
            while (*format == '#' || *format == '0') {
                if (*format == '#') {
                    alt_form = true;
                } else if (*format == '0') {
                    zero_padding = true;
                }
                ++format;
            }

            // Обработка ширины
            int width = 0;
            if (*format == '*') {
                width = va_arg(args, int);
                ++format;
            } else {
                while (*format >= '0' && *format <= '9') {
                    width = width * 10 + (*format - '0');
                    ++format;
                }
            }

            // Обработка точности
            int precision = -1;
            if (*format == '.') {
                precision = 0;
                ++format;

                if (*format == '*') {
                    precision = va_arg(args, int);
                    ++format;
                } else {
                    while (*format >= '0' && *format <= '9') {
                        if (precision < 0) {
                            precision = 0;
                        }
                        precision = precision * 10 + (*format - '0');
                        ++format;
                    }
                }
            }

            // Обработка модификаторов длины
            bool long_modifier = false;
            if (*format == 'l' || *format == 'L') {
                long_modifier = true;
                ++format;
            }

            // Обработка спецификаторов типа
            switch (*format) {
                case 'd':
                case 'i': {
                    long value;
                    if (long_modifier) {
                        value = va_arg(args, long);
                    } else {
                        value = va_arg(args, int);
                    }
                    char* int_str = convert_integer_to_string(value, 10, value < 0);
                    copy_formatted_value(&out_pos, &out_len, int_str);
                    break;
                }
                case 'u': {
                    unsigned long value;
                    if (long_modifier) {
                        value = va_arg(args, unsigned long);
                    } else {
                        value = va_arg(args, unsigned int);
                    }
                    char* int_str = convert_unsigned_to_string(value, 10);
                    copy_formatted_value(&out_pos, &out_len, int_str);
                    break;
                }
                case 'x':
                case 'X': {
                    unsigned long value;
                    if (long_modifier) {
                        value = va_arg(args, unsigned long);
                    } else {
                        value = va_arg(args, unsigned int);
                    }
                    char* int_str = convert_unsigned_to_string(value, 16);
                    if (*format == 'X') {
                        for (int i = 0; int_str[i] != '\0'; ++i) {
                            int_str[i] = toupper(int_str[i]);
                        }
                    }
                    copy_formatted_value(&out_pos, &out_len, int_str);
                    break;
                }
                case 'o': {
                    unsigned long value;
                    if (long_modifier) {
                        value = va_arg(args, unsigned long);
                    } else {
                        value = va_arg(args, unsigned int);
                    }
                    char* int_str = convert_unsigned_to_string(value, 8);
                    copy_formatted_value(&out_pos, &out_len, int_str);
                    break;
                }
                case 'p': {
                    void* value = va_arg(args, void*);
                    char* int_str = convert_unsigned_to_string((unsigned long)value, 16);
                    copy_formatted_value(&out_pos, &out_len, int_str);
                    break;
                }
                case 'f': {
                    double value;
                    if (long_modifier) {
                        value = va_arg(args, double);
                    } else {
                        value = va_arg(args, double);
                    }
                    char* float_str = convert_double_to_string(value, precision);
                    copy_formatted_value(&out_pos, &out_len, float_str);
                    break;
                }
                case 'c': {
                    char value = (char)va_arg(args, int);
                    *out_pos++ = value;
                    ++out_len;
                    break;
                }
                case 's': {
                    char* str_value = va_arg(args, char*);
                    if (!str_value) {
                        str_value = "(null)";
                    }
                    copy_formatted_value(&out_pos, &out_len, str_value);
                    break;
                }
            }

            // Обработка завершающего символа
            ++format;

        } else {
            *out_pos++ = *format++;
            ++out_len;
        }
    }

    va_end(args);

// Добавляем завершающий символ
    *out_pos = '\0';
    ++out_len;

// Выводим отформатированную строку на экран
    printf("%s", out_buf);

    return out_len;
}

static char* convert_integer_to_string(long number, int base, bool negative) {
    static const char digits[] = "0123456789abcdef";
    char buffer[32];
    int pos = 0;

    if (number == 0) {
        buffer[pos++] = '0';
    } else {
        if (negative) {
            number = -number;
        }

        while (number != 0) {
            buffer[pos++] = digits[number % base];
            number /= base;
        }

        if (negative) {
            buffer[pos++] = '-';
        }
    }

    buffer[pos] = '\0';

    reverse_string_in_place(buffer);

    return s21_strdup(buffer);
}

static char* convert_unsigned_to_string(unsigned long number, int base) {
    static const char digits[] = "0123456789abcdef";
    char buffer[32];
    int pos = 0;

    if (number == 0) {
        buffer[pos++] = '0';
    } else {
        while (number != 0) {
            buffer[pos++] = digits[number % base];
            number /= base;
        }
    }

    buffer[pos] = '\0';

    reverse_string_in_place(buffer);

    return s21_strdup(buffer);
}

static char* convert_double_to_string(double number, int precision) {
    char buffer[32];
    int pos = 0;
    bool negative = false;

    if (number < 0.0) {
        negative = true;
        number = -number;
    }

// Обработка целой части
    long whole_part = (long)number;
    char* int_str = convert_integer_to_string(whole_part, 10, negative);
    for (int i = 0; int_str[i] != '\0'; ++i) {
        buffer[pos++] = int_str[i];
    }
    free(int_str);

// Добавляем десятичную точку
    buffer[pos++] = '.';

// Обработка дробной части
    double fractional_part = number - whole_part;
    for (int i = 0; i < precision; ++i) {
        fractional_part *= 10.0;
        int digit = (int)fractional_part;
        buffer[pos++] = '0' + digit;
        fractional_part -= digit;
    }

    buffer[pos] = '\0';

    return s21_strdup(buffer);
}

static void reverse_string_in_place(char* str) {
    int len = s21_strlen(str);
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

static void add_padding(char** out_pos, int* out_len, int width, char pad_char) {
    int padding_length = width - (*out_len);
    for (int i = 0; i < padding_length; ++i) {
        *(*out_pos)++ = pad_char;
    }
    *out_len = width;
}

static void add_alternate_form(char** out_pos, int* out_len, char specifier) {
    if (specifier == 'x' || specifier == 'X' || specifier == 'o') {
        *(*out_pos)++ = '0';
        *out_len += 1;
        if (specifier == 'x' || specifier == 'X') {
            *(*out_pos)++ = specifier;
            *out_len += 1;
        }
    }
}

static void copy_formatted_value(char** out_pos, int* out_len, const char* fmt_buf) {
    int fmt_len = s21_strlen(fmt_buf);
    bool negative = (fmt_buf[0] == '-');

    int width = 0;
    char pad_char = ' ';
    if (negative) {
        --fmt_len;
    }

    if (width > fmt_len) {
        add_padding(out_pos, out_len, width, pad_char);
    }

    if (negative) {
        *(*out_pos)++ = '-';
        ++fmt_buf;
        --fmt_len;
    }

    add_alternate_form(out_pos, out_len, fmt_buf[0]);

    for (int i = 0; i < fmt_len; ++i) {
        *(*out_pos)++ = fmt_buf[i];
    }

    *out_len += fmt_len;
}
