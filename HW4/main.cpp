/*
    Program To Implement And Showcase All 6 Different Disk-Scheduling Algorithms
		FCFS - First Come First Serve
		SSTF - Shortest Seek Time First
		SCAN - The "Elevator" Algorithm
		CSCAN - The "Circular Elevator" Algorithm
		LOOK - Similiar To SCAN But Looks Ahead To See Requests Instead Of Going All The Way
		CLOOK - Circular LOOK, Similar To CSCAN 
*/
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>


// Global Variables For Easy Accessibility Between Functions
int cylinders = 5000;
int requests = 1000;

/*
	Our First Come First Serve Algorithm
	Requests Are Addressed In The Same Order They Come In The Queue
*/
int fcfs(int *arr, int startPosition) 
{
	int work = 0 , currentPos;
	std::vector<int> copyArr(arr,arr+requests);

    for(size_t i = 0; i < copyArr.size(); ++i)
	{
		currentPos = copyArr[i];
    	work += abs(currentPos-startPosition);
		startPosition = currentPos;
	}
     
    return work;
}

/*
	Helper Function For Our Shortest Seek Time First Algorithm
	This Function Assists By Finding The Next Number To Move To
*/
int sstfHelper(std::vector<int> arr, int pos)
{
	int next, closestMin, closestMax;
	bool flag = false, flag2 = false;
	
	for(int i = 0; i < arr.size(); ++i)
	{
		if(arr[i] > pos)	  // Only want less than #s, leave early
			break;
		if(arr[i] <= pos){	  // Get largest # that is less than start position
			closestMin = i;
			flag = true;
		}
	}
		
	for(int i = 0; i < arr.size(); ++i)
	{
		if(arr[i] > pos)	// Get smallest # that is greater than start position
		{
			closestMax = i;
			flag2 = true;
			break;
		}
	}
	
	if(flag == false)			// Never found something smaller, choose next #
		closestMin = closestMax;
	else if(flag2 == false)		// Never found something larger, choose previous #
		closestMax = closestMin;
	
	if(abs(pos-arr[closestMin]) > abs(pos-arr[closestMax]))
		next = closestMax;
	else
		next = closestMin;
	
	
	return next;
}

/*
	Our Shortest Seek Time First Algorithm
	Requests With The Shortest Travel/Seek Time Are Addressed First
*/
int sstf(int *arr, int start)
{
	int work = 0, next, cnt = 0, head = start;
	std::vector<int> copyArr(arr,arr+requests);
	
	std::sort(copyArr.begin(),copyArr.end());
		
	int n = copyArr.size();
	
	while(n>-1)
	{
		if(cnt == 0)
		{
			next = sstfHelper(copyArr,head);	// Find next closest request
			work += abs(copyArr[next] - head);	
			head = copyArr[next];
		}
		else
		{
			next = sstfHelper(copyArr,head);	// Find next closest request
			work += (abs(head-copyArr[next]));
			head = copyArr[next];
			copyArr.erase(copyArr.begin()+next); // Erase visited request to avoid repeat
		}
		
		--n;
		++cnt;
	}
	
	return work;
}


