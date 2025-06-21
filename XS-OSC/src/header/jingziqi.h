// 智能井字棋（人机对战，电脑用极小化极大算法）
#pragma once
#include <bits/stdc++.h>
void print_board(const char board[3][3]) {
    std::cout << "  1 2 3\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << i + 1 << " ";
        for (int j = 0; j < 3; ++j) {
            std::cout << (board[i][j] == 0 ? ' ' : board[i][j]);
            if (j < 2) std::cout << "|";
        }
        std::cout << "\n";
        if (i < 2) std::cout << "  -+-+-\n";
    }
}

char check_winner(const char board[3][3]) {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];
    return 0;
}

bool is_draw(const char board[3][3]) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == 0) return false;
    return true;
}

void player_move(char board[3][3]) {
    while (true) {
        std::cout << "请输入你的落子位置（行 列，1-3）：";
        int x, y;
        std::cin >> x >> y;
        std::cin.ignore(10000, '\n');
        if (x >= 1 && x <= 3 && y >= 1 && y <= 3 && board[x - 1][y - 1] == 0) {
            board[x - 1][y - 1] = 'X';
            break;
        } else {
            std::cout << "输入无效，请重新输入。\n";
        }
    }
}

// 极小化极大算法
int minimax(char board[3][3], bool is_ai) {
    char winner = check_winner(board);
    if (winner == 'O') return 1;
    if (winner == 'X') return -1;
    if (is_draw(board)) return 0;

    int best = is_ai ? -2 : 2;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 0) {
                board[i][j] = is_ai ? 'O' : 'X';
                int score = minimax(board, !is_ai);
                board[i][j] = 0;
                if (is_ai) {
                    if (score > best) best = score;
                } else {
                    if (score < best) best = score;
                }
            }
        }
    }
    return best;
}

void ai_move(char board[3][3]) {
    int best_score = -2;
    int move_i = -1, move_j = -1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 0) {
                board[i][j] = 'O';
                int score = minimax(board, false);
                board[i][j] = 0;
                if (score > best_score) {
                    best_score = score;
                    move_i = i;
                    move_j = j;
                }
            }
        }
    }
    if (move_i != -1 && move_j != -1) {
        board[move_i][move_j] = 'O';
        std::cout << "电脑已落子：" << move_i + 1 << " " << move_j + 1 << std::endl;
    }
}

void tic_tac_toe() {
    char board[3][3] = {};
    std::cout << "\033c";
    std::cout << "井字棋游戏开始！你是 X，电脑是 O。\n";
    print_board(board);

    while (true) {
        player_move(board);
        std::cout << "\033c";
        print_board(board);
        if (check_winner(board) == 'X') {
            std::cout << "恭喜你获胜！\n";
            break;
        }
        if (is_draw(board)) {
            std::cout << "平局！\n";
            break;
        }

        ai_move(board);
        std::cout << "\033c";
        print_board(board);
        if (check_winner(board) == 'O') {
            std::cout << "电脑获胜！\n";
            break;
        }
        if (is_draw(board)) {
            std::cout << "平局！\n";
            break;
        }
    }
    std::cout << "按回车键返回菜单...";
    std::cin.get();
}