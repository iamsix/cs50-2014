/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// the coordinates of the empty square for optimization
int cursorpos[2];

// prototypes
void clear(void);
void greet(void);
void init(void);
void god(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);
void followmovelist(char movelist[], int moves);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {


        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        char* uinput = GetString();
        int tile = 0;
        if (uinput == 0)
        {
            printf("\nInvalid Input.\n");
            usleep(500000);
            continue;
        }
        else if (strcmp("GOD", uinput) == 0)
        {
            god();
            continue;
        }
        else if (strcmp("quit", uinput) == 0)
        {   
            printf("Goodbye.\n");
            break;
        }
        else
        {
            if (strlen(uinput) < 3 && isdigit(uinput[0]))
            {
                tile = atoi(uinput);
            }
            else
            {
                printf("\nIllegal move.\n");
                usleep(500000);
                continue;
            }
        }
        

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(50000);
    }

    // that's all folks
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
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

// TODO maybe fix these up a bit
void moveUp(void){move(board[cursorpos[0] - 1][cursorpos[1]]);}
    
void moveDown(void){move(board[cursorpos[0] + 1][cursorpos[1]]);}
    
void moveLeft(void) {move(board[cursorpos[0]][cursorpos[1] - 1]);}
    
void moveRight(void){move(board[cursorpos[0]][cursorpos[1] + 1]);}



// BREADCRUMBS!!!!!!!!!!1
void movecursor(int goalX, int goalY, int targetleft, int breadcrumbs[])
{
    int i = (d*d - 1);
    while(cursorpos[0] != goalX || cursorpos[1] != goalY)
    {
        if(cursorpos[1] < goalY)
        {
            moveRight();
            breadcrumbs[i] = 1;
        }
        else if(cursorpos[0] < goalX)
        {
            moveDown();
            breadcrumbs[i] = 2;
        }
        else if(cursorpos[1] > goalY)
        {
            moveLeft();
            breadcrumbs[i] = 3;
        }
        else if(cursorpos[0] > goalX)
        {
            moveUp();
            breadcrumbs[i] = 4;
        }
        draw();
        usleep(90000);
        i--;
        save();
        if(cursorpos[1] - 1 >= 0 && board[cursorpos[0]][cursorpos[1] - 1] == targetleft)
        {
            break;
        }
       
        
    }
    
    for(int j = i; j >= 0; j--)
    {
        breadcrumbs[j] = 0;
    }

}

// TODO Change these to not require targetpos, it's easy to do without them
void movetargetUp(int targetpos[])
{
    char movelist[5] = "ULDRU";
    followmovelist(movelist, 5);
    targetpos[0]--;
}

void movetargetDown(int targetpos[])
{
    char movelist[5] = "DLURD";
    followmovelist(movelist, 5);
    targetpos[0]++;
}

// TODO Change these to not require targetpos, it's easy to do without them
void movetargetLeft(int targetpos[])
{
    
    // Special logic for the bottom of the grid
    if(targetpos[0] != d - 1)
    {
        char movelist[5] = "DLLUR";
        followmovelist(movelist, 5);
    }
    else
    {
        char movelist[5] = "ULLDR";
        followmovelist(movelist, 5);
    }
    targetpos[1]--;
}

// TODO Change these to not require targetpos, it's easy to do without them
void movetargetRight(int targetpos[])
{
    // Special logic for the bottom of the grid      
    if(targetpos[0] != d - 1)
    {
        char movelist[5] = "LDRRU";
        followmovelist(movelist, 5);
    }
    else
    {
        char movelist[5] = "LURRD";
        followmovelist(movelist, 5);
    }
    targetpos[1]++;
}

void findtarget(int targetpos[], int target)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] == target)
            {
                targetpos[0] = i;
                targetpos[1] = j;
                break;
            }
        }
        if(targetpos[0] > -1)
        {
            break;
        }
    }
}

void followbreadcrumbs(int breadcrumbs[], int targetpos[], int crumbs)
{
    for (int i = 0; i < crumbs; i++)
    {
        switch(breadcrumbs[i])
        {
            case 0:
                break;
            case 1:
                movetargetLeft(targetpos);
                break;
            case 2:
                movetargetUp(targetpos);
                break;
            case 3:
                movetargetRight(targetpos);
                break;
            case 4:
                movetargetDown(targetpos);
                break;
        }
       
    }
}

void followmovelist(char movelist[], int moves)
{
    for (int i = 0; i < moves; i++)
    {
        switch(movelist[i])
        {
        case 'U':
            moveUp();
            break;
        case 'D':
            moveDown();
            break;
        case 'L':
            moveLeft();
            break;
        case 'R':
            moveRight();
            break;
        }
       draw();
       usleep(90000); 
       save();
        
    }
}

