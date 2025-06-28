#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_QUESTIONS 10
#define MAX_OPTIONS 4

typedef struct {
    char question[256];
    char options[MAX_OPTIONS][100];
    int correctAnswer;
} QuizQuestion;

// Function prototypes
void initializeQuestions(QuizQuestion questions[]);
void displayWelcome();
int playQuiz(QuizQuestion questions[], int numQuestions);
void displayResult(int score, int totalQuestions);
void clearInputBuffer();

int main() {
    QuizQuestion questions[MAX_QUESTIONS];
    int score = 0;
    
    // Seed random number generator
    srand(time(0));
    
    // Initialize questions
    initializeQuestions(questions);
    
    // Display welcome message
    displayWelcome();
    
    // Play the quiz
    score = playQuiz(questions, MAX_QUESTIONS);
    
    // Display final result
    displayResult(score, MAX_QUESTIONS);
    
    return 0;
}

void initializeQuestions(QuizQuestion questions[]) {
    // Question 1
    strcpy(questions[0].question, "What is the correct syntax to print 'Hello World' in C?");
    strcpy(questions[0].options[0], "printf('Hello World');");
    strcpy(questions[0].options[1], "print('Hello World');");
    strcpy(questions[0].options[2], "printf(\"Hello World\");");
    strcpy(questions[0].options[3], "console.log(\"Hello World\");");
    questions[0].correctAnswer = 2; // Option C (index 2)
    
    // Question 2
    strcpy(questions[1].question, "Which of these is not a valid data type in C?");
    strcpy(questions[1].options[0], "int");
    strcpy(questions[1].options[1], "float");
    strcpy(questions[1].options[2], "real");
    strcpy(questions[1].options[3], "char");
    questions[1].correctAnswer = 2; // Option C (index 2)
    
    // Question 3
    strcpy(questions[2].question, "What is the size of an 'int' data type in C (typically)?");
    strcpy(questions[2].options[0], "1 byte");
    strcpy(questions[2].options[1], "2 bytes");
    strcpy(questions[2].options[2], "4 bytes");
    strcpy(questions[2].options[3], "Depends on the compiler");
    questions[2].correctAnswer = 3; // Option D (index 3)
    
    // Question 4
    strcpy(questions[3].question, "Which operator is used to access the memory address of a variable?");
    strcpy(questions[3].options[0], "*");
    strcpy(questions[3].options[1], "&");
    strcpy(questions[3].options[2], "#");
    strcpy(questions[3].options[3], "@");
    questions[3].correctAnswer = 1; // Option B (index 1)
    
    // Question 5
    strcpy(questions[4].question, "What does the '++' operator do?");
    strcpy(questions[4].options[0], "Adds 2 to the variable");
    strcpy(questions[4].options[1], "Increments the variable by 1");
    strcpy(questions[4].options[2], "Doubles the value of the variable");
    strcpy(questions[4].options[3], "Returns the square of the variable");
    questions[4].correctAnswer = 1; // Option B (index 1)
    
    // Question 6
    strcpy(questions[5].question, "Which header file is required for dynamic memory allocation?");
    strcpy(questions[5].options[0], "<stdio.h>");
    strcpy(questions[5].options[1], "<stdlib.h>");
    strcpy(questions[5].options[2], "<math.h>");
    strcpy(questions[5].options[3], "<string.h>");
    questions[5].correctAnswer = 1; // Option B (index 1)
    
    // Question 7
    strcpy(questions[6].question, "What is the correct way to declare a pointer?");
    strcpy(questions[6].options[0], "int ptr;");
    strcpy(questions[6].options[1], "int *ptr;");
    strcpy(questions[6].options[2], "*int ptr;");
    strcpy(questions[6].options[3], "ptr int;");
    questions[6].correctAnswer = 1; // Option B (index 1)
    
    // Question 8
    strcpy(questions[7].question, "Which loop is guaranteed to execute at least once?");
    strcpy(questions[7].options[0], "for loop");
    strcpy(questions[7].options[1], "while loop");
    strcpy(questions[7].options[2], "do-while loop");
    strcpy(questions[7].options[3], "All of the above");
    questions[7].correctAnswer = 2; // Option C (index 2)
    
    // Question 9
    strcpy(questions[8].question, "What is the default return type of a function in C?");
    strcpy(questions[8].options[0], "void");
    strcpy(questions[8].options[1], "int");
    strcpy(questions[8].options[2], "float");
    strcpy(questions[8].options[3], "char");
    questions[8].correctAnswer = 1; // Option B (index 1)
    
    // Question 10
    strcpy(questions[9].question, "Which function is used to compare two strings?");
    strcpy(questions[9].options[0], "strcmp()");
    strcpy(questions[9].options[1], "strcompare()");
    strcpy(questions[9].options[2], "stringcmp()");
    strcpy(questions[9].options[3], "compare()");
    questions[9].correctAnswer = 0; // Option A (index 0)
}

void displayWelcome() {
    printf("================================\n");
    printf("       C PROGRAMMING QUIZ       \n");
    printf("================================\n");
    printf("Test your knowledge of C programming!\n");
    printf("There are %d multiple-choice questions.\n", MAX_QUESTIONS);
    printf("Enter the letter (A, B, C, D) of your answer.\n\n");
}

int playQuiz(QuizQuestion questions[], int numQuestions) {
    int score = 0;
    char answer;
    
    for (int i = 0; i < numQuestions; i++) {
        printf("\nQuestion %d/%d: %s\n", i+1, numQuestions, questions[i].question);
        
        // Display options
        for (int j = 0; j < MAX_OPTIONS; j++) {
            printf("%c) %s\n", 'A' + j, questions[i].options[j]);
        }
        
        // Get user answer
        printf("\nYour answer (A-D): ");
        scanf(" %c", &answer);
        clearInputBuffer();
        
        // Convert to uppercase and validate
        answer = toupper(answer);
        while (answer < 'A' || answer > 'D') {
            printf("Invalid input. Please enter A, B, C, or D: ");
            scanf(" %c", &answer);
            clearInputBuffer();
            answer = toupper(answer);
        }
        
        // Check if answer is correct
        if (answer - 'A' == questions[i].correctAnswer) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Incorrect! The correct answer is %c.\n", 
                  'A' + questions[i].correctAnswer);
        }
    }
    
    return score;
}

void displayResult(int score, int totalQuestions) {
    printf("\n================================\n");
    printf("          QUIZ RESULTS         \n");
    printf("================================\n");
    printf("Your score: %d/%d\n", score, totalQuestions);
    printf("Percentage: %.1f%%\n", (float)score / totalQuestions * 100);
    
    if (score == totalQuestions) {
        printf("Perfect! You're a C expert!\n");
    } else if (score >= totalQuestions * 0.7) {
        printf("Great job! You know C well!\n");
    } else if (score >= totalQuestions * 0.5) {
        printf("Good effort! Keep learning!\n");
    } else {
        printf("Keep practicing! You'll improve!\n");
    }
    printf("================================\n");
}

void clearInputBuffer() {
    while (getchar() != '\n');
}
