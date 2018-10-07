// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>

#define PORT 9080 
#define FILE_INDEX 3
#define TO_FORMAT_INDEX 4
#define NEW_NAME_INDEX 5
#define NAME_SIZE_BYTES 4
#define CONTENT_SIZE_BYTES 8
   
int main(int argc, char const *argv[]) 
{ 
    if(atoi(argv[4]) > 3 || atoi(argv[4]) < 0){
        puts("To Format number is invalid. Must be between 0 and 3");
        exit(1);
    }
    if( access(argv[3], F_OK ) == -1 ) {
        puts("File does not exist");
        exit(1);
    }
    FILE *fp = fopen( argv[3],"r" );
    fseek(fp, 0, SEEK_SET);
    char data[1];
    int tot = fread(data, sizeof(data[0]), 1, fp);
    if (0 == tot) {
        fclose(fp);
        puts("File is empty");
        exit(1);
    }
    fclose(fp);

    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(atoi(argv[2])); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    int s = size_of_message(argv[FILE_INDEX], argv[NEW_NAME_INDEX] );
    printf("%d",s);
    char *message = malloc(s);
    char *curr = message;
    //Add size of new file name to message 
    uint32_t name_size = strlen(argv[NEW_NAME_INDEX]);
    name_size = htonl(name_size);
    memcpy(curr,&name_size,NAME_SIZE_BYTES);
    name_size = ntohl(name_size);
    curr += NAME_SIZE_BYTES;
    //Add new file name to message
    memcpy(curr,argv[NEW_NAME_INDEX],name_size);
    curr += name_size;
    //Add size of file content to message
    struct stat st;
    stat(argv[FILE_INDEX], &st);
    uint64_t file_size = st.st_size;
    file_size = htonl(file_size);
    memcpy(curr,&file_size,CONTENT_SIZE_BYTES);
    file_size = ntohl(file_size);
    curr += CONTENT_SIZE_BYTES;
    //Add content of file to message 
    FILE *file = fopen(argv[FILE_INDEX],"r");
    curr += fread(curr,1,file_size,file);
    fclose(file);
    //Add to format num to message 
    uint8_t to_format = argv[TO_FORMAT_INDEX];
    memcpy(curr,&to_format,1);

    send(sock , message , s , 0 ); 
    printf("Hello message sent\n"); 
    char rec[40];
    valread = read( sock , rec, 1024); 
    printf("%s\n",rec ); 
    return 0; 
} 

int size_of_message(char* file, char* new__file_name){
    struct stat st;
    stat(file, &st);
    int file_size = st.st_size;
    int name_size = strlen(new__file_name);
    return(4+8+1+file_size+name_size);
}

