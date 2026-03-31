# langdle - Project 2
Langdle is a game where you fight back and take AI's job, seeing how well you are able to do token prediction on literature
samples from project gutenberg.

The idea of the game is that, given some language data, you are trying to guess the next token. Much like how a language model does.
Every time you fail you are given more words you have 5 (arbitrary) attempts.
## extensions
- local AI model to compete with
    - llma
- classify and rememeber certain sections as being difficult
    - local record file that stores the average number of guesses for a certain picked word index
# systems
## file handling
- take text file input
- search for words that are at the end of sentence
- pick random sentence end word 
- load previous 100 words as problem section
## input handling
- check if correct word
- display new tokens or give victory
