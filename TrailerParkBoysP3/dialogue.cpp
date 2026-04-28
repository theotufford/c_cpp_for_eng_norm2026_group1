#include "dialogue.h"
#include <iostream>
#include <fstream>

using namespace std;

// Constructs a dialogue object with CHARACTER info(name, portrait) and dialogue lines
Dialogue::Dialogue(string name, string portrait, vector<string> lines) {
    characterName = name;
    asciiPortrait = portrait;
    dialogueLines = lines;
}


// Prints the top panel with ASCII portrait
void Dialogue::printPortraitPanel() {
    cout << "+-----------------------------+" << endl;
    cout << "| " << characterName            << endl;
    cout << "|"                              << endl;
    cout << asciiPortrait;
    cout << "+-----------------------------+" << endl;
}

// Prints the bottom panel with dialogue lines
void Dialogue::printDialoguePanel() {
    cout << "|"                              << endl;
    for (string line : dialogueLines) {
        cout << "| " << line                 << endl;
    }
    cout << "|"                              << endl;
    cout << "+-----------------------------+" << endl;
}



// Prints the portrait and dialogue panels together
void Dialogue::display() {
    printPortraitPanel();
    printDialoguePanel();
}

// Constructs a dialogue node with a state key and dialogue reference
DialogueNode::DialogueNode(string key, Dialogue* dlg) {
    stateKey = key;
    dialogue = dlg;
    next = nullptr;
}

//TODO: Function for loadDialogueLines
vector<string> loadDialogueLines(string filename,
                                  string targetChar,
                                  string targetState) {
    ifstream file(filename);
    vector<string> lines;

    if (!file.is_open()) {
        cout << "Error: Could not open file: " << filename << endl;
        cout << "Make sure the .txt file is in the same folder." << endl;
        return lines;
    }

    string line;
    bool foundChar  = false;
    bool foundState = false;
    bool foundLines = false;

    while (getline(file, line)) {
        if (line == "[CHARACTER:" + targetChar + "]") {
            foundChar  = true;
            foundState = false;
            foundLines = false;
            continue;
        }
        if (foundChar && line == "[STATE:" + targetState + "]") {
            foundState = true;
            foundLines = true;
            continue;
        }
        if (foundLines && line == "[END]") {
            break;
        }
        if (foundLines && line.substr(0, 11) == "[CHARACTER:") {
            foundChar  = false;
            foundState = false;
            foundLines = false;
            lines.clear();
        }
        if (foundLines) {
            lines.push_back(line);
        }
    }

    file.close();
    return lines;
}


// Builds a dialogue object by loading the correct lines for a CHARACTER and state
Dialogue buildDialogue(string filename, string characterName, string state, string portrait) {
    vector<string> lines = loadDialogueLines(filename, characterName, state);

    if (lines.empty()) {
        lines.push_back("...");
    }
    return Dialogue(characterName, portrait, lines);
};

// Searches the linked list to find and display the right state
void triggerDialogue(DialogueNode* head, string state) {
    DialogueNode* current = head;
    while (current != nullptr) {
        if (current->stateKey == state) {
            current->dialogue->display();
            return;
        }
        current = current->next;
    }
    cout << "Error: No dialogue found for state: " << state << endl;
}
