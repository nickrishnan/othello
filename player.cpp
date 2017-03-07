#include "player.hpp"

// Making some small changes to the Code -- Nikhil Krishnan
// ALL OF YOUR BASE
// ARE BELONG TO US

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    our_side = side;
    game_board = new Board();
    parity = 0;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    Side op_side;
    Move move_done(0,0);
    if(our_side == WHITE)
    {
        op_side = BLACK;
    }
    else
    {
        op_side = WHITE;
    }

    game_board->doMove(opponentsMove, op_side);

    if (game_board->hasMoves(our_side))
    {
        for(int i = 0; i <  8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                move_done.setX(i);
                move_done.setY(j);
                fprintf(stderr, "checking setting of move_Done\n");
                if(game_board->checkMove(&move_done, our_side))
                {
                    Move * tbr = new Move(move_done.getX(),move_done.getY());
                    fprintf(stderr, "object construction error\n");
                    game_board->doMove(tbr, our_side);
                    return tbr;
                }
            }
        }
    }
    else
    {
        return nullptr;
    }
}
