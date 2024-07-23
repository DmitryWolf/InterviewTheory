#include <iostream>
using namespace std;

class Device {
    public:
        Device() {
            cout << "Device constructor called" << endl;
        }
        void turn_on() {
            cout << "Device is on." << endl;
        }
};

class Computer: virtual public Device {
    public:
        Computer() {
            cout << "Computer constructor called" << endl;
        }
};

class Monitor: virtual public Device {
    public:
        Monitor() {
            cout << "Monitor constructor called" << endl;
        }
};

class Laptop: public Computer, public Monitor {};

int main() {
    Laptop Laptop_instance;
    Laptop_instance.turn_on();
    return 0;
}