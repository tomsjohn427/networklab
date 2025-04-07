#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // for sleep function

#define TOTAL_FRAMES 5

// Function to simulate sending a frame
int send_frame(int frame_number) {
    printf("Sender: Sending frame %d\n", frame_number);
    // Simulate a 20% chance of frame being lost
    if (rand() % 5 == 0) {
        printf("Sender: Frame %d lost!\n", frame_number);
        return 0; // Frame lost
    }
    return 1; // Frame sent successfully
}

// Function to simulate receiving a frame
int receive_frame(int frame_number) {
    printf("Receiver: Receiving frame %d\n", frame_number);
    // Simulate a 10% chance of ACK being lost
    if (rand() % 10 == 0) {
        printf("Receiver: ACK for frame %d lost!\n", frame_number);
        return 0; // ACK lost
    }
    printf("Receiver: ACK for frame %d received!\n", frame_number);
    return 1; // ACK received successfully
}

int main() {
    srand(time(NULL)); // Seed for random number generation
    int frame_number;

    for (frame_number = 0; frame_number < TOTAL_FRAMES; frame_number++) {
        int sent = 0;
        while (!sent) {
            // Send the frame
            if (send_frame(frame_number)) {
                sent = 1; // Frame sent successfully
            }

            // Wait for ACK
            sleep(1); // Simulate waiting time
            if (!receive_frame(frame_number)) {
                printf("Sender: Retransmitting frame %d\n", frame_number);
                sent = 0; // Retransmit if ACK not received
            }
        }
    }

    printf("All frames sent successfully!\n");
    return 0;
}

