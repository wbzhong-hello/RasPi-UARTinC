// To compile: gcc -Wall -pthread -o helloworld helloworld.c -lpigpio -lrt
// To run: sudo ./helloworld

#include <pigpio.h>
#include <stdio.h>
#include <string.h>

// Polls the serial port every ~50 ms
void *readSer(void *arg)
{
    int hSer = *(int *)arg;
    int len;
    char data[1024];

    while (1)
    {
        len = serRead(hSer, data, sizeof(data));
        if (len < 0)
        {
            printf("Read serial port failed.\n");
            return 0;
        }
        else if (len > 0)
            printf("Rx: %s\n", data);

        gpioSleep(PI_TIME_RELATIVE, 0, 50000); // 50 ms
    }
}

// Writes the serial port every ~3 s
void *writeSer(void *arg)
{
    int hSer = *(int *)arg;
    int len;
    char *data = "Hello, world!";

    while (1)
    {
        len = serWrite(hSer, data, strlen(data));
        if (len != 0)
        {
            printf("Write serial port failed.\n");
            return 0;
        }
        else
            printf("Tx: %s\n", data);

        gpioSleep(PI_TIME_RELATIVE, 3, 0); // 3 s
    }
}

int main()
{
    int retCode = -1;
    int hSer = -1;
    pthread_t *pReadSerThread = NULL;
    pthread_t *pWriteSerThread = NULL;

    if (gpioInitialise() < 0)
    {
        printf("Initialize pigpio library failed.\n");
        return -1;
    }

    hSer = serOpen("/dev/serial0", 115200, 0);
    if (hSer < 0)
    {
        printf("Open /dev/serial0 failed.\n");
        goto CleanUp;
    }

    pReadSerThread = gpioStartThread(readSer, &hSer);
    if (pReadSerThread == NULL)
    {
        printf("Start readSer thread falied.\n");
        goto CleanUp;
    }

    pWriteSerThread = gpioStartThread(writeSer, &hSer);
    if (pWriteSerThread == NULL)
    {
        printf("Start writeSer thread falied.\n");
        goto CleanUp;
    }

    // Now run the main task at here
    printf("Starts to loop test on /dev/serial0.\nPress Enter to exit.\n");
    getchar();

    // Everything seems fine
    retCode = 0;

CleanUp:
    if (pReadSerThread != NULL)
        gpioStopThread(pReadSerThread);
    if (pWriteSerThread != NULL)
        gpioStopThread(pWriteSerThread);
    if (hSer >= 0)
        serClose(hSer);
    gpioTerminate();

    return retCode;
}