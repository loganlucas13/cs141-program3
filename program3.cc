/**------------------------------------------
    Program 3: Typing Tutor

    Course: CS 141, Spring 2023.
    System: Windows, MacOS, and Replit
    Author: Logan Lucas, George Maratos and David Hayes
 ---------------------------------------------**/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <sstream>
using namespace std;

const int NMISP = 5;
const int RNG_RANGE = 100;

void display_header(const int ndict, const int nmiss) {
    cout << "*** Typing Tutor \n";
    cout << "There are " << ndict << " words in the dictionary.\n";
    cout << "There are " << nmiss << " commonly misspelled words.";
}

void display_menu_options() {
    cout << endl << "*** Select a menu option:" << endl
         << "1. Spell commonly misspelled words" << endl
         << "2. Type random words from the full dictionary" << endl
         << "3. Display some words from one of the lists" << endl
         << "4. Use binary search to lookup a word in the dictionary" << endl
         << "5. Exit the program." << endl
         << "Your choice: ";
}

vector<int> randn(const int n, const int max) {
    vector<int> randNums;
    
    for (int i = 0; i < n; i++) {
        randNums.push_back(rand() % (max + 1));
    }
    return randNums;
}

ifstream open(const string& src_file) {
    ifstream src{src_file};
    if (!src.is_open()) {
        cout << "Could not find file " << src_file << "." << endl;
        exit(1);
    }
    return src;
}

void extract_dict(vector<string>& dst, const string& src_file) {
    ifstream src;
    src.open(src_file);

    // adds each line in src_file to the dst vector
    for (string line; getline(src, line);) {
        dst.push_back(line);
    }
}

void extract_misspelled(vector<string>& misspelled, vector<string>& correct, const string& src_file) {
    ifstream src = open(src_file);

    int i = 0;
    string word;
    
    // gets input from src_file and adds to vector
    while (src >> word) {
        if (i % 2 == 0) // checks if first word
            misspelled.push_back(word);
        else
            correct.push_back(word);
        i++; // switches between misspelled and correct word in line
    }
}

void display_elems(const vector<string>& src, const vector<int>& positions) {
    for (int i = 0; i < positions.size(); i++) {
        if (i != positions.size() - 1)
            cout << src.at(positions.at(i)) << " ";
        else
            cout << src.at(positions.at(i));
    }
}

void extract_spelling_answers(vector<string>& dst, const vector<string>& correct, const vector<int> positions) {
    for (int i = 0; i < positions.size(); i++) {
        dst.push_back(correct.at(positions.at(i)));
    }
}

void extract_spelling_mistakes(vector<string>& dst, const string& to_grade, const vector<string>& correct) {
    string word;
    string errorCode;
    
    stringstream ss(to_grade);
    int i = 0;

    while (ss >> word) { // splits string at spaces
        if (word != correct[i]) {
            errorCode = word + " -> " + correct[i];
            dst.push_back(errorCode);
        }
        i++;
    }
}

void report_misspell_mistakes(const vector<string>& mistakes, const int num_words) {
    // displays misspelled words from user input
    if (mistakes.size() == 0) {
        cout << "No misspelled words!" << endl;
    }
    else {
        cout << "Misspelled words: " << endl;
        for (int i = 0; i < mistakes.size(); i++) {
            cout << "    " << mistakes.at(i) << endl;
        }
    }
    
    int posScore = 0;
    int negScore = 0;

    posScore = (num_words - mistakes.size()) * 3; // correct words
    negScore = mistakes.size() * 6; // incorrect words

    cout << posScore << " points: " << (5 - mistakes.size()) << " spelled correctly x 3 points for each." << endl;
    cout << negScore << " point penalty: " << mistakes.size() << " spelled incorrectly x -6 points for each." << endl;
    cout << "Score: " << posScore - negScore << endl;
}

