#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char** argv) {
    if (argc-1 > 4 || argc-1 <1 ){
        perror("add correct amount of files pls (1-4)");fflush(stdout);
        exit(EXIT_FAILURE);
    }

    int pipes[4][2];

    for(int i = 0; i <argc-1;i++){

        pipe(pipes[i]);
        int arg_size = 0;
        for(int j = 0; argv[i+1][j] != '\0';j++){
            arg_size++;
        }
        int fork_size = fork();
            if(fork_size == 0){
                wait(NULL);
                close(pipes[i][1]);
                char buff[arg_size];
                buff[arg_size] = '\0';
                read(pipes[i][0],buff,arg_size);
                char * command[] = {"/bin/ls",buff,"-l",NULL};
                execv(command[0],command);

            }else if(fork_size == -1){
                exit(EXIT_FAILURE);

            }else{
                close(pipes[i][0]);
                write(pipes[i][1],argv[i+1],arg_size);
                wait(NULL);
            
            }
                // I don't understand why we need dup2()
                // I can skip piping the files just by running through argv
    }
    
    dup2(1,1); // look I used it!!!

    return 0;
}