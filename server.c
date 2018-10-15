#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>

#define PORT 9080 
const int NO_TRANS = 0;
const int ZERO_TO_ONE = 1;
const int ONE_TO_ZERO = 2;
const int SWAP = 3;

void no_translation(unsigned char file_name[], uint64_t file_size, unsigned char file_content[]);
void zero_to_one(unsigned char file_name[], uint64_t file_size, unsigned char file_content[]);

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    while(11){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){ 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        int total_read = 0;
        uint32_t name_bytes;  
        recv(new_socket, &name_bytes, sizeof(name_bytes), MSG_WAITALL);
        name_bytes = ntohl(name_bytes);
        printf("name bytes : %d\n", name_bytes);
        fflush(stdout);
        total_read += sizeof(name_bytes);

        unsigned char *name = malloc(name_bytes+1); 
        name[name_bytes] = '\0';
        recv(new_socket, name, name_bytes, MSG_WAITALL);
        printf("name: %s\n", name);
        fflush(stdout);
        total_read += strlen(name);

        uint64_t content_bytes;  
        recv(new_socket, &content_bytes, sizeof(content_bytes), MSG_WAITALL);
        content_bytes = ntohl(content_bytes);
        printf("contnt bytes: %d\n", content_bytes);
        fflush(stdout);
        total_read +=sizeof(content_bytes);

        unsigned char *content = malloc(content_bytes+1); 
        content[content_bytes] = '\0';
        recv(new_socket, content, content_bytes, MSG_WAITALL);
        printf("content: %s\n", content);
        fflush(stdout);
        total_read += content_bytes;

        uint8_t to_format;
        recv(new_socket, &to_format, 1, MSG_WAITALL);
        printf("to format before: %d\n", to_format);
       // to_format = ntohs(to_format);
        printf("to format: %d\n", to_format);
        fflush(stdout);
        total_read += 1;

        printf("Total bytes read: %d\n", total_read);
        if (to_format == NO_TRANS){
            printf("%s\n", "WHOOORESSSS");
            no_translation(name,content_bytes,content);

        }else if(to_format == ZERO_TO_ONE){
            printf("%s\n", "BITCHESSSSSSSS");
            zero_to_one(name,content_bytes,content);

        }else if(to_format == ONE_TO_ZERO){
            printf("%d\n", to_format);
        }else if(to_format == SWAP){
            printf("%d\n", to_format);
        }
        



        if(close(new_socket)<0){
            perror("Socket Close Failure");
            exit(EXIT_FAILURE);
        }
    }

    valread = read( new_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    return 0; 
} 

void no_translation(unsigned char file_name[], uint64_t file_size, unsigned char file_content[]){
    FILE *fptr;
    fptr = fopen(file_name,"w");
    int i = 0;
    printf("file size: %d\n",file_size);

    while(i < file_size){
        unsigned char type = file_content[i++];
        if (type == 0){
            int amount = file_content[i++];
            fprintf(fptr,"%d ",amount);
            int j;
            for(j=0; j < amount-1 ; j++){
                unsigned char first = file_content[i++];
                unsigned char second = file_content[i++];
                unsigned short num = (((unsigned short)first) << 8) | ((unsigned short)second);
                fprintf(fptr,"%d ",num);
            }
            unsigned char first = file_content[i++];
            unsigned char second = file_content[i++];
            unsigned short num = ((unsigned short)first << 8) | (unsigned short)second;
            fprintf(fptr,"%d\n",num);
            
        }else{
            unsigned char amount_s[3];
            amount_s[0] = file_content[i++];
            amount_s[1] = file_content[i++];
            amount_s[2] = file_content[i++];
            int amount = atoi(amount_s);
            fprintf(fptr,"%d ",amount);
            int k;
            while(file_content[i]!= 1 && file_content[i]!= 0){
                fprintf(fptr,"%c",file_content[i++]);
            }
            fprintf(fptr,"%c",'\n');

        }
    }

    fclose(fptr);
}

void zero_to_one(unsigned char file_name[], uint64_t file_size, unsigned char file_content[]){
    FILE *fptr;
    fptr = fopen(file_name,"w");
    int i = 0;
    printf("file size: %d\n",file_size);

    while(i < file_size){
        unsigned char type = file_content[i++];
        if (type == 0){
            int amount = file_content[i++];
            fprintf(fptr,"%d ",amount);
            int j;
            for(j=0; j < amount-1 ; j++){
                unsigned char first = file_content[i++];
                unsigned char second = file_content[i++];
                unsigned short num = (((unsigned short)first) << 8) | ((unsigned short)second);
                fprintf(fptr,"%d,",num);
            }
            unsigned char first = file_content[i++];
            unsigned char second = file_content[i++];
            unsigned short num = ((unsigned short)first << 8) | (unsigned short)second;
            fprintf(fptr,"%d\n",num);
            
        }else{
            unsigned char amount_s[3];
            amount_s[0] = file_content[i++];
            amount_s[1] = file_content[i++];
            amount_s[2] = file_content[i++];
            int amount = atoi(amount_s);
            fprintf(fptr,"%d ",amount);
            int k;
            while(file_content[i]!= 1 && file_content[i]!= 0){
                fprintf(fptr,"%c",file_content[i++]);
            }
            fprintf(fptr,"%c",'\n');

        }
    }

    fclose(fptr);
}