#!/bin/bash
if [ "$#" -ne "1" ]; then
    exit
fi 
username=$(cut -d '.' -f1 $1.passwd)
sshpass -p `cat $1.passwd` ssh $1@maze.labs.overthewire.org -p 2225