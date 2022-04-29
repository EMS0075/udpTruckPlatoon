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
    int socket_desc_1;
    int socket_desc_2;
    struct sockaddr_in client_addr_1;
    struct sockaddr_in client_addr_2;
    char client_message_1[100], server_message[100], client_message_2[100], client_message_3[100];
    char errorMessage[] = "Error: did not receive data";
    char okMessage[] = "ACK";
    int client_struct_length = sizeof(client_addr_1);
    
    //Print assignment and personal info
    printf("\n//////////////////////////////////////////////\n");
    printf("      TITLE: COMP 5360 project 2\n");
    printf("      FILENAME: udp_server_1.c\n");
    printf("      AUTHORS: Elijah Stephenson ems0075\n");
    printf("               Dylan Barnes dkb0023\n");
    printf("  (Undergraduate students, graduate bonus)\n");
    printf("//////////////////////////////////////////////\n\n");

    // Create UDP sockets:
    socket_desc_1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    socket_desc_2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    //Scoket 1
    if(socket_desc_1 < 0){
        printf("Error creating socket\n");
        return -1;
    }
    printf("Socket 1 created successfully\n");

    //Socket 2
    if(socket_desc_2 < 0){
    	printf("Error creating socket\n");
		return -1;
    }
    printf("Socket 2 created successfully\n");

    // Set port and IP of Socket 1:
    client_addr_1.sin_family = AF_INET;
    client_addr_1.sin_port = htons(2000);
    client_addr_1.sin_addr.s_addr = inet_addr("192.168.1.161");
    
    // Set port and IP of Socket 2:
    client_addr_2.sin_family = AF_INET;
	client_addr_2.sin_port = htons(2001);
	client_addr_2.sin_addr.s_addr = inet_addr("192.168.1.161");

    // Bind to the set port and IP to Socket 1:
    if(bind(socket_desc_1, (struct sockaddr*)&client_addr_1, sizeof(client_addr_1)) < 0){
        printf("Couldn't bind socket 1 to the port\n");
        return -1;
    }
    printf("Bound socket 1 to port\n");

    // Bind to the set port and IP to Socket 2:
	/*if(bind(socket_desc_2, (struct sockaddr*)&client_addr_2, sizeof(client_addr_2)) < 0){
		printf("Couldn't bind socket 2 to the port\n");
		return -1;
	}*/
	/*printf("Bound socket 2 to port\n");*/
    
    printf("Listening for incoming messages...\n\n");
    
    //Loop the main logic 100 times to recieve 100 packets
    for(int i = 0; i < 100; i++){
			// Clean buffers:
			memset(client_message_1, '\0', sizeof(client_message_1));
			memset(server_message, '\0', sizeof(server_message));

			// Receive clients's message:
			if (recvfrom(socket_desc_1, client_message_1, sizeof(client_message_1), 0,
				(struct sockaddr*)&client_addr_1, &client_struct_length) < 0){
				printf("Couldn't receive\n");
				return -1;
			}
			strcpy(client_message_2, client_message_1);
			//parse client_message_1
			printf("========== DATA RECEIVED ==========\n");
			printf("Sequence Number: ");
			char *pt;
			pt = strtok(client_message_1,",");
			printf("%s\n", pt);
			pt = strtok(NULL, ",");
			printf("Source Address: %s\n", inet_ntoa(client_addr_1.sin_addr));
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
			printf("Sending ACK to %s for Packet %s\n\n", inet_ntoa(client_addr_1.sin_addr), client_message_1);
			strcpy(server_message, okMessage);
			//send ACK
			if (sendto(socket_desc_1, server_message, strlen(server_message), 0,
				(struct sockaddr*)&client_addr_1, client_struct_length) < 0){
				printf("Can't send\n");
				return -1;
			}

			client_addr_1.sin_port = htons(2001);

			printf("Relaying Data to port\n");
			if (sendto(socket_desc_1, client_message_2, strlen(client_message_2), 0,
				(struct sockaddr*)&client_addr_1, client_struct_length) < 0){
				printf("Can't send\n");
				return -1;
			}
			printf("Data Relayed\n");

			if (recvfrom(socket_desc_1, client_message_3, sizeof(client_message_3), 0,
				(struct sockaddr*)&client_addr_1, &client_struct_length) < 0){
				printf("Couldn't receive\n");
				return -1;
			}
			printf("%s", client_message_3);

			client_addr_1.sin_port = htons(2004);
			if (sendto(socket_desc_1, client_message_3, strlen(client_message_3), 0,
				(struct sockaddr*)&client_addr_1, client_struct_length) < 0){
				printf("Can't send\n");
				return -1;
			}
    }
    // Close sockets:
    close(socket_desc_1);
    close(socket_desc_2);
}