void evaluate_user_misspells(const vector<string>& src, const vector<int>& positions) {
    string userInput;
    vector<string> correctWords;
    vector<string> userErrors;

    getline(cin, userInput);
    
    extract_spelling_answers(correctWords, src, positions);
    extract_spelling_mistakes(userErrors, userInput, correctWords);
    report_misspell_mistakes(userErrors, correctWords.size());
}

void misspelled(const vector<string>& mspelled, const vector<string>& correct) {
    // outputs and runs option 1
    cout << endl << "*** Using commonly misspelled words.\n";
    vector<int> randResult = randn(5, mspelled.size() - 1);
    
    display_elems(mspelled, randResult);
    cout << endl;
    cout << "Type the words above with the correct spelling:\n";
    evaluate_user_misspells(correct, randResult);
}

void full_dict(const vector<string>& dict) {
    // outputs and runs option 2
    cout << endl << "*** Using full dictionary.\n";
    vector<int> randResult = randn(5, dict.size() - 1);

    display_elems(dict, randResult);
    cout << endl;
    cout << "Correctly type the words above:\n";
    evaluate_user_misspells(dict, randResult);
}

void display_words_prompt() {
    // This displays the data set choices for menu option 3
    cout << endl << "*** Displaying word list entries." << endl
         << "Which words do you want to display?" << endl
         << "A. Dictionary of all words" << endl
         << "B. Commonly misspelled words (wrongly spelled)" << endl
         << "C. Commonly misspelled words (correctly spelled)" << endl
         << "Your choice: ";
}

void print_range(const vector<string>& data) {
    bool valid = true;
    int start;
    int end;
    int length = data.size() - 1;

    
    cout << "*** Enter the start and end indices between 0 and " << length << ": ";
    cin >> start;
    cin >> end;
    
    // checks user input for errors
    if (end > length || start > end) {
        cout << "The end index must come after the start, and the indices must be in range. \n";
        valid = false;
    }

    if (valid) {
        for (start; start <= end; start++) {
            cout << start << ". " << data[start] << endl;
        }
    }
        
}

void display_in_range(const vector<string>& dict, const vector<string>& mspelled, const vector<string>& correct) {
    char userChoice;
    display_words_prompt();
    cin >> userChoice;

    // changes the data used in print_range depending on user input
    switch (toupper(userChoice)) {
        case 'A':
            print_range(dict);
            break;

        case 'B':
            print_range(mspelled);
            break;

        case 'C':
            print_range(correct);
            break;
    }
}

void bsearch(const vector<string>& data) {
    int length = data.size();
    int start = 0;
    int mid;
    int count = 1;
    bool found = false;

    string element;
    string userInput;
    cout << "*** Enter word to search: "; 
    cin >> userInput;

    // binary search algorithm
    do {
        mid = (start + length) / 2;
        element = data[mid];
        if (count < 10)
            cout << "    " << count << ". Comparing to: " << element << endl;
        else
            cout << "   " << count << ". Comparing to: " << element << endl;
        count++;
        
        if (element < userInput)
            start = mid + 1;
        else if (element > userInput)
            length = mid;
        else {
            found = true;
            break;
            }
        } while (start < length);
    if (found)
        cout << userInput << " was found.\n";
    else
        cout << userInput << " was NOT found.\n";
}

void extract_all_data(vector<string>& dict, vector<string>& mspelled, vector<string>& correct) {
    extract_dict(dict, "dictionary.txt");
    extract_misspelled(mspelled, correct, "misspelled.txt");
}

int main() {
    srand(1);
    vector<string> dict, mspelled, correct;
    extract_all_data(dict, mspelled, correct);
    display_header(dict.size(), correct.size());
    unsigned int choice;
    do {
        display_menu_options();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
        case 1:
            misspelled(mspelled, correct);
            break;
        case 2:
            full_dict(dict);
            break;
        case 3:
            display_in_range(dict, mspelled, correct);
            break;
        case 4:
            bsearch(dict);
            break;
        case 5:
            cout << "Exiting." << endl;
            break;
        default:
            cout << "No menu option " << choice << ". "
                 << "Please try again." << endl;
        }
    } while ((choice != 5) && !cin.eof());

    return 0;
}
