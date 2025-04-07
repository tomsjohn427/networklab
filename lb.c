#include <stdio.h>

int main() {
    int incoming, outgoing, buck_size, n, store = 8;

    printf("Enter bucket size, outgoing rate and no of inputs: ");
    scanf("%d %d %d", &buck_size, &outgoing, &n);

    while (1) {
        printf("Enter the incoming packet size: ");
        scanf("%d", &incoming);

        printf("Incoming packet size: %d\n", incoming);

        if (incoming <= (buck_size - store)) {
            store += incoming;  // Store the incoming packet
            printf("Bucket buffer size is %d out of %d\n", store, buck_size);
        } else {
            printf("Dropped %d packets. No space in buffer.\n", incoming);
            printf("Bucket buffer size is %d out of %d\n", store, buck_size);
        }

        // Simulate outgoing packets
        store -= outgoing;
        if (store < 0) {
            store = 0;  // Prevent buffer from going negative
        }

        printf("After outgoing, %d packets are left in the buffer out of %d\n", store, buck_size);

        // If you'd like to stop the loop after some number of inputs, you can add a condition here.
    }

    return 0;
}


