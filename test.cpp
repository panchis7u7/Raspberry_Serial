#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>

int main(){
    wiringPiSetupGpio();
    int fd = 0;
    if((fd = serialOpen("/dev/ttyAMA0", 9600)) < 0) {
        std::cout << "Error opening serial port." << "\n";
        return -1;
    }

    for (int i = 0; i < 100; ++i) {
        delay(500);
        serialPuts(fd, "Hola de raspberry!");
        serialFlush(fd);
    }

    std::cout << "Hello world" << '\n';
    serialClose(fd);
    return 0;
}