/*
 * TITLE: COMP 5360 project 1
 * FILENAME: udp_server.c
 * AUTHORS: Elijah Stephenson ems0075
 * 	    Dylan Barnes dkb0023
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(void){
    int socket_desc;
    struct sockaddr_in client_addr;
    char server_message[100], client_message[100];
    int client_struct_length = sizeof(client_addr);
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n\n");
        return -1;
    }
    printf("Socket created successfully\n\n");
    
    // Set port and IP:
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(2000);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //Random set
    srand(time(NULL));

    // Get input from the user:
    //printf("Enter message: ");
    //fgets(server_message, 100, stdin);
    
    //Loop the main logic 100 times to recieve 100 packets
    for(int i = 0; i < 100; i++){

        // Clean buffers:
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));

        //Generate Message ----------------------------------------------------

        //Calculate Sequence Number
        char sequenceNumber[2];
        snprintf(sequenceNumber, sizeof(sequenceNumber), "%d", i+1);

        //Calculate GPS Position
        char gpsPositionPartOne[8];
        char gpsPositionPartTwo[8];
        float gpsPositionPartOneRaw = ((float)rand()/(float)(RAND_MAX))*10*10;
        float gpsPositionPartTwoRaw = ((float)rand()/(float)(RAND_MAX))*10*10;
        snprintf(gpsPositionPartOne, sizeof(gpsPositionPartOne), "%0.5f", gpsPositionPartOneRaw);
        snprintf(gpsPositionPartTwo, sizeof(gpsPositionPartTwo), "%0.5f", gpsPositionPartTwoRaw);

        //Calculate Velocity Direction
        int velocityDirRandIndex = (rand() %(7 - 0 + 1)) + 0;
        char velocityDir[3];
        if (velocityDirRandIndex == 0){
            strcpy(velocityDir, "N");
        }
        else if (velocityDirRandIndex == 1){
        strcpy(velocityDir, "NE");
        }
        else if (velocityDirRandIndex == 2){
            strcpy(velocityDir, "E");
        }
        else if (velocityDirRandIndex == 3){
            strcpy(velocityDir, "SE");
        }
        else if (velocityDirRandIndex == 4){
            strcpy(velocityDir, "S");
        }
        else if (velocityDirRandIndex == 5){
            strcpy(velocityDir, "SW");
        }
        else if (velocityDirRandIndex == 6){
            strcpy(velocityDir, "W");
        }
        else{
            strcpy(velocityDir, "NW");
        }

        //Calculate Velocity Magnitude
        char velocityMag[6];
        float velocityMagRaw = ((float)rand()/(float)(RAND_MAX))*10*10;
        snprintf(velocityMag, sizeof(velocityMag), "%0.1f", velocityMagRaw);

        //Calculate Acceleration Magnitude
        char acceleration[5];
        float accelerationRaw = ((float)rand()/(float)(RAND_MAX))*10;
        int isNegative = (rand() %(9 - 0 + 1)) + 0;
        if (isNegative == 1){
            accelerationRaw = accelerationRaw*(-1);
        }
        snprintf(acceleration, sizeof(acceleration), "%0.1f", accelerationRaw);

        //Calculate brake value
        int brakeControlRaw;
        char brakeControl[3];
        if (accelerationRaw < 0){
            brakeControlRaw =  (rand() %(100 - 10 + 1)) + 10;
        }
        else{
            brakeControlRaw =  0;
        }
        snprintf(brakeControl, sizeof(brakeControl), "%d", brakeControlRaw);

        //Calculate throttle value
        int gasThrottleRaw;
        char gasThrottle[3];
        if (accelerationRaw > 0){
            gasThrottleRaw =  (rand() %(100 - 10 + 1)) + 10;
        }
        else{
            gasThrottleRaw =  0;
        }
        snprintf(gasThrottle, sizeof(gasThrottle), "%d", gasThrottleRaw);

        //Combine all calculated values into message
        strcat(server_message, sequenceNumber);
        strcat(server_message, ",");
        strcat(server_message, gpsPositionPartOne);
        strcat(server_message, ",");
        strcat(server_message, gpsPositionPartTwo);
        strcat(server_message, ",");
        strcat(server_message, velocityDir);
        strcat(server_message, ",");
        strcat(server_message, velocityMag);
        strcat(server_message, ",");
        strcat(server_message, acceleration);
        strcat(server_message, ",");
        strcat(server_message, brakeControl);
        strcat(server_message, ",");
        strcat(server_message, gasThrottle);
        //puts(server_message);

        //Message generated ---------------------------------------------------

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
        
        printf("Packet %d Response: %s\n", i+1, client_message);
        sleep(1);
    }
    // Close the socket:
    close(socket_desc);
    
    return 0;
}