/*
	Our SCAN Algorithm
	Moving In A Specific Direction(Either Up Or Down Like An Elevator)
		Addressing Requests On The Way To The End Of The Disk Then
		Reversing Direction And Addressing Requests Going The Other Way
		
	Going Up Total Movement = abs(start - max floor) + abs(max floor - lowest request)
							= abs(start - (cylinders-1)) + abs((cylinders-1) - arr[0])
	Going Down Total Movement = abs(start - min floor) + abs(min floor - largest request)
							  = abs(start - 0) + abs(0 - arr[arr.size()-1])
*/
int scan(int *arr, int start, int direction)
{
	std::vector<int> copyArr(arr,arr+requests);
	copyArr.push_back(start);
	std::sort(copyArr.begin(),copyArr.end());
	
	int headPos = -1;
	int work = 0;
	
	for(int i = 0; i < copyArr.size(); ++i)
	{
		if(copyArr[i] == start)		// Find starting position
		{
			headPos = i;
			break;
		}
	}
	
	int firstPos = headPos;		// Remember starting position
	
	if(direction == 1)	// Going Up
	{
		for(int i = headPos; i < copyArr.size()-1; ++i) // First pass up
		{
			work += abs(copyArr[i] - copyArr[i+1]);
			headPos += 1;
		}
		
		work += abs(copyArr[headPos] - (cylinders-1));	// Last request to max cylinder
		
		if(firstPos > 0)  // If there is anything to scan downwards to
		{
			work += abs((cylinders-1) - copyArr[firstPos-1]); // Max cylinder down to next
			for(int i = firstPos-1; i > 0; --i) // First pass down
			{
				work += abs(copyArr[i] - copyArr[i-1]);
				headPos = i;
			}
		}
	}
	else if(direction == 0) // Going Down
	{
		int cnt = 0;
		for(int i = headPos; i > 0; --i)	// First pass down
		{
			work += abs(copyArr[i] - copyArr[i-1]);
			headPos -= 1;
			++cnt;
		}
		
		work += abs(copyArr[headPos] - 0);	// First request to min cylinder
		
		if(cnt < copyArr.size()-1)	// If there is anything to scan upwards to
		{
			work += abs(0 - copyArr[firstPos+1]);	// Min cylinder up to next
			for(int i = firstPos+1; i < copyArr.size()-1; ++i) // First pass up
			{
				work += abs(copyArr[i] - copyArr[i+1]);
				headPos = i;
			}
		}
	}
	
	return work;
}

/*
	Our CSCAN Algorithm
	Moving In A Specific Direction(Either Up Or Down Like An Elevator)
		Addressing Requests On The Way To The End Of The Disk Then
		Reversing Direction And Instead Of Addressing Requests Going The
		Other Way We Go To The End And "Reverse" Again To Address Requests
		We Haven't Yet Going The Same Direction As We Did Initially
		
	Going Up Total Movement = abs(start - max floor) + abs(max floor - min floor) + abs(min floor - request right before starting request)
							= abs(start - (cylinders-1)) + abs((cylinders-1) - 0) + abs(0 - arr[firstPos-1])
	Going Down Total Movement = abs(start - min floor) + abs(min floor - max floor) + abs(max floor - request right after starting request)
							  = abs(start - 0) + abs(0 - (cylinders-1)) + abs((cylinders-1) - arr[firstPos+1])
*/
int cscan(int *arr, int start, int direction)
{
	std::vector<int> copyArr(arr,arr+requests);
	copyArr.push_back(start);
	std::sort(copyArr.begin(),copyArr.end());
	
	int headPos = -1;
	int work = 0;
	
	for(int i = 0; i < copyArr.size(); ++i) // Find starting position
	{
		if(copyArr[i] == start)
		{
			headPos = i;
			break;
		}
	}
	
	int firstPos = headPos;		// Remember starting position
	
	if(direction == 1) // Going up
	{
		for(int i = headPos; i < copyArr.size()-1; ++i) // First pass up
		{
			work += abs(copyArr[i] - copyArr[i+1]);
			headPos += 1;
		}
		
		work += abs(copyArr[headPos] - (cylinders-1)); // Last request to max cylinder
		
		if(firstPos > 0)	// If there is anything to scan upwards to
		{
			work += abs(0 - (cylinders-1)); // Max cylinder down to min cylinder
			work += abs(0 - copyArr[0]); 	// Min cylinder up to next
			for(int i = 0; i < firstPos-1; ++i) // Second pass up
			{
				work += abs(copyArr[i] - copyArr[i+1]);
				headPos = i;
			}
		}
	}
	else if(direction == 0) // Going down
	{
		int cnt = 0;
		for(int i = headPos; i > 0; --i)	// First pass down
		{
			work += abs(copyArr[i] - copyArr[i-1]);
			headPos -= 1;
			++cnt;
		}
		
		work += abs(copyArr[headPos] - 0);	// First request to min cylinder
		
		if(cnt < copyArr.size()-1)   // If there is anything to scan downwards to
		{
			work += abs(0 - (cylinders-1));	 	// Min cylinder up to max cylinder
			work += abs((cylinders-1) - copyArr[copyArr.size()-1]); // Max cylinder down to next
			for(int i = copyArr.size()-1; i > firstPos+1; --i) // Second pass down
			{
				work += abs(copyArr[i] - copyArr[i-1]);
				headPos = i;
			}
		}
	}
	
	return work;
}

