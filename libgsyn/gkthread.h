#ifndef __GK_THREAD_H
#define __GK_THREAD_H

// These define portable threading functions and types.

/*
 * Proper prototypes for these functions:
 *
 * extern gk_thread_create(gk_thread_t *th, void*(*thread_func)(void*), void *arg);
 * extern int gk_thread_join(gk_thread_t th, void **thread_return);
 * extern void gk_thread_exit(void *retval);
 *
 * extern int gk_mutex_init(gk_mutex_t *mutex);
 * extern int gk_mutex_lock(gk_mutex_t *mutex);
 * extern int gk_mutex_unlock(gk_mutex_t *mutex);
 * extern int gk_mutex_trylock(gk_mutex_t *mutex);
 * extern int gk_mutex_destroy(gk_mutex_t *mutex);
 */

#ifdef _POSIX
#include <pthread.h>
typedef pthread_t 		gk_thread_t;
typedef pthread_mutex_t gk_mutex_t;
#define GKTHREAD_RET	void *
#define GKTHREAD_TYPE	

#define gk_thread_create(a,b,c) pthread_create(a, NULL, b, c)
#define gk_thread_join(a,b) 	pthread_join(a,b)
#define gk_thread_exit(a) 		pthread_exit(a)

#define gk_mutex_init(a) 		pthread_mutex_init(a, NULL)
#define gk_mutex_lock(a) 		pthread_mutex_lock(a)
#define gk_mutex_trylock(a) 	pthread_mutex_trylock(a)
#define gk_mutex_unlock(a) 		pthread_mutex_unlock(a)
#define gk_mutex_destroy(a) 	pthread_mutex_destroy(a)

#endif

#ifdef WIN32
#include <windows.h>
typedef HANDLE			gk_thread_t;
typedef HANDLE			gk_mutex_t;
#define GKTHREAD_RET	unsigned long
#define GKTHREAD_TYPE	WINAPI

#define gk_thread_create(a,b,c)	{DWORD __BLA_AH; *a = CreateThread(NULL, NULL, b, c, 0, &__BLA_AH);}
#define gk_thread_join(a,b)	{WaitForSingleObject(a, INFINITE);GetExitCodeThread(a, b);}
#define gk_thread_exit(a)	ExitThread(a)
#define gk_mutex_init(a)	{*a = CreateMutex(NULL, FALSE, NULL);}
#define gk_mutex_lock(a)	WaitForSingleObject(*a, INFINITE)
#define gk_mutex_trylock(a)	WaitForSingleObject(*a, 0)
#define gk_mutex_unlock(a)	ReleaseMutex(*a)
#define gk_mutex_destroy(a)	CloseHandle(*a)
#endif

#if 0
extern int gk_thread_create(gk_thread_t *th, void*(*thread_func)(void*), void *arg);
extern int gk_thread_join(gk_thread_t th, void **thread_return);
extern void gk_thread_exit(void *retval);

extern int gk_mutex_init(gk_mutex_t *mutex);
extern int gk_mutex_lock(gk_mutex_t *mutex);
extern int gk_mutex_unlock(gk_mutex_t *mutex);
extern int gk_mutex_trylock(gk_mutex_t *mutex);
extern int gk_mutex_destroy(gk_mutex_t *mutex);
#endif

#endif
