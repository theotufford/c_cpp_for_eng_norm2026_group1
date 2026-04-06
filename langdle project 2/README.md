# langdle - Project 2
## initial brainstorm
Langdle is a game where you fight back and take AI's job, seeing how well you are able to do token prediction on literature
samples from project gutenberg.

The idea of the game is that, given some language data, you are trying to guess the next token. Much like how a language model does.
Every time you fail you are given more words you have 5 (arbitrary) attempts.

### extensions
- local AI model to compete with
    - ran out of time on this :(
    - https://github.com/ggml-org/llama.cpp
## systems
### file handling
- take text file input
- search for words that are at the end of sentence
- pick random sentence end word 
- load previous 100 words as problem section
### input handling
- check if correct word
- display new tokens or give victory

Documentation for filesystem used in the main():
https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
https://en.cppreference.com/w/cpp/filesystem.html
https://www.geeksforgeeks.org/cpp/file-system-library-in-cpp-17/


# final notes
note that you may add any text file to the "sources" folder and it will be able to generate a valid word list. 
the rules for a guess are that it must end in punctuation, not be capitalized, and be longer than 3 letters.
