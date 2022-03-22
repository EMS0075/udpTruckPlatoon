#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void){
    int socket_desc;
    struct sockaddr_in client_addr;
    char server_message[100], client_message[100];
    int client_struct_length = sizeof(client_addr);
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(2000);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Get input from the user:
    printf("Enter message: ");
    fgets(server_message, 100, stdin);
    
    // Send the message to client:
    if(sendto(socket_desc, server_message, strlen(server_message), 0,
         (struct sockaddr*)&client_addr, client_struct_length) < 0){
        printf("Unable to send message\n");
        return -1;
    }
    
    // Receive the client's response:
    if(recvfrom(socket_desc, client_message, sizeof(client_message), 0,
         (struct sockaddr*)&client_addr, &client_struct_length) < 0){
        printf("Error while receiving server's msg\n");
        return -1;
    }
    
    printf("Clients's response: %s\n", client_message);
    
    // Close the socket:
    close(socket_desc);
    
    return 0;
}