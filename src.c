#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Function Prototypes
void display_score(int score1[], int score2[], int set_games1, int set_games2, int current_game_score1, int current_game_score2);
void update_score(int *score, int *games, int *sets, int other_score, int *other_games);
char* get_game_score(int points);

// Global constants for scoring logic
#define MAX_SETS 3 // Best of 3 sets

// Main function
int main() {
    // Arrays to hold the set scores (e.g., set_scores1[0] is the score for Set 1)
    int player1_sets[MAX_SETS] = {0};
    int player2_sets[MAX_SETS] = {0};

    // Keep track of games won in the current set
    int player1_current_games = 0;
    int player2_current_games = 0;

    // Keep track of the points in the current game (0=0, 1=15, 2=30, 3=40, 4=Advantage/Win)
    int player1_current_score = 0;
    int player2_current_score = 0;

    // Keep track of the current set number (0 for the first set, 1 for the second, etc.)
    int current_set = 0;

    char input[10];

    printf("🎾 Tennis Scoreboard Project 🎾\n");
    printf("Enter 'p1' for Player 1 point, 'p2' for Player 2 point, or 'q' to quit.\n");
    printf("--- Game Starts ---\n");

    // Main game loop
    while (player1_sets[current_set] < 2 && player2_sets[current_set] < 2 && current_set < MAX_SETS) {
        // Display the score before getting input
        display_score(player1_sets, player2_sets, player1_current_games, player2_current_games, player1_current_score, player2_current_score);

        printf("Enter point winner (p1/p2): ");
        if (scanf("%s", input) != 1) {
            continue; // Handle scan error
        }

        if (strcmp(input, "q") == 0) {
            printf("\nGame stopped. Final Score:\n");
            break;
        }

        if (strcmp(input, "p1") == 0) {
            // Update Player 1's score and check for game/set win
            update_score(&player1_current_score, &player1_current_games, &player1_sets[current_set], player2_current_score, &player2_current_games);
        } else if (strcmp(input, "p2") == 0) {
            // Update Player 2's score and check for game/set win
            update_score(&player2_current_score, &player2_current_games, &player2_sets[current_set], player1_current_score, &player1_current_games);
        } else {
            printf("Invalid input. Please enter 'p1', 'p2', or 'q'.\n");
            continue;
        }

        // Check if a set has been won
        if (player1_sets[current_set] == 6 || player2_sets[current_set] == 6) {
            if (player1_sets[current_set] >= player2_sets[current_set] + 2) {
                printf("\n--- Player 1 wins Set %d! ---\n", current_set + 1);
                current_set++; // Move to the next set
                player1_current_games = 0;
                player2_current_games = 0;
            } else if (player2_sets[current_set] >= player1_sets[current_set] + 2) {
                printf("\n--- Player 2 wins Set %d! ---\n", current_set + 1);
                current_set++; // Move to the next set
                player1_current_games = 0;
                player2_current_games = 0;
            } else if (player1_sets[current_set] == 6 && player2_sets[current_set] == 6) {
                printf("TIE-BREAK TIME! (Simplified Tie-break rule: next game wins the set)\n");
                // In a real tie-break, a player needs 7 points and a 2-point lead.
                // For this simple project, we'll keep the scoring logic the same for the deciding game.
            }
        }
    }

    // Final score display after the loop breaks
    display_score(player1_sets, player2_sets, player1_current_games, player2_current_games, player1_current_score, player2_current_score);

    // Determine the overall match winner
    int p1_sets_won = 0;
    int p2_sets_won = 0;
    for (int i = 0; i < MAX_SETS; i++) {
        if (player1_sets[i] > player2_sets[i]) {
            p1_sets_won++;
        } else if (player2_sets[i] > player1_sets[i]) {
            p2_sets_won++;
        }
    }

    printf("\n\n--- MATCH OVER ---\n");
    if (p1_sets_won > p2_sets_won) {
        printf("**🏆 Player 1 wins the match (%d sets to %d)!**\n", p1_sets_won, p2_sets_won);
    } else if (p2_sets_won > p1_sets_won) {
        printf("**🏆 Player 2 wins the match (%d sets to %d)!**\n", p2_sets_won, p1_sets_won);
    } else {
        printf("**Match tied or incomplete.**\n");
    }

    return 0;
}

// Function to get the string representation of the current game score
char* get_game_score(int points) {
    switch (points) {
        case 0: return "0";
        case 1: return "15";
        case 2: return "30";
        case 3: return "40";
        case 4: return "AD"; // Advantage
        default: return "??";
    }
}

// Function to display the entire scoreboard
void display_score(int score1[], int score2[], int set_games1, int set_games2, int current_game_score1, int current_game_score2) {
    printf("\n--- Current Score ---\n");
    printf("| Player | Set 1 | Set 2 | Set 3 | Games | Score |\n");
    printf("|--------|-------|-------|-------|-------|-------|\n");
    printf("| P1     | %5d | %5d | %5d | %5d | %5s |\n",
           score1[0], score1[1], score1[2], set_games1, get_game_score(current_game_score1));
    printf("| P2     | %5d | %5d | %5d | %5d | %5s |\n",
           score2[0], score2[1], score2[2], set_games2, get_game_score(current_game_score2));

    // Handle Deuce/Advantage announcement
    if (current_game_score1 == 3 && current_game_score2 == 3) {
        printf("📢 DEUCE!\n");
    } else if (current_game_score1 == 4 && current_game_score2 == 3) {
        printf("📢 ADVANTAGE P1!\n");
    } else if (current_game_score1 == 3 && current_game_score2 == 4) {
        printf("📢 ADVANTAGE P2!\n");
    }
    printf("---------------------\n");
}

// Function to update the score after a point is won
void update_score(int *score, int *games, int *sets, int other_score, int *other_games) {
    // Case 1: Standard scoring (0-15-30-40)
    if (*score < 3 && other_score < 4) {
        (*score)++;
    }
    // Case 2: From 40-40 (Deuce)
    else if (*score == 3 && other_score == 3) {
        (*score)++; // Score goes to Advantage (4)
    }
    // Case 3: Win the game from 40-less or Advantage
    else if (*score == 3 && other_score < 3) { // Win from 40-less
        // Win the game
        (*games)++;
        *score = 0; // Reset game points
        *other_games = 0; // Reset opponent game points
        printf("\n--- P%d WINS GAME ---\n", (*sets > 0 || *sets == 0) ? 1 : 2); // Simple way to print P1/P2 win
    } else if (*score == 4) { // Win from Advantage
        // Win the game
        (*games)++;
        *score = 0; // Reset game points
        *other_games = 0; // Reset opponent game points
        printf("\n--- P%d WINS GAME ---\n", (*sets > 0 || *sets == 0) ? 1 : 2);
    }
    // Case 4: Opponent had Advantage, now it's Deuce again
    else if (*score == 3 && other_score == 4) {
        *other_games = 3; // Opponent score goes back to 40 (3)
    }
    // Case 5: Standard scoring, but already handled above - this is for clarity
    else {
        (*score)++;
    }

    // Check for a Set Win (6 games, lead by 2)
    if (*games >= 6 && *games >= (*other_games + 2)) {
        (*sets) = *games; // Store the final game count for the set
        *games = 0; // Reset games for the new set
        *other_games = 0;
    }
}
