#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>


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
    
    int master_burst;
    int master_priority;
    int age;
    int waitingTime;
    int queue_arrival;

    Process(int pid, int bst, int arr, int pri, int dline, int io) {
        processPid = pid;
        processBst = bst;
        processArr = arr;
        processPri = pri;
        processDline = dline;
        processIO = io;
        
        
        master_burst = bst;
        master_priority = pri;
        age = 0;
        waitingTime = 0;
        queue_arrival = 0;
    }
    Process() {

    }
};

void RTS(vector<Process> processes, fstream& stream, int terminalOutput) {
    bool validInput = false;
    bool isHard = false;
    bool failedProcess = false;
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
    int executeTime = 0;
    int masterBurst = 0;

    unsigned long int waitTime = 0;
    unsigned long int turnAroundTime = 0;
    unsigned long int processAmount = processes.size();
    
    bool haveProcess = false;
    
    // comparing for the queue
    auto cmp = [](Process a, Process b) {
        if(a.processDline != b.processDline) {
            return a.processDline > b.processDline; 
        }else if(a.processArr != b.processArr) {
            return a.processArr > b.processArr; 
        }else {
            return a.processPid > b.processPid;
        }
    };
    // queue
    priority_queue<Process, vector<Process>, decltype(cmp) >  processQueue(cmp);

    Process process;

    while (processQueue.size() != 0 || processes.size() != 0) {
        while(processes.size() != 0){
            Process process = processes.back();
            processQueue.push(process);
            processes.pop_back();
        }

        if(haveProcess) {
            if(processQueue.top().processPid != process.processPid){
                // grab the new process that has come in
                if (terminalOutput) {
                    cout << "Process " << process.processPid << " ran from " << timer - executeTime << " through " << timer-1 << "\n";
                }
                else {
                    stream << "Process " << process.processPid << " ran from " << timer - executeTime << " through " << timer-1 << "\n";
                }
                process = processQueue.top();
                masterBurst = process.processBst;
                executeTime = 0;
            }
            processQueue.pop();
        }else if(processQueue.size() > 0) {
            //grab the top process
            executeTime=0;
            process = processQueue.top(); 
            masterBurst = process.processBst;
            processQueue.pop();
            haveProcess = true;
        }else {
             haveProcess = false;
        }
        
        while(haveProcess && (timer+process.processBst) > process.processDline) {
            waitTime += timer - process.processArr;
            haveProcess = false;
            turnAroundTime += (timer - process.processArr);
            failedProcess = true;
            if (terminalOutput) {
                    cout << "Process " << process.processPid << " removed at clock tick " << timer << " due to having a deadline already passed or that will pass before its burst finishes\n";
            }
            else {
                    stream << "Process " << process.processPid << " removed at clock tick " << timer << " due to having a deadline already passed or that will pass before its burst finishes\n";
            }

            if(processQueue.size() > 0) {
                //grab the top process
                executeTime = 0;
                process = processQueue.top(); //grab earliest deadline
                processQueue.pop();//remove
                haveProcess = true;
            }else {
                //we don't have a process and there is nothing in the queue
                haveProcess = false;
            }
        }
        if (isHard && failedProcess) {
            if (terminalOutput) {
                cout << "Since RTS is hard and a process failed, exiting RTS\n";

            }
            else {
                stream << "Since RTS is hard and a process failed, exiting RTS\n";
            }
            break;
        }
        if(haveProcess) {
            executeTime++;
            //we have a process
            
            process.processBst--;
            if(process.processBst == 0){
                //process has finished
                waitTime += (timer + 1 - process.processArr) - masterBurst;
                turnAroundTime += (timer - process.processArr);
                
                if (terminalOutput) {
                    cout << "Process " << process.processPid << " ran from " << (timer - executeTime) + 1 << " through " << timer << " and finished\n";

                }
                else {
                    stream << "Process " << process.processPid << " ran from " << (timer - executeTime) + 1 << " through " << timer << " and finished\n";

                }
                haveProcess = false;
            }else {
                // burst did not finish, requeue
                processQueue.push(process);
            } 
        }
        timer++;
    }

    if (terminalOutput) {
        cout << "\n";
        cout << "Total wait time: " << waitTime << " clock ticks\n";
        cout << "Total turn around time: " << turnAroundTime << " clock ticks\n";
        cout << "Total processes: " << processAmount << " clock ticks\n";
        cout << "Average turn around time: " << ((float)turnAroundTime)/((float)processAmount) << " clock ticks\n";
        cout << "Average wait time: " << ((float)waitTime)/((float)processAmount) << " clock ticks\n";
    }
    else {
        stream << "\n";
        stream << "Total wait time: " << waitTime << " clock ticks\n";
        stream << "Total turn around time: " << turnAroundTime << " clock ticks\n";
        stream << "Total processes: " << processAmount << " clock ticks\n";
        stream << "Average turn around time: " << ((float)turnAroundTime)/((float)processAmount) << " clock ticks\n";
        stream << "Average wait time: " << ((float)waitTime)/((float)processAmount) << " clock ticks\n";
    }
}


