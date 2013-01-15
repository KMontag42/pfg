// TicTacToe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class CBoard
{
public:
	const std::valarray<char> & GetBoard() const;
	CBoard();
	const void MakeMove(const int &row, const int &col, const char *turn);
	const void MakeMoveAI(const int &index, const char *turn);
	const void DrawBoard();
	const bool CanMove(const int &row, const int &col);
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

const void CBoard::MakeMove(const int &row, const int &col, const char *turn)
{
	if (row == 1)
		vaMatrix[col-1] = *turn;
	else if (row == 2)
		vaMatrix[col+row] = *turn;
	else if (row == 3)
		vaMatrix[((row*2)+col)-1] = *turn;
}

const void CBoard::MakeMoveAI(const int &index, const char *turn)
{
	vaMatrix[index] = *turn;
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

const bool CBoard::CanMove(const int &row, const int &col)
{
	if (row == 1)
		return (vaMatrix[col-1] == 'X' || vaMatrix[col-1] == 'O');
	else if (row == 2)
		return (vaMatrix[col+row] == 'X' || vaMatrix[col+row] == 'O');
	else if (row == 3)
		return (vaMatrix[((row*2)+col)-1] == 'X' || vaMatrix[((row*2)+col)-1] == 'O');
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

	void DecideOnMove(CBoard & board);

private:
	char c_turn[1];
	mutable int m_iMovesMade;
	void RandomMove(CBoard & board);
	void FillWins(CBoard & board, const char *look_for, const char *fill_with);
	void BlockWins(CBoard & board);
	void SetTrap(CBoard & board, const char *look_for, const char *fill_with);
	void BlockTrap(CBoard & board);
	bool move_made;
};	

void CAIPlayer::RandomMove(CBoard & board)
{
	if (!move_made)
	{
		int space = rand() % 8;

		if (!(board.GetBoard()[space] == 'X' || board.GetBoard()[space] == 'O'))
		{
			board.MakeMoveAI(space, c_turn);
			move_made = true;
		}
	}
}

void CAIPlayer::FillWins(CBoard & board, const char *look_for, const char *fill_with)
{
	if (!move_made)
	{
		for (int i = 0; i < 3; i++)
		{
			// rows
			if (board.GetBoard()[0 + (i*3)] == NULL && board.GetBoard()[1 + (i*3)] == look_for[0] && board.GetBoard()[2 + (i*3)] == look_for[0])
			{
				board.MakeMoveAI(0 + (i*3), fill_with);
				move_made = true;
			}
 			else if (board.GetBoard()[0 + (i*3)] == look_for[0] && board.GetBoard()[1 + (i*3)] == NULL && board.GetBoard()[2 + (i*3)] == look_for[0])
			{
				board.MakeMoveAI(1 + (i*3), fill_with);
				move_made = true;
			}
			else if (board.GetBoard()[0 + (i*3)] == look_for[0] && board.GetBoard()[1 + (i*3)] == look_for[0] && board.GetBoard()[2 + (i*3)] == NULL)
			{
				board.MakeMoveAI(2 + (i*3), fill_with);
				move_made = true;
			}

			// cols
			else if (board.GetBoard()[0 + i] == NULL && board.GetBoard()[3 + i] == look_for[0] && board.GetBoard()[6 + i] == look_for[0])
			{
				board.MakeMoveAI(0 + i, fill_with);
				move_made = true;
			}
			else if (board.GetBoard()[0 + i] == look_for[0] && board.GetBoard()[3 + i] == NULL && board.GetBoard()[6 + i] == look_for[0])
			{
				board.MakeMoveAI(3 + i, fill_with);
				move_made = true;
			}
			else if (board.GetBoard()[0 + i] == look_for[0] && board.GetBoard()[3 + i] == look_for[0] && board.GetBoard()[6 + i] == NULL)
			{
				board.MakeMoveAI(6 + i, fill_with);
				move_made = true;
			}
		}

		// diagonal top left to bot right, added the && !move_made to fix a bug where AI would take 2 turns
		if (!move_made)
		{
			if (board.GetBoard()[0] == NULL && board.GetBoard()[4] == look_for[0] && board.GetBoard()[8] == look_for[0])
			{
				board.MakeMoveAI(0, fill_with);
				move_made = true;
			}
			else if (board.GetBoard()[0] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[8] == look_for[0])
			{
				board.MakeMoveAI(4, fill_with);
				move_made = true;
			}
			else if (board.GetBoard()[0] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[8] == NULL)
			{
				board.MakeMoveAI(8, fill_with);
				move_made = true;
			}

			// diagonal top right to bot left
			else if (board.GetBoard()[2] == NULL && board.GetBoard()[4] == look_for[0] && board.GetBoard()[6] == look_for[0])
			{
				board.MakeMoveAI(2, fill_with);
				move_made = true;
			}
			else if (board.GetBoard()[2] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[6] == look_for[0])
			{
				board.MakeMoveAI(4, fill_with);
				move_made = true;
			}
			else if (board.GetBoard()[2] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[6] == NULL)
			{
				board.MakeMoveAI(6, fill_with);
				move_made = true;
			}
		}
	}
}

void CAIPlayer::BlockWins(CBoard & board)
{
	if (!move_made)
	{
		if (c_turn[0] == 'X')
		{
			FillWins(board, "O", "X");
		}
		else if (c_turn[0] == 'O')
		{
			FillWins(board, "X", "O");
		}
	}
}

void CAIPlayer::SetTrap(CBoard & board, const char *look_for, const char *fill_with)
{
	if (!move_made)
	{
#pragma region tl_br_diag
		// trap diagonal top left to bot right
		if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}
#pragma region bottom_row
		// trap bottom row top left, bot mid, bot right (0, 7, 8)
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[7] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(7,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}

		// trap bottom row, top left, bot left, bot right (0, 6, 8)
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}

		// trap bottom row middle, bot mid, bot right (4, 7, 8)
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[6] == NULL && board.GetBoard()[7] == NULL)
		{
			board.MakeMoveAI(7,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[6] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[7] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[6] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}

		// trap bottom row middle, bot left, bot right (4, 6, 8)
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[0] == NULL && board.GetBoard()[7] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[0] == NULL && board.GetBoard()[7] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[0] == NULL && board.GetBoard()[7] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}
#pragma endregion

#pragma region top_row
		// trap top row top left, top mid, bot right (0, 1, 8)
		else if(board.GetBoard()[1] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[1] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(1,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[1] == look_for[0] && board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}

		// trap top row, top left, top right, bot right (0, 2, 8)
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}

		// trap top row middle, top left, top mid (4, 0, 1)
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[1] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(1,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[1] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}

		// trap top row middle, top left, top right (4, 0, 2)
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}
#pragma endregion

