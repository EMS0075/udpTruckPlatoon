/*
 * TITLE: COMP 5360 project 1
 * FILENAME: udp_client.c
 * AUTHORS: Elijah Stephenson ems0075
 * 			Dylan Barnes dkb0023
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void){
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[100], client_message[100];
    char errorMessage[] = "Error: did not receive data";
    char okMessage[] = "OK";
    int server_struct_length = sizeof(server_addr);
    
    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.140");


    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Bound to port\n");
    
    printf("Listening for incoming messages...\n\n");
    
    //Loop the main logic 100 times to recieve 100 packets
    for(int i = 0; i < 100; i++){

        // Clean buffers:
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));

        // Receive servers's message:
        if (recvfrom(socket_desc, server_message, sizeof(server_message), 0,
            (struct sockaddr*)&server_addr, &server_struct_length) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
    
        //parse server_message
        printf("========== DATA RECEIVED ==========\n");
        printf("Sequence Number: "); 
        char *pt;
        pt = strtok(server_message,",");
        printf("%s\n", pt);
        pt = strtok(NULL, ",");
		printf("Source Address: %s\n", inet_ntoa(server_addr.sin_addr));
		printf("GPS Position: %s, ", pt);
		pt = strtok(NULL, ",");
		printf("%s\n", pt);
		pt = strtok(NULL, ",");
		printf("Velocity Direction: %s\n", pt);
		pt = strtok(NULL, ",");
		printf("Velocity Magnitude: %s\n", pt);
		pt = strtok(NULL, ",");
		printf("Acceleration: %s\n", pt);
		pt = strtok(NULL, ",");
		printf("Brake Control: %s%%\n", pt);
		pt = strtok(NULL, ",");
		printf("Gas Throttle: %s%%\n", pt);
		printf("==================================\n\n");
        
        // Respond to server:
		printf("Sending ACK to %s for Packet %s\n\n", inet_ntoa(server_addr.sin_addr), server_message);
        strcpy(client_message, okMessage);
        
        if (sendto(socket_desc, client_message, strlen(client_message), 0,
            (struct sockaddr*)&server_addr, server_struct_length) < 0){
            printf("Can't send\n");
            return -1;
        }
    }
    // Close the socket:
    close(socket_desc);
}
