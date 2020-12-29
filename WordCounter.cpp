/**
 * @author  Francis Kogge
 * @version 1.0
 * @date    12/28/2020
 */

#include "WordCounter.h"
#include <string>

using namespace std;

WordCounter::WordCounter() {
    initialize(MIN_CAPACITY);
}

WordCounter::WordCounter(int capacity) {
    initialize(getValidCapacity(capacity));
}

WordCounter::WordCounter(const WordCounter &other) {
    copy(other);
}

WordCounter &WordCounter::operator=(const WordCounter &rhs) {
    // Check if assignment is not to this instance
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

WordCounter::~WordCounter() {
    clear();
}

int WordCounter::addWord(string word) {
    int wordCount; // Word count to return
    Node *wordNode = getWordNode(word);

    // If the word does not exist in the hash table
    if (wordNode == nullptr) {
        int bucket = getBucket(word, capacity);
        wordTable[bucket] = new Node(word, NEW_WORD_COUNT, wordTable[bucket]);
        uniqueWordCount++;
        wordCount = wordTable[bucket]->wordCount;
        // Check if capacity needs to be increased
        if (getLoadFactor() > MAX_LOAD_FACTOR && capacity < MAX_CAPACITY) {
            // Resize with double capacity
            resize(capacity * 2);
        }
    } else {
        wordNode->wordCount++;
        wordCount = wordNode->wordCount;
    }
    totalWordCount++;
    return wordCount;
}

void WordCounter::removeWord(string word) {
    // Do nothing if the word isn't in the hash table
    if (getWordNode(word) == nullptr) {
        return;
    }

    int bucket = getBucket(word, capacity);
    // If the word is at the head of the bucket
    if (wordTable[bucket]->word == word) {
        Node *toDelete = wordTable[bucket];
        updateWordCountsPostRemoval(wordTable[bucket]->wordCount);
        wordTable[bucket] = wordTable[bucket]->next;
        delete toDelete;
    // If the word is not at the head of bucket but possibly within the
    // chain of words
    } else {
        Node *prev = wordTable[bucket];
        for (Node *current = wordTable[bucket]->next; current != nullptr;
             current = current->next) {
            // If the word is found
            if (current->word == word) {
                updateWordCountsPostRemoval(current->wordCount);
                // Skip over current to the node after current, since we're
                // deleting current
                prev->next = current->next;
                delete current;
                break;
            }
            prev = prev->next;
        }
    }
    // Check if capacity needs to be decreased
    if (getLoadFactor() < MIN_LOAD_FACTOR && capacity > MIN_CAPACITY) {
        // Resize with half capacity
        resize(capacity / 2);
    }
}

int WordCounter::getWordCount(string word) const {
    Node *wordNode = getWordNode(word);
    // Return the word count or 0 if not in the word table
    return wordNode == nullptr ? 0 : wordNode->wordCount;
}

double WordCounter::getLoadFactor() const {
    return (double) uniqueWordCount / capacity;
}

int WordCounter::getUniqueWordCount() const {
    return uniqueWordCount;
}

int WordCounter::getTotalWordCount() const {
    return totalWordCount;
}

bool WordCounter::empty() const {
    return totalWordCount == 0;
}

int WordCounter::getCapacity() const {
    return capacity;
}

int WordCounter::getValidCapacity(int capacity) {
    // Array of valid prime numbers
    const int primes[] = {
            MIN_CAPACITY, 13, 17, 19, 23, 29, 31, 37, 43, 53, 67, 79, 97, 107,
            131, 157, 191, 223, 269, 331, 389, 461, 557, 673, 797, 967, 1151,
            1381, 1657, 1979, 2377, 2851, 3433, 4111, 4931, 5923, 7103, 8513,
            10211, 12251, 14699, 17657, 21169, 25409, 30491, 36583, 43889,
            52667, 63199, 75853, 91009, 109211, 131059, 157259, 188707,
            226451, 271753, 326087, 391331, 469583, 563489, 676171, 811411,
            973691, 1168451, 1402123, 1682531, 2019037, 2422873, 2907419,
            3488897, 4186673, 5024009, 6028807, 7234589, 8681483, 10417769,
            12501331, 15001603, 18001909, 21602311, 25922749, 31107317,
            37328761, 44794513, 53753431, 64504081, 77404907, 92885893,
            111463049, 133755659, 160506817, 192608173, 231129781, 277355759,
            332826869, 399392243, 479270713, 575124829, 690149821, 828179753,
            MAX_CAPACITY
    };

    for (int primeNum : primes) {
        // If the given capacity is a prime number or less than the next
        // prime number in the array
        if (capacity <= primeNum) {
            return primeNum;
        }
    }
    // Case where capacity is larger than the maximum prime number
    return MAX_CAPACITY;
}

void WordCounter::initialize(int capacity) {
    this->capacity = capacity;
    totalWordCount = 0;
    uniqueWordCount = 0;
    // Initialize array of Node pointers
    wordTable = new Node*[capacity];
    for (int bucket = 0; bucket < this->capacity; bucket++) {
        // Initialize each bucket to null
        wordTable[bucket] = nullptr;
    }
}

int WordCounter::getBucket(string word, int capacity) {
    hash<string> h;
    return h(word) % capacity;
}

WordCounter::Node *WordCounter::getWordNode(string word) const {
    int bucket = getBucket(word, capacity);
    // Look through entire linked list at the hash index
    for (Node *curr = wordTable[bucket]; curr != nullptr; curr = curr->next) {
        // If the given word is found
        if (curr->word == word) {
            return curr;
        }
    }
    // Indicates the given word does not exist in the hash table
    return nullptr;
}

void WordCounter::updateWordCountsPostRemoval(int toSubtract) {
    totalWordCount -= toSubtract;
    uniqueWordCount--;
}

void WordCounter::copy(const WordCounter &other) {
    capacity = other.capacity;
    totalWordCount = other.totalWordCount;
    uniqueWordCount = other.uniqueWordCount;
    wordTable = new Node*[capacity];
    // Copy linked list in each bucket
    for (int bucket = 0; bucket < capacity; bucket++) {
        wordTable[bucket] = copyBucket(other.wordTable[bucket]);
    }
}

WordCounter::Node *WordCounter::copyBucket(WordCounter::Node *headToCopy) {
    // Anchor node to point to the head Node to eventually return
    Node anchor("", 0), *current, *tail;
    tail = &anchor;
    // Traverse the linked list to be copied
    for (current = headToCopy; current != nullptr; current = current->next) {
        tail->next = new Node(current->word, current->wordCount);
        tail = tail->next;
    }
    return anchor.next;
}

void WordCounter::resize(int newCapacity) {
    newCapacity = getValidCapacity(newCapacity);
    // Initialize the new hash table
    Node **newWordTable = new Node*[newCapacity];
    for (int bucket = 0; bucket < newCapacity; bucket++) {
        newWordTable[bucket] = nullptr;
    }
    // Iterate through original hash table
    for (int bucket = 0; bucket < capacity; bucket++) {
        Node *originalNode = wordTable[bucket];
        // Iterate through original table's linked list
        while (originalNode != nullptr) {
            string word = originalNode->word;
            // Rehash word using the new capacity
            int newBucket = getBucket(word, newCapacity);
            newWordTable[newBucket] = new Node(word, originalNode->wordCount,
                                               newWordTable[newBucket]);
            originalNode = originalNode->next;
        }
    }
    // Delete old hash table contents after copying data to new hash table
    clear();
    // Update capacity value and wordTable pointer
    capacity = newCapacity;
    wordTable = newWordTable;
}

void WordCounter::clear() {
    for (int bucket = 0; bucket < capacity; bucket++) {
        // Delete each node in the bucket
        while (wordTable[bucket] != nullptr) {
            Node *toDelete = wordTable[bucket];
            wordTable[bucket] = wordTable[bucket]->next;
            delete toDelete;
        }
    }
    delete[] wordTable;
    wordTable = nullptr;
}




