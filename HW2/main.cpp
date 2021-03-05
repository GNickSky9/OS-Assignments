/*
    Test Program To Test Our pID Manager Which Is Defined in pIDmanager.h
    This Program Can Easily Be Modified To Accept User Input.
    This Program Has Been Tested With All Edge Cases That I Could Think Of and
        Has Been Successful Everytime. The Final Version Of This Test Doesn't
        Include All The Edge Cases.
    ****UPDATED 10/14/20****
    This Program Starts Off By Calling allocate_map() To Initialize The pIDs.
    Then The Program Creates Threads Equal To The Amount Of pids We Have And
        Assigns To Every Thread A Different pid. Every Thread Then Sleeps
        For A Random Period Of Time And Then Releases The pid Allocated To It
        In Order For Other Threads To Have Access To That pid. Most Of This
        Work Is Done In The threadWork Function.
    And The Program Prints To Screen The Status Of The Threads And pIDs At
        Every Step.
*/

#include <vector>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <time.h>
#include <stdio.h>
#include "pIDmanager.h"

/*
  Structure For All The Information Regarding A Thread.
    pID Shows Which Process ID Was Allocated To The Thread.
    sleepTime Is The Randomized Amount Of Time The Thread Will Sleep For.
    threadNo Is Essentially The ID Number Of The Thread.
*/
struct threadInfo{
    int pID;
    int sleepTime;
    int threadNo;
};

/*
  threadWork Is The Function Each Thread Will Run Upon Creation.
  This Function Accept A Pointer To A threadInfo Object.
  This Function Allocates A pid To A Thread Telling The Thread Which
    Process To Execute. It Will Then Sleep For A Random Period Of
    Time. Finally It Releases The pid Assigned To The Thread.
  This Function Can Return Any Pointer For Testing Purposes/etc. But
    Isn't Necessary, Could Easily Change Function Return Type To
    Just Void And Return Nothing.
*/
void *threadWork(threadInfo *info)
{
    struct threadInfo temp = *(info);
    printf("\nThread [%d] Fetching pid...\n",temp.threadNo);
    temp.pID = allocate_pid();

    /*
        This Block Of Code Below Is For A Situation Where There Are More
        Threads Created And Asking For A pid Than There Are pids. It Will
        Notify The User And Then Try To Fix The Problem By Sleeping Until
        A pid Is Availale. Here The Sleep Timer Is Set To 10 Seconds But In
        Reality Would Be Longer. However This Should Never Occur And Therefore
        Isn't Necessary.
    */
    /*while(temp.pID == -1){
        printf("[%d] Thread #..., Unable To Allocate Process ID, All pids In Use. \
        Trying Again...\n",temp.threadNo);
        std::this_thread::sleep_for(std::chrono::seconds(10));
        printf("Thread [%d] Fetching pid...\n",temp.threadNo);
        temp.pID = allocate_pid();
    }*/

    printf("\nThread [%d] Allocated pid: [%d]\n",temp.threadNo,temp.pID);
    temp.sleepTime = rand() % (11-5) + 5;
    printArr(pIDs);
    printf("\nThread [%d] Entered Sleep Mode For [%ds] Containing pid [%d]\n" \
        ,temp.threadNo,temp.sleepTime,temp.pID);
    std::this_thread::sleep_for(std::chrono::seconds(temp.sleepTime));
    printf("\nThread [%d] Exited Sleep Mode Containing pid [%d]\n",temp.threadNo,temp.pID);
    release_pid(temp.pID);
    printf("\nThread [%d] Released pid: [%d]\n",temp.threadNo,temp.pID);
    printArr(pIDs);

    return nullptr;
}

int main(int argc, char* argv[])
{
    std::cout << "\nLaunching ProcessID Manager...\n";
    srand(time(NULL));
    std::cout << "\nAllocating Map...\n";

    if(allocate_map())          // Initializing pID Statuses.
        std::cout << "Map Allocation Complete!\n";
    else{
        std::cout << "Map Allocation Unsucessful!\n";
        return -1;
    }

    printArr(pIDs);

    std::cout << "\nCreating Threads!\n";

    std::vector<std::thread> threads; // Array Of Threads.
    threadInfo thInfos[TOTAL_PID];    // Corresponding Array Of Thread Info.

    std::stringstream msg;  // Used For Outputting To The Console.

    for(int i = 0; i < TOTAL_PID; ++i) // Creating # Of Threads Equal To # Of pids.
    {
        thInfos[i].threadNo = i+1;
        msg << "\n\tThread [" << i+1 << "] Created\n";
        std::cout << msg.str();
        msg.str(std::string());
        msg.clear();
        threads.push_back(std::thread(threadWork,&thInfos[i]));
        std::this_thread::sleep_for(std::chrono::seconds(2));
        /* Line Above (107) Not required but helps with output. */

    }

    for(int i = 0; i < threads.size(); ++i)  // Join Threads.
        threads[i].join();

    std::cout << "\nProgram Successfully Terminated...\n\n";

    return 0;
}
