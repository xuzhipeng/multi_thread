#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <pthread.h>

void* thread_function(void* arg);

int g_counter = 0;

pthread_mutex_t work_mutex;

int main(int argc, char* argv[])
{
    pthread_t thread1;
    pthread_t thread2;
    
    pthread_mutex_init(&work_mutex, NULL);//必须先初始化
    
    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);
    
    //等待子线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Main thread finished\n");
    
    pthread_mutex_destroy(&work_mutex);//用完以后销毁
    
    return 0;
}

//该线程函数使用互斥进行同步
void* thread_function(void* arg)
{
    for (int i = 0; i < 200; i++)
    {
        pthread_mutex_lock(&work_mutex);
        //注意！多次调用将导致死锁
        //pthread_mutex_lock(&work_mutex);
        g_counter++;
        printf("%d output g_counter: %d\n", pthread_self(), g_counter);
        pthread_mutex_unlock(&work_mutex);
    }
    
    printf("Child thread finished\n");
}
