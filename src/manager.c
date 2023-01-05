#include "manager.h"
#include "io_line/io_line.h"
#include <unistd.h>
#include <string.h>


#define MAX_INPUT_STRING_LEN 256


void manager_logic(MappedBuffer workers[2])
{
    char input_string[MAX_INPUT_STRING_LEN];

    while(1)
    {
        int res = read_line(STDIN_FILENO, input_string, MAX_INPUT_STRING_LEN);

        if(res != IO_LINE_OK)
            exit(res);

        if(strlen(input_string) == 0)
            break;

        int i = strlen(input_string) > 10;
        send_data(&workers[i], input_string, strlen(input_string));
    }
}