#pragma region middle_row
		// trap middle row top left, mid left, middle (0, 3, 4)
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[3] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[3] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(3,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[3] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}

		// trap middle row bot right, mid left, middle (8, 3, 4)
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[3] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[3] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(3,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[3] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}

		// trap middle row top left, middle, mid right (0, 4, 5)
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[5] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(5,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[5] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[5] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}

		// trap middle row bot right, middle, mid right (8, 4, 5)
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[5] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(5,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[5] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[5] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}
#pragma endregion

#pragma region middle_col
		// trap middle col top left, top mid, middle (0, 1, 4)
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(1,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}

		// trap middle col bot right, top mid, middle (8, 1, 4)
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(1,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}

		// trap middle col top left, middle, bot mid (0, 4, 7)
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[7] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(7,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[7] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}

		// trap middle col bot right, middle, bot mid (8, 4, 7)
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[7] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(7,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[7] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}
#pragma endregion

#pragma endregion

#pragma region tr_bl_diag
		// trap diagonal top left to bot right
		else if(board.GetBoard()[8] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}
#pragma region bottom_row
		// trap bottom row top right, bot mid, bot left (2, 7, 6)
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[7] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(7,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}

		// trap bottom row, top right, bot left, bot right (2, 6, 8)
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[0] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}

		// trap bottom row middle, bot left, bot mid (4, 6, 7)
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL && board.GetBoard()[7] == NULL)
		{
			board.MakeMoveAI(7,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[7] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}

		// trap bottom row middle, bot left, bot right (4, 6, 8)
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[7] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[8] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[7] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[8] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[7] == NULL)
		{
			board.MakeMoveAI(8,fill_with);
			move_made = true;
		}
#pragma endregion

