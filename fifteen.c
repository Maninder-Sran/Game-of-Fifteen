/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(int size);
void draw(int size);
bool move(int tile, int size);
void shiftUp(int row, int col);
void shiftDown(int row, int col);
void shiftLeft(int row, int col);
void shiftRight(int row, int col);
bool won(int size);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    int size = atoi(argv[1]);
    init(size);

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw(size);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(size))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile, size))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(int size)
{
    int num = (size * size) - 1;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            board[i][j] = num;
            num--;
        }
    }
    if (size % 2 == 0)
    {
        int temp = board[size - 1][size - 2];
        board[size - 1][size - 2] = board[size - 1][size - 3];
        board[size - 1][size - 3] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if (board[i][j] == 0)
            {
                printf("_\t");
            }
            else
            {  
                printf("%i\t",board[i][j]);
            }
            
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile, int size)
{
    int row = 0;
    int col = 0;
    int options = 0;
    int corner = 0;
    char side = 'N';
    bool chosen = false;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if (board[i][j] == tile)
            {
                row = i;
                col = j;
            }
        }
    }

    if (row == 0 && col == 0)
    {
        options = 2;
        corner = 1; 
        chosen = true;
    }
    else if (row == 0 && col == size - 1)
    {
        options = 2;
        corner = 2;
        chosen = true;
    }
    else if (row == size - 1 && col == 0)
    {
        options = 2;
        corner = 3;
        chosen = true;
    }
    else if (row == size - 1 && col == size - 1)
    {
        options = 2;
        corner = 4;
        chosen = true;
    }
    if (!chosen)
    {
        if ((col == 0 || col == size - 1) && (row != 0 || row != size - 1))
        {
            options = 3;
            if (col == 0)
            {
                side = 'L';
            }
            else
            {
                side = 'R';
            }
        }
        else if ((row == 0 || row == size - 1) && (col != 0 || col != size - 1))
        {
            options = 3;
            if (row == 0)
            {
                side = 'U';
            }
            else
            {
                side = 'D';
            }
        }
        else
        {
            options = 4;
        }
    }
    switch (options)
    {
        case 2:
            if (corner == 1)
            {
                if (board[row][col + 1] == 0)
                {
                    shiftRight(row,col);
                }
                else if (board[row + 1][col] == 0)
                {
                    shiftDown(row,col);
                }
                else
                {
                    return false;
                }
            }
            else if (corner == 2)
            {
                if (board[row][col - 1] == 0)
                {
                    shiftLeft(row,col);
                }
                else if (board[row + 1][col] == 0)
                {
                    shiftDown(row,col);
                }
                else
                {
                    return false;
                }
            }
            else if (corner == 3)
            {
                if (board[row][col + 1] == 0)
                {
                    shiftRight(row,col);
                }
                else if (board[row - 1][col] == 0)
                {
                    shiftUp(row,col);
                }
                else
                {
                    return false;
                }
            }
            else if (corner == 4)
            {
                if (board[row][col - 1] == 0)
                {
                    shiftLeft(row,col);
                }
                else if (board[row - 1][col] == 0)
                {
                    shiftUp(row,col);
                }
                else
                {
                    return false;
                }
            }
            break;
        case 3:
            if (side == 'U')
            {
                if (board[row][col + 1] == 0)
                {
                    shiftRight(row,col);
                }
                else if (board[row][col - 1] == 0)
                {
                    shiftLeft(row,col);
                }
                else if (board[row + 1][col] == 0)
                {
                    shiftDown(row,col);
                }
                else
                {
                    return false;
                }
            }
            else if (side == 'D')
            {
                if (board[row][col + 1] == 0)
                {
                    shiftRight(row,col);
                }
                else if (board[row][col - 1] == 0)
                {
                    shiftLeft(row,col);
                }
                else if (board[row - 1][col] == 0)
                {
                    shiftUp(row,col);
                }
                else
                {
                    return false;
                }
            }
            else if (side == 'L')
            {
                if (board[row + 1][col] == 0)
                {
                    shiftDown(row,col);
                }
                else if (board[row - 1][col] == 0)
                {
                    shiftUp(row,col);
                }
                else if (board[row][col + 1] == 0)
                {
                    shiftRight(row,col);
                }
                else
                {
                    return false;
                }
            }
            else if (side == 'R')
            {
                if (board[row + 1][col] == 0)
                {
                    shiftDown(row,col);
                }
                else if (board[row - 1][col] == 0)
                {
                    shiftUp(row,col);
                }
                else if (board[row][col - 1] == 0)
                {
                    shiftLeft(row,col);
                }
                else
                {
                    return false;
                }
            }
            break;
        case 4:
            if (board[row][col + 1] == 0)
            {
                shiftRight(row,col);
            }
            else if (board[row][col - 1] == 0)
            {
                shiftLeft(row,col);
            }
            else if (board[row + 1][col] == 0)
            {
                shiftDown(row,col);
            }
            else if (board[row - 1][col] == 0)
            {
                shiftUp(row,col);
            }
            else
            {
                return false;
            }
            break;    
        default:
            return false;
    }
    return true;
}
void shiftUp(int row, int col)
{
    int temp = board[row][col];
    board[row][col] = board[row - 1][col];
    board[row - 1][col] = temp;
}
void shiftDown(int row, int col)
{
    int temp = board[row][col];
    board[row][col] = board[row + 1][col];
    board[row + 1][col] = temp;
}
void shiftLeft(int row, int col)
{
    int temp = board[row][col];
    board[row][col] = board[row][col - 1];
    board[row][col - 1] = temp;
}
void shiftRight(int row, int col)
{
    int temp = board[row][col];
    board[row][col] = board[row][col + 1];
    board[row][col + 1] = temp;
}
/**
 * Returns false if game is not won (board is not in winning configuration), 
 * else true.
 */
bool won(int size)
{
    int checks[size][size];
    int num = 1;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            checks[i][j] = num;
            num++;
        }
    }
    checks[size - 1][size - 1] = 0;
    
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if (board[i][j] != checks[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
