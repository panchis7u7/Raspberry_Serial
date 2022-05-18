#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <termios.h>

int main(){
    wiringPiSetupGpio();
    int fd = 0, frameLen = 0;
    char* dataFrame = (char*)malloc(15 * sizeof(char));
    dataFrame[0] = 0x8A;    // Start of frame.
    dataFrame[1] = 0x01;    // Command ID.
    dataFrame[2] = 0x0B;    // Data length.
    dataFrame[3] = 'H';
    dataFrame[4] = 'o';
    dataFrame[5] = 'l';
    dataFrame[6] = 'a';
    dataFrame[7] = ' ';
    dataFrame[8] = 'M';
    dataFrame[9] = 'u';
    dataFrame[10] = 'n';
    dataFrame[11] = 'd';
    dataFrame[12] = 'o';
    dataFrame[13] = '\0';
    
    for(frameLen = 0; frameLen < 14; ++frameLen)
        dataFrame[14] += dataFrame[frameLen];

    if((fd = serialOpen("/dev/ttyAMA0", 9600)) < 0) {
        std::cout << "Error opening serial port." << "\n";
        return -1;
    }

    struct termios options;

    if(tcgetattr(fd, &options) != 0){
        std::cout << "Error at getting options." << '\n';
         return -1;
    }

    options.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    options.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    options.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    options.c_cflag |= CS8; // 8 bits per byte (most common)
    options.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    options.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    options.c_lflag &= ~ICANON;
    options.c_lflag &= ~ECHO; // Disable echo
    options.c_lflag &= ~ECHOE; // Disable erasure
    options.c_lflag &= ~ECHONL; // Disable new-line echo
    options.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    options.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    options.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // options.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // options.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    options.c_cc[VTIME] = 1;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    options.c_cc[VMIN] = 0;

    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        std::cout << "Error at setting options." << '\n';
        return -1;
    }

    for (int i = 0; i < 100; ++i) {
        delay(1000);
        std::cout << "Sent packet no " << i << '\n';
        for(int j = 0; j < 15; ++j)
            serialPutchar(fd, dataFrame[j]);
    }

    std::cout << "Hello world" << '\n';
    serialClose(fd);
    return 0;
}