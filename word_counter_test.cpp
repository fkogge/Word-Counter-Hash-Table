/**
 * This program reads through a text file and adds each word into a
 * WordCounter object, which keeps track of a set of unique words.
 *
 * @author  Francis Kogge
 * @version 1.0
 * @date    12/28/2020
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "WordCounter.h"
#include "English.h"

using namespace std;

/**
 * Helper method to return a double rounded to three decimal places.
 *
 * @param n Double to round
 * @return  Double passed in as argument, rounded to 3 decimal places
 */
double roundToThree(double n) {
    return (int) (n * 1000.0) / 1000.0;
}

/**
 * Tests functionality of the copy constructor and overloaded assignment
 * operator of the WordCounter class. If any test fails, an approriate
 * message will be printed to the console. Nothing will print to the console
 * if all tests pass.
 *
 * @param wordCounter WordCounter object
 * @param copy        Copy of the WordCounter object to test against
 * @param wordsAdded  Vector of words added to the WordCounter object
 * @param type        Method in which copying was done
 */
void testCopy(const WordCounter &wordCounter, const WordCounter &copy,
              const vector<string> &wordsAdded, const string &type) {
    if (wordCounter.getUniqueWordCount() != copy.getUniqueWordCount())
        cout << type << " failed: mismatching unique word count." << endl;

    if (wordCounter.getTotalWordCount() != copy.getTotalWordCount())
        cout << type << " failed: mismatching total word count." << endl;

    if (wordCounter.getLoadFactor() != copy.getLoadFactor())
        cout << type << " failed: mismatching load factor." << endl;

    for (int i = 0; i < wordsAdded.size(); i++) {
        string checkWord = wordsAdded[i];
        if (wordCounter.getWordCount(checkWord) !=
            copy.getWordCount(checkWord)) {
            cout << type << " failed: mismatching word count for \""
                 << checkWord << "\"." << endl;
        }
    }

}

/**
 * Trims the line of text of leading spaces. Returns the updated string.
 *
 * @param line Line of text
 * @return     Updated line of text without leading spaces
 */
string trimLeadingSpaces(string line) {
    while (line.find(' ') == 0) {
        line.erase(0, 1);
    }
    return line;
}

/**
 * Returns the first word from the given line of text, using a space as the
 * delimiter between words. Removes that word from the line of text (note
 * that line is passed by reference).
 *
 * @param line Line of text
 * @return     First word from the line of text
 */
string getWordFromLine(string &line) {
    const char space = ' '; // Delimiter
    string word;
    line = trimLeadingSpaces(line);
    if (!line.empty()) {
        // Retrieve the first word
        word = line.substr(0, line.find(space));
        if (line.find(space) != string::npos) {
            // Update line to the substring starting from the next space
            // until the end of the line
            line = line.substr(line.find(space), line.length());
        } else {
            line.erase();
        }
    }
    return word;
}

/**
 * Displays counts of each word in the given WordCounter object, from the given
 * set of words to analyze.
 *
 * @param wordsToAnalyze Line of words to analyze
 * @param wordCounter    WordCounter object
 */
void displayWordCounts(string wordsToAnalyze, WordCounter &wordCounter) {
    string word;
    cout << "Analysis of words:" << endl;
    while (wordsToAnalyze.length() != 0) {
        word = getWordFromLine(wordsToAnalyze);
        cout << "        " <<  word << ": " << wordCounter.getWordCount(word)
             << endl;
    }
}

/**
 * Removes common words listed in English::commonWords vector from the
 * WordCounter object.
 *
 * @param wordCounter WordCounter object
 */
void removeCommonWords(WordCounter &wordCounter) {
    vector<string> commonWords = English::commonWords();
    for (int i = 0; i < commonWords.size(); i++) {
        wordCounter.removeWord(commonWords[i]);
    }
}

/**
 * Displays given WordCounter object's unique and total word counts, and load
 * factor.
 *
 * @param wordCounter WordCounter object
 */
void displayStatistics(const WordCounter &wordCounter) {
    cout << "\nWord counter statistics:" << endl;
    cout << "        Capacity: " << wordCounter.getCapacity() << endl;
    cout << "        Unique  : " << wordCounter.getUniqueWordCount() << endl;
    cout << "        Total   : " << wordCounter.getTotalWordCount() << endl;
    cout << "        Load    : " << roundToThree(wordCounter.getLoadFactor())
         << endl;
}

