#include <stdio.h>
#include <stdbool.h>

// Function prototypes
void displayWelcomeMessage();
float calculateSimpleInterest(float principal, float rate, float time);
void displayResult(float principal, float rate, float time, float interest, float total);
bool calculateAgain();

int main() {
    printf("Simple Interest Calculator (Indian Rupees ₹)\n\n");
    
    do {
        displayWelcomeMessage();
        
        // Input variables
        float principal, rate, time;
        
        // Get principal amount
        printf("Enter Principal Amount (₹): ");
        while(scanf("%f", &principal) != 1 || principal <= 0) {
            printf("Invalid input. Please enter a positive number: ");
            while(getchar() != '\n'); // Clear input buffer
        }
        
        // Get annual interest rate
        printf("Enter Annual Interest Rate (%%): ");
        while(scanf("%f", &rate) != 1 || rate <= 0) {
            printf("Invalid input. Please enter a positive number: ");
            while(getchar() != '\n');
        }
        
        // Get time period
        printf("Enter Time Period (in years): ");
        while(scanf("%f", &time) != 1 || time <= 0) {
            printf("Invalid input. Please enter a positive number: ");
            while(getchar() != '\n');
        }
        
        // Calculate simple interest
        float interest = calculateSimpleInterest(principal, rate, time);
        float total = principal + interest;
        
        // Display results
        displayResult(principal, rate, time, interest, total);
        
    } while(calculateAgain());
    
    printf("\nThank you for using the Simple Interest Calculator!\n");
    return 0;
}

void displayWelcomeMessage() {
    printf("\n================================\n");
    printf("   SIMPLE INTEREST CALCULATOR   \n");
    printf("================================\n");
}

float calculateSimpleInterest(float principal, float rate, float time) {
    return (principal * rate * time) / 100;
}

void displayResult(float principal, float rate, float time, float interest, float total) {
    printf("\nCalculation Results:\n");
    printf("--------------------------------\n");
    printf("Principal Amount:      ₹%.2f\n", principal);
    printf("Annual Interest Rate:  %.2f%%\n", rate);
    printf("Time Period:           %.2f years\n", time);
    printf("--------------------------------\n");
    printf("Simple Interest:       ₹%.2f\n", interest);
    printf("Total Amount:          ₹%.2f\n", total);
    printf("--------------------------------\n");
}

bool calculateAgain() {
    char response;
    printf("\nWould you like to calculate again? (y/n): ");
    scanf(" %c", &response);
    while(getchar() != '\n'); // Clear input buffer
    
    return (response == 'y' || response == 'Y');
}