void age_queue(vector<queue<Process> > queues, int ageInterval, int ageAmount) {
    int sizeOfQueue = queues[queues.size()-1].size();

    // Loop through queue to age
    while(sizeOfQueue > 0) {
        // Grab last queue from all queues
        Process frontProcess = queues[queues.size()-1].front();
        queues[queues.size()-1].pop();
        frontProcess.age += ageAmount;

        // if the age is greater than the interval, we will age up
        if(frontProcess.age >= ageInterval) {
            frontProcess.age = 0; // make age 0

            // add it to the queue that is above
            queues[queues.size()-2].push(frontProcess);

            // if we want to output to a file
            //if (fileOutput) {
            //    fs << "The process " << frontProcess.processPID << " has aged and was promoted to queue " << queues.size()-2 << "\n";
            //}
        }
        else {
            queues[queues.size()-1].push(frontProcess);
        }

        sizeOfQueue--;
    }
}


void MFQS(vector<Process> processes, int numQueues, int ageInterval, int timeQuantum, fstream& stream, int terminalOutput, int runIO) {
    // queue marker
    int whichQueue = 0;
    

    vector<queue<Process> > queues;
    for (int i = 0; i < numQueues; i++) {
        queue<Process> queue;
        queues.push_back(queue);
    }


    Process process;

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
            queues[0].push(processes.back());
            processes.pop_back();
            readyProcesses++;
        }


        whichQueue = 0;
        // cycle through queues to find a process
        while(whichQueue < queues.size() && queues[whichQueue].size() == 0) {
            whichQueue++;
        }

        // Check to see if there is a process in a queue
        if(whichQueue < queues.size()) {
            // grab first process
            process = queues[whichQueue].front();

            // Test print out for terminal read out
            //if (terminalOutput) {
            //    cout << "PID Selected: " << process.processPid << "\n";
            //}
            
            // Resetting time quantum for new process
            time_Quantum = pow(2, whichQueue) * masterTimeQuantum;
            masterTimeQuantumQueue = time_Quantum;
        }


        else {
            // none found

            timer = processes.back().processArr;
            // If we want to output to the file
            //if (fileOutput) {
            //    fs << timer << "\n";
            //}
        }

        // If this is the last queue, we will not do the time quantum
        if (whichQueue + 1 == queues.size()) {
            // age
            int ageAmount = process.processBst;
            timer += ageAmount; // elapsed

            masterComplete++;
            process.waitingTime = (timer - process.processArr) - process.master_burst;
            masterTurnAround += process.waitingTime + process.master_burst;
            // process is dead, add the wait time
            masterWaitTime += process.waitingTime;

            if (terminalOutput) {
                cout << "The process " << process.processPid << " started at " << timer -  (masterTimeQuantumQueue - ageAmount) << " and ended at " << timer-1 << " in queue " << whichQueue << " and finished\n";
            } else {
                stream << "The process " << process.processPid << " started at " << timer -  (masterTimeQuantumQueue - ageAmount) << " and ended at " << timer-1 << " in queue " << whichQueue << " and finished\n";
            }
            // 
            queues[queues.size()-1].pop();
            //delete process;
            //process = NULL;
            readyProcesses--;
            

            // age it all
            age_queue(queues, ageInterval, ageAmount);

        }

        // all the queues before the last
        else if(whichQueue < queues.size()) {
            // time quantum

            int ageAmount = 0;
            
            if (process.processBst <= time_Quantum) {
                ageAmount = process.processBst;
                
                age_queue(queues, ageInterval, ageAmount); // age the last queue
                
                masterComplete++;
                process.waitingTime = ((ageAmount + timer) - process.processArr) - process.master_burst;
                masterTurnAround += process.waitingTime + process.master_burst;
                masterWaitTime += process.waitingTime;
                

                if (terminalOutput) {
                    cout << "Process " << process.processPid << " ran form " << timer << " through " << (ageAmount + timer)-1 << " in queue " << whichQueue << " and has finished\n";
                }
                else {
                    stream << "Process " << process.processPid << " ran form " << timer << " through " << (ageAmount + timer)-1 << " in queue " << whichQueue << " and has finished\n";
                }
                
                queues[whichQueue].pop();
                //delete process;
                //process = NULL;
                readyProcesses--;
            } else {
                ageAmount = time_Quantum;
                
                process.processBst -= ageAmount;
                
                age_queue(queues, ageInterval, ageAmount);
                
                // demoting process
                Process proc = process;
                queues[whichQueue].pop();
                queues[whichQueue + 1].push(proc); // bring process down one queue
                
                if (terminalOutput) {
                    cout << "Process " << proc.processPid << " ran from " << (ageAmount+timer)-masterTimeQuantumQueue << " through " << (ageAmount+timer)-1 << " in queue " << whichQueue << 
                    " and has been demoted to queue " << whichQueue+1 << "\n";
                }
                else {
                stream << "Process " << proc.processPid << " ran from " << (ageAmount+timer)-masterTimeQuantumQueue << " through " << (ageAmount+timer)-1 << " in queue " << whichQueue << 
                " and has been demoted to queue " << whichQueue+1 << "\n";
                }
            }
            timer += ageAmount;

        }
        if (terminalOutput) {
            cout << timer << "\n";
        }
        else {
            stream << timer << "\n";
        }

    }

    if (terminalOutput) {
        cout << "\nStats:\n";
        cout << "Total Wait Time: " << masterWaitTime << " clock ticks\n";
        cout << "Total Turnaroud time: " << masterTurnAround  << " clock ticks\n";
        cout << "Total completed: " << masterComplete << " clock ticks\n";
        cout << "Average Turnaround time: " << masterTurnAround/masterComplete << " clock ticks\n";
        cout << "Average wait time: " << masterWaitTime/masterComplete << " clock ticks\n";
    }
    else {
        stream << "\nStats:\n";
        stream << "Total Wait Time: " << masterWaitTime << " clock ticks\n";
        stream << "Total Turnaroud time: " << masterTurnAround  << " clock ticks\n";
        stream << "Total completed: " << masterComplete << " clock ticks\n";
        stream << "Average Turnaround time: " << masterTurnAround/masterComplete << " clock ticks\n";
        stream << "Average wait time: " << masterWaitTime/masterComplete << " clock ticks\n";
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
    
    vector<Process> processes;
    string programType;
    int numQueues;
    string fileName;
    int ageInterval;
    int timeQuantum;
    int terminalOutput;
    
    
    std::cout << "Welcome to our Scheduling Algorithm Program\n";
    std::cout << "Please select which algorithm you'd like to run.\n Type 'a' for a Multi-Level Feedback Queue Scheduler (MFQS)\n Type 'b' for a Real-Time Scheduler (RTS)\n Type 'q' to quit:\n";
        
    std::cin >> programType;
    // Check number of queues
    //cout << "You entered: " << programType << "\n";
        
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
        cout << "You chose the MFQS" << "\n";
        // Prompt and input number of queues
        std::cout << "Select a number of queues in between 1 and 5:" << "\n";
        cin >> numQueues;
        // Program response
        //cout << "You entered: " << numQueues << "\n";

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
                
            
            // We now will sort our processes baseed on arrival time.
            sort(processes.begin(), processes.end(), compareArrival);

            int runIO;

            cout << "Grabbing MFQS Information:\n";
            cout << "What is the age interval?:\n";
            cin >> ageInterval;
            cout << "What is the Time Quantum?:\n";
            cin >> timeQuantum;
            cout << "\n";
            //cout << "Do you want to run IO? (0 = No or 1 = Yes):\n";
            //cin >> runIO;
            // Terminal output ?
            while (terminalOutput != 0 || terminalOutput != 1) {
                cout << "Do you want outputs displayed in the terminal or in a file? (0 = File or 1 = Terminal):\n";
                std::cin >> terminalOutput;
                if (terminalOutput == 1) {
                    break;
                }
                if (terminalOutput == 0) {
                    break;
                }
            }

            string mfqsOutputFileName = "MFQSOutput.txt";
            fstream mfqsOutputFile;

            mfqsOutputFile.open("mfqsOutput.txt", std::fstream::out | std::fstream::trunc);
            if (terminalOutput == 0) {
                cout << "Read output in file: " << mfqsOutputFileName << "\n";
            }
            else {
                cout << "Read output in terminal:\n";
            }
            // Call MFQS function
            MFQS(processes, numQueues, ageInterval, timeQuantum, mfqsOutputFile, terminalOutput, runIO);
            mfqsOutputFile << "\n";
            cout << "Reading the file output: " << mfqsOutputFileName << "\n";
            // Print everything from the file stream
            mfqsOutputFile << flush;
            // Close the file
            mfqsOutputFile.close();

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
        while (terminalOutput != 0 || terminalOutput != 1) {
            cout << "Do you want outputs displayed in the terminal or in a file? (0 = File or 1 = Terminal):\n";
            std::cin >> terminalOutput;
            if (terminalOutput == 1) {
                break;
            }
            if (terminalOutput == 0) {
                break;
            }
        }
        // open output file stream
        string rts_output_fileName = "rts_output.txt";
        fstream rts_output_file;
        rts_output_file.open("rts_output.txt", std::fstream::out  | std::fstream::trunc );
        processes = processCreator(fileName);
        sort(processes.begin(), processes.end(), compareDeadline);
        RTS(processes, rts_output_file, terminalOutput);
        if (terminalOutput == 0) {
            cout << "Read output in file: " << rts_output_fileName << "\n";
        }
        else {
            cout << "Read output in terminal\n";
        }
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

