// A thread is a lighweight process
// Every process starts with one thread (the main thread).
// Multiple threads can run concurrently within one process, sharing the same memory and resources.

#include<stdio.h>
void* thread_code(void* param){
	printf("In Thread code.\n");
}

int main(){
	pthread_t thread;
	pthread_create(&thread, 0, &thread_code, 0);
	printf("In main thread.\n");
	pthread_join(thread, 0);  // To ensure both threads run completely, use pthread_join() to wait for the thread to finish
}
/*
Breakdown

    The main() function starts with the main thread.

    A new thread is created with pthread_create(), running the function thread_code.

    The attributes parameter is 0, so default thread attributes are used.

    No parameter is passed to the new thread, so the last argument is 0.

    Both threads (main and new) will print a message. The output order may vary, 		since threads run independently.

    The pthread_create() function returns 0 if thread creation is successful.
*/


void* thread_code(void* param) {
    printf("In thread code\n");
    pthread_exit("Thread finished"); // Returns a value to be captured by pthread_join()
}
// The function need not explicitly call pthread_exit(). If a thread function completes naturally, pthread_exit() is called implicitly.



#include <pthread.h>
#include <stdio.h>

void* child_thread(void *param) {
    int id = (int)param;              // Recover the integer passed as a void pointer
    printf("Start thread %i\n", id);  // Print the thread's identifier
    return (void*)id;                 // Return value (as void pointer)
}

int main() {
    pthread_t thread[10];
    int return_value[10];
    for (int i = 0; i < 10; i++) {
        pthread_create(&thread[i], 0, &child_thread, (void*)i); // Pass integer id
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(thread[i], (void**)&return_value[i]);      // Retrieve return value from thread
        printf("End thread %i\n", return_value[i]);             // Print returned value
    }
}



