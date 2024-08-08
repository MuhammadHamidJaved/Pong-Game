To run the game:
## On Linux: bash
### `g++ -o pong main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread -lX11`
### `./pong`

## On Windows (assuming SFML is set up properly): bash
### `g++ -o pong main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread`
### `./pong`

## Features

Two-player Gameplay: Player 1 controls the left paddle using the W and S keys, while Player 2 controls the right paddle using the Up and Down arrow keys.

Multithreading: The game leverages pthreads to handle paddle movement and ball movement simultaneously, ensuring smooth gameplay.

Collision Detection: The ball's movement is influenced by collisions with the paddles and the walls.

Score Display: The scores of both players are displayed at the top of the screen.

Win Condition: The game ends when a player reaches a score of 10, displaying a congratulatory message for the winner.

## How It Works

Paddles and Ball: Two paddles and a ball are created as sf::RectangleShape and sf::CircleShape objects, respectively. These shapes are manipulated within threads to update their positions based on player input and ball movement.

## Threads:
MovePaddle1: Controls the movement of Player 1's paddle.

MovePaddle2: Controls the movement of Player 2's paddle.

MoveBall: Manages the ball's movement and collision detection.

Rendering: The game renders the paddles, ball, and background continuously, updating the window with the latest positions of these objects.

Score System: The scores are updated in real-time based on the ball's position, and the game ends when one player reaches 10 points.
