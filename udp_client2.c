/*
 * TITLE: COMP 5360 project 2
 * FILENAME: udp_client2.c
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
    
    //Print assignment and personal info
    printf("\n//////////////////////////////////////////////\n");
    printf("      TITLE: COMP 5360 project 2\n");
    printf("      FILENAME: udp_client2.c\n");
    printf("      AUTHORS: Elijah Stephenson ems0075\n");
    printf("               Dylan Barnes dkb0023\n");
    printf("  (Undergraduate students, graduate bonus)\n");
    printf("//////////////////////////////////////////////\n\n");

    //------------------------- Sending Socket Init -------------------------------
    int socket_send_desc;
    struct sockaddr_in server_addr;
    char client_message[100], server_message[100];
    int server_struct_length = sizeof(server_addr);

    // Create socket:
    socket_send_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socket_send_desc < 0){
        printf("Error while creating socket\n\n");
        return -1;
    }
    printf(" Sending socket created successfully\n\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    //original was 2000
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.20");
    
    //Random set
    srand(time(NULL));

    // -------------------------- Receiving Socket Init ------------------------------
    int socket_rec_desc;
    struct sockaddr_in client_addr;
    char client_message[100], server_message[100];
    char errorMessage[] = "Error: did not receive data";
    char okMessage[] = "ACK";
    int client_struct_length = sizeof(client_addr);

    // Create UDP socket:
    socket_rec_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_rec_desc < 0){
        printf("Error creating socket\n");
        return -1;
    }
    printf("Receiving socket created successfully\n");
    
    // Set port and IP:
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(2000);
    client_addr.sin_addr.s_addr = inet_addr("192.168.1.20");


    // Bind to the set port and IP:
    if(bind(socket_rec_desc, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Bound to port\n");
    
    printf("Listening for incoming messages...\n\n");

    // Get input from the user:
    //printf("Enter message: ");
    //fgets(client_message, 100, stdin);

    //-----------------------------------------Main Loop----------------------------------------------------
    //Loop the main logic 100 times to recieve 100 packets
    for(int i = 0; i < 100; i++){

        // Clean buffers:
        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));

        // -------------------------- Receiving Socket Message ------------------------------

        // Receive clients's message:
        if (recvfrom(socket_rec_desc, client_message, sizeof(client_message), 0,
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

        /*Generate Message ----------------------------------------------------
        //Calculate Sequence Number
        char sequenceNumber[4];
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
        strcat(client_message, sequenceNumber);
        strcat(client_message, ",");
        strcat(client_message, gpsPositionPartOne);
        strcat(client_message, ",");
        strcat(client_message, gpsPositionPartTwo);
        strcat(client_message, ",");
        strcat(client_message, velocityDir);
        strcat(client_message, ",");
        strcat(client_message, velocityMag);
        strcat(client_message, ",");
        strcat(client_message, acceleration);
        strcat(client_message, ",");
        strcat(client_message, brakeControl);
        strcat(client_message, ",");
        strcat(client_message, gasThrottle);
        //puts(client_message);
        *///Message generated ---------------------------------------------------

        // Print message to console
        printf("========== FORWARDING DATA ==========\n");
        //printf("Sequence Number: %s\n", sequenceNumber);
        //printf("Source Address: %s\n", inet_ntoa(server_addr.sin_addr));
        //printf("GPS Position: %s, %s\n", gpsPositionPartOne, gpsPositionPartTwo);
        //printf("Velocity Direction: %s\n", velocityDir);
        //printf("Velocity Magnitude: %s\n", velocityMag);
        //printf("Acceleration: %s\n", acceleration);
        //printf("Brake Control: %s%%\n", brakeControl);
        //printf("Gas Throttle: %s%%\n", gasThrottle);
        //printf("==================================\n");

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

        // Send the message to server:
        if(sendto(socket_send_desc, client_message, strlen(client_message), 0,
            (struct sockaddr*)&server_addr, server_struct_length) < 0){
            printf("Unable to send message\n");
            return -1;
        }
        
        // Receive the server's response:
        if(recvfrom(socket_rec_desc, server_message, sizeof(server_message), 0,
            (struct sockaddr*)&server_addr, &server_struct_length) < 0){
            printf("Error while receiving client's msg\n");
            return -1;
        }
        
        printf("Packet %d Response from Address %s: %s\n\n", i+1, inet_ntoa(server_addr.sin_addr), server_message);
        sleep(.10);
    }
    // Close the socket:
    close(socket_desc);
    
    return 0;
}
