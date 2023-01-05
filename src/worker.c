#include "worker.h"
#include "io_line/io_line.h" // read_line, write_line
#include "remove_vowels/remove_vowels.h" // remove_vowels
#include <string.h> // strlen
#include <stdlib.h> // exit
#include <fcntl.h> // open
#include <unistd.h> // close, STDIN_FILENO

#define MAX_INPUT_LINE_LEN 256
#define FAILED_TO_OPEN_RESULT_FILE 52


int protected_open_result_file_descriptor(char* filepath)
{
	int descriptor;

	if((descriptor = open(filepath, O_WRONLY | O_TRUNC)) == -1)
		exit(FAILED_TO_OPEN_RESULT_FILE);

	return descriptor;
}


void protected_get_data(MappedBuffer* src, char* buff, size_t buff_size)
{
	receive_data(src, buff, buff_size);
}


void protected_write_line(int descriptor, char* line)
{
	int result = write_line(descriptor, line);

	if(result != IO_LINE_OK)
		exit(result);
}


void protected_format_line(char* input_line, char* buff, size_t buff_size)
{
	int result = remove_vowels(input_line, buff, buff_size);

	if(result != REMOVE_VOWELS_OK)
		exit(result);
}


void worker_logic(MappedBuffer* buff, char* result_filepath)
{
    int result_file_descriptor;
	char input_line[MAX_INPUT_LINE_LEN];
	char formatted_line[MAX_INPUT_LINE_LEN];

	result_file_descriptor = protected_open_result_file_descriptor(result_filepath);
	protected_get_data(buff, input_line, MAX_INPUT_LINE_LEN);

	while(strlen(input_line) != 0)
	{
		protected_format_line(input_line, formatted_line, MAX_INPUT_LINE_LEN);
		protected_write_line(result_file_descriptor, formatted_line);
		protected_get_data(buff, input_line, MAX_INPUT_LINE_LEN);
	}

	close(result_file_descriptor);
	exit(0);
}
