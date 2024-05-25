#define _CRT_SECURE_NO_WARNINGS

#include "turrel-interface.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>

int initSerialPort(const char* serialport)
{
    struct termios tty;
    int fd = open(serialport, O_RDWR | O_NONBLOCK);

    if (fd < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    if (tcgetattr(fd, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS; //--std=cgnu99

    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 1;

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tcsetattr(fd, TCSANOW, &tty);
    if (tcsetattr(fd, TCSAFLUSH, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
}

int closeSerialPort(int fd) {
    return close(fd);
}

int writeSerialPort(int fd, const char* str) {
    int length = strlen(str);
    int n = write(fd, str, length);

    if (n != length) {
        printf("writeToSerialPort: string length comparison error.");
        return -1;
    }
    return 0;
}

int readuntilSerialPort
(int fd, char* buffer, char until, int bufferMax, int timeout)
{
    char b[1];
    int i = 0;
    do { 
        int n = read(fd, b, 1);

        if (n == -1) {
            return -1;
        }

        if (n == 0) {
            usleep(1 * 1000);
            timeout--;
            if (timeout == 0) {
                return -2;
            }
            continue;
        }

        buffer[i] = b[0]; 
        i++;
    } while (b[0] != until && i < bufferMax && timeout > 0);

    buffer[i] = 0;
    return 0;
}

int flushSerialPort(int fd) {
    sleep(2);
    return tcflush(fd, TCIOFLUSH);
}