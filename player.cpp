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
    fprintf(stderr, "Side is: %d\n", our_side);

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
    std::vector<Move> next_moves;
    std::vector<Move> next_next_moves;
    Board * board_layer1;
    Board * board_layer2;
    Move optimal_move(0,0);
    int min_gain = 100, max = -100, temp; // 100 is an arbitrary number bigger than 64

    if(our_side == WHITE)
    {
        op_side = BLACK;
    }
    else
    {
        op_side = WHITE;
    }

    game_board->doMove(opponentsMove, op_side);
    next_moves = game_board->listAvailMoves(our_side);
    if(next_moves.size() == 0)
    {
        return nullptr;
    }
    std::vector<Move>::iterator i;
    std::vector<Move>::iterator j;

    for(i = next_moves.begin(); i!= next_moves.end(); i++)
    {
        board_layer1 = game_board->copy();
        board_layer1->doMove(&*i, our_side);
        board_layer2 = board_layer1->copy();

        next_next_moves = board_layer2->listAvailMoves(op_side);
        for(j = next_next_moves.begin(); j != next_next_moves.end(); j++)
        {
            board_layer2->doMove(&*j, op_side);
            temp = board_layer2->count(our_side) - board_layer2->count(op_side);
            if(temp < min_gain)
            {
                min_gain = temp;
            }
            board_layer2 = board_layer1->copy();
        }
        if(min_gain > max)
        {
            max = min_gain;
            optimal_move = *i;
        }
    }
    Move * to_return = new Move(optimal_move.getX(),optimal_move.getY());
    game_board->doMove(to_return, our_side);
    return to_return;
}
