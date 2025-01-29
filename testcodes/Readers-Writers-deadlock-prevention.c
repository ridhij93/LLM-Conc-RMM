#include  <unistd.h>
#include  <signal.h>
#include  <cstdio>
#include  <cstdlib>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>         
#include <sys/sem.h>	     


#define FILE_PATH "res.txt"

union semuns {
    int		val;		
    struct semid_ds	*buf;		
    unsigned short	*array;		
}arg;

void print_contents(char * conts, off_t size){
    std::cout << "Current State of resources: " << std::endl;
    for(int i = 0; i < size; i++){
        std::cout << conts[i];
    }
    std::cout << std::endl;
}


int main() {

    pid_t child_pid_t;
    

    int sem_id;
    union semuns sem_val{};

    int sys_call_ret;
    sem_id = semget(2000,1,IPC_CREAT | 0666);
    if(sem_id == -1){
        perror("Error creating sem");
        exit(1);
    }
    
    sem_val.val = 1;

    
    sys_call_ret = semctl(sem_id,0,SETVAL,sem_val);
    if(sys_call_ret == -1){
        perror("Error initializing system sem");
        exit(1);
    }
    printf("sem val of %d initialized in system\n",2000);



    struct stat buffer{};
    int fd = open(FILE_PATH, O_RDWR); 

    
    fstat(fd,&buffer);
    
    off_t size = buffer.st_size;

    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    close(fd);

    

    if (addr == (void*) -1 ) exit(SIGBUS);


    

    if((child_pid_t = fork()) == -1){
        perror("fork");
        exit(-1);
    }

    if(child_pid_t == 0){
        
        while(1) {
            struct sembuf sem_op{};
            sleep(10);
            char *vec;
            const size_t ps = sysconf(_SC_PAGESIZE);
            size_t pageIndex;
            std::cout << "Page Size: " << getpagesize()/4096 << std::endl;
            print_contents((char *)addr,size);
            vec = (char *)calloc(1, (size + ps - 1) / ps);

            mincore(addr, size, vec);
            for (pageIndex = 0; pageIndex <= size/ps; pageIndex++) {
                if (vec[pageIndex]&1) {
                    printf("Page: %zd\n Resident", pageIndex);
                }
            }
        }
    }else{
        
        
        while(true){

            std::string resource;
            int units = 0;

            std::string response;

            std::cout << "New Resources To be Added?" << std::endl;
            std::cin >> response;

            
            struct sembuf sem_op{};
            if(response == "y") {
                std::cout << "Enter type and amount" << std::endl;
                std::cin >> resource;
                std::cin >> units;
                
                
                sem_op.sem_num = 0;
                sem_op.sem_op = -1;
                sem_op.sem_flg = 0;
                semop(sem_id,&sem_op,1);
                char *shared = (char *) addr;
                for (off_t j = 0; j < size; j++) {
                    char temp = shared[j];
                    if (temp == resource[0] && resource.size() == 1 && shared[j+1] == '\t' && j % 4 == 0) {
                        char units_t = shared[j + 2];
                        shared[j + 2] = units_t + units;
                    }
                    if (temp == resource[0] && shared[j + 1] == resource[1] && resource.size() == 2 && j % 4 == 0) {
                        char units_t = shared[j + 3];
                        shared[j + 3] = units_t + units;
                    }
                }

                if (::msync((void *) shared, size, MS_SYNC) < 0) {
                    perror("msync failed with error:");
                } else {
                    printf("%s", "Synced File from memory\n");
                }

                sem_op.sem_num = 0;
                sem_op.sem_op = 1;
                sem_op.sem_flg = 0;
                semop(sem_id,&sem_op,1);

            }else{
                munmap(addr, size);
                kill(child_pid_t,SIGTERM);
                exit(1);
            }

        }

    }
}
