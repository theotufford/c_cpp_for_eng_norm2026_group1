#ifndef DIALOGUE_H
#define DIALOGUE_H

// Everything goes in here

#include <string>
#include <vector>

using namespace std;


class Dialogue {
    private:
    string characterName; // Character speaking's name
    string asciiPortrait; // Character's ASCII picture
    vector<string> dialogueLines; // What they say in their dialogue

    void printPortraitPanel();
    void printDialoguePanel();

    public:
    Dialogue(string name, string portrait, vector<string> lines);
    void display();
};

 struct DialogueNode {
    string stateKey; // "intro", "item_yes", "item_no" 
    Dialogue* dialogue; // points to the dialogue object to display
    DialogueNode* next; // next node in the tree

    DialogueNode(string key, Dialogue* dlg);
};

    // TODO: functions for loading dialogue lines from text file, functions for building character dialogue, and function to trigger the dialogue state

#endif