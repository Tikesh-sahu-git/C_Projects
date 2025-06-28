#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

// Function prototypes
void displayMenu();
float celsiusToFahrenheit(float celsius);
float fahrenheitToCelsius(float fahrenheit);
float celsiusToKelvin(float celsius);
float kelvinToCelsius(float kelvin);
float fahrenheitToKelvin(float fahrenheit);
float kelvinToFahrenheit(float kelvin);
void convertTemperature();
bool continueConversion();

int main() {
    printf("Temperature Converter\n\n");
    
    do {
        displayMenu();
        convertTemperature();
    } while(continueConversion());
    
    printf("\nThank you for using the Temperature Converter!\n");
    return 0;
}

void displayMenu() {
    printf("\n================================\n");
    printf("    TEMPERATURE CONVERTER\n");
    printf("================================\n");
    printf("1. Celsius to Fahrenheit\n");
    printf("2. Fahrenheit to Celsius\n");
    printf("3. Celsius to Kelvin\n");
    printf("4. Kelvin to Celsius\n");
    printf("5. Fahrenheit to Kelvin\n");
    printf("6. Kelvin to Fahrenheit\n");
    printf("================================\n");
}

float celsiusToFahrenheit(float celsius) {
    return (celsius * 9/5) + 32;
}

float fahrenheitToCelsius(float fahrenheit) {
    return (fahrenheit - 32) * 5/9;
}

float celsiusToKelvin(float celsius) {
    return celsius + 273.15;
}

float kelvinToCelsius(float kelvin) {
    return kelvin - 273.15;
}

float fahrenheitToKelvin(float fahrenheit) {
    return celsiusToKelvin(fahrenheitToCelsius(fahrenheit));
}

float kelvinToFahrenheit(float kelvin) {
    return celsiusToFahrenheit(kelvinToCelsius(kelvin));
}

void convertTemperature() {
    int choice;
    float temperature, convertedTemp;
    
    printf("\nEnter your choice (1-6): ");
    while(scanf("%d", &choice) != 1 || choice < 1 || choice > 6) {
        printf("Invalid input. Please enter a number between 1 and 6: ");
        while(getchar() != '\n'); // Clear input buffer
    }
    
    printf("Enter temperature to convert: ");
    while(scanf("%f", &temperature) != 1) {
        printf("Invalid input. Please enter a number: ");
        while(getchar() != '\n');
    }
    
    switch(choice) {
        case 1:
            convertedTemp = celsiusToFahrenheit(temperature);
            printf("\n%.2f°C = %.2f°F\n", temperature, convertedTemp);
            break;
        case 2:
            convertedTemp = fahrenheitToCelsius(temperature);
            printf("\n%.2f°F = %.2f°C\n", temperature, convertedTemp);
            break;
        case 3:
            convertedTemp = celsiusToKelvin(temperature);
            printf("\n%.2f°C = %.2fK\n", temperature, convertedTemp);
            break;
        case 4:
            convertedTemp = kelvinToCelsius(temperature);
            printf("\n%.2fK = %.2f°C\n", temperature, convertedTemp);
            break;
        case 5:
            convertedTemp = fahrenheitToKelvin(temperature);
            printf("\n%.2f°F = %.2fK\n", temperature, convertedTemp);
            break;
        case 6:
            convertedTemp = kelvinToFahrenheit(temperature);
            printf("\n%.2fK = %.2f°F\n", temperature, convertedTemp);
            break;
    }
}

bool continueConversion() {
    char response;
    printf("\nWould you like to perform another conversion? (y/n): ");
    scanf(" %c", &response);
    while(getchar() != '\n'); 
    
    return tolower(response) == 'y';
}
