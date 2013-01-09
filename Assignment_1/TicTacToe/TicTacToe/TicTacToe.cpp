// TicTacToe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class CBoard
{
public:
	const std::valarray<char> & GetBoard() const;
	CBoard();
	const void MakeMove(int row, int col, const char *turn);
	const void DrawBoard();
private:
	std::valarray<char> vaMatrix;
	mutable int m_iMovesMade;
};

	const std::valarray<char> & CBoard::GetBoard() const
	{
		return vaMatrix;
	}

	CBoard::CBoard()
	{
		vaMatrix = std::valarray<char>(3*3);
	}

	const void CBoard::MakeMove(int row, int col, const char *turn)
	{
		if (row == 1)
			vaMatrix[col-1] = *turn;
		else if (row == 2)
			vaMatrix[col+row] = *turn;
		else if (row == 3)
			vaMatrix[((row*2)+col)-1] = *turn;
	}

	const void CBoard::DrawBoard()
	{
		std::cout << "+--------------------------------------+" << std::endl;
		std::cout << "+            |            |            +" << std::endl;
		std::cout << "+    " << vaMatrix[0] << "       |     " << vaMatrix[1] << "      |     " << vaMatrix[2] << "      +" << std::endl;
		std::cout << "+            |            |            +" << std::endl;
		std::cout << "+------------|------------|------------+" << std::endl;
		std::cout << "+            |            |            +" << std::endl;
		std::cout << "+    " << vaMatrix[3] << "       |     " << vaMatrix[4] << "      |     " << vaMatrix[5] << "      +" << std::endl;
		std::cout << "+            |            |            +" << std::endl;
		std::cout << "+------------|------------|------------+" << std::endl;
		std::cout << "+            |            |            +" << std::endl;
		std::cout << "+    " << vaMatrix[6] << "       |     " << vaMatrix[7] << "      |     " << vaMatrix[8] << "      +" << std::endl;
		std::cout << "+            |            |            +" << std::endl;
		std::cout << "+--------------------------------------+" << std::endl;
	}

class IPlayer
{
public:
	virtual void SetTurn(const char * turn) = 0;
	virtual const char * GetTurn() const = 0;
};

class CHumanPlayer : public IPlayer
{
public:
	virtual void SetTurn(const char * turn)
	{
		::strcpy(c_turn, turn);
	}
	virtual const char * GetTurn() const
	{
		return c_turn;
	}

private:
	char c_turn[1];
	mutable int m_iMovesMade;
};	

class CAIPlayer : public IPlayer
{
public:
	virtual void SetTurn(const char * turn)
	{
		::strcpy(c_turn, turn);
	}
	virtual const char * GetTurn() const
	{
		return c_turn;
	}

	void DecideOnMove()
	{
			
	};

private:
	char c_turn[1];
	mutable int m_iMovesMade;
};	


class CTicTacToe
{
public:
	void StartGame(const char turn_pick);
	void NextTurn();
	bool CheckForWin();
	CBoard GetBoard();
	void EndGame();
private:
	CBoard board;
	CHumanPlayer human_player;
	CAIPlayer ai_player;
	char c_current_turn[1];
};

void CTicTacToe::StartGame(const char turn_pick) {
	// init new board, player and AI player and determine who goes first
	board = CBoard();
	human_player = CHumanPlayer();
	ai_player = CAIPlayer();

	if (turn_pick == 'X' || turn_pick == 'x')
	{
		human_player.SetTurn("X");
		ai_player.SetTurn("O");
	}
	else if (turn_pick == 'O' || turn_pick == 'o')
	{
		human_player.SetTurn("O");
		ai_player.SetTurn("X");
	}
	
	::strcpy(c_current_turn, "X");
}

