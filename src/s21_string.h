#ifndef S21_STRING_S21_STRING_H
#define S21_STRING_S21_STRING_H

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
//#define ERR_MSG_ARR sys_errlist


//#define s21_size_t unsigned long int

typedef unsigned long s21_size_t;
//#ifdef __APPLE__
//#define ERR_MSG_ARR sys_errlist
//#else
//#define ERR_MSG_ARR _sys_errlist
//#endif
//
////
//#define s21_size_t unsigned long int
//#define s21_NULL NULL







void *s21_memchr(const void *s, int c, s21_size_t n);
int s21_memcmp(const void *s1, const void *s2, size_t n);
void s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *s, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1 ,const char *str2, s21_size_t n);
const char *s21_strncpy(char *dest, const char *src, s21_size_t n) ;
s21_size_t s21_strcspn(const char *str1, const char *str2);
//void s21_snprintf(char *s, s21_size_t n, const char *format, int errnum, const char *const string);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1,const char *str2) ;
char *s21_strrchr(const char *str, int ch);
char *s21_strstr(const char *haystack, const char *needle);
//char *strtok(char *str, const char *delim);
char* s21_strcat(char *dest, const char *src);
char* s21_strcpy(char *dest, const char *src);
char * s21_strdup(const char *str);
void *s21_trim(char* str);
void s21_memmove(void* dest, const void* src, size_t n);
char *s21_insert(char *src, const char *str, s21_size_t index) ;
int s21_sprintf(const char *format, ...);
static char *reverse_string(char *str);
static char *convert_integer(long number, char *buf, int base, bool negative);
//char *s21_strpbrk(const char *str,  char *  charset);
s21_size_t s21_strspn(const char* s1, const char* s2 );
char *s21_strtok( char *str, const char *delim);
int s21_strcmp(const char *s1, const char *s2);

int s21_is_delim(char c, const char *delim);







        int s21_sscanf(const char *str, const char *format, ...);


#endif //S21_STRING_S21_STRING_H
