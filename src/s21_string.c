#include "s21_string.h"

#include <stddef.h>
#include <stdio.h>


#define s21_size_t size_t

void *s21_memchr(const void *s, int c, s21_size_t n) {
  const unsigned char *p = s;
  for (s21_size_t i = 0; i < n; i++) {
    if (p[i] == (unsigned char)c) {
      return (void *)(p + i);
    }
  }
  return NULL;
}

int s21_memcmp(const void *st1, const void *st2, s21_size_t n) {
    unsigned const char *str1 = st1, *str2 = st2;
    int result = 0;
    while (n--) {
        if (*str1!= *str2) {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return result;
}


void s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *d = dest;
  const char *s = src;
  while (n--) {
    *d++ = *s++;
  }
}

void s21_memmove(void *dest, const void *src, s21_size_t n) {
  char *dest_c = (char *)dest;
  const char *src_c = (const char *)src;

  if (dest_c > src_c) {
    for (s21_size_t i = n; i > 0; i--) {
      dest_c[i - 1] = src_c[i - 1];
    }
  } else {
    for (s21_size_t i = 0; i < n; i++) {
      dest_c[i] = src_c[i];
    }
  }
}

void *s21_memset(void *s, int c, s21_size_t n) {
  unsigned char *p = s;
  while (n--) {
    *p++ = (unsigned char)c;
  }
  return s;
}

s21_size_t s21_strlen(const char *str) {
  size_t len = 0;
  while (*str != '\0') {
    ++len;
    ++str;
  }
  return len;
}

char *s21_strcat(char *dest, const char *src) {
  char *temp = dest;
  while (*temp != '\0') {
    temp++;
  }
  while ((*temp++ = *src++) != '\0')
    ;
  return dest;
}

char *s21_strchr(const char *str, int c) {
  while (*str != '\0') {  //проходим строку ищем симолы
    if (*str == c) {  // если ссимвол совпал с искомым
      return (char *)str;  // возвращаем указатель на этот символ
    }
    str++;  // продвигаемся по всем символам
  }
  if (c == '\0') {       // если находим 0
    return (char *)str;  //вовращаем указатель на строку
  } else {
    return NULL;  // иначе искомый символ не найден
  }
}

char *s21_strcpy(char *dest, const char *src) {
  char *temp = dest;
  while ((*temp++ = *src++) != '\0')
    ;
  return dest;
//  free(dest);
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  while (*str1 != '\0') {
    if (s21_strchr(str2, *str1) != NULL) {
      return count;
    }
    count++;
    str1++;
  }
  return count;
}

char *s21_strdup(const char *str) {
    size_t len = strlen(str);
    char *dest = malloc(len + 1); // allocate space for string + null terminator
    if (dest == NULL)
        return NULL; // malloc failed, so we return NULL to indicate an error
    memcpy(dest, str, len + 1); // copy string and null terminator to dest
    return dest;
}

char *s21_strerror(int errnum) {
    char *msg;
#ifdef __APPLE__
    msg = strerror(errnum);
#else
    char errbuf[256];
    if (strerror_r(errnum, errbuf, sizeof(errbuf)) != 0) {
        snprintf(errbuf, sizeof(errbuf), "Unknown error: %d", errnum);
    }
    msg = strdup(errbuf);
#endif
    return msg;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t dest_len = s21_strlen(dest);
  s21_size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[dest_len + i] = src[i];
  }
  dest[dest_len + i] = '\0';
  return dest;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    if (str1[i] != str2[i]) {
      return str1[i] - str2[i];
    }
    if (str1[i] == '\0') {
      return 0;
    }
  }
  return 0;
}



//const char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
//    char *p = dest;
//    while (n--) {
//        *dest = *src;
//        dest++;
//        src++;
//    }
//    return p;
//}



//const char *s21_strncpy(char *dest, const char *src, size_t n) {
//    for (size_t i=0; i < n && src[i] != '\0'; i++) {
//        dest[i] = src[i];
//    }
//    return dest;
//}


