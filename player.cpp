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
    std::string disp_side;
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
    int alpha = -9000;
    int beta = 9000;
    int depth = 0;
    int num_poss_moves = 0;
    bool playerIsBlack = false;
    int presentMoveScore = -1000;
    int bestMoveScore = -11000;
    Move * player_move = new Move(0,0);
    Move * optimal_move = new Move(0,0);
    std::vector<Move> next_moves;
    std::vector<Move> next_next_moves;
    Board * board_layer1;
    int min_gain = 100, max = -100, temp; // 100 is an arbitrary number bigger than 64

    if(our_side == WHITE)
    {
        op_side = BLACK;
    }
    else
    {
        op_side = WHITE;
        playerIsBlack = true;
    }

    game_board->doMove(opponentsMove, op_side);
    next_moves = game_board->listAvailMoves(our_side);
    num_poss_moves = next_moves.size();
    if(num_poss_moves == 0)
    {
        return nullptr;
    }
    if(num_poss_moves >=1 && num_poss_moves <= 5)
    {
        depth = 7;
    }
    else
    {
        depth = 5;
    }


    std::vector<Move>::iterator i;
    for(i = next_moves.begin(); i != next_moves.end(); i++)
    {
        player_move = &*i;
        std::cerr << "Move's X: " << player_move->getX() << "Move's Y: " << player_move->getY() << std::endl;
        board_layer1 = game_board->copy();
        board_layer1->doMove(player_move, our_side);
        int pm_x = player_move->getX();
        int pm_y = player_move->getY();

        // optimizing for corner priority


        presentMoveScore = this->search(board_layer1, depth - 1, alpha, beta, playerIsBlack);
        if((pm_x == 0 && pm_y == 0) || (pm_x == 0 && pm_y == 7) ||
            (pm_x == 7 && pm_y == 0) || (pm_x == 7 && pm_y == 7))
        {
            presentMoveScore += 10000;
        }

        if((pm_x == 1 && pm_y == 0) || (pm_x == 0 && pm_y == 1) ||
           (pm_x == 0 && pm_y == 6) || (pm_x == 6 && pm_y == 0) ||
           (pm_x == 1 && pm_y == 7) || (pm_x == 7 && pm_y == 1) ||
           (pm_x == 6 && pm_y == 7) || (pm_x == 7 && pm_y == 6))
        {
            presentMoveScore -= 10000;
        }

        if(presentMoveScore > bestMoveScore)
        {
            bestMoveScore = presentMoveScore;
            optimal_move->setX(pm_x);
            optimal_move->setY(pm_y);
        }
    }
    Move * to_return = new Move(optimal_move->getX(),optimal_move->getY());
    game_board->doMove(to_return, our_side);
    return to_return;
}

double Player::search(Board * board_state, int depth, int alpha, int beta, bool playerIsBlack)
{
    Move * to_pass = new Move(0,0);
    if(depth == 0 || board_state->isDone())
    {
        return evalScore(board_state, our_side);
    }
    else
    {
        if((playerIsBlack == true && board_state->isTurnBlack == true) ||
            (playerIsBlack == false && board_state->isTurnBlack == false))
        {
            for(int x = 0; x < 8; x++)
            {
                for(int y = 0; y < 8; y++)
                {
                    to_pass->setX(x);
                    to_pass->setY(y);
                    if(board_state->checkMove(to_pass, this->our_side))
                    {
                        Board * next_state = board_state->copy();
                        next_state->doMove(to_pass, this->our_side);
                        int tempMax = search(next_state, depth -1, alpha, beta, playerIsBlack);
                        if(tempMax > alpha)
                        {
                            alpha = tempMax;
                        }
                        if(alpha >= beta)
                        {
                            return alpha;
                        }
                    }
                }
            }
            return alpha;
        }
        else
        {
            for(int x = 0; x < 8; x++)
            {
                for(int y = 0; y < 8; y++)
                {
                    to_pass->setX(x);
                    to_pass->setY(y);
                    if(board_state->checkMove(to_pass, this->op_side))
                    {
                        Board * opponent_state = board_state->copy();
                        opponent_state->doMove(to_pass, this->op_side);
                        int tempMin = search(opponent_state, depth - 1, alpha, beta, playerIsBlack);
                        if(tempMin < beta)
                        {
                            beta = tempMin;
                        }
                        if(alpha >= beta)
                        {
                            return beta;
                        }
                    }
                }
            }
            return beta;
        }
    }
}

