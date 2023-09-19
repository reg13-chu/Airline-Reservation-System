#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//struct for the seat row and column
typedef struct passengerSeat
{
    int row;
    char column[3];
}passengerSeat;

//for the passengers information
typedef struct passengerInfo
{
    char name[15];
    int age;
    char address[15];
    struct passengerSeat seat;
}passengerInfo; 

//declaring the neccessary function
void addPassenger();
void mainMenu();

//Displaying the Passengers Informatin
void listOfPassengers()
{
    printf("==============================================================\n");
    system("cls || clear");
    printf("\tRS Airline Reservation System\n");
    printf("==============================================================\n");

    FILE *fp = fopen("passengers.txt", "rt");

    passengerInfo info;

    if(fp == NULL)
    {
        printf("\n File not found\n");
        exit(1);
    }

    printf("\n\t\t\t\t\t\tLIST OF PASSENGERS\n\n");
    printf("\tPassenger Name\t\tAge\t\tAddress\t\t\t  Seat No.\n\n");
    
    while (fscanf(fp, "%s%d%s%d%s", info.name, &info.age, info.address, &info.seat.row, info.seat.column) != EOF)
    {
        printf("\t%s    ", info.name);
        printf("\t\t\t%d", info.age);
        printf("\t\t%s      ", info.address);
        printf("\t\t%d", info.seat.row);
        printf("%s\n", info.seat.column);
    }

    fclose(fp);

    char choice;
    printf("\nEnter any key to go back to the menu ");
    printf(">> ");
    scanf(" %c", &choice);
    mainMenu();
}

//for cancelling the passengers reservation
void cancelReservation()
{   
    system("cls || clear");
    printf("=============================================\n");
    printf("\tRS Airline Reservation System\n");
    printf("=============================================\n");

    printf("\n\tCANCEL RESERVATION\n\n");
    passengerInfo cancel;
    
    printf("Enter name\n");
    printf(">> ");
    scanf("%s", cancel.name);
    printf("Enter Age\n");
    printf(">> ");
    scanf("%d", &cancel.age);
    printf("Enter Address\n");
    printf(">> ");
    scanf("%s", cancel.address);
    printf("Enter passenger current seat (e.g. 1A)\n");
    printf(">> ");
    scanf("%d%s", &cancel.seat.row, cancel.seat.column);

    // Validate row and column inputs here if needed.

    FILE* file = fopen("passengers.txt", "r");
    FILE* tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error in opening files.\n");
        return;
    }

    passengerInfo info;
    bool found = false;

    while (fscanf(file, "%s %d %s %d %s", info.name, &info.age, info.address, &info.seat.row, info.seat.column) == 5) 
    {
        if (info.seat.row == cancel.seat.row &&
            info.seat.column[0] == cancel.seat.column[0]) {
            // Reservation found and canceled
            found = true;
            continue; // Skip writing this record to tempFile
        }

        fprintf(tempFile, "%s %d %s %d %s\n", info.name, info.age, info.address, info.seat.row, info.seat.column);
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        remove("temp.txt"); // If the reservation was not found, delete the temp file
        printf("Reservation not found.\n");
        return;
    }

    remove("passengers.txt");
    rename("temp.txt", "passengers.txt");
    printf("Reservation canceled successfully.\n");

    char menu;
    printf("\nEnter any key to go back to the menu >> ");
    scanf(" %c", &menu);
    mainMenu();
}