const char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
    s21_size_t i;
    for (i=0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    if (i < n) {
        dest[i] = '\0';
    }
    return dest;
}


char *s21_strrchr(const char *str, int ch) {
  const char *last = NULL;
  while (*str != '\0') {
    if (*str == ch) {
      last = str;
    }
    str++;
  }
  if (*str == ch) {
    return (char *)str;
  }
  return (char *)last;
}

char *s21_strstr(const char *haystack, const char *needle) {
  int needle_len = strlen(needle);
  if (needle_len == 0) {
    return (char *)haystack;
  }
  while (*haystack) {
    if (s21_strncmp(haystack, needle, s21_strlen(needle)) == 0) {
      return (char *)haystack;
    }
    haystack++;
  }
  return NULL;
}


void *s21_trim(char *str) {
    char *start = str;
    while ( *start == ' ' || *start =='\t' || *start == '\n') {
        start++;
    }
    char *end = str + s21_strlen(str) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '*')) {
        end--;
    }
    *(end + 1) = '\0';
    char *trimmed_str = malloc(s21_strlen(start) +1);
    s21_strcpy(trimmed_str, start);
    return trimmed_str;
}

char *s21_insert(char *src, const char *str, s21_size_t index) {
    if (src == NULL) {
        return NULL;
    }

    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);

    if (index > src_len) {
        return NULL;
    }

    char *result = malloc((src_len + str_len + 1) * sizeof(char));
    if (!result) {
        exit(1);
    }

    s21_strncpy(result, src, index);
    s21_strcpy(&result[index], str);
    s21_strcpy(&result[index + str_len], &src[index]);

    return result;
}


s21_size_t s21_strspn(const char *s1, const char *s2) {
  s21_size_t i, j;
  for (i = 0; s1[i] != '\0'; i++) {
    for (j = 0; s2[j] != '\0'; j++) {
      if (s1[i] == s2[j]) {
        break;
      }
    }
    if (s2[j] == '\0') {
      return i;
    }
  }
  return i;
}

char *s21_strpbrk(const char *str1, const char *str2) {
    char *result = NULL;
    int i = 0;
    int j ;
    for (; str1[i] != '\0'; i++) {
        for (j = 0; str2[j] != '\0'; j++) {
            if (str1[i] == str2[j]) {
                result = (char *)&str1[i];
                break;
            }
        }
        if (result != NULL) {
            break;
        }
    }
    return result;
}

char *s21_strtok(char *str, const char *delim) {
    int flag = 0;
    static char *last;  // класс static позволяет сохранять значение переменной
    // между вызовами функции
    char *res;
    if (str == NULL) {
        str = last;  // если str == NULL, то берем последнее значение str
    }
    if (str == NULL) {
        flag = 1;  // если str == NULL, то возвращаем NULL
    }
    if (!flag) {
        while (s21_is_delim(*str, delim)) {
            str++;  // пропускаем все разделители
        }
        if (*str == '\0') {
            flag = 1;  // если строка закончилась, то возвращаем NULL
        }
        res = str;
        while (*str != '\0' && !s21_is_delim(*str, delim)) {
            str++;  // ищем конец строки или разделитель
        }
        if (*str != '\0') {
            *str = '\0';  // если строка не закончилась, то ставим терминатор
            str++;  // и переходим к следующему символу
        }
        last = str;  // запоминаем последнее значение str
    }
    if (flag) res = NULL;
    return res;
}

int s21_is_delim(char c, const char *delim) {
    int res = 0;  // 1 если символ является разделителем, 0 если нет
    while (*delim != '\0') {
        if (c == *delim) {
            res = 1;
        }
        delim++;
    }
    return res;
}

int s21_strcmp(const char *s1, const char *s2) {
  while (*s1 && *s2 && (*s1 == *s2)) {
    s1++;
    s2++;
  }

  if (*s1 == *s2) {
    return 0;
  } else if (*s1 < *s2) {
    return -1;
  } else {
    return 1;
  }
}