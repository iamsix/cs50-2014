//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 5

#define PADDLEW 60
#define PADDLEY 550
#define BRICKWIDTH 36
#define BRICKHEIGHT 15

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
void bounceWindowEdges(double velocity[], GWindow window, GOval ball);

int main(int argc, char* argv[])
{
    bool godmode = false;
    if(argc == 2 && strcmp(argv[1], "GOD\n"))
    {
        printf("God mode activated");
        godmode = true;
    }
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // don't play until click
    waitForClick();
    
    // keep playing until game over
    // 0 = horizontal/X, 1 = vertical/Y.
    double paddleWidth = PADDLEW;
    double velocity[2] = {drand48() + 2, drand48() + 2};
    while (lives > 0 && bricks > 0)
    {
        // movement
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL && !godmode)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure paddle follows the cursor
                double x = getX(event) - paddleWidth / 2;;
                setLocation(paddle, x, PADDLEY);
            }
        }
        if(godmode)
        {
            setLocation(paddle, getX(ball) + RADIUS - (paddleWidth / 2), PADDLEY);
        }
        
        // ball mechanics        
        bounceWindowEdges(velocity, window, ball);
        GObject collide = detectCollision(window, ball);
        if(collide != NULL)
        {
            if(collide == paddle)
            {
                // if we hit the left 10 pixels it always bounces left. rigt 5 pixels always goes right
                if ((getX(ball) + RADIUS * 2) < getX(paddle) + 15)
                {
                    velocity[0] = (velocity[0] < 0) ? velocity[0] : -velocity[0];
                }
                else if (getX(ball) > getX(paddle) + paddleWidth - 15)
                {
                    velocity[0] = (velocity[0] < 0) ? -velocity[0] : velocity[0];
                }
                // make sure it's above the paddle to prevent strange things that happen when it hits the 'edge'
                setLocation(ball, getX(ball), PADDLEY - (2*RADIUS));
                // bounce vertical
                velocity[1] = -velocity[1];
            }
            if((strcmp(getType(collide), "GRect") == 0) && getWidth(collide) == BRICKWIDTH && collide != paddle)
            {
                int row = (getY(collide) - 40) / (BRICKHEIGHT + BRICKHEIGHT / 2);
                switch(row)
                {
                    case 0:
                        points += 5;
                        break;
                    case 1:
                        points += 4;
                        break;
                    case 2:
                        points += 3;
                        break;
                    case 3:
                        points += 2;
                        break;
                    default:
                        points++;
                        break;
                }
                
                removeGWindow(window, collide);
                bricks--;
                paddleWidth--;
                setSize(paddle, paddleWidth, 10);
                velocity[0] = (velocity[0] < 0) ? velocity[0] - 0.1 : velocity[0] + 0.1;
                velocity[1] = (velocity[1] < 0) ? velocity[1] - 0.1 : velocity[1] + 0.1;
                
                // bounce vertical if we hit the top/bottom
                if(getY(ball) < getY(collide) + BRICKHEIGHT && getY(ball) + (RADIUS * 2) > getY(collide) + BRICKHEIGHT)
                {
                    // we hit the bottom, so bounce DOWN
                    velocity[1] = (velocity[1] < 0) ? -velocity[1] : velocity[1];
                }
                else if(getY(ball) + (RADIUS * 2) > getY(collide) && getY(ball) < getY(collide))
                {
                    // we hit the top, so bounce UP
                    velocity[1] = (velocity[1] < 0) ? velocity[1] : -velocity[1];
                }
                //bounce sideways if we hit the side
                if(getX(ball) + (RADIUS * 2) > getX(collide) && getX(ball) < getX(collide))
                {
                    // we hit the left side, so bounce RIGHT
                    velocity[0] = (velocity[0] < 0) ? velocity[0] : -velocity[0];
                }
                else if (getX(ball) < getX(collide) + BRICKWIDTH && getX(ball) + (RADIUS * 2) > getX(collide) + BRICKWIDTH)
                {
                    // we hit the right side, so bounce LEFT
                    velocity[0] = (velocity[0] < 0) ? -velocity[0] : velocity[0];
                }
            }
        }
      
        
        move(ball, velocity[0], velocity[1]);
        updateScoreboard(window, label, points);
        
        
        if(getY(ball) + (RADIUS * 2) >= HEIGHT)
        {
            lives--;
            setLocation(ball, (getWidth(window) / 2) - RADIUS, getHeight(window) / 2);
            if(lives > 0)
            {
                waitForClick();
            }
            else
            {
                setLabel(label, "Game Over");
                double x = (getWidth(window) - getWidth(label)) / 2;
                double y = (getHeight(window) - getHeight(label)) / 2;
                setLocation(label, x, y);
            }
        }
        pause(10);
    }

    // wait for click before exiting

    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

void bounceWindowEdges(double velocity[], GWindow window, GOval ball)
{
    if (getX(ball) + RADIUS * 2 >= WIDTH)
    {
        setLocation(ball, WIDTH - (RADIUS * 2) - 1, getY(ball));
        velocity[0] = -velocity[0];
    }
    else if (getX(ball) <= 0)
    {
        setLocation(ball, 1, getY(ball));
        velocity[0] = -velocity[0];
    }
    
    if (getY(ball) + RADIUS * 2 >= HEIGHT || (getY(ball) <= 0))
    {
        velocity[1] = -velocity[1];
    }
}


/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int top = 40;
    for (int i = 0; i < ROWS; i++)
    {
        int left = 6;
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(left, top, BRICKWIDTH, BRICKHEIGHT);
            setFilled(brick, true);
            switch (i)
            {
                case 0:
                    setColor(brick, "RED");
                    break;
                case 1:
                    setColor(brick, "ORANGE");
                    break;
                case 2:
                    setColor(brick, "YELLOW");
                    break;
                case 3:
                    setColor(brick, "GREEN");
                    break;
                default:
                    setColor(brick, "BLUE");
                    break;
            }
            add(window, brick);
            left += 39;
        }
        top += BRICKHEIGHT + BRICKHEIGHT / 2;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval((getWidth(window) / 2) - RADIUS, getHeight(window) / 2, RADIUS * 2, RADIUS * 2);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect((getWidth(window) / 2) - 30, PADDLEY, PADDLEW, 10);
    setFilled(paddle, true);
    setColor(paddle, "GRAY");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    setColor(label, "LIGHT_GRAY");
    add(window, label);
    sendToBack(label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
