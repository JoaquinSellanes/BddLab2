#!/bin/bash

ecpg prog1.pgc

cc -I/usr/include/postgresql -c prog1.c

cc -o prog1 prog1.o -L/usr/lib/postgresql/9.5/lib -lecpg
