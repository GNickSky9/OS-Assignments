/*
    Test Program To Test Our pID Manager Which Is Defined in pIDmanager.h
    This Program Can Easily Be Modified To Accept User Input.
    This Program Has Been Tested With All Edge Cases That I Could Think Of and
        Has Been Successful Everytime. The Final Version Of This Test Doesn't
        Include All The Edge Cases.
    This Program Starts Off By Calling allocate_map() To Initialize The pIDs.
    Then The Program Calls allocate_pid() In A Loop To Allocate All the pIDs.
    Then The program Calls release_pid(int) A Few Times To Release Arbitrary pIDs.
    And The Prgraom Prints To Screen The Status Of The pIDs At Every Step.
*/

#include "pIDmanager.h"

int main(int argc, char* argv[])
{
    std::cout << "\nLaunching ProcessID Manager...\n";

    std::cout << "Allocating Map...\n";

    if(allocate_map())									// Initializing pID Statuses.               
        std::cout << "Map Allocation Complete!\n";
    else
        std::cout << "Map Allocation Unsucessful!\n";

    printArr(pIDs);

    int temp = 0;

    for(int i = 0; i < TOTAL_PID; i++){
        temp = allocate_pid();							// Allocating Every pID.
        if(temp == -1)
            std::cout << "Failure Allocating Process ID!\n";
        else
            std::cout << "Allocating Process ID: " << temp << "\n";
    }

    printArr(pIDs);
														// Releasing Random pIDs.
    std::cout << "Releasing Process ID: 342" << "\n";
    release_pid(342);									

    std::cout << "Releasing Process ID: 350" << "\n";
    release_pid(350);

    std::cout << "Releasing Process ID: 317" << "\n";
    release_pid(317);

    std::cout << "Releasing Process ID: 300" << "\n";
    release_pid(300);

    printArr(pIDs);

    std::cout << "Program Successfully Terminated...\n";

    return 0;
}
