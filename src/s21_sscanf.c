#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>   //бибилиотека целочисленного типа заданной длинны может принимать
                        // заданное числловое знгачение либо стандартное библиотечное
#include "s21_string.h"




int s21_sscanf(const char *str, const char *format, ...) {
  int num_matched = 0;
  const char *str_ptr = str;
  const char *format_ptr = format;
  va_list args;

  va_start(args, format);

  while (*str_ptr && *format_ptr) {
    while (isspace(*str_ptr)) {  // игнорирует пробелы в строке
      str_ptr++;
    }
    if (*str_ptr == '\0') {  // если достигли конца строки  прерывает цикл
      break;
    }
    if (*format_ptr !=
        '%') {  // если следующий символ в шаблоне не спецификатор, сверяет со
                // следующим символом строки
      if (*format_ptr != *str_ptr) {
        break;
      }
      format_ptr++;
      str_ptr++;
      continue;
    }

    format_ptr++;  // Найдя спецификаторб мы читаем аргумент

    int width = 0;  // Читаем широкий спецификатор , если он присутствует
    while (isdigit(*format_ptr)) {
      width = (width * 10) + (*format_ptr - '0');
      format_ptr++;
    }

    switch (*format_ptr) {
      case 'd': {
        int *int_ptr = va_arg(args,
                              int *);  // считываем интовые значения
        int sign = 1;
        int value = 0;
        if (*str_ptr == '-') {  // проверяем на знаки
          sign = -1;
          str_ptr++;
        } else if (*str_ptr == '+') {
          str_ptr++;
        }
        while (isdigit(*str_ptr)) {  // Считываем цифры
          value = (value * 10) + (*str_ptr - '0');
          str_ptr++;
        }

        *int_ptr =
            sign * value;  // принимаем знак и сохраняем результат в буфер
        num_matched++;
      } break;
      case 'i': {
        int *int_ptr = va_arg(
            args, int *);  // считываем значения чисел (принимая восмерихные б
                           // десятиричные и шестнадцатиричные)
        int sign = 1;
        int base = 10;
        int value = 0;
        if (*str_ptr == '-') {  // проверям на знак
          sign = -1;
          str_ptr++;
        } else if (*str_ptr == '+') {
          str_ptr++;
        }

        if (*str_ptr ==
            '0') {  // обработка восьмеричного/шестнадцатеричного ввода
          str_ptr++;
          if (*str_ptr == 'x' || *str_ptr == 'X') {
            base = 16;
            str_ptr++;
          } else {
            base = 8;
          }
        }

        while (isxdigit(*str_ptr)) {  // считываем числа
          int digit_value;
          if (isdigit(*str_ptr)) {
            digit_value = *str_ptr - '0';
          } else {
            digit_value = tolower(*str_ptr) - 'a' + 10;
          }

          if (digit_value >= base) {
            break;
          }

          value = value * base + digit_value;
          str_ptr++;
        }

        *int_ptr = sign * value;
        num_matched++;
      } break;
      case 'u': {
        unsigned int *uint_ptr =
            va_arg(args, unsigned int *);  // считываем беззнаковые целые числа
        unsigned int value = 0;

        while (isdigit(*str_ptr)) {  // считываем числа
          value = (value * 10) + (*str_ptr - '0');
          str_ptr++;
        }

        *uint_ptr = value;
        num_matched++;
      } break;
      case 'o': {
        unsigned int *uint_ptr =
            va_arg(args, unsigned int *);  // считываем восьмиричные значения
        unsigned int value = 0;

        while (*str_ptr >= '0' && *str_ptr <= '7') {  //считываем числа
          value = (value << 3) + (*str_ptr - '0');
          str_ptr++;
        }

        *uint_ptr = value;
        num_matched++;
      } break;
      case 'x':
      case 'X': {
        unsigned int *uint_ptr =
            va_arg(args,
                   unsigned int *);  // считываем шестнадцатериччнфе значения
        unsigned int value = 0;

        while (isxdigit(*str_ptr)) {  //считываем числа
          int digit_value;
          if (isdigit(*str_ptr)) {
            digit_value = *str_ptr - '0';
          } else {
            digit_value = tolower(*str_ptr) - 'a' + 10;
          }
          value = value * 16 + digit_value;
          str_ptr++;
        }

        *uint_ptr = value;
        num_matched++;
      }

      break;
      case 'f':
      case 'e':
      case 'g': {
        double *double_ptr =
            va_arg(args,
                   double *);  //считываем значения с плавающей точкой
        char buffer[DBL_MAX_10_EXP +
                    1];  // выделяем буфер под сохранение числа в строку
        int buffer_index = 0;

        while (isdigit(*str_ptr)) {  // считываем цифровую часть
          buffer[buffer_index] = *str_ptr++;
        }

        if (*str_ptr == '.' ||
            *str_ptr == ',') {  // считываем после разделителя, если таковой
                                // присутствует
          buffer[buffer_index++] = *str_ptr++;
          while (isdigit(*str_ptr)) {
            buffer[buffer_index++] = *str_ptr++;
          }
        }

        if (*str_ptr == 'e' ||
            *str_ptr == 'E') {  //считываем экспоненту если присутствует
          buffer[buffer_index++] = *str_ptr++;

          if (*str_ptr == '+' || *str_ptr == '-') {
            buffer[buffer_index++] = *str_ptr++;
          }

          while (isdigit(*str_ptr)) {
            buffer[buffer_index++] = *str_ptr++;
          }
        }

        buffer[buffer_index] = '\0';  //прерываем строку символом "0"

        *double_ptr = strtod(
            buffer,
            NULL);  // переводим строку в дабл и сохоаняем результат в буфер
        num_matched++;
      } break;
      case 'c': {
        char *char_ptr = va_arg(args,
                                char *);  // считываем символ
        if (width ==
            0) {  // если не находит широкого флагаб считывает один символ
          *char_ptr = *str_ptr++;
        } else {  // иначе считывает все флаги лил до конца ввода или строки
          int max_length = width;
          while (*str_ptr && max_length--) {
            *char_ptr++ = *str_ptr++;
          }
        }

        num_matched++;
      } break;
      case 's': {
        char *strring_ptr = va_arg(args, char *);  // считываем строку
        int max_length = width ? width : INT_MAX;
        while (*str_ptr && !isspace(*str_ptr) &&
               max_length--) {  // копируем символы со строки ввода по
                                // достижению конца строки
          *strring_ptr++ = *str_ptr++;
        }
        *strring_ptr =
            '\0';  // добавляем символ "0" для обозначения конца строки
        num_matched++;

      } break;
      case 'p': {
        void **void_ptr =
            va_arg(args, void **);  // считываем значение указателя
        long value = 0;

        if (*str_ptr == '0' &&
            (*(str_ptr + 1) == 'x' ||
             *(str_ptr + 1) == 'X')) {  // производим шестнадцатеричный ввод
          str_ptr += 2;
          while (isxdigit(*str_ptr)) {
            int digit_value;
            if (isdigit(*str_ptr)) {
              digit_value = *str_ptr - '0';
            } else {  // производим десятиричный ввод
              digit_value = tolower(*str_ptr) - 'a' + 10;
            }

            value = value * 16 + digit_value;
            str_ptr++;
          }

        } else {
          while (isdigit(*str_ptr)) {
            value = value * 10 + (*str_ptr - '0');
            str_ptr++;
          }
        }

        *void_ptr =
            (void *)(uintptr_t)value;  // преобразуем значение указателя в
                                       // пустой указатель и записываем в буфер
        num_matched++;
      } break;
      case '%': {
        if (*str_ptr == '%') {  // обработка спецификатора
          str_ptr++;
          num_matched++;
        } else {
          break;
        }
      } break;
      default:  // неподдерживаемый формат
        break;
    }
    format_ptr++;
  }

  va_end(args);
  return num_matched;
}
