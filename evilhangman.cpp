//============================================================================
// Name        : Evil Hangman
// Author      : Dilnur Yuldashev
// Version     :
// Copyright   : All rights reserved.
// Description : Final Project for CMPSC 121(Intro to Programming in C++)
// The Pennsylvania State University, Spring 2012
// The beauty of this evil hangman project is that it does not commit to a certain word.
// Take a look at the .pdf file in the directory if you want more information.
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//maximum word length
const int SIZE=30;
using namespace std;
//displays a vector of any class:string, int, char, and etc.
template <class T>

void displayVector(const vector<T> &a){
	for(int i=0;i<a.size();i++){
		cout<<a[i]<<" ";
	}
}

//returns if two words have the same pattern in terms of c
bool haveTheSamePattern(string a, string b, char c);

//copies the values of a vector to an empty vector
void copyVectorToEmptyVector(vector<string> &a, vector<string> &b);

//function for dividing words into families
//for finding the largest word family
//and changing the contents of a vector, leaving only words belonging
//to the largest family
//also, takes in a string that is used for printing out the hidden word
// during a game
void countPattern(vector<string> &words, char c, string &toPrint);

//returns toPrint string value
//changing it by comparing to largestWordFamilyPattern string
string toOut(string largestWordFamilyPattern, char a, string toPrint);

//checking a winning state
bool hasWon(string toPrint);

//computes patterns of strings
//_-for the letters other than c in word string
//c-for chars==c in word string
string computePattern(string word, char c);

//a component of merge sort function
//merges the elements of vector a
void merge(vector<string> &a,int p,int q,int r);

//sorts the elements of vector a
//Running time=O(n*log(n)) in any case
void msort(vector<string> &a,int p,int r );

//determines the most frequent pattern of a sorted patterns array
string determineMostFrequentPattern(vector<string> &patterns);

//checks whether a guess is accpetable
//guess-must be one character long and must be a letter
//must not have guessed before
bool isGuessAcceptable(vector<char>guessedLetters, string guess);

int main() {
	ifstream in;
	//the name of a dictionary file
    //our file's name is dictionary.txt
	string fileName;
    //desirable length of a word player chooses
    int wordLength;
    //for counting total words in a dictionary
    int totalWords=0;
    //# of guesses player wants to have
    int numberOfGuesses;
    //whether player wants to know the running total of remaining words
    bool wantToKnowRemainingWords;
    //returns whether a player wants to quit playing
    bool wantToQuitGame=false;
    //for controlling the two while loops
    char yesOrNo;
    //for storing all the words in dictionary.txt
    vector<string>wordsOfDifferentLengths[SIZE];
    //for reading in words one by one
    string word;
	cout<<"What is the name of the dictionary file?"<<endl;
	cin>>fileName;
	in.open(fileName.c_str());
    if(in.is_open()){
		while(in>>word){
			totalWords++;
			wordsOfDifferentLengths[word.length()-1].push_back(word);
		}
        in.close();
    }
    
	//user prompting part
	cout<<"Total number of words: "<<totalWords<<endl;
	vector<string> gameUse;
	while(wantToQuitGame==false){
	
    cout<<"Enter the word length:"<<endl;
	//checking if the entered word length is valid
	cin>>wordLength;
	//prompting the player to enter a valid wordLength
	//till he enters a valid wordLength
	
    while(!(wordLength<30 && wordLength>0 && wordsOfDifferentLengths[wordLength-1].size()!=0)){
		cout<<"The word length is not valid. Try again."<<endl;
		cin>>wordLength;
	}
	//fill the vector "gameUse" with the words of length=wordLength from the dictionary
	copyVectorToEmptyVector(wordsOfDifferentLengths[wordLength-1], gameUse);
	
    cout<<"Enter the number of guesses you want to have:"<<endl;
	cin>>numberOfGuesses;
	
    cout<<"Do you want to know the running total of remaining words?(Y(y)/N(n)"<<endl;
	cin>>yesOrNo;
	wantToKnowRemainingWords=(yesOrNo=='y');
	// Evil- Hangman part
	//returns if the game is in process
	bool evilHangmanGoingOn=true;
	//a char to store the values of guessed letters
	char guess;
	//for outputting the hidden word, with guessed letters revealed to a player
	string toPrint;
	//initial value-no letters guessed
	for(int i=0;i<wordLength;i++)
		toPrint+="_";
	//vector to store guessed letters
	vector<char> guessedLetters;
	while(evilHangmanGoingOn==true){
		string ifValidToGuess;
		cout<<"What letter?"<<endl;
		cin>>ifValidToGuess;
		//prompting the player to enter a valid letter
		//again and again till he enters a valid one
		while(isGuessAcceptable(guessedLetters,ifValidToGuess)==false){
			cout<<"Not a valid guess.Try again."<<endl;
			cin>>ifValidToGuess;
		}
		guess=ifValidToGuess[0];
		//to test if any letter was guessed after
		//the last guess
		string toTestIfLetterWasGuessed=toPrint;
		countPattern(gameUse, guess, toPrint);
		//only when guessed incorrectly
		if(toTestIfLetterWasGuessed==toPrint)
		numberOfGuesses--;
		guessedLetters.push_back(guess);
		//print out if the letter is in the word
		cout<<"Number of guesses remaining: "<<numberOfGuesses<<endl;
		//countPattern(gameUse, guess, toPrint);
		if(wantToKnowRemainingWords==true)
		cout<<"Number of words remaining:"<<gameUse.size()<<endl;
		cout<<"Letters that have been guessed: ";
		displayVector(guessedLetters);
		cout<<endl;
		cout<<toPrint<<endl;
		//checking winning and losing states
		if(hasWon(toPrint)==true){
			cout<<"Congratulations!!!"<<endl;
			cout<<"You guessed the word: "<<toPrint<<endl;
			cout<<"Do you want to play again?(Y(y)/N(n)"<<endl;
			cin>>yesOrNo;
			evilHangmanGoingOn=false;
			wantToQuitGame=(yesOrNo=='n');
		}
		else if(numberOfGuesses==0){
			cout<<"You lost!"<<endl;
			//in here, choose a random word from your list
			msort(gameUse, 0, gameUse.size()-1);
			//returning the smallest string in gameUse vector
			cout<<"The hidden word was: "<<gameUse[0]<<endl;
			cout<<"Do you want to play again?(Y(y)/N(n)"<<endl;
			cin>>yesOrNo;
			evilHangmanGoingOn=false;
			wantToQuitGame=(yesOrNo=='n');
		}
	}
	}
	return 0;
}

