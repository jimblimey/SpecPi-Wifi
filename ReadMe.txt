Proof of concept for reading keypresses on an original ZX Spectrum keyboard connected to a Raspberry Pi GPIO over a network connection.

Building
--------

The server is a simple C program that can be built with (requires the wiringPi package to be installed):

cd server
cc -lwiringPi kbserv.c -o kbserv

The client demo is a QMake/Qt5 based C++ program that can be built from the command line or from within Qt Creator

cd clientdemo
qmake
make

BE SURE TO CHANGE THE HOST VALUE IN mainwindow.h BEFORE BUILDING!

Running
-------

Run the server on the Pi before launching the client demo - this will automatically connect on startup.

Find me on twitter @jimblimey!