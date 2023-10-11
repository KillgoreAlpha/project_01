//Collaboration between Anthony Rubick and Jeffrey Winters
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct IntArray {
    int* arr;
    int size;
    int capacity;
} IntArray;

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** puzzle);
void searchPuzzle(char** puzzle, char* word);
int bSize;
char asUpper(char c);
char* toUpper(char* str);
int calcFlatIndex(int rowIndex, int colIndex);
int calcRow(int flatIndex);
int calcCol(int flatIndex);
char getCharFlat(char** puzzle, int flatIndex);
char getChar(char** puzzle, int row, int col);
IntArray searchAdjacent(char** puzzle, int flatIndex, char c);
bool isWordAtIndex(char** puzzle, char* word, int flatIndex);
IntArray getPath(char** puzzle, char* word, int flatIndex);

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

void printPuzzle(char** puzzle) {
	// This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    // Your implementation here...

    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%c ", *(*(puzzle + i) + j));
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
    char* upper = (char*)malloc(strlen(str)+1 * sizeof(char));
    for (int i = 0; i < strlen(str); i++) {
        *(upper + i) = asUpper(*(str + i));
    }
    *(upper + strlen(str)) = '\0';
    return upper;
}

/**
 * @brief calculate the flat index of the 2D array of size bSize, given row and col
 * 
 * @param row 
 * @param col 
 * @return int 
 */
int calcFlatIndex(int row, int col) {
    return row * bSize + col;
}

/**
 * @brief calculate the row corresponding to the given flatIndex of a 2D array of size bSize
 * 
 * @param flatIndex 
 * @return int 
 */
int calcRow(int flatIndex) {
    return flatIndex / bSize;
}

/**
 * @brief calculate the col corresponding to the given flatIndex of a 2D array of size bSize
 * 
 * @param flatIndex 
 * @return int 
 */
int calcCol(int flatIndex) {
    return flatIndex % bSize;
}

/**
 * @brief get the char at the flat index of the 2D array
 * 
 * @param puzzle 
 * @param flatIndex 
 * @return char 
 */
char getCharFlat(char** puzzle, int flatIndex) {
    return getChar(puzzle, calcRow(flatIndex), calcCol(flatIndex));
}
/**
 * @brief Get the char at the given row and col of the 2D array
 * 
 * @param puzzle 
 * @param row 
 * @param col 
 * @return char 
 */
char getChar(char** puzzle, int row, int col) {
    return *(*(puzzle + row) + col);
}
/**
 * @brief check the squares adjacent to the the square at flatIndex for char c,
 *       list of flat indices of adjacent squares that contain c
 * 
 * @param puzzle 
 * @param flatIndex 
 * @param c 
 * @return IntArray  
 */
IntArray searchAdjacent(char** puzzle, int flatIndex, char c) {
    // strategy regarding this IntArray, start with it being size 8, because that's the upper bound
    // track how many elements we actually use, and then resize it to that size at the end before returning it   
    int row = calcRow(flatIndex), col = calcCol(flatIndex);
    // if the starting square is out of bounds, return an empty array
    if (row < 0 || row >= bSize || col < 0 || col >= bSize) {
        return (IntArray) {
            .arr = (int*)malloc(0),
            .size = 0,
            .capacity = 0
        };
    }
    IntArray adj = {
        .arr = (int*)malloc(8 * sizeof(int)),
        .size = 0,
        .capacity = 8
    };

    // check the 8 adjacent squares for the char c
    // if the adjacent square is out of bounds, skip it

    // top left
    if (row - 1 >= 0 && col - 1 >= 0 && getChar(puzzle, row - 1, col - 1) == c) {
        *(adj.arr + adj.size) = calcFlatIndex(row - 1, col - 1);
        adj.size++;
    }
    // top
    if (row - 1 >= 0 && getChar(puzzle, row - 1, col) == c) {
        *(adj.arr + adj.size) = calcFlatIndex(row - 1, col);
        adj.size++;
    }
    // top right
    if (row - 1 >= 0 && col + 1 < bSize && getChar(puzzle, row - 1, col + 1) == c) {
        *(adj.arr + adj.size) = calcFlatIndex(row - 1, col + 1);
        adj.size++;
    }
    // left
    if (col - 1 >= 0 && getChar(puzzle, row, col - 1) == c) {
        *(adj.arr + adj.size) = calcFlatIndex(row, col - 1);
        adj.size++;
    }
    // right
    if (col + 1 < bSize && getChar(puzzle, row, col + 1) == c) {
        *(adj.arr + adj.size) = calcFlatIndex(row, col + 1);
        adj.size++;
    }
    // bottom left
    if (row + 1 < bSize && col - 1 >= 0 && getChar(puzzle, row + 1, col - 1) == c) {
        *(adj.arr + adj.size) = calcFlatIndex(row + 1, col - 1);
        adj.size++;
    }
    // bottom
    if (row + 1 < bSize && getChar(puzzle, row + 1, col) == c) {
        *(adj.arr + adj.size) = calcFlatIndex(row + 1, col);
        adj.size++;
    }
    // bottom right
    if (row + 1 < bSize && col + 1 < bSize && getChar(puzzle, row + 1, col + 1) == c) {
        *(adj.arr + adj.size) = calcFlatIndex(row + 1, col + 1);
        adj.size++;
    }

    // resize the array to the number of elements we actually used
    int* old = adj.arr;
    adj.arr = (int*)malloc(adj.size * sizeof(int));
    memcpy(adj.arr, old, adj.size * sizeof(int));
    free(old);
    adj.capacity = adj.size;

    return adj;
}

