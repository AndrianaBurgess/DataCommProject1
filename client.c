// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h>
#define PORT 9080 
   
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


    return 0;

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
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 
    return 0; 
} 