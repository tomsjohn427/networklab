#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main() {
    int nf, N;
    int tr = 0;
    srand(time(NULL));

    // Input number of frames and window size
    printf("Enter the number of frames: ");
    scanf("%d", &nf); // Fixed scanf format specifier
    printf("Enter the Window Size: ");
    scanf("%d", &N); // Fixed scanf format specifier

    int i = 1;

    // Loop over all frames
    while(i <= nf) {
        int x = 0;
        
        // Loop to send frames within the window size
        for(int j = i; j < i + N && j <= nf; j++) {
            printf("Sent Frame %d\n", j); // Fixed printf format string
            tr++;
        }

        // Loop to check for acknowledgment of sent frames
        for(int j = i; j < i + N && j <= nf; j++) {
            int flag = rand() % 2;  // Randomly simulate acknowledgment (0 = no, 1 = yes)
            
            if(flag == 1) {
                printf("%d: Acknowledged!\n", j);
                x++;
            } else {
                printf("Frame %d Not Received\n", j);
                printf("Retransmitting Window\n");
                break; // Exit the loop and retransmit the window
            }
        }

        printf("\n");
        i += x; // Move the window forward by the number of acknowledged frames
    }

    // Output total number of transmissions
    printf("Total number of transmissions: %d\n", tr);

    return 0;
}


