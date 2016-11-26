GAME: SNAKE
gameplay:
    The objective of this game is to eat up the blue dots, which are present only one at a time, to gain points.
    Use the arrow keys to change the direction of the snake.
    There are red dots that are present on the game board, which the snake has to avoid.
    [1 ~ 5] red dots will appear on the screen.
    The red dots will randomly change their position and their number after certain time interval.
    A warning will show up on the right side briefly before the red dots are randomly assigned in the grid.
    Going off the grid will also result in a game over.
menu and title screen:
    Pressing select will make the game reset and go to the title screen. If this happens during the game, scores won't be stored.
    A state machine has been implemented for the menu.
    speed control can be done by up or down button in speed selector page
    The sequence is : title -> speed selector -> help/instruction page -> game -> game over page
    Move through the sequence by pressing A key.
scores:
    The leaderboard will show top 3 scores.
    It will record the scores since the game has started running.