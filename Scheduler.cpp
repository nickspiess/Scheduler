#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

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
    cout << "Process PID" << processPID << endl;
};

int main()
{
    
    Process one;
    
    string programType;
    int numQueues;
    string fileName;
    
    std::cout << "Welcome to our Scheduling Algorithm Program\n";
    std::cout << "Please select which algorithm you'd like to run.\n Type 'a' for a Multi-Level Feedback Queue Scheduler (MFQS)\n Type 'b' for a Real-Time Scheduler (RTS):" << endl;
        
    std::cin >> programType;
    // Check number of queues
    cout << "You entered: " << programType << endl;
        
        
    
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
        std::vector<std::queue<int> > queues(numQueues);
        //vector<queue<int>> vec;
        // Can be accessed through queues[0], queues[1], ...,  queues[4]
        
        // Check number of queues
        // Correct number of queues
        if (numQueues >= 1 || numQueues <= 5) {
            cout << "Please enter the name of the file you'd like to input:\n";
            // Grab file name
            std::cin >> fileName;

            // Open input file stream
            std::ifstream in(fileName);
            // Declare line
            string line;
            // While the file still has lines:
            while (std::getline(in, line)) {

                in >> pid >> bst >> arr >> pri >> dline >> IO;

                one.processPid = pid;
                one.processBst = bst;
                one.processArr = arr;
                one.processPri = pri;
                one.processDline = dline;
                one.processIO = IO;
                //cout << "PID = " << one.processPid << " BST = " << one.processBst << " arr = " << one.processArr << " pri = " << one.processPri << " dline = " << one.processDline << " IO = " << one.processIO << endl;
                
                queues[0].push(one);
                cout << queues[0].front();
                

                // Call mfqs corresponding functions according to these variables
                //
            }
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