#pragma region top_row
		// trap top row top right, top mid, bot left (2, 1, 6)
		else if(board.GetBoard()[1] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[1] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(1,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[1] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[4] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}

		// trap top row, top left, top right, bot left (0, 2, 6)
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[0] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[0] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[4] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}

		// trap top row middle, top right, top mid (4, 0, 1)
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[1] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(1,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[1] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[2] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}

		// trap top row middle, top left, top right (4, 0, 2)
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[6] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == look_for[0] && board.GetBoard()[0] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[8] == NULL)
		{
			board.MakeMoveAI(0,fill_with);
			move_made = true;
		}
#pragma endregion

#pragma region middle_row
		// trap middle row top left, mid left, middle (2, 3, 4)
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[3] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[3] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(3,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[3] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}

		// trap middle row bot right, mid left, middle (6, 3, 4)
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[3] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[3] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(3,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[3] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[5] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}

		// trap middle row top left, middle, mid right (2, 4, 5)
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[5] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(5,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[5] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[5] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}

		// trap middle row bot right, middle, mid right (6, 4, 5)
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[5] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(5,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[5] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[5] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[3] == NULL && board.GetBoard()[0] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}
#pragma endregion

#pragma region middle_col
		// trap middle col top left, top mid, middle (2, 1, 4)
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(1,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}

		// trap middle col bot right, top mid, middle (6, 1, 4)
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(1,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[4] == look_for[0] && board.GetBoard()[1] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[7] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}

		// trap middle col top left, middle, bot mid (2, 4, 7)
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[7] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(7,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[2] == look_for[0] && board.GetBoard()[7] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[2] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[6] == NULL)
		{
			board.MakeMoveAI(2,fill_with);
			move_made = true;
		}

		// trap middle col bot right, middle, bot mid (6, 4, 7)
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[7] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(7,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[6] == look_for[0] && board.GetBoard()[7] == look_for[0] && board.GetBoard()[4] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(4,fill_with);
			move_made = true;
		}
		else if(board.GetBoard()[7] == look_for[0] && board.GetBoard()[4] == look_for[0] && board.GetBoard()[6] == NULL && board.GetBoard()[1] == NULL && board.GetBoard()[2] == NULL)
		{
			board.MakeMoveAI(6,fill_with);
			move_made = true;
		}
#pragma endregion

#pragma endregion
	}
}

void CAIPlayer::BlockTrap(CBoard & board)
{
	if (!move_made)
	{
		if (c_turn[0] == 'X')
		{
			SetTrap(board, "O", "X");
		}
		else if (c_turn[0] == 'O')
		{
			SetTrap(board, "X", "O");
		}
	}
}