/*
	Our LOOK Algorithm
	Moving In A Specific Direction(Either Up Or Down Like An Elevator)
		Addressing Requests On The Way To The Last Request(Inclusive)
		Reversing Direction And Addressing Requests Going The Other Way
	
	Going Up Total Movement = abs(start - largest request) + abs(largest request - lowest request)
							= abs(start - arr[arr.size()-1]) + abs(arr[arr.size()-1] - arr[0])
	Going Down Total Movement = abs(start - lowest request) + abs(lowest request - largest request)
							  = abs(start - arr[0]) + abs(arr[0] - arr[arr.size()-1])
*/
int look(int *arr, int start, int direction)
{
	std::vector<int> copyArr(arr,arr+requests);
	copyArr.push_back(start);
	std::sort(copyArr.begin(),copyArr.end());
	
	int headPos = -1;
	int work = 0;
	
	for(int i = 0; i < copyArr.size(); ++i) // Find starting position
	{
		if(copyArr[i] == start)
		{
			headPos = i;
			break;
		}
	}
	
	int firstPos = headPos; 	// Remember starting position
	
	if(direction == 1) // Going up
	{
		for(int i = headPos; i < copyArr.size()-1; ++i) // First pass up
		{
			work += abs(copyArr[i] - copyArr[i+1]);  
			headPos += 1;
		}

		if(firstPos > 0) // If there is anything to scan downwards to
		{
			work += abs(copyArr[copyArr.size()-1] - copyArr[firstPos-1]); // Last request down to next
			for(int i = firstPos-1; i > 0; --i)	// First pass down
			{
				work += abs(copyArr[i] - copyArr[i-1]);
				headPos = i;
			}
		}
	}
	else if(direction == 0) // Going down
	{
		int cnt = 0;
		for(int i = headPos; i > 0; --i) // First pass down
		{
			work += abs(copyArr[i] - copyArr[i-1]);
			headPos -= 1;
			++cnt;
		}
		
		if(cnt < copyArr.size()-1) // If there is anything to scan upwards to
		{
			work += abs(copyArr[0] - copyArr[firstPos+1]); 	// First request up to next
			for(int i = firstPos+1; i < copyArr.size()-1; ++i) // First pass up
			{
				work += abs(copyArr[i] - copyArr[i+1]);
				headPos = i;
			}
		}
	}
	
	return work;
}

/*
	Our CLOOK Algorithm
	Moving In A Specific Direction(Either Up Or Down Like An Elevator)
		Addressing Requests On The Way To The Last Request(Inclusive) Then
		Reversing Direction And Instead Of Addressing Requests Going The
		Other Way We Go To The Last Request In That Direction And "Reverse" 
		Again To Address Requests We Haven't Yet Going The Same Direction As 
		We Did Initially
		
	Going Up Total Movement = abs(start - largest request) + abs(largest request - lowest request) + abs(lowest request - request right before starting request)
							= abs(start - (cylinders-1)) + abs((cylinders-1) - 0) + abs(0 - arr[firstPos-1])
	Going Down Total Movement = abs(start - lowest request) + abs(lowest request - largest request) + abs(largest request - request right after starting request)
							  = abs(start - 0) + abs(0 - (cylinders-1)) + abs((cylinders-1) - arr[firstPos+1])
*/

