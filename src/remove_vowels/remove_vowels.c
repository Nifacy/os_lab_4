#include "remove_vowels.h"
#include <string.h> // strlen
#include <stdbool.h> // bool


char* __vowels = "eyuioaEYUIOA";


bool __is_vowel(char symbol)
{
	for(char *c = __vowels; *c != '\0'; c++)
		if(*c == symbol)
			return true;

	return false;
}


int remove_vowels(char* s, char* buff, size_t buff_size)
{
	int index = 0;
	size_t s_length = strlen(s);
	int ret_value = REMOVE_VOWELS_OK;

	for(int j = 0; (j < s_length) && (index < buff_size - 1); j++)
	{
		if(__is_vowel(s[j]))
			continue;

		buff[index++] = s[j];
	}

	buff[index] = '\0';

	if(index == buff_size - 1)
		return REMOVE_VOWELS_ERROR_NOT_ENOUGH_BUFFER_LEN;
	return REMOVE_VOWELS_OK;
}
