#ifndef IO_LINE_H
#define IO_LINE_H


#include <stdlib.h> // size_t


#define IO_LINE_OK 0
#define IO_LINE_INVALID_DESCRIPTOR 11
#define IO_LINE_ACCESS_DENIED 12
#define IO_LINE_NOT_ENOUGH_BUFFER_SIZE 13


typedef int Descriptor;
typedef char* String;
typedef size_t StringSize;


int write_line(Descriptor d, String line);
/*
	Возвращаемые значения:
		IO_LINE_ACCESS_DENIED - режим дескриптора не позволяет записать строку
		IO_LINE_INVALID_DESCRIPTOR - неверный дексриптор
		IO_LINE_OK - строка успешно записана
*/


int read_line(Descriptor d, String buffer, StringSize buffer_size);
/*
	Возвращаемые значения:
		IO_LINE_ACCESS_DENIED - режим дескриптора не позволяет считать строку
		IO_LINE_INVALID_DESCRIPTOR - неверный дексриптор
		IO_LINE_NOT_ENOUGH_BUFFER_SIZE - размера буфера недостаточно для полного считывания
		IO_LINE_OK - строка успешно считана
*/


#endif