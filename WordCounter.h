#pragma once

#include <string>
#include <vector>

/**
 * Hash table of words (std::string), implemented via an array of linked
 * lists. Each word that is added to the hash table is unique, and each word
 * has a count associated, that is, the number of times add has been called
 * on that word. Separate chaining is utilized in the event of collisions,
 * where if a word hashes to the same bucket as a different word, that word
 * will be added to the front of the linked list in that bucket. Hash table
 * automatically resizes when the load factor exceeds or drops below a certain
 * threshold to ensure optimal performance.
 *
 * @author  Francis Kogge
 * @version 1.0
 * @date    12/28/2020
 */
class WordCounter {
public:
    /**
     * Default constructor - initializes the hash table with the default
     * capacity.
     */
    WordCounter();

    /**
     * Constructor - initializes the hash table with the given capacity.
     *
     * @param capacity Capacity of the hash table
     */
    WordCounter(int capacity);

    /**
     * Copy constructor.
     *
     * @param other WordCounter object to copy
     */
    WordCounter(const WordCounter &other);

    /**
     * Overloaded assignment operator.
     *
     * @param rhs WordCounter object to copy (right-hand side of operator)
     * @return    this WordCounter object
     */
    WordCounter &operator=(const WordCounter &rhs);

    /**
     * Destructor - deallocates memory associated with hash table (including
     * Node objects created).
     */
    ~WordCounter();

    /**
     * Adds a word to the hash table, if the word does not already exist in
     * the hash table. If it does exist, then the count associated with that
     * word is incremented by 1. Returns the  number of times the given word has
     * appeared in the hash table.
     *
     * Note: different words may hash to the same bucket in the hash table,
     * in which case the word will just be added to the front of word chain,
     * and will have its own count associated with it.
     *
     * @param word Word to add to the hash table
     * @return     Number of times the word has been added to the table
     */
    int addWord(std::string word);

    /**
     * Removes the given word (the Node object associated with the word and all
     * its data) from the hash table.
     *
     * @param word Word to remove
     */
    void removeWord(std::string word);

    /**
     * Returns the count of the given word in the hash table.
     *
     * @param word Word to get count of
     * @return     Count of the given word, or 0 if the word doesn't exist in
     *             the hash table
     */
    int getWordCount(std::string word) const;

    /**
     * Returns the current load factor of the hash table.
     *
     * @return Load factor
     */
    double getLoadFactor() const;

    /**
     * Returns the number of unique words added to the hash table.
     *
     * @return Count of unique words
     */
    int getUniqueWordCount() const;

    /**
     * Returns the total number of words encountered, including duplicates
     * (either added to the hash table or attempted to be added to the table).
     *
     * @return Count of total words encountered
     */
    int getTotalWordCount() const;

    /**
     * Returns whether or not hash table is empty.
     *
     * @return True if no elements are present in the hash table
     *         False if an element is present in the hash table
     */
    bool empty() const;

    /**
     * Returns current capacity of the hash table.
     *
     * @return Current capacity
     */
    int getCapacity() const;

private:
    static const int MIN_CAPACITY = 11; // Minimum default capacity
    static const int MAX_CAPACITY = 993815743; // Maximum allowed capacity
    static const int NEW_WORD_COUNT = 1; // Initial count when new word is added
    const double MAX_LOAD_FACTOR = 0.750, MIN_LOAD_FACTOR = 0.30;

    /*
     * Node object represents a word that is added to the hash table
     */
    struct Node {
        std::string word; // Word to add
        Node *next; // Next Node in the table
        int wordCount; // Number of times the given word has been added

        /**
         * Convenience constructor
         *
         * @param word Word to add
         */
        Node(std::string word, int wordCount, Node *next = nullptr) {
            this->word = word;
            this->next = next;
            this->wordCount = wordCount;
        }

    };

    int capacity; // Capacity of hash table
    int totalWordCount; // Total number of words added
    int uniqueWordCount; // Number of unique words added
    Node **wordTable; // Hash table (array of Node pointers) of linked Node
                      // objects containing data associated with each word
                      // the Node represents

    /**
     * Returns a valid, prime-number capacity to initialize the hash table
     * capacity.
     *
     * @param capacity Capacity to convert into a valid, prime-number capacity
     * @return         Valid, prime-number capacity
     */
    static int getValidCapacity(int capacity);

    /**
     * Helper method for initializing the hash table and other data members.
     *
     * @param capacigty Capacity of the table
     */
    void initialize(int capacigty);

    /**
     * Returns bucket index for the given word via std::hash.
     *
     * @param word     Word to hash
     * @param capacity Capacity of the table
     * @return         Bucket index
     */
    static int getBucket(std::string word, int capacity);

    /**
     * Returns the Node object which contains the given word, or null if no
     * Node exists in the hash table associated with that word. This method
     * can be called to check for the existence of a word in the hash table.
     *
     * @param word Word to look up in hash table
     * @return     Node object with the given word, or nullptr if the word does
     *             not exist in the hash table
     */
    Node *getWordNode(std::string word) const;

    /**
     * Updates the total and unique word counts after removing an entry from
     * the hash table.
     *
     * @param toSubtract Number to subtract from the total word count
     */
    void updateWordCountsPostRemoval(int toSubtract);

    /**
     * Helper method for copying hash table and other data members from the
     * other WordCounter object to this WordCounter.
     *
     * @param other WordCounter object to copy
     */
    void copy(const WordCounter &other);

    /**
     * Helper method for copying an entire bucket (the linked list in the
     * given bucket). Returns the head Node of that bucket (which is chained
     * to all other Node objects in the bucket).
     *
     * @param headToCopy Bucket head to be copied
     * @return           Copy of the head Node passed in as an argument,
     *                   chained to all other nodes in the bucket
     */
    static Node *copyBucket(Node *headToCopy);

    /**
     * Resizes the hash table to fit the new given capacity. If newCapacity is
     * less than the current capacity, the hash table will be shrunk to the
     * new capacity. If newCapacity is larger than the current capacity, it
     * will be expanded to the new capacity.
     *
     * @param newCapacity New capacity to resize hash table to
     */
    void resize(int newCapacity);

    /**
     * Helper method for deleting each Node in each bucket in the hash table.
     */
    void clear();
};