/**
 * @brief checks if the word is in the puzzle at the given flatIndex
 * 
 * @param puzzle 
 * @param word 
 * @param flatIndex 
 * @return true 
 * @return false 
 */
bool isWordAtIndex(char** puzzle, char* word, int flatIndex) {
    // if the first letter of the word is not at the given flatIndex, return false
    if (getCharFlat(puzzle, flatIndex) != *word) {
        return false;
    }
    // if the word is only one letter long, return true
    if (strlen(word) == 1) {
        return true;
    }
    // otherwise, check the adjacent squares for the next letter of the word
    IntArray adj = searchAdjacent(puzzle, flatIndex, *(word + 1));
    // if there are no adjacent squares with the next letter, return false
    if (adj.size == 0) {
        return false;
    }
    // otherwise, check if the word is at any of the adjacent squares
    for (int i = 0; i < adj.size; i++) {
        if (isWordAtIndex(puzzle, word + 1, *(adj.arr + i))) {
            return true;
        }
    }
    // if the word is not at any of the adjacent squares, return false
    return false;
}

/**
 * @brief create a path of the word in the puzzle at the given flatIndex
 * @precondition there exists a valid path of the word starting at the given flatIndex
 * 
 * @param puzzle 
 * @param word 
 * @param flatIndex 
 * @return IntArray 
 */
IntArray getPath(char** puzzle, char* word, int flatIndex) {
    // if the word is only one letter long, return the flatIndex
    if (strlen(word) == 1) {
        IntArray path = {
            .arr = (int*)malloc(sizeof(int)),
            .size = 1,
            .capacity = 1
        };
        *(path.arr) = flatIndex;
        return path;
    }
    // otherwise, check the adjacent squares for the next letter of the word
    IntArray adj = searchAdjacent(puzzle, flatIndex, *(word + 1));
    // if there are no adjacent squares with the next letter, return an empty array
    if (adj.size == 0) {
        return (IntArray) {
            .arr = (int*)malloc(0),
            .size = 0,
            .capacity = 0
        };
    }
    // otherwise, check if the word is at any of the adjacent squares
    for (int i = 0; i < adj.size; i++) {
        if (isWordAtIndex(puzzle, word + 1, *(adj.arr + i))) {
            // if the word is at any of the adjacent squares, prepend the flatIndex to the path of the word at that square
            // TODO: optimize this by not allocating a new array unless we need to
            IntArray path = getPath(puzzle, word + 1, *(adj.arr + i));
            int* old = path.arr;
            path.arr = (int*)malloc((path.size + 1) * sizeof(int));
            memcpy(path.arr + 1, old, path.size * sizeof(int));
            free(old);
            *(path.arr) = flatIndex;
            path.size++;
            path.capacity = path.size;
            return path;
        }
    }
    // if the word is not at any of the adjacent squares, return an empty array
    return (IntArray) {
        .arr = (int*)malloc(0),
        .size = 0,
        .capacity = 0
    };
}

