 #include <iostream>
 #include <string>
 #include <fstream>
 #include <vector>
 #include <string>
 #include <cctype>
 #include <cmath>

 using namespace std;
 
struct secret { // create a structure to recive infor about the secretword
string word; // what is the secret word?
int index = 0; // what is the position of secret word in indexlist_filename
int position = 0; // what is the position of the secret word in the source textfile 
};    

  secret get_valid_secret_index(string filename) { // choose a file to get a secret for (stirng filename will be refered to as sourcefile)
  
  secret secretWord; // create a secret we can mess with 

  int chosen_Index; // this will get put into secretWord.index later
  bool isMyJobDone = false; // used to determine if a index of valid secret words already exists
  vector <string> secret; // used to store valid secret words from the source text file 
  srand(time(0)); // initialize random to avoid seed prediction
    
  ifstream checkFile; // create a read stream called checkfile 
  checkFile.open("indexlists_" + filename); // try to open the index of valid secret words for the called source file
  while (checkFile.good()) { // if it exists and is not empty this will run.
      string tempString; // will hold the word of interest from the index of valid secret words 
      checkFile >> tempString; // put the word if intrest into tempstring 
      secret.push_back(tempString); // add the word of interest to a string we can work with on this file
    } // the above wile loop turns the index of valid secret words for the source file into a local vector contianing the same information called secret
    if (secret.size()) { // check to see if secret was created
        secretWord.index = rand() % secret.size(); // choose a random word from secret and keep its position in the vector as the index of our secret word
        string dummyString = secret.at(secretWord.index); // pull the actual entry at index it's importaint to note that each secret word in the index of secret words also has its position in the source file after the word
        for (int i = 0; i < dummyString.size(); i++) { // itterate over the dummystring
            if (isalpha(dummyString.at(i))) { // only look for the alphebetic portion of the secret word NOT its sourcefile position
                secretWord.word.push_back(dummyString.at(i)); // store the alphabetic portion in as our word in secretWord
            }
        }
        string tempString; // a place to store just the position of the secret word in the sourcefile
        for (int i = 0; i < dummyString.size(); i++) { // iterate of the dummystring 
            if (isdigit(dummyString.at(i))) {// pull just the digits in dummyString (which represent the position of the secret word in the source file)
                tempString.push_back(i); // add them to a vector so we can turn it into an integer value later 
        }
        secretWord.position = stoi(tempString);// tkaes the string of just digits and casts it to an integer   
        isMyJobDone = true; // the index of secret words already exists
    }
    while (isMyJobDone == false) { // if the index of secretwords does not exist make it 
        vector <string> input; // the string that will hond all the valid secret words in the source doc and their positions in sourcedoc
        ifstream in_file; // open a readstream
        in_file.open(filename + ".txt"); // read the origonal source docuemnnt 

        int i = -1; // this is the how many words have we assesed counter regardless of if they were valid secrets or not -1 because we itterate before we log and we want to start at 0 for secretWord.index.
        while (in_file.good()) { // only stop reading the doc if youre reading into nonspace
            string tempString;  // string to store the word being assesed in the source doc
            in_file >> tempString; // take the string the stream is assesing and put it into tempstring 
            i++; // itterate the number of words counted 
            // the following is the logic that defines a valid secret word for the purpose of the game
            if (tempString.size() > 3) { // make sure the secret words are not just filler words which are usally short
                if ((tempString.find(".")) == -1 && (tempString.find("!")) == -1 && (tempString.find("?")) == -1 && (tempString.find(";")) == -1 && (tempString.find("-")) == -1 && (tempString.find("'")) == -1 && (tempString.find("\"")) == -1 && (tempString.find("(")) == -1 && (tempString.find(")")) == -1 && islower(tempString.at(0))) { // check that secret words are not at the end of a sentance, quote or aside; are not abreviated or possesive.
                input.push_back(tempString + to_string(i)); // if it is a valid secret word log it to input as well as its position in source file represented by i
                }
            }
        }
        in_file.close(); // close ya files doc 

        ofstream out_file; // open an writesream
        out_file.open("indexlists_" + filename); // write to a document called indexlists_sourcefile that is the index of valid secret words we checked for in line 27
        for (int i = 0; i < input.size() - 1; i ++) { // itterate over input
        string tempString = input.at(i); // pull the string from the input vector at i
        out_file << tempString + " "; // add the string to the index of valid secret words
        } 
        out_file.close(); // close ya files doc
        
        ifstream checkFile; // everything from here is the same as lines 26 - 47 and is just initalizing the mehtods of secretWord
        checkFile.open("indexlists_" + filename);
        while (checkFile.good()) {
            string tempString;
            checkFile >> tempString;
            secret.push_back(tempString);
            }
            if (secret.size()) {
            secretWord.index = rand() % secret.size();  
            string dummyString = secret.at(secretWord.index);
            for (int i = 0; i < dummyString.size(); i++) {
                if (isalpha(dummyString.at(i))) {
                    secretWord.word.push_back(dummyString.at(i));
                }
            }
            string tempString;
            for (int i = 0; i < dummyString.size(); i++) {
                if (isdigit(dummyString.at(i))) {
                    tempString.push_back(i);
                }
            }
            secretWord.position = stoi(tempString);
            isMyJobDone = true;
        }
    } 
  return secretWord; // returns secret word for whatever they need it for 
}
}
