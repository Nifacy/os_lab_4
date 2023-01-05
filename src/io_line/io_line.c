#include "io_line.h"
#include <stdio.h> // EOF
#include <unistd.h> // read, write
#include <string.h> // strlen
#include <errno.h> // errno
#include <stdbool.h> // bool


bool __is_endline_symbol(char symbol)
{
	return (symbol == EOF || symbol == '\n');
}


bool __try_to_write(Descriptor d, void* data, size_t data_size)
{
	return write(d, data, data_size) == data_size;
}


bool __try_to_read(Descriptor d, void* data, size_t data_size)
{
	return read(d, data, data_size) == data_size;
}


int __handle_file_error(int error)
{
	if(error == EACCES)
		return IO_LINE_ACCESS_DENIED;

	if(error == EBADF)
		return IO_LINE_INVALID_DESCRIPTOR;
}


int __get_char(Descriptor d, char* symbol)
{
	if(!__try_to_read(d, symbol, 1 * sizeof(char)))
		return __handle_file_error(errno);

	return IO_LINE_OK;
}


int write_line(Descriptor d, String line)
{
	StringSize line_length = strlen(line);

	if(!__try_to_write(d, line, line_length * sizeof(char)))
		return __handle_file_error(errno);

	if(!__try_to_write(d, "\n", 1 * sizeof(char)))
		return __handle_file_error(errno);

	return IO_LINE_OK;
}


int read_line(Descriptor d, String buffer, StringSize buffer_size)
{
	char symbol;
	size_t index = 0;
	int result;

	while(1)
	{
		result = __get_char(d, &symbol);
		
		if(result != IO_LINE_OK)
			return result;

		if(__is_endline_symbol(symbol))
			break;

		if(symbol == '\r')
			continue;

		if(index >= buffer_size - 1)
			return IO_LINE_NOT_ENOUGH_BUFFER_SIZE;

		buffer[index++] = symbol;
	}

	buffer[index] = '\0';
	return IO_LINE_OK;
}
