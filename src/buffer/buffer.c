#include "buffer.h"
#include <string.h> // memcpy
#include <sys/mman.h> // mmap, munmap, PROT_READ, PROT_WRITE, MAP_SHARED, MAP_ANONYMOUS
#include <semaphore.h> // sem_t, sem_init, sem_post, sem_wait, sem_getvalue


typedef struct __BufferMetadata
{
    sem_t empty;
    sem_t full;
    void *buff;
} BufferMetadata;


int init_buffer(MappedBuffer* buffer, size_t size)
{
    BufferMetadata* md = mmap(
        NULL, sizeof(BufferMetadata),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        0, 0
    );

    md->buff = mmap(
        NULL, size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        0, 0
    );

    buffer->pointer = md;
    buffer->size = size;
    
    sem_init(&md->full, 1, 0);
    sem_init(&md->empty, 1, 1);

    return BUFFER_OK;
}


int send_data(MappedBuffer* buffer, void* data, size_t size)
{
    BufferMetadata *md = buffer->pointer;

    while(size > 0)
    {
        size_t copy_size = (size < buffer->size) ? size : buffer->size;

        sem_wait(&md->empty);
        memcpy(md->buff, data, copy_size);
        sem_post(&md->full);

        size -= copy_size;
        data += copy_size;
    }

    return BUFFER_OK;
}


int receive_data(MappedBuffer* buffer, void* buff, size_t size)
{
    BufferMetadata *md = buffer->pointer;

    while(size > 0)
    {
        size_t copy_size = (size < buffer->size) ? size : buffer->size;
        
        sem_wait(&md->full);
        memcpy(buff, md->buff, copy_size);
        sem_post(&md->empty);

        size -= copy_size;
        buff += copy_size;
    }

    return BUFFER_OK;
}


void close_buffer(MappedBuffer* buffer)
{
    BufferMetadata *md = buffer->pointer;

    munmap(md->buff, buffer->size);
    sem_destroy(&md->full);
    sem_destroy(&md->empty);
    munmap(buffer->pointer, sizeof(BufferMetadata));
}
