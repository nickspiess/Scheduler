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

    Process(int pid, int bst, int arr, int pri, int dline, int io) {
        processPid = pid;
        processBst = bst;
        processArr = arr;
        processPri = pri;
        processDline = dline;
        processIO = io;
    }
    Process() {

    }
};

void RTS(vector<Process> processes, fstream& stream) {
    bool validInput = false;
    bool isHard = false;
    cout << "Do you want to run hard or soft RTS? Type 'a' for hard and 'b' for soft \n";
    while (!validInput) {
        string rtsType;
        cin >> rtsType;
        if (rtsType == "a") {
            isHard = true;
            validInput = true;
        }else if (rtsType == "b") {
            isHard = false;
            validInput = true;
        } else {
            cout << "Invalid input: Please try again. Type 'a' for hard and 'b' for soft \n";
        }
    }
    int timer = 0;
    int numberRemoved = 0;
    int executeTime = 0;
    int masterBurst = 0;

    unsigned long int waitTime = 0;
    unsigned long int turnAroundTime = 0;
    unsigned long int completed = 0;
    
    bool haveProcess = false;
    
    // comparing for the queue
    auto cmp = [](Process a, Process b) {
        if(a.processDline == b.processDline){
            if(a.processPri == b.processPri){
                // sort by lowest pid
                return a.processPid > b.processPid;
            }else{
                // sort by lowest priority
                return a.processPri > b.processPri; 
            }
        }
        // sort by lowest deadline
        return a.processDline > b.processDline; 
    };
    // queue
    priority_queue<Process, vector<Process>, decltype(cmp) >  priority_queue(cmp);

    Process process;

    while (priority_queue.size() != 0 || processes.size() != 0) {
        while(processes.size() != 0){
            Process process = processes.back();
            priority_queue.push(process);
            processes.pop_back();
        }

        if(haveProcess) {
            if(priority_queue.top().processPid != process.processPid){
                // grab the new process that has come in
                stream << "Process " << process.processPid << " ran from " << timer - executeTime << " through " << timer-1 << "\n";
                process = priority_queue.top();
                masterBurst = process.processBst;
                executeTime = 0;
            }
            priority_queue.pop();
        }else if(priority_queue.size() > 0) {
            //grab the top process
            executeTime=0;
            process = priority_queue.top(); 
            masterBurst = process.processBst;
            priority_queue.pop();
            haveProcess = true;
        }else {
             haveProcess = false;
        }
        
        while((haveProcess && (timer + process.processBst > process.processDline && isHard))) {
            waitTime += timer - process.processArr;
            haveProcess = false;
            turnAroundTime += (timer - process.processArr);
            numberRemoved++; 
            
            stream << "Process " << process.processPid << " removed at clock tick " << timer << " due to having a deadline already passed or that will pass before its burst finishes\n";

            if(priority_queue.size() > 0) {
                //grab the top process
                executeTime = 0;
                process = priority_queue.top(); //grab earliest deadline
                priority_queue.pop();//remove
                haveProcess = true;
            }else {
                //we don't have a process and there is nothing in the queue
                haveProcess = false;
            }
        }
        
        if(haveProcess) {
            executeTime++;
            //we have a process
            
            process.processBst--;
            if(process.processBst == 0){
                //process has finished
                completed++;
                waitTime += (timer + 1 - process.processArr) - masterBurst;
                turnAroundTime += (timer - process.processArr);
                    
                stream << "Process " << process.processPid << " ran from " << (timer - executeTime) + 1 << " through " << timer << " and finished\n";
                haveProcess = false;
            }else {
                // burst did not finish, requeue
                priority_queue.push(process);
            } 
        }
        timer++;
    }
    stream << "\n";
    stream << "Total wait time: " << waitTime << " clock ticks\n";
    stream << "Total turn around time: " << turnAroundTime << " clock ticks\n";
    stream << "Total completed: " << completed << "\n";
    stream << "Total removed: " << numberRemoved << "\n";
    stream << "Average turn around time: " << ((float)turnAroundTime)/((float)(completed + numberRemoved)) << " clock ticks\n";
    stream << "Average wait time: " << ((float)waitTime)/((float)(completed + numberRemoved)) << " clock ticks\n";
}