/**
 * Plays the game for the user.
 * Hard to do because I'm not very good at playing the game either
 */
void god(void)
{
    // We want to get the top row, left column solved repeatedly
    // the problem is how to actually get those things IN to those spots
    // especially hard is that the last 2 of any row/column will need to be solved together

    int breadcrumbs[d*d];
    
    // start at the top-left so that our breadcrumbs work :D
    movecursor(0, 0, 0, breadcrumbs);
    bool special = false;
    int solvedrows = 0, solvedcolumns = 0;
    
    // Despite popular convention I originally wrote this while tired
    // so Y = horizontal, X = vertical.
    while(true)
    {
        //this solves 1 row.
        for(int target = solvedrows*d + solvedcolumns + 1; target <= (d * solvedrows) + d; target++)
        {
            // Special case for goal Y = d-2 and d-1.
            // find our NEXT goal
            // X uses int truncation hack, but it works.
            int goalX = target / d;
            int goalY = (target - (d * goalX));
            

            if(goalY == d - 1 && !special)
            {
                // Special case if the target is currently to the right of the cursor :/
                if(board[cursorpos[0]][cursorpos[1] + 1] == target)
                {
                    char movelist[10] = "RDLURDDLUU";
                    followmovelist(movelist, 10);
                }
                
                target++; // 
            }
            // special case for when the 2nd last tile is in the last tile's home spot
            if (special && (board[cursorpos[0] - 1][cursorpos[1] + 1] == target))
            {
                char movelist[16] = "URDDLUURDLDRUULD";
                followmovelist(movelist, 16);
            }
            
            int targetpos[2] = {-1, -1};
            //first we find where our target currently is.
            findtarget(targetpos, target);
            int crumbs = (d*d - 1);
            // actually move the cursor.       
            if(targetpos[1] + 1 == d) //
            {
								// special case for when the target is on the right edge of the board
								// because getting to its right there would be impossible
                movecursor(targetpos[0], targetpos[1] - 1, target, breadcrumbs);
                char movelist[1] = "R";
                followmovelist(movelist, 1);
                targetpos[1]--;
                crumbs = (d*d);
            }
            else
            {
                movecursor(targetpos[0], targetpos[1] + 1, target, breadcrumbs);
                draw();
            }
                  
            // The cursor is now on the RIGHT of the target piece.
           
            // Now drag it to its solved location by following the breadrumbs :        
            
            if(targetpos[1] < goalY - 1)
            {
                crumbs = (d*d);
                // due to the way the movements work our first movement must be to move
                // the target right 1 place if the target is left of the goal
                breadcrumbs[0] = 3;
            }
            
            //follow my breadcrumbs back :D
            followbreadcrumbs(breadcrumbs, targetpos, crumbs);
            
            // special = last 2 items
            if(special)
            {   // TODO movelist
                // Horizontal only.
                char movelist[3] = "ULD";
                followmovelist(movelist, 3);

                special = false;
                target += 1;
                
                // set x/y for the next row here
                goalX = (target / d);
                goalY = (target - (d * goalX)) + solvedcolumns;
            }
            else if(goalY + 1 == d)
            {       
                goalX = (target / d);
                goalY = (target - (d * (goalX - 1))) - 2;  
                target -= 2;
                special = true;
            }
            // set cursor to NEXT goal.
            movecursor(goalX, goalY, target, breadcrumbs);
            //  
            
        }
        solvedrows++;
        
        
        // #######################################
        //this solves 1 column
        for(int target = (d* solvedrows) + solvedrows; target <= d * (d -1) + 1; target += d)
        {
            // find our NEXT goal
            // X uses int truncation hack, but it works.
            int goalX = (target / d) + 1;
            int goalY = solvedcolumns;
            
            if(goalX == d - 1 && !special)
            {
                // Special case if the target is currently below the cursor :/
                if(board[cursorpos[0] + 1][cursorpos[1]] == target)
                {
                    char movelist[10] = "RDLURRDLLU";
                    followmovelist(movelist, 10);
                }
                target += d;
            }
            if (special && (board[cursorpos[0] + 1][cursorpos[1] - 1] == target))
            {   
                char movelist[16] = "LDRRULLDRURDLLUR";
                followmovelist(movelist, 16);
            }
            
            int targetpos[2] = {-1, -1};
            //first we find where our target currently is.
            findtarget(targetpos, target);
            int crumbs = (d*d - 1);
            // actually move the cursor.       
            if(targetpos[1] + 1 == d) //
            {
                movecursor(targetpos[0], targetpos[1] - 1, target, breadcrumbs);
                char movelist[1] = "R";
                followmovelist(movelist, 1);
                targetpos[1]--;
                crumbs = (d*d);
            }
            else
            {
                movecursor(targetpos[0], targetpos[1] + 1, target, breadcrumbs);
                draw();
            }
            
            //follow my breadcrumbs back :D
            followbreadcrumbs(breadcrumbs, targetpos, crumbs);
            
            if(special)
            {   
                char movelist[5] = "DLLUR";
                followmovelist(movelist, 5);
                special = false;
                target += d;
                
                goalX = solvedrows;
                goalY = solvedcolumns + 1;
            }
            else if(goalX + 1 == d)
            {
                goalX = (target / d) -1;
                goalY = solvedcolumns + 1;
                target -= d*2;
                special = true;
            }
            
            // set cursor to NEXT goal.
            movecursor(goalX, goalY, target, breadcrumbs);
        }
        solvedcolumns++;
        
        if(d - solvedrows == 2 && d - solvedcolumns == 2)
        {
            // first move to the bottom-right
            movecursor(d - 1, d - 1, 0, breadcrumbs);
            // then just keep flipping the tiles until we've won.
            while(!won())
            {
                // TODO: movelist this.
                char movelist[4] = "LURD";
                followmovelist(movelist, 4);
            }
            // breaks out of the infinte loop
            break;
            
        }
    
    }
    
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // init the board normally in a known-solvable state.
    int val = (d * d) - 1;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = val--;  
        }
    }
    // empty space is largest number instead of 0
    board[d-1][d-1] = (d*d);
    
    // swap if odd number
    val = (d * d) - 1;  
    if ((val % 2) != 0)
    {
        int temp = board[d-1][d-3];
        board[d-1][d-3] = board[d-1][d-2];
        board[d-1][d-2] = temp;
    }
    
    cursorpos[0] = d - 1;
    cursorpos[1] = d - 1;
    
    // now jumble them all.
    // let's just feed it d^3 random moves, that should be sufficiently messed up    
    srand((unsigned int) time(NULL));
    for(int i = 0; i < (d*d*d); i++)
    {
        int direction = rand() % 4;
        // note: breaks are inside ifs because I want it to cascade to the next possible move
        switch (direction)
        {
            case 0: // left
                if(cursorpos[1] - 1 >= 0)
                {
                  moveLeft();
                  break;
                }
            case 1: // right
                if(cursorpos[1] + 1 < d)
                {
                  moveRight();
                  break;
                }
            case 2: // up
                if(cursorpos[0] - 1 >= 0)
                {
                  moveUp();
                  break;
                }
            case 3: // down
                if(cursorpos[0] + 1 < d)
                {
                  moveDown();
                  break;
                }
        }
    }
    
}


