#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define SHM_KEY 0x1234
#define BOARD_SIZE 3
#define EMPTY ' '

struct game_state {
    char board[BOARD_SIZE][BOARD_SIZE];
    int turn;         // 1: Player 1's turn (X), 2: Player 2's turn (O)
    int move_count;   // Count of moves played
    int status;       // 0: ongoing, 1: player1 wins, 2: player2 wins, 3: draw
};

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf(" ");
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c ", board[i][j]);
            if (j < BOARD_SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < BOARD_SIZE - 1)
            printf(" ---+---+---\n");
    }
    printf("\n");
}

int check_winner(char board[BOARD_SIZE][BOARD_SIZE], char mark) {
    // check rows and columns
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((board[i][0] == mark && board[i][1] == mark && board[i][2] == mark) ||
            (board[0][i] == mark && board[1][i] == mark && board[2][i] == mark))
            return 1;
    }
    // check diagonals
    if ((board[0][0] == mark && board[1][1] == mark && board[2][2] == mark) ||
        (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark))
        return 1;
    return 0;
}

int main() {
    int shmid = shmget(SHM_KEY, sizeof(struct game_state), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
    struct game_state *game = (struct game_state *)shmat(shmid, NULL, 0);
    if (game == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    // Initialization by first process
    if (game->move_count == 0 && game->status == 0 && game->turn == 0) {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                game->board[i][j] = EMPTY;
        game->turn = 1;        // Player 1 starts
        game->move_count = 0;
        game->status = 0;
    }
    int player_num;
    char mark;
    printf("Enter player number (1 or 2): ");
    scanf("%d", &player_num);
    if (player_num != 1 && player_num != 2) {
        printf("Invalid player number\n");
        shmdt(game);
        exit(EXIT_FAILURE);
    }
    mark = (player_num == 1) ? 'X' : 'O';

    int has_printed_wait = 0;  // Flag to print waiting message only once

    while (1) {
        if (game->status != 0) {
            print_board(game->board);
            if (game->status == 3)
                printf("Game ended in a draw.\n");
            else
                printf("Player %d (%c) wins!\n", game->status, (game->status == 1) ? 'X' : 'O');
            break;
        }
        if (game->turn != player_num) {
            if (!has_printed_wait) {
                printf("Waiting for Player %d's move...\n", game->turn);
                has_printed_wait = 1;
            }
            sleep(1);
            continue;
        }
        has_printed_wait = 0;  // Reset flag when this player's turn

        print_board(game->board);

        int row, col;
        printf("Player %d (%c), enter your move (row and column: 0 1 2): ", player_num, mark);
        scanf("%d %d", &row, &col);

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            printf("Invalid move. Try again.\n");
            continue;
        }
        if (game->board[row][col] != EMPTY) {
            printf("Cell already occupied. Try again.\n");
            continue;
        }

        game->board[row][col] = mark;
        game->move_count++;

        if (check_winner(game->board, mark)) {
            game->status = player_num;
        } else if (game->move_count == BOARD_SIZE * BOARD_SIZE) {
            game->status = 3;
        } else {
            game->turn = (player_num == 1) ? 2 : 1;
        }
    }

    shmdt(game);
    if (player_num == 1) {
        shmctl(shmid, IPC_RMID, NULL);
        printf("Shared memory removed.\n");
    }
    return 0;
}