bool isGuessAcceptable(vector<char>guessedLetters, string guess){
	//checking if guess is one letter char
	bool wasNotGuessedBefore=true;
	if(guess.length()!=1)
		//if guess is not a letter
		return false;
	else if(guess.length()==1 && guess[0]>='a' && guess[0]<='z'){
		//if it was already guessed
		for(int i=0;i<guessedLetters.size();i++){
			if(guessedLetters[i]==guess[0])
				wasNotGuessedBefore=false;
		}
		return wasNotGuessedBefore;
	}
	else return false;
}

bool hasWon(string toPrint){
	//if all the letters are revealed
	for(int i=0;i<toPrint.length();i++){
		if(toPrint[i]=='_')
			return false;
	}
	return true;
}

string toOut(string word, char a, string c){
	//changing '_' chars to c chars if if they are
	//present in a word
	for(int i=0;i<word.length();i++)
		if(word[i]==a)
			c[i]=a;
	return c;
}

void countPattern(vector<string> &words, char c,  string &toPrint){
	string mostFrequentPattern;
	vector<string>patterns;
	for(int i=0;i<words.size();i++)
		//computing all the possible patterns of vector words
		patterns.push_back(computePattern(words[i], c));
	//sorting using merge sort
	msort(patterns, 0,patterns.size()-1);
	vector<string>copy;
	mostFrequentPattern=determineMostFrequentPattern(patterns);
	//putting all the words that match mostFrequentPattern into copy vector
	for(int i=0;i<words.size();i++){
		if(haveTheSamePattern(mostFrequentPattern, words[i], c)==true){
			copy.push_back(words[i]);
		}
	}
	//then swapping words and copy vectors
	words.swap(copy);
	copy.clear();
	//toPrint is called by reference in the function
	//so we must change it in order to inform players
	//of their progress in guessing the hidden word
	toPrint=toOut(mostFrequentPattern, c, toPrint);
}

void copyVectorToEmptyVector(vector<string> &a, vector<string> &b){
	for(int i=0; i<a.size();i++){
		b.push_back(a[i]);
	}
}

string computePattern(string word, char c){
	string toReturn;
	for(int i=0;i<word.length();i++){
		if(word[i]==c)
			toReturn+=c;
		else
			toReturn+='_';
	}
	return toReturn;
}

bool haveTheSamePattern(string a, string b, char c){
	//they are of the same length
	for(int i=0;i<a.length();i++){
		//if even one of their elements are unequal
		if((a[i]==c && b[i]!=c) || (b[i]==c && a[i]!=c))
			return false;
	}
	return true;
}

void merge(vector<string> &a,int p,int q,int r){
	int n1=q-p+1;
	int n2=r-q;
	vector <string> L;
	vector <string> R;
	for(int i=0;i<n1;i++)
		L.push_back(a[p+i]);
	for(int j=0;j<n2;j++)
		R.push_back(a[q+j+1]);
	string max="zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
	L.push_back(max);
	R.push_back(max);
	int s=0; int u=0;
    for(int k=p;k<=r;k++){
    	if(L[s]<=R[u]){
    		a[k]=L[s];
    		s++;
    	}
    	else{
    		a[k]=R[u];
    		u++;
    	}
    }
    L.clear();
    R.clear();
}

void msort(vector<string> &a,int p,int r ){
	if(p<r){
		int q=(p+r)/2;
		msort(a,p, q);
		msort(a,q+1,r);
		merge(a,p,q,r);
	}
}

string determineMostFrequentPattern(vector<string> &patterns){
	int numberOfRepetitions=1;
	int maxNumberOfRepetitions=-1;
	string mostFrequentPattern;
	for(int i=0;i<patterns.size();i++){
		if(i==patterns.size()-1){
			//end of the vector
			if(maxNumberOfRepetitions<numberOfRepetitions){
				maxNumberOfRepetitions=numberOfRepetitions;
				mostFrequentPattern=patterns[i];
			}
		}
		else if(patterns[i]!=patterns[i+1]){
			//if two unequal consecutive strings are found
			if(maxNumberOfRepetitions<numberOfRepetitions){
				maxNumberOfRepetitions=numberOfRepetitions;
				mostFrequentPattern=patterns[i];
			}
			numberOfRepetitions=1;
		}
		else{
			//if consecutive strings are equal
			numberOfRepetitions++;
		}
	}
	return mostFrequentPattern;
}