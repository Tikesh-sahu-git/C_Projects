#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Function prototypes
void displayWelcomeMessage();
int generateRandomNumber(int min, int max);
void playGame(int secretNumber, int maxAttempts);
bool playAgain();

int main() {
    // Seed the random number generator
    srand(time(0));
    
    // Game configuration
    const int MIN_NUMBER = 1;
    const int MAX_NUMBER = 100;
    const int MAX_ATTEMPTS = 10;
    
    printf("Welcome to the Number Guessing Game!\n\n");
    
    do {
        // Generate a random number between MIN_NUMBER and MAX_NUMBER
        int secretNumber = generateRandomNumber(MIN_NUMBER, MAX_NUMBER);
        
        // Display game instructions
        displayWelcomeMessage();
        printf("I'm thinking of a number between %d and %d.\n", MIN_NUMBER, MAX_NUMBER);
        printf("You have %d attempts to guess it.\n\n", MAX_ATTEMPTS);
        
        // Start the game
        playGame(secretNumber, MAX_ATTEMPTS);
        
    } while (playAgain());
    
    printf("\nThanks for playing! Goodbye.\n");
    return 0;
}

void displayWelcomeMessage() {
    printf("================================\n");
    printf("      NUMBER GUESSING GAME      \n");
    printf("================================\n");
}

int generateRandomNumber(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void playGame(int secretNumber, int maxAttempts) {
    int guess;
    int attempts = 0;
    bool hasWon = false;
    
    while (attempts < maxAttempts && !hasWon) {
        printf("Attempt %d/%d - Enter your guess: ", attempts + 1, maxAttempts);
        scanf("%d", &guess);
        
        // Clear input buffer
        while (getchar() != '\n');
        
        attempts++;
        
        if (guess == secretNumber) {
            hasWon = true;
            printf("\nCongratulations! You guessed the number in %d attempts!\n", attempts);
        } else if (guess < secretNumber) {
            printf("Too low! ");
        } else {
            printf("Too high! ");
        }
        
        // Give a hint if not the last attempt and not won yet
        if (!hasWon && attempts < maxAttempts) {
            int remaining = maxAttempts - attempts;
            printf("You have %d %s left.\n", remaining, remaining == 1 ? "attempt" : "attempts");
        }
    }
    
    if (!hasWon) {
        printf("\nGame over! The number was %d.\n", secretNumber);
    }
}

bool playAgain() {
    char response;
    printf("\nWould you like to play again? (y/n): ");
    scanf(" %c", &response);
    
    // Clear input buffer
    while (getchar() != '\n');
    
    return (response == 'y' || response == 'Y');
}
