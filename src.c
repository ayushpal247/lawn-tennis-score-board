#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global Variables to track the score
int player1_point = 0;
int player2_point = 0;
int player1_game = 0;
int player2_game = 0;
int player1_set = 0;
int player2_set = 0;

// --- Function Prototypes ---
void display_score();
void update_points(int winner);
void check_game_win(int winner);
void check_set_win(int winner);
const char* get_point_display(int score);
int check_match_end();

// ===============================================
// 1. POINT DISPLAY CONVERSION
// ===============================================

// Converts the internal integer point score into the tennis-specific string (Love, 15, 30, 40, Ad)
const char* get_point_display(int score) {
    if (score <= 3) {
        switch (score) {
            case 0: return "Love";
            case 1: return "15";
            case 2: return "30";
            case 3: return "40";
        }
    }
    // Scores of 4+ are handled by Deuce/Advantage logic in display_score,
    // but this prevents errors if this function is called at those scores.
    return "N/A";
}

// ===============================================
#include <stdio.h>
#include <stdlib.h> // Required for system() and file operations

// ... (Keep all your global variables, check_match_end, 
//      get_point_display, check_game_win, update_points functions) ...

// --- REVISED display_score() FUNCTION ---
void display_score() {
    // 1. Open the file for writing (and overwrite existing content)
    FILE *fp = fopen("scoreboard.html", "w"); 
    
    if (fp == NULL) {
        printf("Error: Could not open scoreboard.html for writing.\n");
        return;
    }

    // Determine the point display strings (including Deuce/Adv logic)
    const char *p1_point_str;
    const char *p2_point_str;
    
    // Deuce/Advantage Logic for Display
    if (player1_point >= 3 && player2_point >= 3) {
        if (player1_point == player2_point) {
            p1_point_str = "Deuce";
            p2_point_str = "Deuce";
        } else if (player1_point > player2_point) {
            p1_point_str = "Advantage";
            p2_point_str = get_point_display(player2_point); // Shows the opposing player's score
        } else {
            p1_point_str = get_point_display(player1_point); // Shows the opposing player's score
            p2_point_str = "Advantage";
        }
    } else {
        p1_point_str = get_point_display(player1_point);
        p2_point_str = get_point_display(player2_point);
    }

    // 2. Write the HTML file content
    fprintf(fp, "<!DOCTYPE html>\n");
    fprintf(fp, "<html>\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "    <title>Tennis Scoreboard</title>\n");
    fprintf(fp, "    <style>\n");
    fprintf(fp, "        body { font-family: Arial, sans-serif; background-color: #f0f0f0; text-align: center; }\n");
    fprintf(fp, "        h1 { color: #006400; }\n");
    fprintf(fp, "        table { width: 60%%; margin: 20px auto; border-collapse: collapse; box-shadow: 0 4px 8px rgba(0,0,0,0.1); background-color: white; }\n");
    fprintf(fp, "        th, td { border: 1px solid #ddd; padding: 12px; text-align: center; }\n");
    fprintf(fp, "        th { background-color: #008000; color: white; font-size: 1.1em; }\n");
    fprintf(fp, "        .highlight { font-weight: bold; color: #cc0000; }\n");
    fprintf(fp, "    </style>\n");
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");
    fprintf(fp, "    <h1>🎾 Live Tennis Scoreboard 🎾</h1>\n");
    
    // HTML Table structure
    fprintf(fp, "    <table>\n");
    fprintf(fp, "        <thead>\n");
    fprintf(fp, "            <tr><th>Player</th><th>Sets</th><th>Games</th><th>Points</th></tr>\n");
    fprintf(fp, "        </thead>\n");
    fprintf(fp, "        <tbody>\n");
    
    // Player 1 Row
    fprintf(fp, "            <tr>\n");
    fprintf(fp, "                <td>Player 1</td>\n");
    fprintf(fp, "                <td>%d</td>\n", player1_set);
    fprintf(fp, "                <td>%d</td>\n", player1_game);
    // Highlight points if they are Deuce or Advantage
    fprintf(fp, "                <td class=\"%s\">%s</td>\n", 
            (strstr(p1_point_str, "Deuce") || strstr(p1_point_str, "Advantage")) ? "highlight" : "", 
            p1_point_str);
    fprintf(fp, "            </tr>\n");
    
    // Player 2 Row
    fprintf(fp, "            <tr>\n");
    fprintf(fp, "                <td>Player 2</td>\n");
    fprintf(fp, "                <td>%d</td>\n", player2_set);
    fprintf(fp, "                <td>%d</td>\n", player2_game);
    // Highlight points if they are Deuce or Advantage
    fprintf(fp, "                <td class=\"%s\">%s</td>\n", 
            (strstr(p2_point_str, "Deuce") || strstr(p2_point_str, "Advantage")) ? "highlight" : "", 
            p2_point_str);
    fprintf(fp, "            </tr>\n");
    
    fprintf(fp, "        </tbody>\n");
    fprintf(fp, "    </table>\n");
    
    // Add instruction for refreshing
    fprintf(fp, "    <p>Refresh your browser to see the updated score after each point!</p>\n");
    
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");

    // 3. Close the file
    fclose(fp);

    // 4. Print message to the console for the user
    printf("\n*** Score updated! Check 'scoreboard.html' and refresh your browser. ***\n\n");
}

void check_set_win(int winner) {
    int p1 = player1_game;
    int p2 = player2_game;

    // A set is won if a player reaches 6 games AND has a 2-game margin.
    // NOTE: This basic version does NOT implement a Tie-Break at 6-6.
    if ((p1 >= 6 || p2 >= 6) && (abs(p1 - p2) >= 2)) {
        if (winner == 1) {
            player1_set++;
            printf("\n*** Player 1 wins the Set! (%d-%d) ***\n", p1, p2);
        } else {
            player2_set++;
            printf("\n*** Player 2 wins the Set! (%d-%d) ***\n", p1, p2);
        }

        // Reset game scores for the new set
        player1_game = 0;
        player2_game = 0;
    }
}

// ===============================================
// 4. GAME WIN LOGIC
// ===============================================

void check_game_win(int winner) {
    int p1 = player1_point;
    int p2 = player2_point;

    // Check for game win (4+ points AND 2-point margin)
    if ((p1 >= 4 || p2 >= 4) && (abs(p1 - p2) >= 2)) {
        
        // Reset point scores for the new game
        player1_point = 0;
        player2_point = 0;
        
        if (winner == 1) {
            player1_game++;
            printf("\n*** Player 1 wins the Game! ***\n");
            check_set_win(1);
        } else {
            player2_game++;
            printf("\n*** Player 2 wins the Game! ***\n");
            check_set_win(2);
        }
    }
}

// ===============================================
// 5. POINT UPDATE LOGIC
// ===============================================

void update_points(int winner) {
    int* winner_ptr = (winner == 1) ? &player1_point : &player2_point;
    int* loser_ptr = (winner == 1) ? &player2_point : &player1_point;

    // --- Core Deuce/Advantage Logic ---
    if (*winner_ptr >= 3 && *loser_ptr >= 3) {
        if (*winner_ptr == *loser_ptr) {
            // Case 1: Score is Deuce (3-3, 4-4, etc.). Winner gets Advantage.
            (*winner_ptr)++;
        } else if (*winner_ptr > *loser_ptr) {
            // Case 2: Winner has Advantage. They win the Game.
            (*winner_ptr)++; 
        } else {
            // Case 3: Loser had Advantage. Score returns to Deuce.
            // Example: P1=4(Adv), P2=3(40). P2 wins point. P1_point becomes 3, P2_point becomes 4.
            // Oh wait, no. The score needs to be equalized back to 3 (Deuce).
            // Example: P1=4(Adv), P2=3. If P2 wins, P1 point decreases, P2 point increases, resulting in 3-3 (Deuce).
            (*loser_ptr)--; // The player who previously had the Adv loses their extra point (returns to 40)
            (*winner_ptr)++; // The current point winner gets 40
            // Since we only track >=3, the actual internal scores should be 3-3 (Deuce) 
            // after the change. Let's simplify the logic to avoid negative scores.

            // Simpler: If score is 4-3 (Adv) and LOSER wins, return to 3-3 (Deuce)
            if (abs(*winner_ptr - *loser_ptr) == 1) {
                 *winner_ptr = 3;
                 *loser_ptr = 3;
            } else {
                // If not in Deuce/Adv situation, just increment point
                (*winner_ptr)++;
            }
        }
    } else {
        // Standard score progression (0-15, 15-30, 30-40)
        (*winner_ptr)++;
    }

    // After updating the point, check if a game was won
    check_game_win(winner);
}

// ===============================================
// 6. MATCH END CHECK
// ===============================================

// Basic Best-of-3 Match check
int check_match_end() {
    if (player1_set == 2) {
        printf("\n!!! PLAYER 1 WINS THE MATCH (%d sets to %d) !!!\n", player1_set, player2_set);
        return 1;
    }
    if (player2_set == 2) {
        printf("\n!!! PLAYER 2 WINS THE MATCH (%d sets to %d) !!!\n", player2_set, player1_set);
        return 1;
    }
    return 0;
}

// ===============================================
// 7. MAIN FUNCTION (The execution loop)
// ===============================================

int main() {
    int winner;

    printf("Welcome to the C Tennis Scoreboard!\n");
    printf("The match is Best-of-3 Sets.\n");

    while (!check_match_end()) {
        display_score();

        printf("\nEnter the winner of the last point (1 for Player 1, 2 for Player 2, 0 to quit): ");
        if (scanf("%d", &winner) != 1) {
            printf("\n*** Invalid input. Please enter a number. ***\n");
            // Clear the input buffer to prevent infinite loop
            while (getchar() != '\n');
            continue;
        }

        if (winner == 1 || winner == 2) {
            update_points(winner);
        } else if (winner == 0) {
            printf("\nExiting scoreboard. Final score: P1 Sets %d, P2 Sets %d\n", player1_set, player2_set);
            break;
        } else {
            printf("\n*** Invalid input. Please enter 1, 2, or 0. ***\n");
        }
    }

    printf("\nThank you for using the scoreboard!\n");
    return 0;
}