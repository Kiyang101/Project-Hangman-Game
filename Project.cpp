#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <random>
#include <cstdlib>

using namespace std;

void clearScreen(){
    cout << "\033[2J\033[H";
}

class Word{
    private:
        vector<string> words;
        string secretWord;

    public:
        void putWord(string word){
            words.push_back(word);
        }

        vector<string> getWords(void){
            return words;
        }

        string getSecretWord(void){
            return secretWord;
        }

        void getRandomWord(void);
        
};

void Word::getRandomWord(void) {
    if (words.empty()){
        secretWord = "";
    }

    random_device rd;
    mt19937 generator(rd());

    uniform_int_distribution<int> distribution(0, words.size() - 1);
    int random_index = distribution(generator);

    secretWord = words[random_index];
}

class Name{
    public:
        string name;
};

class Player: public Name{
    public:
        int lives;
};

class HangmanGame : public Player{
    private:
        vector<char> guessedWord;
        string secretWord;
        vector<string> stickman;

    public:
        HangmanGame(string w){
            secretWord = w;
            lives = 6;
            int wordLenght = secretWord.length();
            guessedWord = vector<char>(wordLenght, '_');
        };
        
        HangmanGame(string w, int l){
            secretWord = w;
            lives = l;
            int wordLenght = secretWord.length();
            guessedWord = vector<char>(wordLenght, '_');
        };

        void putName(string n){
            if (n != ""){
                name = n;
            }else{
                name = "Anonymous";
            }
        }


        bool isGameOver(){
            return lives == 0 || secretWord == string(guessedWord.begin(), guessedWord.end());
        }

        void guessLetter(char letter){
            letter = tolower(letter);
            bool found = false;
            
            for (int i = 0; i < secretWord.size(); ++i){
                if (secretWord[i] == letter){
                    guessedWord[i] = letter;
                    found = true;
                }
            }

            if (!found){
                --lives;
            }
        }

        void putState(vector<string> stages){
            stickman = stages;
        }

        void displayStickman(void){
            cout << stickman[lives*2] << endl;
        }

        void play();
};

void HangmanGame::play(){
    while (!isGameOver()){
        clearScreen();
        // cout << "secretWord = " << secretWord << endl;
        cout << "Name: " << name << endl;
        cout << "\nlives: " << lives << endl;

        displayStickman();

        cout << endl;
        for (char c : guessedWord){
            cout << c << " ";
        }

        cout << "\n\nGuess a letter: ";

        char guess;
        cin >> guess;

        if (!isalpha(guess)){
            cout << "Invalid input. Please enter a letter." << endl;
            continue;
        }

        guessLetter(guess);
    }

    clearScreen();
    cout << "\nYour lives: " << lives << endl;
    displayStickman();
    cout << endl;
    for (char c : guessedWord){
        cout << c << " ";
    }

    if (secretWord == string(guessedWord.begin(), guessedWord.end())){
        cout << "\n\nCongratulations "<< name << " Win!!\n\n";
        cout << "The correct word is '" << secretWord << "'" << endl;
    }
    else{
        cout << "\n\nYou lose\nThe correct word is '" << secretWord << "'\n\n";
    }
}

int main(){
    clearScreen();
    Word word;

    ifstream secretWord("./secretWord.txt");
    ifstream Stickman("./Stickman.txt");

    if (!secretWord.is_open() || !Stickman.is_open()){
        cout << "Error opening file." << endl;
        return 1;
    };

    vector<string> stages;
    string stage;
    
    while (getline(Stickman, stage, '\'')) {
        if (!stage.empty()) {
            stages.push_back(stage);
        };
    };

    string w;
    while (getline(secretWord, w)){
        transform(w.begin(), w.end(), w.begin(), ::tolower);
        word.putWord(w);
    };

    word.getRandomWord();   
    string config;
    int lives;
    cout << "Your default lives is 6 and it's can't set more 6" << endl;
    cout << "You want to config your lives?(y/n): ";
    
    cin >> config;

    if (config[0] == 'y' || config[0] == 'Y') {
        cout << "Enter the number of lives: ";
        cin >> lives;
        if (lives > 6) {
            lives = 6;
        }
    }else{
        lives = 6;
    }

    HangmanGame game(word.getSecretWord(), lives);

    string name;
    cout << "Input your name : ";
    cin >> name;

    game.putName(name);

    game.putState(stages);
    game.play();

    return 0;
}