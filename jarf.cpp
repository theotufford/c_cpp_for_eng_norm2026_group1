#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){ 
    int jart;
    int I = 0; 
    srand(time(NULL));
    int secret = rand() % 10;
    cout << "what number am i thinking of\n";
    cout << ">";
    cin >> jart;
    
    if (jart == secret){
        cout << "haha, yup";
    } while( jart != secret) {
        cout << "no try again\n" << ">";
        cin >> jart;
        I += 1;
        if(I >= 3) {
        cout << "\nchud cough* cough*\n";
        I = 0;  
        }
    } cout << "haha, yup";

return 0; 
}