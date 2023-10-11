//Collaboration between Anthony Rubick and Jeffrey Winters
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int* arr;
    int size;
} IntArray;

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;
char asUpper(char c);
char* toUpper(char* str);
int flatIndex(int row, int col);
char getChar(char** arr, int flatIndex);
IntArray searchAdjacent(char** arr, int flatIndex, char c);

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
	// This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    // Your implementation here...

    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * @brief convert a char to upper case if it is a lower case letter
 * 
 * @param c 
 * @return char 
 */
char asUpper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

/**
 * @brief convert a string to upper case
 * 
 * @param str 
 * @return char* 
 */
char* toUpper(char* str) {
    char* upper = (char*)malloc(strlen(str) * sizeof(char));
    for (int i = 0; i < strlen(str); i++) {
        *(upper + i) = asUpper(*(str + i));
    }
    return upper;
}

/**
 * @brief calculate the flat index of the 2D array of size bSize, given row and col
 * 
 * @param row 
 * @param col 
 * @return int 
 */
int flatIndex(int row, int col) {
    return row * bSize + col;
}
/**
 * @brief get the char at the flat index of the 2D array
 * 
 * @param arr 
 * @param flatIndex 
 * @return char 
 */
char getChar(char** arr, int flatIndex) {
    int row = flatIndex / bSize;
    int col = flatIndex % bSize;
    return *(*(arr + row) + col);
}
/**
 * @brief check the squares adjacent to the the square at flatIndex for char c,
 *       list of flat indices of adjacent squares that contain c
 * 
 * @param arr 
 * @param flatIndex 
 * @param c 
 * @return IntArray  
 */
IntArray searchAdjacent(char** arr, int flatIndex, char c) {
    // TODO: 
}


void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

    // first convert word to upper case for comparison
    char* upper = toUpper(word);

    // then search for the first letter of the word in the puzzle

    // for each occurence of the first letter, check if the word is in the puzzle at that point by recursively checking adjacent squares for the next letter

    // for each occurence of the first letter where the word is in the puzzle, call a function to create the path of the solution (maybe a linked list?)

    // for extra credit, find every path, not just the first one
    // issue: it's not clear how we output the paths if they overlap beyond the first letter.

    // print out the paths of valid solutions (probably with a 2 or 3d array?)

}
