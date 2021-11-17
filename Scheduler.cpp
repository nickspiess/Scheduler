#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int pid;
int bst;
int arr;
int pri;
int dline;
int IO;

class Process {
public:
    int processPid;
    int processBst;
    int processArr;
    int processPri;
    int processDline;
    int processIO;
};



// This function is to create and return a vector of processes
vector<Process> processCreator(string fileName) {
    // Create a vector of all our processes
    vector<Process> processes;

    // Open input file stream
    std::ifstream in(fileName);
    // Declare line
    string line;
    // While the file still has lines:
    while (std::getline(in, line)) {

        // Grabbing the input from the line
        in >> pid >> bst >> arr >> pri >> dline >> IO;

        // Sanitizing input
        if (pid >= 0 && bst >= 0 && arr >= 0 && pri >= 0 && dline >= 0 && IO >= 0) {
            // Creating a process
            Process process;
            process.processPid = pid;
            process.processBst = bst;
            process.processArr = arr;
            process.processPri = pri;
            process.processDline = dline;
            process.processIO = IO;
            // Add process to our vector queue
            processes.push_back(process);
        }

        // We will sort our processes
        //cout << "PID = " << one.processPid << " BST = " << one.processBst << " arr = " << one.processArr << " pri = " << one.processPri << " dline = " << one.processDline << " IO = " << one.processIO << endl;
                
                //queues[0].push(one);
                //cout << queues[0].front();
                

                // Call mfqs corresponding functions according to these variables
                //
            }

    return processes;
}


bool compareArrival(const Process beg, const Process end){
    return beg.processArr > end.processArr;
}


int main()
{
    
    
    string programType;
    int numQueues;
    string fileName;
    int ageInterval;
    int timeQuantum;
    
    std::cout << "Welcome to our Scheduling Algorithm Program\n";
    std::cout << "Please select which algorithm you'd like to run.\n Type 'a' for a Multi-Level Feedback Queue Scheduler (MFQS)\n Type 'b' for a Real-Time Scheduler (RTS):" << endl;
        
    std::cin >> programType;
    // Check number of queues
    cout << "You entered: " << programType << endl;
        
    // Vector queue of our processes using the Process class
    vector<Process> processesCreated;

    // MFQS
    if (programType == "a") {
        // declare input string
        string input = "";
        // Program response
        cout << "You chose the MFQS" << endl;
        // Prompt and input number of queues
        std::cout << "Select a number of queues in between 1 and 5:" << endl;
        cin >> numQueues;
        // Program response
        cout << "You entered: " << numQueues << endl;

        // Create and initialize vector container of queues based on user input
        vector<std::queue<int> > queues(numQueues);
        
        // Create a vector of processes
        vector<Process> processes;
        

        // Can be accessed through queues[0], queues[1], ...,  queues[4]
        

        // Check number of queues
        // Correct number of queues
        if (numQueues >= 1 || numQueues <= 5) {
            cout << "Please enter the name of the file you'd like to input:\n";
            // Grab file name
            std::cin >> fileName;

            // Add all processes to queue of processes from the returned queue of processes
            processes = processCreator(fileName);

            cout << "processes size: = " << processes.size();
            //cout << "created processes size: = " << processesCreated.size();

            if(processes.size() > 0) {
                //for(int i = 0; i < createdProcesses.size(); i++) {
                    //Process OldProc = *
                //}
            }

            // We now will sort our processes baseed on arrival time.
            sort(processes.begin(), processes.end(), compareArrival);

            cout << "Grabbing MFQS Information:\n";
            cout << "What is the age interval?:\n";
            cin >> ageInterval;
            cout << "What is the Time Quantum?:\n";
            cin >> timeQuantum;
            
            // size of processes array
            int size = processes.size();
            std::cout << "size = " << size << "\n";
            Process proc;
            //proc = processes.begin();
            //cout << "PID = " << proc.processPid;


            //for (int i = 0; i <= size; i++) {
             //   cout << "PID = " << processes.begin().processPid;
                //<< " BST = " << bst << " arr = " << arr << " pri = " << pri << " dline = " << dline << " IO = " << IO << endl;
            //}

            //cout << "PID = " << pid << " BST = " << bst << " arr = " << arr << " pri = " << pri << " dline = " << dline << " IO = " << IO << endl;




        }
        // incorrect # of queues, exit
        else {
            cout << "Incorrect number of queues, please start over and select a queue number between 1 and five" << endl;
            exit(1);
            
        }
    }
    // RTS
    else if (programType == "b") {
        cout << "You chose the RTS\n";
        cout << "Please enter the name of the file you'd like to input:\n";
        // Grab file name
        std::cin >> fileName;

        // Open input file stream
        std::ifstream in(fileName);
        // Declare line
        string line;
        // While the file still has lines:
        while (std::getline(in, line)) {
            
            //lineParser(line);
            in >> pid >> bst >> arr >> pri >> dline >> IO;
            cout << "PID = " << pid << " BST = " << bst << " arr = " << arr << " pri = " << pri << " dline = " << dline << " IO = " << IO << endl;
            // Call RTS corresponding functions according to these variables
            //
        }
    }
    // Invalid input
    else {
        cout << "Incorrect input, please try again.";
        exit(1);
    }
}


void MFQS(int numQueues) {
    
}

void RTS() {
    
}


void lineParser(string line) {



}
