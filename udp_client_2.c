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
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int main(void){
    int socket_desc;
    struct sockaddr_in server_addr_1;
    struct sockaddr_in server_addr_2;
    char client_message[100], server_message[100], server_message_2[100];
    int server_struct_length = sizeof(server_addr_1);
    
    //Print assignment and personal info
    printf("\n//////////////////////////////////////////////\n");
    printf("      TITLE: COMP 5360 project 2\n");
    printf("      FILENAME: udp_client_1.c\n");
    printf("      AUTHORS: Elijah Stephenson ems0075\n");
    printf("               Dylan Barnes dkb0023\n");
    printf("  (Undergraduate students, graduate bonus)\n");
    printf("//////////////////////////////////////////////\n\n");

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_desc < 0){
        printf("Error while creating socket\n\n");
        return -1;
    }
    printf("Socket created successfully\n\n");
    
    //input address
    // Set port and IP:
    server_addr_1.sin_family = AF_INET;
    //original was 2000
    server_addr_1.sin_port = htons(2001);
    server_addr_1.sin_addr.s_addr = inet_addr("192.168.1.161");
    
    //output address
    // Set port and IP:
	server_addr_2.sin_family = AF_INET;
	//original was 2000
	server_addr_2.sin_port = htons(2002);
	server_addr_2.sin_addr.s_addr = inet_addr("192.168.1.161");


	// Bind to the set port and IP to Socket 2:
	if(bind(socket_desc, (struct sockaddr*)&server_addr_1, sizeof(server_addr_1)) < 0){
		printf("Couldn't bind socket 1 to the port\n");
		return -1;
	}
	//Random set
    srand(time(NULL));

    // Get input from the user:
    //printf("Enter message: ");
    //fgets(client_message, 100, stdin);

    //Loop the main logic 100 times to recieve 100 packets
    for(int i = 0; i < 100; i++){

        // Clean buffers:
        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));

        if (recvfrom(socket_desc, server_message, sizeof(server_message), 0,
			(struct sockaddr*)&server_addr_1, &server_struct_length) < 0){
			printf("Couldn't receive\n");
			return -1;
		}
        strcpy(client_message, server_message);

        // Print message to console
        printf("========== SENDING DATA ==========\n");
        printf("Sequence Number: ");
		char *pt;
		pt = strtok(server_message,",");
		printf("%s\n", pt);
		pt = strtok(NULL, ",");
		printf("Source Address: %s\n", inet_ntoa(server_addr_1.sin_addr));
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
        printf("==================================\n");

        //Time Related Stuff.......
        // variables to store the date and time components
        int hours, minutes, seconds;
 
        // `time_t` is an arithmetic time type
        time_t now;
 
        // Obtain current time
        time(&now); // `time()` returns the current time of the system as a `time_t` value
        struct timeval time_now;
        gettimeofday(&time_now, NULL); //get milliseconds differently because....just because idk
        ctime(&now); // Convert to local time format
        struct tm *local = localtime(&now);
    
        hours = local->tm_hour;  // get hours since midnight (0-23)
        minutes = local->tm_min; // get minutes passed after the hour (0-59)
        seconds = local->tm_sec; // get seconds passed after a minute (0-59)
    
        // print local time
        if (hours < 12) {    // before midday
            printf("Sent at %02d:%02d:%02d:%06d am\n", hours, minutes, seconds, time_now.tv_usec);
        }
        else {    // after midday
            printf("Sent at %02d:%02d:%02d:%06d pm\n", hours - 12, minutes, seconds, time_now.tv_usec);
        }

        // Send the message to output server:
        if(sendto(socket_desc, client_message, strlen(client_message), 0,
            (struct sockaddr*)&server_addr_2, server_struct_length) < 0){
            printf("Unable to send message\n");
            return -1;
        }
        
        // Receive the output server's response:
        if(recvfrom(socket_desc, server_message_2, sizeof(server_message_2), 0,
            (struct sockaddr*)&server_addr_2, &server_struct_length) < 0){
            printf("Error while receiving client's msg\n");
            return -1;
        }
        
        //relay output servers response
        if(sendto(socket_desc, server_message_2, strlen(server_message_2), 0,
			(struct sockaddr*)&server_addr_1, server_struct_length) < 0){
			printf("Unable to send message\n");
			return -1;
		}

        printf("Packet %d Response from Address %s: %s\n\n", i+1, inet_ntoa(server_addr_1.sin_addr), server_message_2);
        sleep(.10);
    }
    // Close the socket:
    close(socket_desc);
    
    return 0;
}