void CAIPlayer::DecideOnMove(CBoard & board)
{
	move_made = false;
	FillWins(board, c_turn, c_turn);
	BlockWins(board);
	SetTrap(board, c_turn, c_turn);
	BlockTrap(board);
	RandomMove(board);
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
	bool CheckForTie();
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
			do {
				std::cout << "Take your turn, pick a space that is empty\n";
				do {
					std::cout << "Row? (1, 2, or 3) ";
					while (!(std::cin >> row_in))
					{
						std::cout << "Please enter an integer value between 1 and 3 inclusive\n>";
						std::cin.clear();
						std::cin.ignore(1000, '\n');
					}
				} while (row_in != 1 && row_in != 2 && row_in != 3);

				do {
					std::cout << "Col? (1, 2, or 3) ";
					while (!(std::cin >> col_in))
					{
						std::cout << "Please enter an integer value between 1 and 3 inclusive\n>";
						std::cin.clear();
						std::cin.ignore(1000, '\n');
					}
				} while (col_in != 1 && col_in != 2 && col_in != 3);

			} while (board.CanMove(row_in, col_in));

			board.MakeMove(row_in, col_in, human_player.GetTurn());
		}

		else if (ai_player.GetTurn()[0] == 'X')
		{
			ai_player.DecideOnMove(board);
		}
		c_current_turn[0] = 'O';
	}
	else if (c_current_turn[0] == 'O')
	{
		if (human_player.GetTurn()[0] == 'O')
		{
			int row_in, col_in;
			do {
				std::cout << "Take your turn, pick a space that is empty\n";
				do {
					std::cout << "Row? (1, 2, or 3) ";
					while (!(std::cin >> row_in))
					{
						std::cout << "Please enter an integer value between 1 and 3 inclusive\n>";
						std::cin.clear();
						std::cin.ignore(1000, '\n');
					}
				} while (row_in != 1 && row_in != 2 && row_in != 3);

				do {
					std::cout << "Col? (1, 2, or 3) ";
					while (!(std::cin >> col_in))
					{
						std::cout << "Please enter an integer value between 1 and 3 inclusive\n>";
						std::cin.clear();
						std::cin.ignore(1000, '\n');
					}
				} while (col_in != 1 && col_in != 2 && col_in != 3);

			} while (board.CanMove(row_in, col_in));

			board.MakeMove(row_in, col_in, human_player.GetTurn());
		}

		else if (ai_player.GetTurn()[0] == 'O')
		{
			ai_player.DecideOnMove(board);
		}

		c_current_turn[0] = 'X';
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

	// horizontal
	else if (board.GetBoard()[0] == 'O' && board.GetBoard()[1] == 'O' && board.GetBoard()[2] == 'O')
		return true;
	else if (board.GetBoard()[3] == 'O' && board.GetBoard()[4] == 'O' && board.GetBoard()[5] == 'O')
		return true;
	else if (board.GetBoard()[6] == 'O' && board.GetBoard()[7] == 'O' && board.GetBoard()[8] == 'O')
		return true;

	// diagonal
	else if (board.GetBoard()[0] == 'O' && board.GetBoard()[4] == 'O' && board.GetBoard()[8] == 'O')
		return true;
	else if (board.GetBoard()[2] == 'O' && board.GetBoard()[4] == 'O' && board.GetBoard()[6] == 'O')
		return true;

	// vertical
	else if (board.GetBoard()[0] == 'O' && board.GetBoard()[3] == 'O' && board.GetBoard()[6] == 'O')
		return true;
	else if (board.GetBoard()[1] == 'O' && board.GetBoard()[4] == 'O' && board.GetBoard()[7] == 'O')
		return true;
	else if (board.GetBoard()[2] == 'O' && board.GetBoard()[5] == 'O' && board.GetBoard()[8] == 'O')
		return true;
	else
		return CheckForTie();
}

bool CTicTacToe::CheckForTie()
{
	if (board.GetBoard()[0] != NULL && board.GetBoard()[1] != NULL && board.GetBoard()[2] != NULL
		 && board.GetBoard()[3] != NULL && board.GetBoard()[4] != NULL && board.GetBoard()[5] != NULL
		  && board.GetBoard()[6] != NULL && board.GetBoard()[7] != NULL && board.GetBoard()[8] != NULL)
		  return true;
	else
		return false;
}

void CTicTacToe::EndGame() {
	// read the current turn
	// previous player won the game
	// show end screen
	// prompt for new game
	char choice = 0;
	if (human_player.GetTurn() == c_current_turn)
	{
		do
		{
			std::cout << "YOU LOSE." << std::endl;
			std::cout << "New Game? (y/n) ";
			while (!(std::cin >> choice))
			{
				std::cout << "Please enter either y or n.\n>";
				std::cin.clear();
				std::cin.ignore(1000,'\n');
			}
		} while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
	}
	else
	{
		do
		{
			std::cout << "Good job, you didn't lose." << std::endl;
			std::cout << "New Game? (y/n) ";
			while (!(std::cin >> choice))
			{
				std::cout << "Please enter either y or n.\n>";
				std::cin.clear();
				std::cin.ignore(1000,'\n');
			}
		} while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
	}
}

CBoard CTicTacToe::GetBoard()
{
	return board;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CTicTacToe game = CTicTacToe();
	char turn_pick;

	do
	{
		std::cout << "Choose your turn, either pick X's or O's:\n";
		while (!(std::cin >> turn_pick))
		{
			std::cout << "Please enter either x or o to pick your turn\n>";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	} while (turn_pick != 'X' && turn_pick != 'x' && turn_pick != 'O' && turn_pick != 'o');

	game.StartGame(turn_pick);
	game.GetBoard().DrawBoard();

	while (!game.CheckForWin())
	{
		game.NextTurn();
		game.GetBoard().DrawBoard();
	}

	game.EndGame();
}