void CTicTacToe::NextTurn() {
	// check which player has the next turn
	// if human player, prompt for move
	// if ai player, make move
	// advance turn counter
	if (c_current_turn[0] == 'X')
	{
		if (human_player.GetTurn()[0] == 'X')
		{
			int row_in, col_in;
			std::cout << "Row? ";
			std::cin >> row_in;
			std::cout << "Col? ";
			std::cin >> col_in;

			board.MakeMove(row_in, col_in, human_player.GetTurn());
		}

		else
		{
			ai_player.DecideOnMove();
		}
		c_current_turn[1] = 'O';
	}
	else if (c_current_turn[0] == 'O')
	{
		if (human_player.GetTurn()[0] == 'O')
		{
			int row_in, col_in;
			std::cout << "Row? ";
			std::cin >> row_in;
			std::cout << "Col? ";
			std::cin >> col_in;

			board.MakeMove(row_in, col_in, human_player.GetTurn());
		}

		else
		{
			ai_player.DecideOnMove();
		}

		c_current_turn[1] = 'X';
	}
}

bool CTicTacToe::CheckForWin() {
	// check current board state for 3 in a row
	// if 3 in a row, return True
	// else return False
	// horizontal
	if (board.GetBoard()[0] == 'X' && board.GetBoard()[1] == 'X' && board.GetBoard()[2] == 'X')
		return true;
	else if (board.GetBoard()[3] == 'X' && board.GetBoard()[4] == 'X' && board.GetBoard()[5] == 'X')
		return true;
	else if (board.GetBoard()[6] == 'X' && board.GetBoard()[7] == 'X' && board.GetBoard()[8] == 'X')
		return true;

	// diagonal
	else if (board.GetBoard()[0] == 'X' && board.GetBoard()[4] == 'X' && board.GetBoard()[8] == 'X')
		return true;
	else if (board.GetBoard()[2] == 'X' && board.GetBoard()[4] == 'X' && board.GetBoard()[6] == 'X')
		return true;

	// vertical
	else if (board.GetBoard()[0] == 'X' && board.GetBoard()[3] == 'X' && board.GetBoard()[6] == 'X')
		return true;
	else if (board.GetBoard()[1] == 'X' && board.GetBoard()[4] == 'X' && board.GetBoard()[7] == 'X')
		return true;
	else if (board.GetBoard()[2] == 'X' && board.GetBoard()[5] == 'X' && board.GetBoard()[8] == 'X')
		return true;

	else if (board.GetBoard()[0] == board.GetBoard()[1] == board.GetBoard()[2] == 'O')
		return true;
	else if (board.GetBoard()[3] == board.GetBoard()[4] == board.GetBoard()[5] == 'O')
		return true;
	else if (board.GetBoard()[6] == board.GetBoard()[7] == board.GetBoard()[8] == 'O')
		return true;

	// diagonal
	else if (board.GetBoard()[0] == board.GetBoard()[4] == board.GetBoard()[8] == 'O')
		return true;
	else if (board.GetBoard()[2] == board.GetBoard()[4] == board.GetBoard()[6] == 'O')
		return true;

	// vertical
	else if (board.GetBoard()[0] == board.GetBoard()[3] == board.GetBoard()[6] == 'O')
		return true;
	else if (board.GetBoard()[1] == board.GetBoard()[4] == board.GetBoard()[7] == 'O')
		return true;
	else if (board.GetBoard()[2] == board.GetBoard()[5] == board.GetBoard()[8] == 'O')
		return true;
	else
		return false;
}

void CTicTacToe::EndGame() {
	// read the current turn
	// previous player won the game
	// show end screen
	// prompt for new game
	if (human_player.GetTurn() == c_current_turn){}
	else{}
}

CBoard CTicTacToe::GetBoard()
{
	return board;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CTicTacToe game = CTicTacToe();
	char turn_pick;

	std::cout << "Player pick X's or O's:" << std::endl << "Enter either 'X' or 'O'" << std::endl;
	std::cin >> turn_pick;
	
	game.StartGame(turn_pick);
	game.GetBoard().DrawBoard();
	
	while (!game.CheckForWin())
	{
		game.NextTurn();
		game.GetBoard().DrawBoard();
	}

	game.EndGame();
}

