#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* thread_function_mutex(void* arg);
void* thread_function_sem(void* arg);

void* thread_function_father(void* arg);
void* thread_function_mother(void* arg);
void* thread_function_son1(void* arg);
void* thread_function_son2(void* arg);
void* thread_function_daughter(void* arg);

//void multi_quick_sort(int a[], int low, int high);
void* multi_quick_sort(void* arg);

int g_counter = 0;

pthread_mutex_t work_mutex;
sem_t bin_sem;

sem_t sem_blank;
sem_t sem_orange;
sem_t sem_apple;

int main(int argc, char* argv[])
{
#if 0
    pthread_t thread1;
    pthread_t thread2;
    
    pthread_mutex_init(&work_mutex, NULL);//必须先初始化
    
    pthread_create(&thread1, NULL, thread_function_mutex, NULL);
    pthread_create(&thread2, NULL, thread_function_mutex, NULL);
    
    //等待子线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Main thread finished\n");
    
    pthread_mutex_destroy(&work_mutex);//用完以后销毁
#endif

#if 0
    sem_init(&bin_sem, 0, 2);
    
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    
    pthread_create(&thread1, NULL, thread_function_sem, NULL);
    pthread_create(&thread2, NULL, thread_function_sem, NULL);
    pthread_create(&thread3, NULL, thread_function_sem, NULL);
    pthread_create(&thread4, NULL, thread_function_sem, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    
    printf("Main thread finished\n");
    
    sem_destroy(&bin_sem);//用完以后销毁
#endif

#if 0    
    sem_init(&sem_blank, 0, 1);
    sem_init(&sem_orange, 0, 0);
    sem_init(&sem_apple, 0, 0);
    
    pthread_t thread_father;
    pthread_t thread_mother;
    pthread_t thread_son1;
    pthread_t thread_son2;
    pthread_t thread_daughter;
    
    pthread_create(&thread_father, NULL, thread_function_father, NULL);
    pthread_create(&thread_mother, NULL, thread_function_mother, NULL);
    pthread_create(&thread_son1, NULL, thread_function_son1, NULL);
    pthread_create(&thread_son2, NULL, thread_function_son2, NULL);
    pthread_create(&thread_daughter, NULL, thread_function_daughter, NULL);
    
    pthread_join(thread_father, NULL);
    pthread_join(thread_mother, NULL);
    pthread_join(thread_son1, NULL);
    pthread_join(thread_son2, NULL);
    pthread_join(thread_daughter, NULL);
    
    printf("Main thread finished\n");
    
    sem_destroy(&sem_blank);
    sem_destroy(&sem_orange);
    sem_destroy(&sem_apple);
#endif

    int a[12] = {45, 0, 23, 3, 4, 21, 89, 33, 21, 47, 83, 1};
    int low = 0;
    int high = sizeof(a) / sizeof(int) - 1;
    void* para[3] = {(void*)a, &low, &high};
    
    printf("Before sort:\n");
    for (int i = 0; i < sizeof(a) / sizeof(int); i++)
    {
        printf("%d\n", a[i]);
    }
    
    pthread_t sort;
    pthread_create(&sort, NULL, multi_quick_sort, para);
    pthread_join(sort, NULL);
    
    printf("After sort:\n");
    for (int i = 0; i < sizeof(a) / sizeof(int); i++)
    {
        printf("%d\n", a[i]);
    }
    
    printf("Main thread finished\n");
    return 0;
}

//该线程函数使用互斥量进行同步
void* thread_function_mutex(void* arg)
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

//该线程函数使用同步量进行同步
void* thread_function_sem(void* arg)
{
    for (int i = 0; i < 200; i++)
    {
        sem_wait(&bin_sem);
        g_counter++;
        printf("%d output g_counter: %d\n", pthread_self(), g_counter);
        sem_post(&bin_sem);
    }
    
    printf("Child thread finished\n");
}

//生产桔子
void* thread_function_father(void* arg)
{
    while (true)
    {
        sem_wait(&sem_blank);
        printf("Father put an orange\n");
        sem_post(&sem_orange);
        sleep(1);
    }
}

//生产苹果
void* thread_function_mother(void* arg)
{
    while (true)
    {
        sem_wait(&sem_blank);
        printf("Mother put an apple\n");
        sem_post(&sem_apple);
        sleep(1);
    }
}

//吃桔子
void* thread_function_son1(void* arg)
{
    while (true)
    {
        sem_wait(&sem_orange);
        printf("Son1 get an orange\n");
        sem_post(&sem_blank);
        sleep(1);
    }
}

//吃桔子
void* thread_function_son2(void* arg)
{
    while (true)
    {
        sem_wait(&sem_orange);
        printf("Son2 get an orange\n");
        sem_post(&sem_blank);
        sleep(1);
    }
}

//吃苹果
void* thread_function_daughter(void* arg)
{
    while (true)
    {
        sem_wait(&sem_apple);
        printf("Daughter get an apple\n");
        sem_post(&sem_blank);
        sleep(1);
    }
}

int partition(int a[], int low, int high)
{
    printf("low is: %d;high is: %d\n", low ,high);
    
	int tmp = a[low];

	while (low < high)
	{
		while (low < high && a[high] >= tmp)
		{
			high--;
		}
		a[low] = a[high];

		while (low < high && a[low] <= tmp)
		{
			low++;
		}
		a[high] = a[low];
	}

	a[low] = tmp;
	return low;
}

/*
void quick_sort(int a[], int low, int high)
{
    printf("low is: %d;high is: %d\n", low ,high);
	if (low < high)
	{
		int pos = partition(a, low, high);
		printf("pos is: %d\n", pos);
		quick_sort(a, low, pos - 1);
		quick_sort(a, pos + 1, high);
	}
}
*/

void* multi_quick_sort(void* arg)
{
    //int* a = (int*)arg[0];
    //int low = (int)*arg[1];
    //int high = (int)*arg[2];
    
    int* a = (int*)((void**)arg)[0];
    int low = *(int*)((void**)arg)[1];
    int high = *(int*)((void**)arg)[2];
    
    
	if (low < high)
	{
		int pos = partition(a, low, high);
		int pos1 = pos - 1;
		int pos2 = pos + 1;
		
		pthread_t low_part;
        pthread_t high_part;
        
        void* low_para[3] = {(void*)a, &low, &pos1};
        void* high_para[3] = {(void*)a, &pos2, &high};
        pthread_create(&low_part, NULL, multi_quick_sort, low_para);
        pthread_join(low_part, NULL);
        
        pthread_create(&high_part, NULL, multi_quick_sort, high_para);
        pthread_join(high_part, NULL);
	}
}