void searchPuzzle(char** puzzle, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

    // first convert word to upper case for comparison
    char* upper = toUpper(word);

    // we need to save the starts of valid paths, but we don't know how many there are...
    // allocating an array of size bSize^2 seems wasteful,
    // but passing through the puzzle twice seems inefficient
    // compromise: count occurrences of the first letter of the word and allocate an array of that size, then resize later if needed
    /// count occurrences of the first letter of the word
    int countFirstLetter = 0;
    for (int i = 0; i < bSize * bSize; i++) {
        if (getCharFlat(puzzle, i) == *upper) {
            countFirstLetter++;
        }
    }
    /// search for valid paths
    IntArray starts = {
        .arr = (int*)malloc(countFirstLetter * sizeof(int)),
        .size = 0,
        .capacity = countFirstLetter
    };
    for (int i = 0; i < bSize * bSize; i++) {
        if (getCharFlat(puzzle, i) == *upper) {
            if (isWordAtIndex(puzzle, upper, i)) {
                *(starts.arr + starts.size) = i;
                starts.size++;
            }
            if (starts.size >= countFirstLetter) {break;}
        }
    }

    // if there are no valid paths, print a message and return
    if (starts.size == 0) {
        printf("Word not found!\n");
        return;
    }

    // now, we need to figure out what the valid paths are ...
    // for each valid start, get the path of the word
    IntArray* paths = (IntArray*)malloc(starts.size * sizeof(IntArray));
    int numPaths = 0;
    for (int i = 0; i < starts.size; i++) {
        IntArray path = getPath(puzzle, upper, *(starts.arr + i));
        // if the path is empty, skip it
        if (path.size == 0) {
            printf("oops\n"); // this should never happen
            continue;
        }
        // otherwise, add it to the list of paths
        *(paths + numPaths) = path;
        numPaths++;
    }

    // now, we need to render the paths in a grid, similar to how we rendered the puzzle
    // we know that the bounds of the grid are the bounds of the puzzle
    // we know that our paths don't start at the same square, but not that they don't overlap
    // so, first we need to re-interpret our paths to a 3d array (size bSize x bSize x (up to numPaths)) of "indexes" (in the path, not in the puzzle)
    // then, we can render the grid by iterating through the 3d array and printing
    
    // create our 3d array
    IntArray** pathGrid = (IntArray**)malloc(bSize * sizeof(IntArray*));
    // initialize each row and element of the grid with empty paths
    for (int i = 0; i < bSize; i++) {
        *(pathGrid + i) = (IntArray*)malloc(bSize * sizeof(IntArray));
        for (int j = 0; j < bSize; j++) {
            *(*(pathGrid + i) + j) = (IntArray) {
                .arr = (int*)malloc(0), // so we don't need to deal with actual null pointers
                .size = 0,
                .capacity = 0,
            };
        }
    }
    // for each path, add it to the grid
    for (int i = 0; i < numPaths; i++) {
        IntArray path = *(paths + i);
        for (int j = 0; j < path.size; j++) {
            int flatIndex = *(path.arr + j);
            int row = calcRow(flatIndex), col = calcCol(flatIndex);
            // resize the pathGrid[i][j] if needed, then add j+1 to it
            IntArray* temp = *(pathGrid + row) + col;
            if (temp->size == temp->capacity) {
                // if we are resizing from 0, we need increment capacity first
                if (temp->capacity == 0) temp->capacity++;
                temp->capacity *= 2;
                temp->arr = (int*)realloc(temp->arr, temp->capacity * sizeof(int));
            }
            // add j+1 to the pathGrid[i][j]
            *(temp->arr + temp->size) = j + 1;
            temp->size++;
        }
    }

    // now, we can render the grid
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            IntArray* temp = *(pathGrid + i) + j;
            // if the array at this index is empty, print 0
            if (temp->size == 0) {
                printf("0\t");
            }
            // otherwise, dump the array
            else {
                for (int k = 0; k < temp->size; k++) {
                    printf("%d", *(temp->arr + k));
                }
                printf("\t");
            }
        }
        printf("\n");
    }
}
