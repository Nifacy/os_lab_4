#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

#define BUFFER_OK 0
#define BUFFER_ERROR_INIT 1
#define BUFFER_ERROR_RECEIVE 2
#define BUFFER_ERROR_SEND 3


struct __MappedBuffer
{
    void* pointer;
    size_t size;
};

typedef struct __MappedBuffer MappedBuffer;


int init_buffer(MappedBuffer* buffer, size_t buf_size);
/*
    Возвращаемые значения:
		BUFFER_OK - buffer успешно инициализирован
		BUFFER_ERROR_INIT - не удалось инициализировать buffer
*/

int send_data(MappedBuffer* buffer, void* data, size_t size);
/*
    Возвращаемые значения:
        BUFFER_OK - данные успешо отправлены
        BUFFER_ERROR_SEND - произошла ошибка при отправке сообщений
*/

int receive_data(MappedBuffer* buffer, void* buff, size_t size);
/*
    Возвращаемые значения:
        BUFFER_OK - данные были успешно получены
        BUFFER_ERROR_RECEIVE - произошла ошибка при получении данных
*/


void close_buffer(MappedBuffer* buffer);

#endif
