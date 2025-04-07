#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep function
#include <string.h>

#define WINDOW_SIZE 4
#define FRAME_COUNT 8

typedef struct {
    int seq_num;
    int ack_received;
    char data[20];
} Frame;

void sendFrames(Frame frames[], int count) {
    int sent_frames = 0;
    int base = 0;

    while (sent_frames < count) {
        // Sending frames in the current window
        for (int i = base; i < base + WINDOW_SIZE && i < count; i++) {
            if (!frames[i].ack_received) {
                printf("Sending frame %d: %s\n", frames[i].seq_num, frames[i].data);
                frames[i].ack_received = 1;  // Simulate ACK received for this frame
                sent_frames++;
            }
        }
        sleep(2); // Simulate some time delay before receiving ACK

        // Checking ACK for frames in the current window
        for (int i = base; i < base + WINDOW_SIZE && i < count; i++) {
            if (frames[i].ack_received) {
                printf("Acknowledgement received for frame %d\n", frames[i].seq_num);
                base++; // Move window forward
            }
        }
    }
}

int main() {
    Frame frames[FRAME_COUNT];

    // Initialize frames with sequence numbers and data
    for (int i = 0; i < FRAME_COUNT; i++) {
        frames[i].seq_num = i;
        frames[i].ack_received = 0;
        sprintf(frames[i].data, "Frame-%d", i);
    }

    // Start sending frames
    sendFrames(frames, FRAME_COUNT);

    return 0;
}

	
