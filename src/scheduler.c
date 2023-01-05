#include "manager.h" // manager_logic
#include "worker.h" // worker_logic
#include "buffer/buffer.h" // init_buffer, close_buffer, send_data

#include <stdlib.h> // exit
#include <unistd.h> // close, access, fork
#include <fcntl.h> // open
#include <sys/wait.h> // waitpid, pid_t
#include <sys/types.h> // pid_t
#include <sys/stat.h> // O_CREAT, S_IWRITE, S_IREAD


#define BUFFER_SIZE 256
#define MAX_INPUT_STRING_LEN 256
#define WORKER_PATH "worker"

#define CREATE_PROCESS_ERROR 41
#define MANAGER_ERROR_CREATE_RESULT_FILE 44
#define MANAGER_ERROR_RESULT_FILE_NOT_SPECIFIED 45
#define MANAGER_ERROR_OPEN_RESULT_FILE 46


/* result files check */

void protected_create_file(char* filename)
{
	int descriptor = open(filename, O_CREAT, S_IWRITE | S_IREAD);

	if(descriptor == -1)
		exit(MANAGER_ERROR_CREATE_RESULT_FILE);

	close(descriptor);
}


char* protected_get_result_file(int argc, char* argv[], int index)
{
	int descriptor;

	if(index >= argc)
		exit(MANAGER_ERROR_RESULT_FILE_NOT_SPECIFIED);

	// file doesn't exists
	if(access(argv[index], F_OK) != 0)
		protected_create_file(argv[index]);
	
	descriptor = open(argv[index], O_WRONLY);

	if(descriptor == -1)
		exit(MANAGER_ERROR_OPEN_RESULT_FILE);

	close(descriptor);
	return argv[index];
}


/* start worker */

pid_t create_worker(MappedBuffer* buff, char* result_filepath)
{
	pid_t pid = fork();

	if(pid < 0)
		exit(CREATE_PROCESS_ERROR);

	if(pid == 0)
		worker_logic(buff, result_filepath);

	return pid;
}


void close_worker(pid_t worker_id, MappedBuffer* buff)
{	
	send_data(buff, "\0", 1);
	waitpid(worker_id, NULL, 0);
}


int main(int argc, char* argv[])
{
	char input_string[MAX_INPUT_STRING_LEN];

	char* result_filepaths[2];
	MappedBuffer worker_buffers[2];
	pid_t worker_ids[2];

	// reading output files' paths
	for(int i = 0; i < 2; i++)
		result_filepaths[i] = protected_get_result_file(argc, argv, i + 1);

	// init buffers
	for(int i = 0; i < 2; i++)
		init_buffer(&worker_buffers[i], BUFFER_SIZE);

	// creating workers
	for(int i = 0; i < 2; i++)
		worker_ids[i] = create_worker(&worker_buffers[i], result_filepaths[i]);

	manager_logic(worker_buffers);

	// closing workers
	for(int i = 0; i < 2; i++)
		close_worker(worker_ids[i], &worker_buffers[i]);

	// clsing buffers
	for(int i = 0; i < 2; i++)
		close_buffer(&worker_buffers[i]);

	exit(0);
}
