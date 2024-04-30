// Copyright 2010 Luc Lieber with fixes by Alan Ferrenberg
#include "BufferToggle.h"

void BufferToggle::off() {
    // get the current terminal I/O structure
    tcgetattr(STDIN_FILENO, &t);
    // Manipulate the flag bits to do what you want it to do
    t.c_lflag &= ~ICANON;
    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void BufferToggle::on(void) {
    // get the current terminal I/O structure
    tcgetattr(STDIN_FILENO, &t);
    // Manipulate the flag bits to do what you want it to do
    t.c_lflag |= ICANON;
    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
