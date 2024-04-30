// Copyright 2010 Luc Lieber with fixes by Alan Ferrenberg
#ifndef BUFFERTOGGLE_H_
#define BUFFERTOGGLE_H_

#define _POSIX_SOURCE
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

class BufferToggle {
 private:
    struct termios t;

 public:
    /*
    * Disables buffered input
    */

    void off();  // Disables buffered input
    void on(void);  // Enables buffered input
};
#endif  // BUFFERTOGGLE_H_
