#include <stdio.h>
#include <stdbool.h>
#include <string.h>
// GLOB
int nbr_users = 0;
int nbr_flights = 0;
struct User{
    char fullname[2][16];
    char pseudo[9];
    char pin[9];
};
struct Flight {
    int id;
    char destination[15][15];
    char day[10];
    char hour[8];
    char duration[6];
    int price;
    int seat;
};

struct User activeUser;
bool adminconnected = false;

// Initialization of all functions
void fileInit();
void updateUserNumber();
void UpdateFlightNumber();

void clearScreen();
void clearInput();

void welcomePage();

void loginMenu( bool test);
// user related functions
void signup();
void saveUser(struct User newUser);
void userLogin(bool test);
bool checkUserLogin(char credentials[9][9]);
void userMode();
void bookFlight();
struct Flight findOneFlight(bool t, int a);
void seeReservations(char fn[16], char ln[16], char username[9], bool t);
void cancelReservation();
//void researchFlight();
// admin related functions
void adminlogin(bool test);
void adminMode();
void addFlight();
void updateFlight();
void deleteFlight();
void flightStatus();
void sortFlights();
void showUsers();
void showPassengers();
void searchPassenger();
void renitializePassenger();
void adminLogout();
void showFlights(bool isAdmin);

void exitProgram();

void main (){
    fileInit();
    welcomePage();
    clearScreen();
    loginMenu(false);
}
void fileInit(){ //imitialize the number of flights and users
    // initialize numbers of users
    FILE *fp;
    fp = fopen("users/nbr_users", "r");
    if (fp == NULL){
        nbr_users = 0;
    }else{
    fscanf(fp, "%d", &nbr_users);
    fclose(fp);
    }

    // initialize numbers of flight
    FILE *fp1;
    fp1 = fopen("flights/nbr_flights", "r");
    if (fp1 == NULL){
        nbr_flights = 0;
    }else{
    fscanf(fp1, "%d", &nbr_flights);
    fclose(fp1);
    }
}

void updateUserNumber(){
    nbr_users++;
    FILE *nfp = fopen("users/nbr_users", "w");
    fprintf(nfp, "%d", nbr_users);
    fclose(nfp);
}
void UpdateFlightNumber(){
    nbr_flights++;
    FILE *nfp = fopen("flights/nbr_flights", "w");
    fprintf(nfp, "%d", nbr_flights);
    fclose(nfp);
}

void clearScreen(){
    //clear screen
    system("clear");
}