//Changing the Seat Number of the Passenger
void changeSeatNum()
{
    system("cls || clear");
    printf("=============================================\n");
    printf("\tRS Airline Reservation System\n");
    printf("=============================================\n");

    printf("\n\t\t\tCHANGING SEAT NUMBER\n\n");

    FILE *fp = fopen("passengers.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if(fp == NULL || tempFile == NULL)
    {
        printf("\n Can't open the file\n");
        exit(1);
    }

    passengerInfo info;
    int r = 0;

    while (fscanf(fp, "%s %d %s %d %s", info.name, &info.age, info.address, &info.seat.row, info.seat.column) != EOF)
    {
        r++;
    }

    rewind(fp);
    
    char name[20];
    int age;
    char address[20];
    int currentSeatRow;
    char currentSeatCol[10];
    
    printf("Enter name\n");
    printf(">> ");
    scanf("%s", name);
    printf("Enter Age\n");
    printf(">> ");
    scanf("%d", &age);
    printf("Enter Address\n");
    printf(">> ");
    scanf("%s", address);
    printf("Enter passenger current seat (e.g. 1A)\n");
    printf(">> ");
    scanf("%d%s", &currentSeatRow, currentSeatCol);

    int found = 0;
    char menu;

    while (fscanf(fp, "%s %d %s %d %s", info.name, &info.age, info.address, &info.seat.row, info.seat.column) != EOF)
    {
        if(strcmp(info.name, name) == 0 && info.age == age && strcmp(info.address, address) == 0 && info.seat.row == currentSeatRow && strcmp(info.seat.column, currentSeatCol) == 0)
        {
            found = 1;
            printf("Enter a new seat (e.g. 1A)\n");
            printf(">> ");
            scanf("%d%s", &info.seat.row, info.seat.column);
        }  
        fprintf(tempFile, "%s %d %s %d%s\n", info.name, info.age, info.address, info.seat.row, info.seat.column);
    }

    fclose(fp);
    fclose(tempFile);

    if (found)
    {
        rename("temp.txt", "passengers.txt");
        printf("Seat number updated successfully\n\n");
        printf("Enter any key to go back to the menu >> ");
        scanf(" %c", &menu);
        mainMenu();
    }
    else
    {
        remove("temp.txt");
        printf("Passenger not found.\n");
        exit(1);
    }
}   

//displaying the Seat Map
void availableSeats()
{
    int i, j;
    char menu;
    FILE* file = fopen("passengers.txt", "r");

    if (file == NULL) {
        printf("Error in opening the file.\n");
        return;
    }

    char seats[5][4];
    for (i = 0; i < 5; i++) 
    {
        for (j = 0; j < 4; j++) 
        { 
            seats[i][j] = ' '; 
        }
    }

    passengerInfo info;
    while (fscanf(file, "%s%d%s%d%s", info.name, &info.age, info.address, &info.seat.row, info.seat.column) == 5) 
    {
        int row = info.seat.row - 1; 
        int col = info.seat.column[0] - 'A';
        seats[row][col] = 'X';  
    }

    printf("\t\tAVAILABLE SEATS\n\n");

    for (i = 0; i < 5; i++) 
    {
        printf("      %d", i + 1); 
        for (j = 0; j < 4; j++) 
        { 
            if (seats[i][j] == 'X') 
            { 
                printf("\t    X  ");
            } else {
                printf("\t    %c  ", 'A' + j);  
            }
        }
        printf("\n");
    }

    fclose(file);

    printf("\nEnter any key to go back to the menu >> ");
    scanf(" %c", &menu);
    mainMenu();
}

//once the client agree to the seat, get their information
void gettingPassengerInfo(const struct passengerSeat *seat)
{
    system("cls || clear");
    
    FILE *file;
    file = fopen("passengers.txt", "a");
    
    printf("=============================================\n");
    printf("\tRS Airline Reservation System\n");
    printf("=============================================\n\n");

    passengerInfo info;

    printf("GETTING PASSENGER'S INFORMATION\n\n");

//Asking the client to input their information for reservation
    printf("Enter passenger's name\n");
    printf(">> ");
    scanf("%s", info.name);
    printf("Enter passenger's age\n");
    printf(">> ");
    scanf("%d", &info.age);
    printf("Enter passenger's address\n");
    printf(">> ");
    scanf("%s", info.address);

//checking if the information entered is correct
    system("cls || clear");
    printf("=============================================\n");
    printf("\tRS Airline Reservation System\n");
    printf("=============================================\n\n");
    
    printf("\tPASSENGER'S INFORMATION\n\n");
    printf("Name: %s\n", info.name);
    printf("Age: %d\n", info.age);
    printf("Address: %s\n", info.address);
    printf("Seat: %d%s\n", seat->row, seat->column);

//if the information is correct, enter continue
    char choice;
    char menu;
    printf("\nContinue\n");
    printf(">> ");
    scanf(" %c", &choice);

    if(choice == 'Y' || choice == 'y')
    {
        fprintf(file,"%s", info.name);
        fprintf(file,"\t\t%d", info.age);
        fprintf(file,"\t%s", info.address);
        fprintf(file,"\t\t%d%s\n", seat->row, seat->column);
        fclose(file);
        printf("Seat successfully reserved.\n\n");
        printf("See the Available Seats to see the check the passenger's reservation.\n\n");
        printf("Enter any key to go back to the menu >> ");
        scanf(" %c", &menu);
        mainMenu();
    } 
    else{
        gettingPassengerInfo(seat);
    }

}

//Checking the Availability of the Seat
void isSeatAvailable(const struct passengerSeat *seat)
{
    passengerSeat recordSeat;
    passengerInfo recordInfo;
    FILE *fp;
    fp = fopen("passengers.txt", "r");

    system("cls || clear");
    printf("=============================================\n");
    printf("\tRS Airline Reservation System\n");
    printf("=============================================\n\n");

    if(fp == NULL)
    {
        printf("Could not open the file.\n");
        exit(1);
    }

//check if the seat has been input already
    bool isSeatAvailable = true;
    while(fscanf(fp,"%s%d%s%d%s", recordInfo.name, &recordInfo.age, recordInfo.address, &recordSeat.row, recordSeat.column) == 5)
    {
        if(recordSeat.row == seat->row && recordSeat.column == seat->column)
        {
            isSeatAvailable = false;
            break;
        }
    }

    fclose(fp);

    char choice;
    if(isSeatAvailable)
    {
        printf("The seat is available.\n\n");
        printf("Would you like to reserve it?\n");
        printf(">> ");
        scanf(" %c", &choice);
        if(choice == 'Y' || choice == 'y')
        {
            gettingPassengerInfo(seat);
        }
    }
    else
    {
        printf("The seat is already occupied.\n");
        printf("Would you like to choose another seat?\n");
        printf(">> ");
        scanf(" %c", &choice);
        if(choice == 'Y' || choice == 'y')
        {
            addPassenger();
        }
    }
}   

//ADDING PASSENGER
void addPassenger()
{
    passengerSeat seat;
    
    system("cls || clear"); //clear the console
    printf("=============================================\n");
    printf("\tRS Airline Reservation System\n");
    printf("=============================================\n");

    printf("\n");
    bool isValid = false;
    while(isValid == false)
    {
        printf("Enter your desired seat (ex. 1A)\n");
        printf(">> ");
        scanf("%d%s", &seat.row, seat.column);
    
      //Check if the user entered a valid row and column
        if(seat.row >= 1 && seat.row <= 5)
        {
            if(strlen(seat.column) == 1 && seat.column[0] >= 'A' && seat.column[0] <= 'D')
            {
                isValid = true;
                isSeatAvailable(&seat);
            }
        }
        else
        {
            printf("Seat is unavailable.\n");
            printf("Please enter a valid seat\n\n");
        }
    }
}

//MAIN MENU
void mainMenu()
{
    int choice;

    system("cls || clear");
    printf("=============================================\n");
    printf("\tAirline Reservation System\n");
    printf("=============================================\n");

    printf("\n");
    printf("\t\tMAIN MENU\n\n");

    printf("\t[1] AVAILABLE SEATS\n");
    printf("\t[2] ADD PASSENGER\n");
    printf("\t[3] CHANGE SEAT NUMBER\n");
    printf("\t[4] CANCEL RESERVATION\n");
    printf("\t[5] LIST OF PASSENGERS\n");
    printf("\t[6] EXIT\n");

    printf("\n");
    printf("\t>> ");
    scanf("%d", &choice);

//check the user input in the menu
    switch(choice)
    {
        case 1:
            system("cls || clear");
            printf("=============================================\n");
            printf("\tRS Airline Reservation System\n");
            printf("=============================================\n\n");
            availableSeats();
            break;
        case 2:
            addPassenger();
            break;
        case 3:
            changeSeatNum();
            break;
        case 4:
            cancelReservation();
            break;
        case 5:
            listOfPassengers();
            break;
        case 6:
            printf("Exiting...");
            exit(1);
        default:
            printf("\n\t\t\tInvalid choice. \n\t\t\tPlease enter a valid choice\n");
            mainMenu();   
    }
}   
    
int main(void) {

    char inputUsername[15];
    char inputPassword[15];

    printf("=============================================\n");
    printf("\tRS Airline Reservation System\n");
    printf("=============================================\n");

    //Log In to the System
    printf("\n");
    printf("\t\tSystem Log In\n\n");

    printf("Username\n");
    printf(">> ");
    scanf("%s", inputUsername);
    printf("Password\n");
    printf(">> ");
    scanf("%s", inputPassword);

    //compare if the password and username match
    char menu;
    if(strcmp(inputUsername, "rsadmin1304") == 0 && strcmp(inputPassword,     "rsairline@13") == 0)
    {
        printf("\n\nLog In Successfully\n");
        printf("Enter any key to go to the menu >>");
        scanf(" %c", &menu);
        mainMenu();
    }
    else
    {
        system("cls || clear");
        printf("\nSorry! Incorrect username and password\n");
        exit(1);
    }

    mainMenu();
}   