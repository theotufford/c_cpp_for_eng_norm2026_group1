#ifndef DIALOGUE_H // Header guard to prevent multiple inclusions of the same
                   // header file
#define DIALOGUE_H

#include <string>
#include <vector>

using namespace std;

// Provides abstraction for displaying dialogue (automatically tracks character
// info and dialogue lines)
class Dialogue {
private:
  string characterName;         // Name of character speaking
  string asciiPortrait;         // ASCII portrait of the speaking Character's
  vector<string> dialogueLines; // List of dialogue lines for the character

  void printPortraitPanel(); // Prints portrait panel to the screen
  void printDialoguePanel(); // Prints the dialogue panel to the screen

public:
  Dialogue(string name, string portrait,
           vector<string> lines); // Constructs a dialogue object with character
                                  // info and lines
  void display(); // // Displays the full dialogue with name, portrait, and
                  // dialogue text
};

struct DialogueNode {
  string stateKey;    // Identifies the dialogue state ("intro", "item_yes",
                      // "item_no", "item_wrong")
  Dialogue *dialogue; // Pointer to the dialogue associated with the state
  DialogueNode *next; // Pointer to the next dialogue node in the list

  DialogueNode(string key, Dialogue *dlg); // Constructs a dialogue node with a
                                           // state key and dialogue reference
};

// Loads dialogue lines from a source file(filename.txt) for the requested
// character and dialogue state
vector<string> loadDialogueLines(string filename, string targetChar,
                                 string targetState);

// Builds a dialogue object from a source file so that all the right dialogue
// states and portraits are grouped with the right character
Dialogue buildDialogue(string filename, string characterName, string state,
                       string portrait);

// Trigger the matching dialogue state from the dialogue tree starting at the
// provided node head(node head is the entry point of a linked list)
void triggerDialogue(DialogueNode *head, string state);

#endif