void drawdivider(void)
{
    printf("|");
    for(int i = 0; i < d; i++)
    {
        printf("----|");
    }
    printf("\n");
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // clear the screen
    clear();
    
    drawdivider();
    for(int i = 0; i < d; i++)
    {
        printf("|");
        for(int j = 0; j < d; j++)
        {
            int val = board[i][j];
            if(val == (d*d))
            {
                printf("    |");
            }
            else
            {
                printf(" %2i |", val);
            }
        }
        printf("\n");
        drawdivider();
    }
    
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{

    // figure out if the move is valid (next to the blankPt)
    // by working from the blank position as a starting point
    int j = cursorpos[0];
    int k = cursorpos[1];
    if (((cursorpos[0] + 1) < d) && (board[cursorpos[0] + 1][cursorpos[1] ] == tile))
    {
        j++;
    }
     
    if (((cursorpos[0] - 1) >= 0) && (board[cursorpos[0] - 1][cursorpos[1] ] == tile))
    {
        j--;
    }
    
    if (((cursorpos[1] + 1) < d) && (board[cursorpos[0] ][cursorpos[1] + 1] == tile))
    {
        k++;
    }
            
    if (((cursorpos[1] - 1) >= 0) && (board[cursorpos[0] ][cursorpos[1] - 1] == tile))
    {
        k--;
    }
    
    // swap the 2 values
    if (j != cursorpos[0] || k != cursorpos[1])
    {
        board[j][k] = (d*d);
        board[cursorpos[0]][cursorpos[1]] = tile;
        cursorpos[0] = j;
        cursorpos[1] = k;
        
        return true;
    }
        
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int tmp = 0;
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] < tmp)
            {
                return false;
            }
            tmp = board[i][j];
         }
    }
    return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