/**
 * Appends the first word from the next line of the input file, to the word
 * argument, then returns the updated word.
 *
 * @param word      Word to append to
 * @param line      Line of text (passed by reference so calling method can
 *                  get the new line of text)
 * @param inputFile Input file of text
 * @return          Original word plus with the next word appended to it
 */
string appendNextWord(string word, string &line, ifstream &inputFile) {
    word.erase(word.length() - 1, 1);
    // This updates line so that loop can continue
    getline(inputFile, line);
    // Append first word from the line
    string toAppend = getWordFromLine(line);
    word.append(toAppend);
    return word;
}

/**
 * Reads words from a file and adds each word to the WordCounter object. Each
 * word is cleaned via the English class, prior to being added to WordCounter.
 *
 * @param fileName    Name of the file
 * @param wordCounter WordCounter object
 * @param wordsAdded  Vector of words added to WordCounter object
 */
void addWordsFromFile(const string &fileName, WordCounter &wordCounter,
                      vector<string> &wordsAdded) {
    ifstream inputFile(fileName);
    if (inputFile) {
        string line; // Holds line of text from file
        // Retrieves each line from the file
        while (getline(inputFile, line)) {
            string word;
            while (line.length() != 0) {
                word = getWordFromLine(line);
                word = English::cleanWord(word);
                // If word is not an empty string
                if (!word.empty()) {
                    // If the word ends in a hyphen
                    if (word.find('-') == word.length() - 1) {
                        // If line is empty, we've processed the entire line
                        // meaning the first word from the next line needs to
                        // be appended
                        if (line.empty()) {
                            word = appendNextWord(word, line, inputFile);
                            // If line isn't empty yet, the hyphen can be removed
                        } else {
                            word.erase(word.length() - 1, 1);
                        }

                        word = English::cleanWord(word);
                    }
                    int oldUnique = wordCounter.getUniqueWordCount();
                    // Add word to WordCounter and word tracking vector
                    wordCounter.addWord(word);
                    if (wordCounter.getUniqueWordCount() > oldUnique) {
                        wordsAdded.push_back(word);
                    }
                }
            }
        }
    } else {
        cout << "Error: unable to read file." << endl;
    }
    inputFile.close();
}

/**
 * Returns a set of words to analyze from the user.
 *
 * @return Words to analyze
 */
string getWordsToAnalyze() {
    string wordsToAnalyze;
    cout << "\nEnter words (separated by a space): ";
    getline(cin, wordsToAnalyze);
    return wordsToAnalyze;
}

/**
 * Returns table capacity retrieved from the user. Optional method if user
 * wants to specify the size of the table.
 *
 * @return Capacity
 */
int getCapacity() {
    int capacity;
    cout << "What is the capacity? ";
    cin >> capacity;
    cin.ignore();
    return capacity;
}

/**
 * Returns file name retrieved from the user.
 *
 * @return File name
 */
string getFileName() {
    string fileName;
    cout << "What is the filename? ";
    getline(cin, fileName);
    return fileName;
}

/**
 * Tests functionality of the WordCounter class. Words are added from the
 * user-provided file to the WordCounter and the user provides words to
 * analyze against the what is stored in the WordCounter object. Various
 * methods are tested as well.
 *
 * @return EXIT_SUCCESS Indicates successful program
 */
int main() {
    vector<string> wordsAdded; // To keep track of words (for testing purposes)

    // Retrieve file name from user
    string fileName = getFileName();
    WordCounter wordCounter;

    // Read through file and update word counter
    addWordsFromFile(fileName, wordCounter, wordsAdded);
    removeCommonWords(wordCounter);
    displayStatistics(wordCounter);

    // Retrieve set of words to analyze from the user
    string wordsToAnalyze = getWordsToAnalyze();
    displayWordCounts(wordsToAnalyze, wordCounter);

    // Nothing should print to console if implemented correctly
    WordCounter copyConstructor(wordCounter);
    testCopy(wordCounter, copyConstructor, wordsAdded, "Copy constructor");
    WordCounter assignOperator = wordCounter;
    testCopy(wordCounter, assignOperator, wordsAdded, "Assignment operator");

    return EXIT_SUCCESS;
}


