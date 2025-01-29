

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/wait.h>

#define INSTRUCTIONS "Instructions:\n\n\
first argument:\n\
  -proc or -thread to run process or thread version\n\
second argument:\n\
  -p: run the producer/consumer problem\n\
    -n {N}: number of producers (required if using -p)\n\
    -c {C}: number of consumers (required if using -p)\n\
  -d: run the dining philosopher's problem\n\
  -b: run the potion brewers problem\n\
examples:\n\
  ./bairdjo_hw4 -proc -p -n 3 -c 2\n\
  ./bairdjo_hw4 -thread -d\n\
  ./bairdjo_hw4 -proc -b\n"


#define PC_BUFFER_LEN 10

#define RESOURCE_COUNT 3
#define RESOURCE_TOTAL ((RESOURCE_COUNT * (RESOURCE_COUNT-1)) / 2)  

extern int errno;


void open_sem(sem_t **semaphore, char *name, int init_val) {
    *semaphore = sem_open(name, O_CREAT | O_EXCL, 0770, init_val);
    if(*semaphore == SEM_FAILED) {
        fprintf(stderr, "Error in sem_open %s: %s\n", name, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void destroy_sem(sem_t *semaphore, char *name) {
    if(sem_close(semaphore) == -1) {
        fprintf(stderr, "Error in sem_close %s: %s\n", name, strerror(errno));
    }
    if(sem_unlink(name) == -1) {
        fprintf(stderr, "Error in sem_unlink %s: %s\n", name, strerror(errno));
    }
}







struct pc_process_args {
    int id;
    int producer_loops;
    int consumer_loops;
    int pipeFD;
};


void init_pc_process_args(struct pc_process_args *args, int id, int producer_loops, int consumer_loops, int fd) {
    args->id = id;
    args->producer_loops = producer_loops;
    args->consumer_loops = consumer_loops;
    args->pipeFD = fd;
}


void do_process_producer_work(struct pc_process_args *args) {
    int val;
    for (int i = 0; i < args->producer_loops; i++) {
        sleep(rand() % 3);  
        val = (args->id * 1000) + i + 1;  
        write(args->pipeFD, (void *)&val, sizeof(int));  
        printf("producer %d put:     %d\n", args->id, val);
        fflush(stdout);  
    }
    exit(EXIT_SUCCESS);
}


void do_process_consumer_work(struct pc_process_args *args) {
    int val;
    for (int i = 0; i < args->consumer_loops; i++) {
        sleep(rand() % 3);  
        read(args->pipeFD, &val, sizeof(int));  
        printf("consumer %d got:           %d\n", args->id, val);
        fflush(stdout);  
    }
    exit(EXIT_SUCCESS);
}



int run_pc_processes(int producer_count, int consumer_count) {
    printf("[PROCESSES] Running Producer/Consumer simulation with %d producers, %d consumers.\n\n", producer_count, consumer_count);
    
    int pipeFDs[2]; 
    if (pipe(pipeFDs) == -1) {
        printf("Call to pipe() failed\n");
        exit(EXIT_FAILURE);
    }
    pid_t producers[producer_count];
    pid_t consumers[consumer_count];
    struct pc_process_args args;
    
    for (int i = 0; i < producer_count; i++) {
        init_pc_process_args(&args, i + 1, consumer_count * 3, producer_count * 3, pipeFDs[1]);
        if((producers[i] = fork()) == 0) do_process_producer_work(&args);
    }
    for (int i = 0; i < consumer_count; i++) {
        init_pc_process_args(&args, i + 1, consumer_count * 3, producer_count * 3, pipeFDs[0]);
        if((consumers[i] = fork()) == 0) do_process_consumer_work(&args);
    }
    
    int wstatus;
    for (int i = 0; i < producer_count; i++)
        waitpid(producers[i], &wstatus, 0);
    for (int i = 0; i < consumer_count; i++)
        waitpid(consumers[i], &wstatus, 0);
    return EXIT_SUCCESS;
}











struct PC_Buffer {
    sem_t *full_spots;  
    sem_t *empty_spots;  
    sem_t *mutex;  
    size_t put_index;  
    size_t get_index;  
    int count;  
    int array[PC_BUFFER_LEN];  
};
struct PC_Buffer pc_buffer;
char *pc_full_sem_name = "/pc_full_spots_semaphore";
char *pc_empty_sem_name = "/pc_empty_spots_semaphore";
char *pc_mutex_sem_name = "/pc_mutex_semaphore";


void init_pc(struct PC_Buffer *buff) {
    open_sem(&buff->full_spots, pc_full_sem_name, 0);  
    open_sem(&buff->empty_spots, pc_empty_sem_name, PC_BUFFER_LEN);  
    open_sem(&buff->mutex, pc_mutex_sem_name, 1);  
    buff->put_index = 0;
    buff->get_index = 0;
    buff->count = 1;
    memset(buff->array, -1, PC_BUFFER_LEN * sizeof(int));  
}


void destroy_pc(struct PC_Buffer *buff) {
    destroy_sem(buff->full_spots, pc_full_sem_name);
    destroy_sem(buff->empty_spots, pc_empty_sem_name);
    destroy_sem(buff->mutex, pc_mutex_sem_name);
}


void pc_put(int val, struct PC_Buffer *buff) {
    sem_wait(buff->empty_spots);  
    sem_wait(buff->mutex);  
    buff->array[buff->put_index] = val;  
    buff->count++;  
    buff->put_index = (buff->put_index + 1) % PC_BUFFER_LEN;  
    sem_post(buff->mutex);  
    sem_post(buff->full_spots);  
}


int pc_get(struct PC_Buffer *buff) {
    int val;
    sem_wait(buff->full_spots);  
    sem_wait(buff->mutex);  
    val = buff->array[buff->get_index];  
    buff->array[buff->get_index] = -1;  
    buff->get_index = (buff->get_index + 1) % PC_BUFFER_LEN;  
    sem_post(buff->mutex);  
    sem_post(buff->empty_spots);  
    return val;
}


struct pc_thread_args {
    int thread_id;
    int producer_loops;
    int consumer_loops;
};


void *do_producer_work(void *arg) {
    int val;
    struct pc_thread_args *args = (struct pc_thread_args *)arg;
    for (int i = 0; i < args->producer_loops; i++) {
        sleep(rand() % 3);  
        val = (args->thread_id * 1000) + i + 1;  
        pc_put(val, &pc_buffer);
        printf("producer %d put:     %d\n", args->thread_id, val);
        fflush(stdout);  
    }
    return NULL;
}


void *do_consumer_work(void *arg) {
    struct pc_thread_args *args = (struct pc_thread_args *)arg;
    int val;
    for (int i = 0; i < args->consumer_loops; i++) {
        sleep(rand() % 3);  
        val = pc_get(&pc_buffer);
        printf("consumer %d got:           %d\n", args->thread_id, val);
        fflush(stdout);  
    }
    return NULL;
}


void init_pc_thread_args(struct pc_thread_args *args, int id, int producer_loops, int consumer_loops) {
    args->thread_id = id;
    args->producer_loops = producer_loops;
    args->consumer_loops = consumer_loops;
}


int run_pc_threads(int producer_count, int consumer_count) {
    printf("[THREADS] Running Producer/Consumer simulation with %d producers, %d consumers.\n\n", producer_count, consumer_count);
    
    init_pc(&pc_buffer);
    pthread_t producer_threads[producer_count];
    pthread_t consumer_threads[consumer_count];
    int total_threads = producer_count > consumer_count ? producer_count : consumer_count;
    struct pc_thread_args all_args[total_threads];
    
    for (int i = 0; i < total_threads; i++) {
        init_pc_thread_args(&all_args[i], i + 1, consumer_count * 3, producer_count * 3);
        if(i < producer_count) pthread_create(&producer_threads[i], NULL, do_producer_work, &all_args[i]);
        if(i < consumer_count) pthread_create(&consumer_threads[i], NULL, do_consumer_work, &all_args[i]);
    }
    
    for (int i = 0; i < total_threads; i++) {
        if(i < producer_count) pthread_join(producer_threads[i], NULL);
        if(i < consumer_count) pthread_join(consumer_threads[i], NULL);
    }
    destroy_pc(&pc_buffer);
    return EXIT_SUCCESS;
}










void init_fork_semaphores(sem_t *forks[], char *fork_sems[]) {
    for(int i = 0; i < 5; i++)  open_sem(&forks[i], fork_sems[i], 1);
}

void destroy_fork_semaphores(sem_t *forks[], char *fork_sems[]) {
    for(int i = 0; i < 5; i++)  destroy_sem(forks[i], fork_sems[i]);
}


void get_forks(int p, sem_t *left_fork, sem_t *right_fork) {
    
    printf("Philosopher %d is acquiring forks...\n", p);
    fflush(stdout);
    if(p == 4) {
        sem_wait(right_fork);
        sem_wait(left_fork);
    } else {
        sem_wait(left_fork);
        sem_wait(right_fork);
    }
}

void put_forks(int p, sem_t *left_fork, sem_t *right_fork) {
    sem_post(left_fork);
    sem_post(right_fork);
}


void think(int p) {
    printf("Philosopher %d is thinking...\n", p);
    fflush(stdout);
    sleep((rand() % 20) + 1);  
}
void eat(int p) {
    printf("Philosopher %d is eating...\n", p);
    fflush(stdout);
    sleep((rand() % 8) + 2);  
}


int left(int p) { return p; }
int right(int p) { return (p + 1) % 5; }


int do_philosopher_work(int id, sem_t *left_fork, sem_t *right_fork) {
    for(int i = 0; i < 5; i++) {
        think(id);
        get_forks(id, left_fork, right_fork);
        eat(id);
        put_forks(id, left_fork, right_fork);
    }
    return EXIT_SUCCESS;
}


struct dining_thread_arg {
    int id;
    sem_t *left_fork;
    sem_t *right_fork;
};


void *do_philosopher_work_thread(void *data) {
    struct dining_thread_arg *arg = (struct dining_thread_arg *)data;
    do_philosopher_work(arg->id, arg->left_fork, arg->right_fork);
    return NULL;
}


int run_dining_philosophers_threads() {
    printf("[THREADS] Running Dining Philosopher's simulation. Each will eat 5 times before ending.\n\n");
    
    sem_t *forks[5];  
    char *fork_sems[5] = {"/fork_sem0", "/fork_sem1", "/fork_sem2", "/fork_sem3", "/fork_sem4"}; 
    init_fork_semaphores(forks, fork_sems);
    
    struct dining_thread_arg args[5];
    for(int i = 0; i < 5; i++) {
        args[i].id = i;
        args[i].left_fork = forks[left(i)];
        args[i].right_fork = forks[right(i)];
    }
    
    pthread_t philosophers[5];
    for (int i = 0; i < 5; i++)
        pthread_create(&philosophers[i], NULL, do_philosopher_work_thread, &args[i]);
    
    for (int i = 0; i < 5; i++)
        pthread_join(philosophers[i], NULL);
    destroy_fork_semaphores(forks, fork_sems);
    return EXIT_SUCCESS;
}






int run_dining_philosophers_processes() {
    printf("[PROCESSES] Running Dining Philosopher's simulation. Each will eat 5 times before ending.\n\n");
    
    sem_t *forks[5];  
    char *fork_sems[5] = {"/fork_sem0", "/fork_sem1", "/fork_sem2", "/fork_sem3", "/fork_sem4"}; 
    init_fork_semaphores(forks, fork_sems);
    
    pid_t philosophers[5];
    for (int i = 0; i < 5; i++)
        if((philosophers[i] = fork()) == 0)
            return do_philosopher_work(i, forks[left(i)], forks[right(i)]);
    
    int wstatus;
    for (int i = 0; i < 5; i++)
        waitpid(philosophers[i], &wstatus, 0);
    destroy_fork_semaphores(forks, fork_sems);
    return EXIT_SUCCESS;
}









struct brewmaster_sems {
    sem_t *agentSem;
    char *agentSemName;
    sem_t *resourceSems[RESOURCE_COUNT];
    char *resourceSemNames[RESOURCE_COUNT];
    sem_t *brewersSems[RESOURCE_COUNT];
    char *brewerSemNames[RESOURCE_COUNT];
    sem_t *resourceStateMutex;
    char *resourceStateMutexName;
} sems = {
        .agentSemName = "/agent_brewmaster",
        .resourceSemNames = {"/bezoars", "/unicorn_horns", "/mistletoe_berries"},
        .brewerSemNames = {"/bezoars_brewer", "/unicorn_horns_brewer", "/mistletoe_berries_brewer"},
        .resourceStateMutexName = "/pusher_state"
};;

struct ResourceState {
    int count;  
    int total;  
} resourceState = {0, 0};


void init_brewmaster_sems(struct brewmaster_sems *sems) {
    open_sem(&sems->agentSem, sems->agentSemName, 1);
    open_sem(&sems->resourceStateMutex, sems->resourceStateMutexName, 1);
    for(int i = 0; i < RESOURCE_COUNT; i++) {
        open_sem(&sems->resourceSems[i], sems->resourceSemNames[i], 0);
        open_sem(&sems->brewersSems[i], sems->brewerSemNames[i], 0);
    }
}

void destroy_brewmaster_sems(struct brewmaster_sems *sems) {
    destroy_sem(sems->agentSem, sems->agentSemName);
    destroy_sem(sems->resourceStateMutex, sems->resourceStateMutexName);
    for(int i = 0; i < RESOURCE_COUNT; i++) {
        destroy_sem(sems->resourceSems[i], sems->resourceSemNames[i]);
        destroy_sem(sems->brewersSems[i], sems->brewerSemNames[i]);
    }
}



int do_agent_work(int missing_resource) {
    for(int i = 0; i < 10; i++) {      
        sem_wait(sems.agentSem);
        usleep(250000);  
        for(int j = 0; j < RESOURCE_COUNT; j++) {
            
            if(j != missing_resource) {
                printf("Agent produced:          %s\n", sems.resourceSemNames[j]+1);
                fflush(stdout);
                sem_post(sems.resourceSems[j]);
            }
        }
    }
    return EXIT_SUCCESS;
}
void *do_agent_work_threads(void *arg) {
    int missing_resource = *(int *)arg;
    do_agent_work(missing_resource);
    return NULL;
}



int do_pusher_work(int assigned_resource) {
    for(int i = 0; i < (10 * (RESOURCE_COUNT - 1)); i++) {
        
        sem_wait(sems.resourceSems[assigned_resource]);
        sem_wait(sems.resourceStateMutex);
        resourceState.count++;
        resourceState.total += assigned_resource;
        
        if(resourceState.count == (RESOURCE_COUNT - 1)) {
            sem_post(sems.brewersSems[RESOURCE_TOTAL - resourceState.total]);
            resourceState.count = 0;
            resourceState.total = 0;
        }
        sem_post(sems.resourceStateMutex);
    }
    return EXIT_SUCCESS;
}
void *do_pusher_work_threads(void *arg) {
    int assigned_resource = *(int *)arg;
    do_pusher_work(assigned_resource);
    return NULL;
}


int do_brewer_work(int assigned_resource) {
    for(int i = 0; i < 10; i++) {
        sem_wait(sems.brewersSems[assigned_resource]);
        printf("Potion by brewer with:   %s\n", sems.resourceSemNames[assigned_resource]+1);
        fflush(stdout);
        sem_post(sems.agentSem);
    }
    return EXIT_SUCCESS;
}
void *do_brewer_work_threads(void *arg) {
    int assigned_resource = *(int *)arg;
    do_brewer_work(assigned_resource);
    return NULL;
}


int run_brew_master_threads() {
    printf("[THREADS] Running the brew master simulation. 10 potions of each type will be produced.\n\n");
    init_brewmaster_sems(&sems);
    int resource[RESOURCE_COUNT] = {0, 1, 2};  
    
    pthread_t agents[RESOURCE_COUNT];  
    pthread_t pushers[RESOURCE_COUNT];  
    pthread_t brewers[RESOURCE_COUNT];  
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        pthread_create(&agents[i], NULL, do_agent_work_threads, &resource[i]);
        pthread_create(&pushers[i], NULL, do_pusher_work_threads, &resource[i]);
        pthread_create(&brewers[i], NULL, do_brewer_work_threads, &resource[i]);
    }
    
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        pthread_join(agents[i], NULL);
        pthread_join(pushers[i], NULL);
        pthread_join(brewers[i], NULL);
    }
    destroy_brewmaster_sems(&sems);
    return EXIT_SUCCESS;
}






int do_agent_work_processes(int missing_resource) {return do_agent_work(missing_resource);}
int do_brewer_work_processes(int assigned_resource) {return do_brewer_work(assigned_resource);}
int do_pusher_work_processes(int assigned_resource, int pipeFD) {
    
    for(int i = 0; i < (10 * (RESOURCE_COUNT - 1)); i++) {
        sem_wait(sems.resourceSems[assigned_resource]);
        write(pipeFD, (void *)&assigned_resource, sizeof(int));
    }
    return EXIT_SUCCESS;
}
int do_aggregator_work_processes(int pipeFD) {
    
    int count = 0;
    int total = 0;
    int produced_resource = 0;
    for(int i = 0; i < (10 * (RESOURCE_COUNT - 1) * RESOURCE_COUNT); i++) {
        read(pipeFD, &produced_resource, sizeof(int));
        count++;
        total += produced_resource;
        
        if(count == (RESOURCE_COUNT - 1)) {
            sem_post(sems.brewersSems[RESOURCE_TOTAL - total]);
            count = 0;
            total = 0;
        }
    }
    return EXIT_SUCCESS;
}

int run_brew_master_processes() {
    printf("[PROCESSES] Running the brew master simulation. 10 potions of each type will be produced.\n\n");
    init_brewmaster_sems(&sems);
    
    int pipeFDs[2]; 
    if (pipe(pipeFDs) == -1) {
        printf("Call to pipe() failed\n");
        exit(EXIT_FAILURE);
    }
    
    pid_t agents[RESOURCE_COUNT];  
    pid_t pushers[RESOURCE_COUNT];  
    pid_t brewers[RESOURCE_COUNT];  
    pid_t aggregator;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if((agents[i] = fork()) == 0) return do_agent_work_processes(i);
        if((pushers[i] = fork()) == 0) return do_pusher_work_processes(i, pipeFDs[1]);
        if((brewers[i] = fork()) == 0) return do_brewer_work_processes(i);
    }
    if((aggregator = fork()) == 0) return do_aggregator_work_processes(pipeFDs[0]);
    
    int wstatus;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        waitpid(agents[i], &wstatus, 0);
        waitpid(pushers[i], &wstatus, 0);
        waitpid(brewers[i], &wstatus, 0);
    }
    waitpid(aggregator, &wstatus, 0);
    destroy_brewmaster_sems(&sems);
    return EXIT_SUCCESS;
}







int exit_and_print_instructions() {
    printf("%s", INSTRUCTIONS);
    return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc < 3)
        return exit_and_print_instructions();
    
    int is_thread;
    if(strncmp(argv[1], "-thread", 7) == 0) {
        is_thread = 1;
    } else if (strncmp(argv[1], "-proc", 5) == 0) {
        is_thread = 0;
    } else {
        return exit_and_print_instructions();
    }
    
    if(strncmp(argv[2], "-p", 2) == 0) {
        
        if(argc < 7)
            return exit_and_print_instructions();
        if(strncmp(argv[3], "-n", 2) != 0 || strncmp(argv[5], "-c", 2) != 0)
            return exit_and_print_instructions();
        int producers = atoi(argv[4]);
        int consumers = atoi(argv[6]);
        return is_thread ? run_pc_threads(producers, consumers) : run_pc_processes(producers, consumers);
    } else if (strncmp(argv[2], "-d", 2) == 0) {
        
        return is_thread ? run_dining_philosophers_threads() : run_dining_philosophers_processes();
    } else if (strncmp(argv[2], "-b", 2) == 0) {
        
        return is_thread ? run_brew_master_threads() : run_brew_master_processes();
    } else {
        return exit_and_print_instructions();
    }
}