double Player::evalScore(Board * curr_board, Side playerSide) // i used a heuristic from a paper
{
    int my_squares = 0;
    int opp_squares = 0;
    int i, j, k;
    int my_front_squares = 0;
    int opp_front_squares = 0;
    int x, y;
    double p = 0., l = 0., c = 0., m = 0., f = 0., d = 0.;
    Move * tbc = new Move(0,0);

    int X1[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int Y1[8]= {0, 1, 1, 1, 0, -1, -1, -1};
    int scores[8][8] = {{25, -3, 10, 9, 9, 10, -3, 25}, {-3, -7, -4, 1, 1, -4, -7, -3},
                        {12, -4, 2, 2, 2, 2, -4, 12}, {9, 1, 2, -3, -3, 2, 1, 8},
                    {9, 1, 2, -3, -3, 2, 1, 8}, {12, -4, 2, 2, 2, 2, -4, 12},
                {-3, -7, -4, 1, 1, -4, -7, -3}, {25, -3, 10, 9, 9, 10, -3, 25}};

    if(our_side == BLACK)
    {
        my_squares = curr_board->countBlack();
        opp_squares = curr_board->countWhite();
    }
    else
    {
        my_squares = curr_board->countWhite();
        opp_squares = curr_board->countBlack();
    }

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            tbc->setX(i);
            tbc->setY(j);
            if(curr_board->get(our_side, i, j))
            {
                d += scores[i][j];
            }
            else if(curr_board->get(op_side, i, j))
            {
                d -= scores[i][j];
            }
            else
            {
                for(k = 0; k < 8; k++)
                {
                    x = X1[k] + i;
                    y = Y1[k] + j;
                    if(x >= 0 && x < 8 && y >= 0 && y < 8)
                    {
                        if(curr_board->get(our_side, i, j))
                        {
                            my_front_squares++;
                        }
                        else
                        {
                            opp_front_squares++;
                        }
                        break;
                    }
                }
            }
        }
    }
    if(my_squares > opp_squares)
    {
        p = (100. * my_squares) / (my_squares + opp_squares);
    }
    else if(opp_squares > my_squares)
    {
        p = -(100. * opp_squares) / (my_squares + opp_squares);
    }
    else
    {
        p = 0.;
    }
    if(my_front_squares > opp_front_squares)
    {
        f = (100. * my_front_squares) / (my_front_squares + opp_front_squares);
    }
    else if(opp_squares > my_squares)
    {
        f = -(100. * opp_front_squares) / (my_squares + opp_squares);
    }
    else
    {
        f = 0.;
    }

//corner scoring
    my_squares = opp_squares = 0;
    if(curr_board->get(our_side, 0, 0))
    {
        my_squares++;
    }
    else if(curr_board->get(op_side, 0, 0))
    {
        opp_squares++;
    }
    if(curr_board->get(our_side, 0, 7))
    {
        my_squares++;
    }
    else if(curr_board->get(op_side, 0, 7))
    {
        opp_squares++;
    }
    if(curr_board->get(our_side, 7, 0))
    {
        my_squares++;
    }
    else if(curr_board->get(op_side, 7, 0))
    {
        opp_squares++;
    }
    if(curr_board->get(our_side, 7, 7))
    {
        my_squares++;
    }
    else if(curr_board->get(op_side, 7, 7))
    {
        opp_squares++;
    }
    c = 25. * (my_squares - opp_squares);

    my_squares = curr_board->numPossMoves(our_side);
    opp_squares = curr_board->numPossMoves(op_side);

    if(my_squares > opp_squares)
    {
        m = (100. * my_squares) / (my_squares + opp_squares);
    }
    else if(opp_squares > my_squares)
    {
        m = -(100. * opp_squares) / (my_squares + opp_squares);
    }
    else
    {
        m = 0;
    }

    double score = (10 * p) + (900 * c) + (80 * m) + (75 * f) + 10 * d;
    return score;
      // if(playerSide == BLACK)
    // {
    //     return curr_board->countBlack() - curr_board->countWhite();
    // }
    // else
    // {
    //     return curr_board->countWhite() - curr_board->countBlack();
    // }
}
