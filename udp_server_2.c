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
    struct sockaddr_in client_addr;
    char client_message[100], server_message[100];
    char errorMessage[] = "Error: did not receive data";
    char okMessage[] = "ACK";
    int client_struct_length = sizeof(client_addr);
    
    //Print assignment and personal info
    printf("\n//////////////////////////////////////////////\n");
    printf("      TITLE: COMP 5360 project 2\n");
    printf("      FILENAME: udp_server_1.c\n");
    printf("      AUTHORS: Elijah Stephenson ems0075\n");
    printf("               Dylan Barnes dkb0023\n");
    printf("	  USAGE: Truck Z\n");
    printf("		(dynamic data bonus)\n");
    printf("//////////////////////////////////////////////\n\n");

    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(2002);
    client_addr.sin_addr.s_addr = inet_addr("192.168.1.161");


    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Bound to port\n");
    
    printf("Listening for incoming messages...\n\n");
    
    //Loop the main logic 100 times to recieve 100 packets
    for(int i = 0; i < 2; i++){

        // Clean buffers:
        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));

        // Receive clients's message:
        if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
            (struct sockaddr*)&client_addr, &client_struct_length) < 0){
            printf("Couldn't receive\n");
            return -1;
        }
    
        //parse client_message
        printf("========== DATA RECEIVED ==========\n");
        printf("Sequence Number: "); 
        char *pt;
        pt = strtok(client_message,",");
        printf("%s\n", pt);
        pt = strtok(NULL, ",");
		printf("Source Address: %s\n", inet_ntoa(client_addr.sin_addr));
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
        
        // Respond to client:
		printf("Sending ACK to %s for Packet %s\n\n", inet_ntoa(client_addr.sin_addr), client_message);
        strcpy(server_message, okMessage);
        
        if (sendto(socket_desc, server_message, strlen(server_message), 0,
            (struct sockaddr*)&client_addr, client_struct_length) < 0){
            printf("Can't send\n");
            return -1;
        }
        

    }
    // Close the socket:
    close(socket_desc);
}
