#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

#define IsStarPosition(x, y) \
        ( ((x)==3 && (y)==3) || ((x)==3 && (y)==11) || ((x)==7 && (y)==7) || ((x)==11 && (y)==3) || ((x)==11 && (y)==11) )

const int WIN_STONE_NUM = 5;
const int LINE_NUM = 15;
const int MAX_STEPS = LINE_NUM * LINE_NUM;

typedef unsigned char Byte;

const Byte WHITE_STONE = 0;
const Byte BLACK_STONE = 1;
const Byte EMPTY_STONE = 2;
const Byte STAR_STONE  = 3;

const char StoneMapping[] = {
    'O',  // WHITE_STONE
    'X',  // BLACK_STONE
    '.',  // EMPTY_STONE
    '+'   // STAR_STONE
};


void DrawBoard(const vector<vector<Byte>>& board)
{
    stringstream alphabet_line; 
    alphabet_line << "   "; 
    for (int i=0; i<LINE_NUM; i++) {
        alphabet_line << char(int('A')+i) << ' ';
    }
    alphabet_line << '\n';
    cout << alphabet_line.str();
    
    stringstream ss_board;
    for (int i=0; i<LINE_NUM; i++) {
        int line_title = LINE_NUM - i;
        int indent = line_title > 9 ? 1 : 0;

        if (indent == 1) {
            ss_board << line_title << ' ';
        }
        else {
            ss_board << ' ' << line_title << ' ';
        }
        
        for (int j=0; j<LINE_NUM; j++) {
            // check star positions
            if ( IsStarPosition(i, j) 
                 && 
                 (board[i][j] == EMPTY_STONE) ) {
                ss_board << StoneMapping[STAR_STONE] << ' ';
            } 
            else {  // non-star positions 
                ss_board << StoneMapping[board[i][j]] << ' ';
            }
        }
        ss_board << line_title << '\n';
    }
    
    cout << ss_board.str();
    cout << alphabet_line.str() << endl;
}

pair<int, int> ParseMove(const string& move)
{
    int row = -1, col = -1;
    
    if ('A' <= move[0] && move[0] < 'A'+LINE_NUM) {
        col = move[0] - 'A';
    }
    else if ('a' <= move[0] && move[0] < 'a'+LINE_NUM) {
        col = move[0] - 'a';
    }
    
    if (move.size() == 2) {
        row = move[1] - '0'; 
    }
    else if (move.size() == 3) {
        row = (move[1]-'0') * 10 + move[2] - '0';
    }
    
    if (row > LINE_NUM) {
        row = -1;
    }
    else {
        row = LINE_NUM - row;
    }
    
    return make_pair(row, col);
}

// Given the position, check if the stone party on the position win 
// Return:
//      true  - win 
//      false - not win yet
bool CheckPosWin(const vector<vector<Byte>>& board, const pair<int, int> pos)
{
    int x = pos.first, y = pos.second;
    
    if (x < 0 || y < 0 || x >= LINE_NUM || y >= LINE_NUM) return false; 
    if (board[x][y] == EMPTY_STONE) return false; 
    
    Byte color = board[x][y]; 
    
    int col_count = 1, row_count = 1, left_count = 1, right_count = 1;
    
    while (--x >= 0 && board[x][y] == color) {
        col_count ++;
    }
    if (col_count >= WIN_STONE_NUM) {
        return true;
    }
    x = pos.first;
    
    while (++x < LINE_NUM && board[x][y] == color) {
        col_count ++;
    }
    if (col_count >= WIN_STONE_NUM) {
        return true;
    }
    x = pos.first;
    
    while (--y >= 0 && board[x][y] == color) {
        row_count ++;
    }
    if (row_count >= WIN_STONE_NUM) {
        return true;
    }
    y = pos.second;
    
    while (++y < LINE_NUM && board[x][y] == color) {
        row_count ++;
    }
    if (row_count >= WIN_STONE_NUM) {
        return true;
    }
    y = pos.second;
    
    while (--x >=0 && --y >= 0 && board[x][y] == color) {
        left_count ++;
    }
    if (left_count >= WIN_STONE_NUM) {
        return true;
    }
    x = pos.first;
    y = pos.second; 
    
    while (++x < LINE_NUM && ++y < LINE_NUM && board[x][y] == color) {
        left_count ++;
    }
    if (left_count >= WIN_STONE_NUM) {
        return true;
    }
    x = pos.first;
    y = pos.second; 
    
    while (--x >= 0 && ++y < LINE_NUM && board[x][y] == color) {
        right_count ++;
    }
    if (right_count >= WIN_STONE_NUM) {
        return true;
    }
    x = pos.first;
    y = pos.second;
    
    while (++x < LINE_NUM && --y >= 0 && board[x][y] == color) {
        right_count ++;
    }
    if (right_count >= WIN_STONE_NUM) {
        return true;
    }

    return false;
}

int main()
{
    vector<vector<Byte>> board;
    board.reserve(LINE_NUM);
    
    vector<Byte> empty_line;
    empty_line.reserve(LINE_NUM);
    for (int i=0; i<LINE_NUM; i++) {
        empty_line.push_back(EMPTY_STONE);
    }
    
    for (int i=0; i<LINE_NUM; i++) {
        board.push_back(empty_line);
    }
    
    DrawBoard(board);
    
    vector< pair<int, int> > moves_vec;
    
    int step = 0;
    while (step <= MAX_STEPS) {
        step ++;
        string move;
        string party = step % 2 ? string("Black") : string("White");
        
    INPUT_MOVE:
        cout << "Please input the move [" << party << "]: " ;
        cin >> move ;
        
        if (move == "exit") break;
        
        if (move == "undo") {
            if (step >= 1) {
                step -= 2;
                pair<int, int> last_pos = moves_vec[moves_vec.size()-1];
                moves_vec.pop_back();
                
                int x = last_pos.first, y = last_pos.second;
                if ( IsStarPosition(x, y) ) {
                    board[x][y] = STAR_STONE;
                }
                else {
                    board[x][y] = EMPTY_STONE;
                }
            }
            DrawBoard(board);
            continue;
        }
        
        pair<int, int> pos = ParseMove(move);
        if (pos.first < 0 || pos.second < 0 || ( board[pos.first][pos.second] != EMPTY_STONE && board[pos.first][pos.second] != STAR_STONE )) {
            cout << "Invalid Move, please input again\n";
            goto INPUT_MOVE;
        }
        else {
            moves_vec.push_back(pos);
        }
        board[pos.first][pos.second] = step % 2;
        DrawBoard(board);
        
        if (CheckPosWin(board, pos)) {
            cout << party << " Win!" << endl;
            break;
        }
    }

    return 0;
}