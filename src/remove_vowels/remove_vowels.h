#ifndef REMOVE_VOWELS_H
#define REMOVE_VOWELS_H


#include <stdlib.h> // size_t


#define REMOVE_VOWELS_OK 0
#define REMOVE_VOWELS_ERROR_NOT_ENOUGH_BUFFER_LEN 31


int remove_vowels(char* s, char* buff, size_t buff_size);
/*
	Убирает гласные из входной строки s. Результат записывает в buff.

	Возвращаемые значения:
		REMOVE_VOWELS_ERROR_NOT_ENOUGH_BUFFER_LEN - результирующая строка не поместилась в буфер
		REMOVE_VOWELS_OK - удаление гласных прошло успешно
*/


#endif