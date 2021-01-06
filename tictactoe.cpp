#include "iostream"
#include "ctime"
#include "cstdlib"
#include "vector"
using namespace std;

class TicTacToe
{
    char board[3][3];
    char winner;

public:
    TicTacToe() { reset(); }

    void resetBoard()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                board[i][j] = ' ';
        }
    }
    void resetWinner() { winner = ' '; }
    void reset()
    {
        resetBoard();
        resetWinner();
    }

    // setters and getters
    void setChar(char symbol, int i) { board[i / 3][i % 3] = symbol; }
    void setChar(char symbol, int i, int j) { board[i][j] = symbol; }
    char getChar(int i) { return board[i / 3][i % 3]; }
    char getChar(int i, int j) { return board[i][j]; }
    char whoWon() { return winner; }

    // possible moves
    bool isMovePossible()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == ' ')
                    return true;
            }
        }
        return false;
    }
    vector<int> allPossibleMoves()
    {
        vector<int> move_set;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == ' ')
                    move_set.push_back(3 * i + j);
            }
        }
        return move_set;
    }

    // check winner
    bool checkWinner(char symbol)
    {
        // horizontal check
        for (int i = 0; i < 3; i++)
        {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][2] == symbol)
                return true;
        }

        // vertical check
        for (int i = 0; i < 3; i++)
        {
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[2][i] == symbol)
                return true;
        }

        // diagonal 1 check
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] == symbol)
            return true;

        // diagonal 2 check
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] == symbol)
            return true;

        return false;
    }

    // do move
    void doMove(char symbol, int index)
    {
        int i = index / 3, j = index % 3;
        board[i][j] = symbol;

        if (checkWinner(symbol))
            winner = symbol;
    }
    void printBoard()
    {
        for (int i = 0; i <= 6; i += 3)
        {
            cout << "\n " << getChar(i) << " | " << getChar(i + 1) << " | " << getChar(i + 2);
        }
    }
};

class Player // abstract class
{
    char symbol;

public:
    // constructor
    Player(char s) { symbol = s; }

    // getter
    char getSymbol() { return symbol; }

    // virtual function
    virtual int getMove(TicTacToe game) = 0;
};

class HumanPlayer : public Player
{
public:
    //constructor
    HumanPlayer(char s) : Player(s) {}

    // getMove() over-ride
    int getMove(TicTacToe game)
    {
        int index;
        while (1)
        {
            cout << "\nEnter index: ";
            cin >> index;

            if (game.getChar(index - 1) == ' ')
                return index - 1;

            cout << "Wrong Move";
        }
    }
};

struct ttt_move
{
    int index;
    long score;
};

class ComputerAI : public Player
{
public:
    // constructor
    ComputerAI(char s) : Player(s) {}

    // getMove() over-ride
    int getMove(TicTacToe game)
    {
        ttt_move move;
        srand(time(0));

        if (game.allPossibleMoves().size() == 9)
            move.index = rand() % 9;
        else
            move = minimax(game, getSymbol());

        return move.index;
    }

    // minimax
    ttt_move minimax(TicTacToe state, char curr_player)
    {
        char max_player = this->getSymbol();
        char other_player = (curr_player == 'X') ? 'O' : 'X';
        ttt_move best, current;

        // base condition for win - lose
        if (state.whoWon() == other_player)
        {
            best.score = state.allPossibleMoves().size() + 1;
            if (other_player != max_player)
                best.score = -best.score;
            return best;
        }
        // base condtion for draw
        if (!state.isMovePossible())
        {
            best.score = 0;
            return best;
        }

        if (curr_player == max_player)
            best.score = -1000000;
        else
            best.score = 1000000;

        vector<int> move_set = state.allPossibleMoves();

        for (int i = 0; i < move_set.size(); i++)
        {
            state.doMove(curr_player, move_set[i]);
            current = minimax(state, other_player);
            state.setChar(' ', move_set[i]);
            state.resetWinner();
            current.index = move_set[i];

            if (curr_player == max_player)
            {
                if (current.score > best.score)
                    best = current;
            }
            else
            {
                if (current.score < best.score)
                    best = current;
            }
        }
        return best;
    }
};
void play(TicTacToe game, Player *x_player, Player *o_player)
{
    char letter = 'X';
    int index;
    game.reset();
    while (game.isMovePossible())
    {
        if (letter == 'O')
            index = o_player->getMove(game);
        else
            index = x_player->getMove(game);

        game.doMove(letter, index);
        cout << endl
             << letter << " makes move to " << index + 1 << endl;
        game.printBoard();
        cout << endl;

        if (game.checkWinner(letter))
        {
            cout << endl
                 << letter << " wins !";
            return;
        }

        letter = (letter == 'X') ? 'O' : 'X';
    }
    cout << endl
         << "Draw";
}
int main()
{
    TicTacToe game;

    char ch;
    int mode;

    HumanPlayer x_human('X');
    HumanPlayer o_human('O');
    ComputerAI x_AI('X');
    ComputerAI o_AI('O');

    Player *x, *o;

    do
    {
        cout << endl;
        cout << "Select game mode :" << endl
             << "1. Human v/s Human" << endl
             << "2. Human v/s Computer" << endl
             << "3. Computer v/s Human" << endl
             << "4. Computer v/s Computer" << endl
             << endl
             << "Mode : ";
        cin >> mode;

        switch (mode)
        {
        case 1:
            x = &x_human;
            o = &o_human;
            break;

        case 2:
            x = &x_human;
            o = &o_AI;
            break;

        case 3:
            x = &x_AI;
            o = &o_human;
            break;

        case 4:
            x = &x_AI;
            o = &o_AI;
            break;

        default:
            cout << "Wrong choice";
        }

        play(game, x, o);

        cout << endl
             << "Play again (y/n) : ";
        cin >> ch;

    } while (ch == 'y' || ch == 'Y');

    return 0;
}