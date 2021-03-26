#define Pthread_mutex_init(m, v)    assert(pthread_mutex_init(m, v) == 0);
#define Pthread_mutex_lock(m)       assert(pthread_mutex_lock(m) == 0);
#define Pthread_mutex_unlock(m)     assert(pthread_mutex_unlock(m) == 0);
#define Pthread_create(u,x,y,z)     assert(pthread_create(u,x,y,z) == 0);
#define Pthread_join(m,v)           assert(pthread_join(m,v)==0);
#define ONE_MILLION 1e6