void MFQS(vector<Process> processes, int numQueues, int ageInterval, int timeQuantum, fstream& stream) {
    // queue marker
    int whichQueue = 0;

    vector<queue<Process*> > queues;
    for (int i = 0; i < numQueues; i++) {
        queue<Process*> queue;
        queues.push_back(queue);
    }


    Process* processor = NULL;

    unsigned long long int masterWaitTime = 0;
    unsigned long long int masterComplete = 0;
    unsigned long long int masterTurnAround = 0;
    int timer = 0;
    const int masterTimeQuantum = timeQuantum;
    int masterTimeQuantumQueue = 0;
    int time_Quantum = masterTimeQuantum;
    int readyProcesses = 0;


    // We are going to gather fresh processes when arrival arrives
    while (readyProcesses > 0 || processes.size() > 0) {
        while (processes.size() > 0 && processes.back().processArr <= timer) {
            //queues[0].push(processes.back());
            processes.pop_back();
            readyProcesses++;
        }


        whichQueue = 0;
        // cycle through queues to find a process
        while(whichQueue < queues.size() && queues[whichQueue].size() == 0) {
            whichQueue++;
        }
    }




}


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
    return beg.processArr < end.processArr;
}

bool compareDeadline(const Process beg, const Process end){
    return beg.processDline < end.processDline;
}


int main()
{
    // Create a vector of processes
    vector<Process> processes;
    string programType;
    int numQueues;
    string fileName;
    int ageInterval;
    int timeQuantum;
    string mfqsOutputFileName = "MFQSOutput.txt";

    fstream mfqsOutputFile;
    
    std::cout << "Welcome to our Scheduling Algorithm Program\n";
    std::cout << "Please select which algorithm you'd like to run.\n Type 'a' for a Multi-Level Feedback Queue Scheduler (MFQS)\n Type 'b' for a Real-Time Scheduler (RTS)\n Type 'q' to quit:" << endl;
        
    std::cin >> programType;
    // Check number of queues
    cout << "You entered: " << programType << endl;
        
    // Vector queue of our processes using the Process class
    vector<Process> processesCreated;

    if (programType == "q") {
            exit(1);
        }

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
                
            //Process proc = processes.begin();
            
            // We now will sort our processes baseed on arrival time.
            sort(processes.begin(), processes.end(), compareArrival);

            cout << "Grabbing MFQS Information:\n";
            cout << "What is the age interval?:\n";
            cin >> ageInterval;
            cout << "\nWhat is the Time Quantum?:\n";
            cin >> timeQuantum;
            
            // size of processes array
            //int size = processes.size();
            //std::cout << "size = " << size << "\n";
            for (int i = 0; i <= processes.size() - 1; i++) {
                cout << "PID = " << processes[i].processPid << "   Arrival: " << processes[i].processArr << "\n";
            }

            //mfqsOutputFile.open(mfqsOutputFileName, std::fstream::out || std::fstream::trunc);
            // Call MFQS function
            //MFQS(processes, int numQueues, int ageInterval, int timeQuantum, mfqsOutputFile);
            //void MFQS(vector<Process> processes, int numQueues, int ageInterval, int timeQuantum, fstream& stream) {
            //mfqsOutputFile << "\n";
            //cout << "Reading the file output: " << mfqsOutputFileName << "\n";
            // Print everything from the file stream
            //mfqsOutputFile << flush;
            // Close the file
            //mfqsOutputFile.close();



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
        // open output file stream
        string rts_output_fileName = "rts_output.txt";
        fstream rts_output_file;
        rts_output_file.open("rts_output.txt", std::fstream::out  | std::fstream::trunc );
        processes = processCreator(fileName);
        sort(processes.begin(), processes.end(), compareDeadline);
        RTS(processes, rts_output_file);
        cout << "read output in file: " << rts_output_fileName << "\n";
        rts_output_file << "\n";
        rts_output_file << flush; // print everything in the buffer
        rts_output_file.close();
    }
    // Invalid input
    else {
        cout << "Incorrect input, please try again.";
        exit(1);
    }
}

