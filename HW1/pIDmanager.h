/*
  Definition for the process ID manager.

  allocate_map() is a function that initializes the status of each pid address.
    Returns -1 if unsucessful, returns 1 if successful.
  allocate_pid() is a function that allocates the first available pid to a process
    changing the value from a 0 to a 1. Returns the address/index if sucessful,
    returns -1 if there isn't an avaiable pid.
  release_pid(int) is a function that accepts an integer as the index and
    deallocates/releases a pid, changing the value from a 1 to a 0 making
    that address avaiable again. Returns nothing.
  print_arr(bool []) is a function that prints the contents of the underlying
    array holding the pid statuses. For testing purposes.
*/

#ifndef _pIDmanager_h
#define _pIDmanager_h

#include <iostream>

const int MIN_PID = 300;   // Smallest pID Available.
const int MAX_PID = 350;   // Largest pID Available.
const int TOTAL_PID = (MAX_PID-MIN_PID)+1;  // Total Amount Of pIDs Available.

bool pIDs [TOTAL_PID]; // Our data structure to represent pids and statuses.
                       // 0 -> available, 1 -> unavailable.

int allocate_map(){
    int i;

    for(i = 0; i < TOTAL_PID; i++)
        pIDs[i] = 0;

    if(i == TOTAL_PID-1) // If Reached The End That Means Allocation Was Successful.
        return 1;

    return -1;          // Something Went Wrong During Allocating.

}

int allocate_pid(){
    for(int i = 0; i < TOTAL_PID; i++){
        if(pIDs[i] == 0){       // Check If pID Is Available.
            pIDs[i] = 1;
            return i+MIN_PID;   // Return The pID Location If Successful.
        }
    }

    return -1;      // No pID Location Was Available.
}

void release_pid(int pid){
    if(pid < MIN_PID || pid > MAX_PID){    // bounds check
        std::cout << "Error Releasing pID, Out Of Range!\n" << std::endl;
        return;
    }

    pIDs[pid-MIN_PID] = 0;    // Update pID Status, Making It Available.
}

// For Testing.
void printArr(bool arr[])
{
    std::cout << "\nDisplaying Pids And Their Status:\n";

    for(int i = 0; i < TOTAL_PID; i++){
        std::cout << "pid: " << i+MIN_PID << " Status: " << arr[i] << "\t\t";
        if((i+1) % 4 == 0)
            std::cout << "\n";
    }

    std::cout << "\n\n";
}

#endif