int clook(int *arr, int start, int direction)
{
	
	std::vector<int> copyArr(arr,arr+requests);
	copyArr.push_back(start);
	std::sort(copyArr.begin(),copyArr.end());
	
	int headPos = -1;
	int work = 0;
	
	for(int i = 0; i < copyArr.size(); ++i) // Find starting position
	{
		if(copyArr[i] == start)
		{
			headPos = i;
			break;
		}
	}
	
	int firstPos = headPos; 	// Remember starting position
	
	if(direction == 1) // Going up
	{
		for(int i = headPos; i < copyArr.size()-1; ++i) // First pass up
		{
			work += abs(copyArr[i] - copyArr[i+1]);
			headPos += 1;
		}
				
		if(firstPos > 0) // If there is anything to scan downwards to
		{
			work += abs(copyArr[0] - copyArr[copyArr.size()-1]); // Last request down to first
			for(int i = 0; i < firstPos-1; ++i)  // Second pass up
			{
				work += abs(copyArr[i] - copyArr[i+1]);
				headPos = i;
			}
		}
	}
	else if(direction == 0) // Going down
	{
		int cnt = 0;
		for(int i = headPos; i > 0; --i)	// First pass down
		{
			work += abs(copyArr[i] - copyArr[i-1]);
			headPos -= 1;
			++cnt;
		}
		
		if(cnt < copyArr.size()-1) // If there is anything to scan upwards to
		{
			work += abs(copyArr[0] - copyArr[copyArr.size()-1]); // First request up to last
			
			for(int i = copyArr.size()-1; i > firstPos+1; --i) // Second pass down
			{
				work += abs(copyArr[i] - copyArr[i-1]);
				headPos = i;
			}
		}
	}
	
	return work;
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "Error! : No Starting Index Given, Exiting...\n";
		return -1;
	}
	
	if(std::stoi(std::string(argv[1])) > 4999)
	{
		std::cout << "Error! : Given Index Too High, Exiting...\n";
		return -1;
	}
	
	if(std::stoi(std::string(argv[1])) < 0)
	{
		std::cout << "Error! : Given Index Too Low, Exiting...\n";
		return -1;
	}
	
	int startPosition = std::stoi(std::string(argv[1]));
	int arr[requests];
	
	srand(time(NULL));
	
	for(size_t i = 0; i < requests; ++i)
	{
		arr[i] = rand() % cylinders;
		//std::cout << arr[i] << "\n"; // Used to see random values in order to verify results
	}
	
	// ** Change The 1 To A 0 To Change Direction **
	std::cout << "\nFCFS Algorithm Seek Time:\t" << fcfs(arr,startPosition);
	std::cout << "\nSSTF Algorithm Seek Time:\t" << sstf(arr,startPosition);
	std::cout << "\nUP SCAN Algorithm Seek Time:\t" << scan(arr,startPosition,1); 
	std::cout << "\nDOWN SCAN Algorithm Seek Time:\t" << scan(arr,startPosition,0); 
	std::cout << "\nUP CSCAN Algorithm Seek Time:\t" << cscan(arr,startPosition,1);
	std::cout << "\nDOWN CSCAN Algorithm Seek Time:\t" << cscan(arr,startPosition,0);
	std::cout << "\nUP LOOK Algorithm Seek Time:\t" << look(arr,startPosition,1);
	std::cout << "\nDOWN LOOK Algorithm Seek Time:\t" << look(arr,startPosition,0);
	std::cout << "\nUP CLOOK Algorithm Seek Time:\t" << clook(arr,startPosition,1);
	std::cout << "\nDOWN CLOOK Algorithm Seek Time:\t" << clook(arr,startPosition,0);
	std::cout << "\n";
	
	requests = 5;
	cylinders = 200;
	int arr2[5] = {45, 98, 32, 109, 74}; 
	std::cout << "\nDOWN CLOOK:\t" << clook(arr2,73,0);
	std::cout << "\nUP SCAN:\t" << scan(arr2,73,1);
	std::cout << "\nUP SCAN:\t" << scan(arr2,73,0);
	
	/* Test Unit I Used To Confirm Results And Functionality.	
	   To Make Use Of This Unit You Must Change Global Variables
	   From Constants To Non-Constants. 
		
	std::cout << "\n\n*********************** TEST ONE ***********************\n";	
		
	requests = 5;
	cylinders = 200;
	int arr2[5] = {23, 89, 132, 42, 187}; 
    int head = 100; 

	std::cout << "\nFCFS:\t\t" << fcfs(arr2,head);
	std::cout << "\nSSTF:\t\t" << sstf(arr2,head);
	std::cout << "\nUP SCAN:\t" << scan(arr2,head,1);
	std::cout << "\nDOWN SCAN:\t" << scan(arr2,head,0);
	std::cout << "\nUP CSCAN:\t" << cscan(arr2,head,1);
	std::cout << "\nDOWN CSCAN:\t" << cscan(arr2,head,0);
	std::cout << "\nUP LOOK:\t" << look(arr2,head,1);
	std::cout << "\nDOWN LOOK:\t" << look(arr2,head,0);
	std::cout << "\nUP CLOOK:\t" << clook(arr2,head,1);
	std::cout << "\nDOWN CLOOK:\t" << clook(arr2,head,0);
	
	
	std::cout << "\n\n*********************** TEST TWO ***********************\n";
	
	requests = 7;
	cylinders = 200;
	int avrr[7] = {82,170,43,140,24,16,190}; 
    head = 50; 

	std::cout << "\nFCFS:\t\t" << fcfs(avrr,head);
	std::cout << "\nSSTF:\t\t" << sstf(avrr,head);
	std::cout << "\nUP SCAN:\t" << scan(avrr,head,1);
	std::cout << "\nDOWN SCAN:\t" << scan(avrr,head,0);
	std::cout << "\nUP CSCAN:\t" << cscan(avrr,head,1);
	std::cout << "\nDOWN CSCAN:\t" << cscan(avrr,head,0);
	std::cout << "\nUP LOOK:\t" << look(avrr,head,1);
	std::cout << "\nDOWN LOOK:\t" << look(avrr,head,0);
	std::cout << "\nUP CLOOK:\t" << clook(avrr,head,1);
	std::cout << "\nDOWN CLOOK:\t" << clook(avrr,head,0);
	
	std::cout << "\n\n*********************** TEST THREE ***********************\n";
	
	requests = 8;
	cylinders = 200;
	int thArr[8] = {98,183,37,122,14,124,65,67}; 
    head = 53; 

	std::cout << "\nFCFS:\t\t" << fcfs(thArr,head);
	std::cout << "\nSSTF:\t\t" << sstf(thArr,head);
	std::cout << "\nUP SCAN:\t" << scan(thArr,head,1);
	std::cout << "\nDOWN SCAN:\t" << scan(thArr,head,0);
	std::cout << "\nUP CSCAN:\t" << cscan(thArr,head,1);
	std::cout << "\nDOWN CSCAN:\t" << cscan(thArr,head,0);
	std::cout << "\nUP LOOK:\t" << look(thArr,head,1);
	std::cout << "\nDOWN LOOK:\t" << look(thArr,head,0);
	std::cout << "\nUP CLOOK:\t" << clook(thArr,head,1);
	std::cout << "\nDOWN CLOOK:\t" << clook(thArr,head,0);

	std::cout << "\n\n*********************** TEST FOUR ***********************\n";
	
	requests = 5;
	cylinders = 200;
	int zArr[5] = {20,35,45,55,67};
    head = 50; 

	std::cout << "\nFCFS:\t\t" << fcfs(zArr,head);
	std::cout << "\nSSTF:\t\t" << sstf(zArr,head);
	std::cout << "\nUP SCAN:\t" << scan(zArr,head,1);
	std::cout << "\nDOWN SCAN:\t" << scan(zArr,head,0);
	std::cout << "\nUP CSCAN:\t" << cscan(zArr,head,1);
	std::cout << "\nDOWN CSCAN:\t" << cscan(zArr,head,0);
	std::cout << "\nUP LOOK:\t" << look(zArr,head,1);
	std::cout << "\nDOWN LOOK:\t" << look(zArr,head,0);
	std::cout << "\nUP CLOOK:\t" << clook(zArr,head,1);
	std::cout << "\nDOWN CLOOK:\t" << clook(zArr,head,0);
	*/
		
	return 0;
}