void clearInput(){
    ///clear standard input
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void welcomePage(){
    printf("\n\n\n\n");
    printf("\t\t\t\t FLIGHTS BOOKING SYSTEM\n");
    printf("\t\t\t\t_________________________");
    printf("\n\n\t\t\t\t  Welcome to our program :)");
    printf("\n\n\n\n\tDevelopped by: \n\n\n\t\t KAMBALE NGUNUNU DANIEL\t\t(+250) 783575014\n\t\tE-mail: kambalengununudaniel@gmail.com\n\t\tt\t");

    printf("\n\n\n\n\nPress enter to continue...");
    getchar();

}

void loginMenu( bool test){
    char choice;
    printf("\n\n\n\n\n\t\t\t\t\t LOGIN MENU\n");
    printf("\t\t\t\t\t____________");
    printf("\n\n\n\t\t0. Sign up\n\n\t\t1. Login\n\n\t\t2. Login as admin\n\n\t\te. Exit\n\n\n");
    if (test){printf("\n\tInvalid choice !!!    Choose again: ");}else{    printf("\n    Enter your choice: ");}
    choice = getchar();
    switch(choice){
        case 'e':
            clearScreen();
            exitProgram();
        case '0':
            clearScreen();
            signup();
            break;
        case '1':
            clearScreen();
            userLogin(true);
            break;
        case '2':
            clearScreen();
            adminlogin(true);
            break;
        default:
            clearScreen();
            loginMenu(true);
            break;
    }
}

void signup(){
    struct User newUser;
    printf("\n\n\n\t\t\t\t\t Sign up\n");
    printf("\t\t\t\t\t_________\n");

    printf("\nType your first name: ");
    scanf("%15s", newUser.fullname[0]);

    printf("\nType your last name: ");
    scanf("%15s", newUser.fullname[1]);

    printf("\nChoose a pseudo (Max 8 characters): ");
    scanf("%8s", newUser.pseudo);

    printf("\nChoose a PIN: ");
    scanf("%s", newUser.pin);

    printf("\n\nFull name: %s %s \nPseudo: %s \nPIN: %s", newUser.fullname[0], newUser.fullname[1], newUser.pseudo, newUser.pin);
    printf("\n\nend ");
    saveUser(newUser);
}

void saveUser(struct User newUser){
    // if there's is  no users, just write the first element
    if (nbr_users<1){
        FILE *ufp;
        ufp = fopen("users/users", "wb");
        fwrite(&newUser, sizeof(struct User), 1, ufp);
        fclose(ufp);
        //update the index of the number of users
        updateUserNumber();
    }else{ // else append the new user
        FILE *ufpp;
        ufpp = fopen("users/users", "ab");
        fwrite(&newUser, sizeof(struct User), 1, ufpp);
        fclose(ufpp);
        //update the index of the number of users
        updateUserNumber();
    }
    clearScreen();
    userLogin(true);
}
void userLogin(bool test){
    char credential[9][9];
    printf("\n\n\t\t\t\t\t LOGIN PAGE\n");
    printf("\t\t\t\t\t____________\n\n\n");

    if(!test){ printf("Wrong credentials, try again\n"); }

    printf("\t Enter your credentials ");
    printf("\n\t\t username: ");
    scanf("%s", credential[0]);

    printf("\n\t\t password: ");
    scanf("%s", credential[1]);
    test = checkUserLogin(credential);
    clearInput();
    if (test){
        clearScreen();
        userMode();
    }else{

        clearScreen();
        userLogin(test);
    }
}
bool checkUserLogin(char credentials[9][9]){
    struct User foundUser;
    FILE * fp = fopen("users/users", "rb");
    if (fp == NULL){
        printf("No registerd user !");
        return false;
    }else{
        while (fread(&foundUser, sizeof(struct User), 1, fp)) {
            if (strcmp(foundUser.pseudo, credentials[0]) == 0 && strcmp(foundUser.pin, credentials[1]) == 0 ){
                strcpy(activeUser.fullname[0], foundUser.fullname[0]);
                strcpy(activeUser.fullname[1], foundUser.fullname[1]);
                strcpy(activeUser.pseudo, foundUser.pseudo);
                fclose(fp);
                return true;
            }
    }
    fclose(fp);
}
    return false;
}

void userMode(){
    char choice;
    printf("\n\n\t\t\t\t USER HOME");
    printf("\n\t\t\t\t___________\n");
    printf("\n\tLogged in as:  %s %s (@%s)\n\n\n", activeUser.fullname[0], activeUser.fullname[1], activeUser.pseudo);
   // printf("\n\t\t1. Book flight\n\t\t2. See my reservations\n\t\t3. Cancel reservation\n\t\t4. Research flights\n\t\t5. Show all flights\n\n\n\t\t0. Logout\n\t\te. Exit the program\n");
    printf("\n\t\t1. Book flight\n\t\t2. See my reservations\n\t\t3. Cancel reservation\n\t\t4. Show all flights\n\t\t5. See flights sorted by price\n\n\n\t\t0. Logout\n\t\te. Exit the program\n");
    printf("Enter your choice: ");

    choice = getchar();

    switch (choice){
        case '0':
            clearScreen();
            loginMenu(false);
            break;
        case 'e':
            clearScreen();
            exitProgram();
            break;
        case '1':
            clearScreen();
            bookFlight();
            break;
        case '2':
            clearScreen();
            seeReservations(activeUser.fullname[0], activeUser.fullname[1], activeUser.pseudo, true);
            break;
        case '3':
            clearScreen();
            cancelReservation();
            break;
            //If we finish on time we could see if we can add this feature;
        /*case '4':
            clearScreen();
            researchFlight();
            break;*/
        case '4':
            clearScreen();
            showFlights(false);
            break;
        case '5':
            clearScreen();
            sortFlights();
            break;
    }
    clearInput();
    clearScreen();
    userMode();
}

void bookFlight(){
    struct Flight allFlights;
    int i = 1;
    int c,t;
    FILE * fp = fopen("flights/flights", "rb");

    printf("\n\t\t\t\t BOOK A FLIGHT");
    printf("\n\t\t\t\t________________\n\n");

    if (fp == NULL){
        printf("No flights available");
        return;
    }else{
        while (fread(&allFlights, sizeof(struct Flight), 1, fp)) {
            printf("\n\t%d. %s - %s: %s, at %s (Duration: %s), Price: %d", i, allFlights.destination[0], allFlights.destination[1], allFlights.day, allFlights.hour, allFlights.duration, allFlights.price ); i++;
        }
        fclose(fp);
        printf("\n\n Choose a flight :");
        scanf("%d", &c);
        // this function return the flight with the given ID
        allFlights =  findOneFlight(false, c);

        printf("\nYou choose: ");
        printf("\n\t%s - %s: %s, at %s (Duration: %s), Price: %d", allFlights.destination[0], allFlights.destination[1], allFlights.day, allFlights.hour, allFlights.duration, allFlights.price );
        printf("\n\n\tType 1 to confirm, and any other key to cancel..");
        scanf("%d", &t);
        if ( t == 1 ){
            FILE *ufpp;
            ufpp = fopen("flights/passengers", "a");
            // save fullname[0] fullname[1] pseudo code_of_the_flight
            fprintf(ufpp, "%s %s %s %d\n", activeUser.fullname[0], activeUser.fullname[1], activeUser.pseudo, allFlights.id);
            fclose(ufpp);
        }else { clearInput(); clearScreen(); userMode();}
    }
    clearInput();
    clearScreen();
    userMode();
}

struct Flight findOneFlight(bool t, int a){
    struct Flight allFlights;
    // search by order of appearence :
    if (!t){
        int ijk = 1;
        FILE * fp = fopen("flights/flights", "rb");
        while (fread(&allFlights, sizeof(struct Flight), 1, fp)) {
            if ( ijk == a )break;
            ijk++;
            }
        fclose(fp);
        return allFlights;
    }else{ // search by id:
        FILE * fp = fopen("flights/flights", "rb");
        while (fread(&allFlights, sizeof(struct Flight), 1, fp)) {
            if ( allFlights.id == a )break;
            }
        fclose(fp);
        return allFlights;
    }
}

void seeReservations(char fn[16], char ln[16], char username[9], bool t){
    printf("\n\t\t\t\t MY RESERVATIONS");
    printf("\n\t\t\t\t__________________\n\n");
    printf("\n\n Name: %s %s (@%s)", fn, ln, username);
    printf("\n__________________________________________________________\n\n");

    FILE *ufpp;
    char p [3][20]; //passenger details
    int f_id;
    struct Flight myFlight;
    int counter = 0;
    ufpp = fopen("flights/passengers", "r");

    if (ufpp == NULL){ printf("\n\nNO DATA"); }else{
        while (fscanf(ufpp, "%s %s %s %d\n", p[0], p[1], p[2], &f_id) != EOF) {
            if( strcmp( p[0], fn ) == 0 && strcmp( p[1], ln ) == 0 && strcmp( p[2], username ) == 0 ){
                myFlight = findOneFlight(true, f_id);
                counter++;
                printf("\n\t%d. %s - %s: %s at %s Price: %d RWF", counter, myFlight.destination[0], myFlight.destination[1], myFlight.day, myFlight.hour, myFlight.price);
            }

        }
        fclose(ufpp);
    }
    if (counter<1){ printf("\n\n NO RESERVATION"); }
    if (t){
        printf("\n\nPress enter to come back to the user menu");
        getchar();
        clearInput();
        clearScreen();
        userMode();
    }
}

void cancelReservation(){
    FILE *old, *fnew;
    int ind_count = 0;
    int ind_del;
    char u [3][20]; //userData that will be read from the passenger file
    int fl_id;
    seeReservations(activeUser.fullname[0], activeUser.fullname[1], activeUser.pseudo, false);
    old = fopen("flights/passengers", "r");
    fnew = fopen("flights/newpassengers", "w");
    printf("\n\n Choose the flight to cancel: ");

    scanf("%d", &ind_del);
    while (fscanf(old, "%s %s %s %d\n", u[0], u[1], u[2], &fl_id) != EOF) {
        if( strcmp( u[0], activeUser.fullname[0] ) == 0 && strcmp( u[1], activeUser.fullname[1] ) == 0 && strcmp( u[2], activeUser.pseudo ) == 0 ){
            ind_count++;
            if (ind_del == ind_count ){continue;}
        }
        fprintf(fnew, "%s %s %s %d\n", u[0], u[1], u[2], fl_id);
    }
    clearInput();
    fclose(old);
    fclose(fnew);
    if( ind_count < 1 ){
        printf("\nNo data to Delete\n Press enter to return to the user menu...");
        getchar();
        clearInput();
        clearScreen();
        userMode();
    } else if(ind_del > ind_count){
        printf("\n\nInvalid choice, choose a number between 1 and %d\n\n", ind_count);
        cancelReservation();
    } else{
        printf("\n\nDeleting the %d reservation, press y to confirm or another key to go to the user menu: ", ind_del);
        char confirm = getchar();
        if (confirm == 'y' || confirm == 'Y'){
            rename("flights/passengers", "flights/passengers_old");
            remove("flights/passengers");
            rename("flights/newpassengers", "flights/passengers");
        }
        clearInput();
        clearScreen();
        userMode();
    }
}

void adminlogin(bool test){
    char adminuserName[6][9] = {"daniel", "kambale", "ngununu", "knd", "dani", "dan"};
    char adminPassword [6][9] = {"daniel", "kambale", "ngununu","knd", "dani", "dan"};
    char credentials[2][9];
    printf("\n\n\t\t\t\t   ADMIN LOGIN PAGE");
    printf("\n\t\t\t\t_____________________\n");

    if(!test){printf("\n\n  Invalid credentials !! Try again\n");}

    printf("\n\tWhat is your username : ");
    scanf("%s", credentials[0]);

    printf("\n\tWhat is your password : ");
    scanf("%s", credentials[1]);

    for(int a = 0 ; a < 6 ; a++ ){
        if(strcmp(adminuserName[a], credentials[0]) == 0 && strcmp(adminPassword[a], credentials[1]) == 0 ){
        adminconnected = true;
        clearInput();
        clearScreen();
        adminMode();
        return;
        }
    }
    clearScreen();
    getchar(); //absorb th new line left by the scanf
    adminlogin(false);
}
void adminMode(){
    if (!adminconnected){adminlogin(true);}
    char choice ;
    printf("\n\n\t\t\t\t   ADMIN  PAGE");
    printf("\n\t\t\t\t_________________\n");
    printf("\n\n\n\t\t1. Add flights\n\t\t2. Show flights\n\t\t3. Update Flight\n\t\t4. Delete flights\n\t\t5. Remaining flight seats\n\t\t6. See sorted flight\n\t\t7. See all users\n\t\t8. See passengers\n\t\t9. Search a passenger\n\t\td. Delete passengers information\n\n\t\t0. Logout\n\t\te. Exit the program\n");
    printf("Enter your choice: ");
    choice = getchar();
    switch(choice){
        case '0':
            adminLogout();
            break;
        case 'e':
            exitProgram();
            break;
        case '1':
            clearScreen();
            addFlight();
            break;
        case '2':
            clearScreen();
            showFlights(true);
            break;
        case '3':
            clearScreen();
            updateFlight();
            break;
        case '4':
            clearScreen();
            deleteFlight();
            break;
        case '5':
            clearScreen();
            flightStatus();
            break;

        case '6':
            clearScreen();
            sortFlights();
            break;
        case '7':
            clearScreen();
            showUsers();
            break;
        case '8':
            clearScreen();
            showPassengers();
            break;
        case '9':
            clearScreen();
            searchPassenger();
            break;
        case 'd':
            clearScreen();
            renitializePassenger();
            break;

    }
    clearInput();
    clearScreen();
    adminMode();
}

void addFlight(){
    struct Flight newFlight;

    printf("\n\t\t\t\t ADD NEW FLIGHT");
    printf("\n\t\t\t\t________________\n\n");

    printf("\tEnter the Destination,\n\t\t\t\t\ From: "); scanf("%s", newFlight.destination[0]);
    printf("\n\t\t\t\t To: "); scanf("%s", newFlight.destination[1]);
    printf("\n\t Day: "); scanf("%s", newFlight.day);
    printf("\n\t Hour (eg: 12:30pm): "); scanf("%s", newFlight.hour);
    printf("\n\t Duration (eg: 12h30): "); scanf("%s", newFlight.duration);
    printf("\n\t Price: "); scanf("%d", &newFlight.price);
    printf("\n\t Number of seats: "); scanf("%d", &newFlight.seat);
    newFlight.id = nbr_flights+1;

    if (nbr_flights<1){
        FILE *ufp;
        ufp = fopen("flights/flights", "wb");
        fwrite(&newFlight, sizeof(struct Flight), 1, ufp);
        fclose(ufp);
        //update the index of the number of flights
        UpdateFlightNumber();
    }else{ // else append the new user
        FILE *ufpp;
        ufpp = fopen("flights/flights", "ab");
        fwrite(&newFlight, sizeof(struct Flight), 1, ufpp);
        fclose(ufpp);
        //update the index of the number of flights
        UpdateFlightNumber();
    }
    clearInput();
    clearScreen();
    adminMode();
}

void showFlights(bool isAdmin){
    int i = 1;
    char c;
    struct Flight allFlights;
    FILE * fp = fopen("flights/flights", "rb");
    printf("\n\t\t\t\t LIST OF ALL FLIGHTS");
    printf("\n\t\t\t\t_____________________\n\n");

    if (fp == NULL){
        printf("No data");
        return;
    }else{
        while (fread(&allFlights, sizeof(struct Flight), 1, fp)) {
            if (isAdmin){
                printf("\n%d. From %s to %s: %s, at %s (Duration: %s), Price: %d, Code: %d", i, allFlights.destination[0], allFlights.destination[1], allFlights.day, allFlights.hour, allFlights.duration, allFlights.price, allFlights.id ); i++;
            }else{
                printf("\n\t\t%d. %s - %s: %s, at %s (Duration: %s), Price: %d", i, allFlights.destination[0], allFlights.destination[1], allFlights.day, allFlights.hour, allFlights.duration, allFlights.price ); i++;
            }
        }
    }
    fclose(fp);
    getchar();
    if (isAdmin){
        printf("\n\n\nPress enter to return to the admin menu");
        getchar();
        clearScreen();
        adminMode();
    }else{
        printf("\n\n\nType 1 to return to the user menu, 2 to go to reservation menu");
        c = getchar();
        if( c == '1'){ clearInput(); clearScreen(); userMode(); } else{ clearInput(); clearScreen(); bookFlight(); }
    }

}

void updateFlight(){
    int id;
    int i = 1;
    struct Flight allFlights;
    struct Flight nFlight;

    FILE * fp = fopen("flights/flights", "rb");
    FILE * ofile;
    FILE * nfile;

    printf("\n\n\t\t\t\t   UPDATE PAGE");
    printf("\n\t\t\t\t________________\nList of all flights\n");

    if (fp == NULL){
        printf("No data");
        return;
    }else{
        while (fread(&allFlights, sizeof(struct Flight), 1, fp)) {
            printf("\nCode: %d| %s - %s: %s, at %s (Duration: %s), Price: %d, Seats: %d", allFlights.id, allFlights.destination[0], allFlights.destination[1], allFlights.day, allFlights.hour, allFlights.duration, allFlights.price, allFlights.seat ); i++;
        }
    }
    fclose(fp);
    clearInput();
    printf("\n\n\t Choose the flight to modify by its code: ");
    scanf("%d", &id);
    nFlight.id = id;
    printf("\n\  Type the new details about the flight\n");
    printf("\tEnter the Destination,\n\t\t\t\t\ From: "); scanf("%s", nFlight.destination[0]);
    printf("\n\t\t\t\t To: "); scanf("%s", nFlight.destination[1]);
    printf("\n\t Day: "); scanf("%s", nFlight.day);
    printf("\n\t Hour (eg: 12:30pm): "); scanf("%s", nFlight.hour);
    printf("\n\t Duration (eg: 12h30): "); scanf("%s", nFlight.duration);
    printf("\n\t Price: "); scanf("%d", &nFlight.price);
    printf("\n\t Number of seats: "); scanf("%d", &nFlight.seat);

    ofile = fopen("flights/flights", "rb");
    nfile = fopen("flights/new_flights", "wb");
    if(ofile == NULL){
        printf("No Flights !!");

    }else{
        while (fread(&allFlights, sizeof(struct Flight), 1, ofile)) {
            if(allFlights.id == nFlight.id){
                fwrite(&nFlight, sizeof(struct Flight), 1, nfile);
                continue;
            }
            fwrite(&allFlights, sizeof(struct Flight), 1, nfile);
        }
    }
    fclose(ofile);
    fclose(nfile);
    rename("flights/flights", "flights/old_flights");
    rename("flights/new_flights", "flights/flights");
    printf("\n\n Flight %d updated, press enter to go to the admin menu", id);
    getchar();
    clearInput();
    clearScreen();
    adminMode();

}

void deleteFlight(){ // almost the same with update.
    int id;
    int i = 1;
    struct Flight allFlights;

    FILE * fp = fopen("flights/flights", "rb");
    FILE * ofile;
    FILE * nfile;

    printf("\n\n\t\t\t\t DELETE FLIGHT PAGE");
    printf("\n\t\t\t\t_______________________\nList of all flights\n");

    if (fp == NULL){
        printf("No data");
        return;
    }else{
        while (fread(&allFlights, sizeof(struct Flight), 1, fp)) {
            printf("\nCode: %d| %s - %s: %s, at %s (Duration: %s), Price: %d, Seats: %d", allFlights.id, allFlights.destination[0], allFlights.destination[1], allFlights.day, allFlights.hour, allFlights.duration, allFlights.price, allFlights.seat ); i++;
        }
    }
    fclose(fp);
    clearInput();
    printf("\n\n\t Choose the flight to Delete by its code: ");
    scanf("%d", &id);

    ofile = fopen("flights/flights", "rb");
    nfile = fopen("flights/new_flights", "wb");
    if(ofile == NULL){
        printf("No Flights !!");

    }else{
        while (fread(&allFlights, sizeof(struct Flight), 1, ofile)) {
            if(allFlights.id == id){continue;}
            fwrite(&allFlights, sizeof(struct Flight), 1, nfile);
        }
    }
    fclose(ofile);
    fclose(nfile);
    rename("flights/flights", "flights/old_flights");
    rename("flights/new_flights", "flights/flights");
    printf("\n\n Flight %d has been deleted sucessfully, press enter to go to the admin menu", id);
    getchar();
    clearInput();
    clearScreen();
    adminMode();
}

void sortFlights(){
    int i = 0;
    int j,s,t;

    int zz =0;
    char c;
    struct Flight allFlights;
    struct Flight rowFlight[nbr_flights];
    struct Flight temp;

    FILE * fp = fopen("flights/flights", "rb");
    printf("\n\t\t\t\t FLIGHTS SORTED BY PRICE");
    printf("\n\t\t\t\t__________________________\n\n");

    if (fp == NULL){
        printf("No flights");
        return;
    }else{
        while (fread(&allFlights, sizeof(struct Flight), 1, fp)) {
           rowFlight[i] = allFlights;
           i++;
        }
        j=i;
        /// Sorting array

        for ( s = 0; s < j-1; s++) {
            for ( t = 0; t < j-s-1; t++) {
                // If the current element is greater than the next element, swap them
                if (rowFlight[t].price < rowFlight[t+1].price) {
                    temp = rowFlight[t];
                    rowFlight[t] = rowFlight[t+1];
                    rowFlight[t+1] = temp;
                }
            }
        }

        //printing array
        for ( zz = 0; zz < j; zz++ ){
            printf("\n\t%d. From %s to %s , Price: %d", zz+1, rowFlight[zz].destination[0], rowFlight[zz].destination[1], rowFlight[zz].price ); i++;
        }
        printf("\n\n\ \t Press enter to go back ...");
    }
    fclose(fp);
    getchar();


}

void flightStatus(){
    int sellSeats[nbr_flights];
    int flightCode[nbr_flights];
    char p[3][20];
    int fCode;
    int j = 0;
    struct Flight allFlights;

    FILE * ofile = fopen("flights/flights", "rb");
    while (fread(&allFlights, sizeof(struct Flight), 1, ofile)) {
        flightCode[j] = allFlights.id;
        j++;
        }
    fclose(ofile);
    clearInput();
    for(int yy = 0 ; yy < nbr_flights ; yy++){ sellSeats[yy] = 0 ;} // initialzize sold seat to 0
    FILE * ufile = fopen("flights/passengers", "rb");
    while (fscanf(ufile, "%s %s %s %d\n", p[0], p[1], p[2], &fCode) != EOF) {
        for (int zz = 0; zz < nbr_flights ; zz++ ){
            if(fCode == flightCode[zz]){sellSeats[zz]++;} //if a flight is detected
        }

    }
    fclose(ufile);

    FILE * fp = fopen("flights/flights", "rb");
    printf("\n\n\t\t\t\t FLIGHTS STATUS PAGE");
    printf("\n\t\t\t\t_______________________\nList of all flights\n");

    if (fp == NULL){
        printf("No data");
        return;
    }else{
        j = 0;
        while (fread(&allFlights, sizeof(struct Flight), 1, fp)) { //printing the status
            printf("\nCode: %d| %s - %s: %s, at %s , Price: %d, Seats: %d (Remaining: %d)", allFlights.id, allFlights.destination[0], allFlights.destination[1], allFlights.day, allFlights.hour, allFlights.price, allFlights.seat, allFlights.seat-sellSeats[j]); j++;
        }
    }
    printf("\n\n\n Press enter to go back to te admin menu...");
    getchar();
    clearInput();
}

void showUsers(){
    int i = 1;
    struct User foundUser;
    FILE * fp = fopen("users/users", "rb");
    printf("\n\t\t\t\t LIST OF ALL USERS");
    printf("\n\t\t\t\t___________________\n\n");

    if (fp == NULL){
        printf("No data");
    }else{
       // printf("\n\n\t\t NAMES \t\t\t\t PSEUDO");
       // printf("\n\t\t______ \t\t\t\t________");
        while (fread(&foundUser, sizeof(struct User), 1, fp)) {
        printf("\n\t\t%d. %s %s  (pseudo: %s)", i, foundUser.fullname[0], foundUser.fullname[1], foundUser.pseudo); i++;
            }
    }
    printf("\n\n Press enter to return to admin mode");
    getchar();
    fclose(fp);
}

void showPassengers(){
    printf("\n\t\t\t\t LIST OF PASSENGERS");
    printf("\n\t\t\t\t__________________\n\n");


    FILE *ufpp;
    char p [3][20]; //passenger details
    int f_id;
    struct Flight myFlight;
    int counter = 0;
    ufpp = fopen("flights/passengers", "r");

    if (ufpp == NULL){ printf("\n\nNO DATA"); }else{
        while (fscanf(ufpp, "%s %s %s %d\n", p[0], p[1], p[2], &f_id) != EOF) {
            myFlight = findOneFlight(true, f_id);
            counter++;
            printf("\n\t%d. %s %s | %s - %s: %s at %s Price: %d RWF", counter, p[0], p[1], myFlight.destination[0], myFlight.destination[1], myFlight.day, myFlight.hour, myFlight.price);
        }
        fclose(ufpp);
    }
    if (counter<1){ printf("\n\n NO RESERVATION"); }
    printf("\n\n Press enter to go back to the admin menu...");
    getchar();
    clearInput();
    clearScreen();
    adminMode();
}

void searchPassenger(){
    printf("\n\t\t\t\t SEARCH PASSENGERS");
    printf("\n\t\t\t\t__________________\n\n");

    FILE *ufpp;
    char p [3][20]; //passenger details
    char se[20];
    int f_id;
    struct Flight myFlight;
    int counter = 0;
    ufpp = fopen("flights/passengers", "r");

    printf("\n Type the name or the pseudo of the passenger: ");
    scanf("%s", se);

    if (ufpp == NULL){ printf("\n\nNO DATA"); }else{
        while (fscanf(ufpp, "%s %s %s %d\n", p[0], p[1], p[2], &f_id) != EOF) {
            if( strcmp( p[0], se ) == 0 || strcmp( p[1], se ) == 0 || strcmp( p[2], se ) == 0 ){
                myFlight = findOneFlight(true, f_id);
                counter++;
            printf("\n\t%d. %s %s | %s - %s: %s at %s Price: %d RWF", counter, p[0], p[1], myFlight.destination[0], myFlight.destination[1], myFlight.day, myFlight.hour, myFlight.price);
            }
        }
        fclose(ufpp);
    }
    if (counter<1){ printf("\n\n No passengers found !\n\n"); }
    printf("\n\n\n Type enter to go back to the admin menu...");
    getchar();
}

void renitializePassenger(){
    char ch;
    printf("\n\t\t\t\t DELETE ALL PASSENGERS DATA");
    printf("\n\t\t\t\t___________________________\n\n");
    printf("\n\t Are you sure you want to delete all passengers information ? (Y/N)   ");
    clearInput();
    ch = getchar();
    if (ch == 'Y' || ch == 'y'){
        rename("flights/passengers", "flights/passengers_old");
        printf("\n\n\t Data sucessfully deleted, press enter to continue");
    }else{
        printf("Press enter to go back to the admin menu");
    }
    getchar();
}

void adminLogout(){

    adminconnected = false;
    clearInput();
    clearScreen();
    loginMenu(false);

}

void exitProgram(){
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tTHANKS TO USE OUR PROGRAM\n\n\t\t\t\t    If you find a bug, let us know :)  \n\n\n\n\n\n\n\n\n\n\n");
        exit(0);
}
