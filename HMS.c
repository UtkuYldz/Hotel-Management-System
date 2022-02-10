#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

// HMS 2.1

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // FOR COLOR

int discard;
int UserType;
int GuestChoice,OwnerChoice;

struct OwnerInfo
{
    char UserName[20];
    char Password[20];
};

struct Rooms
{
    int Status;
    int Lock;
    char Name[20];
    char Surname[20];
    char Mail[30];
    char PhoneNum[20];
};

struct OwnerInfo Owner[1];
struct Rooms Room[9];
struct Rooms *RoomNo = Room; // Sets pointer to Room[0]

// MISC Functions

void delay(int Seconds)
{
    //Converting input into milli_seconds
    int MilliSeconds = 1000 * Seconds;

    //Storing start time
    clock_t start_time = clock();

    //looping till required time is achieved
    while (clock() < start_time + MilliSeconds)
        ;
}

int GetAnswerInt()
{
    char IntAnswer;
    IntAnswerSelection:

    scanf(" %c", &IntAnswer);
    while ((discard = getchar()) != '\n' && discard != EOF)
        ; //clears buffer so it only gets first character
    
    if (isdigit(IntAnswer) == 0)
    {
        printf("please answer with numbers(do not enter letters)\n");
        delay(1);
        goto IntAnswerSelection;
    }
    else if (IntAnswer == '0')
        return -1;
    else if (IntAnswer == '1')
        return 1;
    else if (IntAnswer == '2')
        return 2;
    else if (IntAnswer == '3')
        return 3;
    else if (IntAnswer == '4')
        return 4;
    else if (IntAnswer == '5')
        return 5;
    else if (IntAnswer == '6')
        return 6;
    else if (IntAnswer == '7')
        return 7;
    else if (IntAnswer == '8')
        return 8;
    else
        return 0;
}

int GetAnswerChar()
{
	char CharAnswer;
	CharAnswerSelection:
	scanf(" %c", &CharAnswer);
	while((discard = getchar()) != '\n');
		
	if(isdigit(CharAnswer)==1){
		printf("please answer with Y or N(Not numbers)\n");
		goto CharAnswerSelection;
	}
	else if(CharAnswer=='y' || CharAnswer=='Y')
		return 1;
	else if(CharAnswer=='n' || CharAnswer=='N')
		return 0;
	else{
		printf("please answer with Y or N(Not other letters)\n");
		goto CharAnswerSelection;
	}
}

// [0 - WHITE] [1 - RED] [2 - GREEN] [3 - BLUE]
void ColorChanger(int Color)
{
    if(Color == 0)
        SetConsoleTextAttribute(hStdOut, (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
    else if(Color == 1)
        SetConsoleTextAttribute(hStdOut, (FOREGROUND_RED));
    else if(Color == 2)
        SetConsoleTextAttribute(hStdOut, (FOREGROUND_GREEN));
    else if(Color == 3)
        SetConsoleTextAttribute(hStdOut, (FOREGROUND_BLUE));
}

void RoomSwitcher(int RoomChoice)
{
    RoomNo = Room;
    switch(RoomChoice){
    	case 1:
    		RoomNo += 1;
    		break;
    	case 2:
    		RoomNo += 2;
    		break;
    	case 3:
    		RoomNo += 3;
    		break;
    	case 4:
    		RoomNo += 4;
    		break;
    	case 5:
    		RoomNo += 5;
    		break;
    	case 6:
    		RoomNo += 6;
    		break;
    	case 7:
    		RoomNo += 7;
    		break;
    	case 8:
    		RoomNo += 8;
    		break;
    	default:
    		RoomNo = Room;
    		printf("\n\n[ERROR] ROOM SWITCHER\n\n");
    		break;	
			}

    return;
}

int RandomNumber(int MinNum, int MaxNum)
{
    int RandomNum;
    RandomNum = rand() % (MaxNum + 1 - MinNum) + MinNum;
    return RandomNum;
}

// DEBUG Functions
void CreateRoom()
{
    RoomNo = &Room[1];
    for (int i = 1; i < 9; i++)
    {
        RoomNo->Status = RandomNumber(0, 1);
        RoomNo++;
    }
    RoomNo = &Room[1];
}

// FILE Functions

FILE *databaseF;
FILE *settingsF;

// DATABASE

void LoadFromDatabase()
{
    databaseF = fopen("Database.bin", "rb");
    if(databaseF == NULL)
    {
        ColorChanger(1);
        printf("Database is Missing\tPlease Contact Hotel Owner\n");
        ColorChanger(0);
        delay(3);
        return;
    }

    fread(Room,sizeof(struct Rooms),9,databaseF);

    fclose(databaseF);

    delay(3);
}

void WriteToDatabase()
{
    databaseF = fopen("Database.bin", "wb");
    if(databaseF == NULL)
    {
        ColorChanger(1);
        printf("Database is Corrupt\n");
        ColorChanger(0);
        delay(3);
        return;
    }

    fwrite(Room,sizeof(struct Rooms),9,databaseF);

    fclose(databaseF);

    delay(3);
}

// SETTINGS

void SaveSettings()
{
    settingsF = fopen("Settings.bin", "wb");
    if(settingsF == NULL)
    {
        ColorChanger(1);
        printf("Settings is Corrupt\n");
        ColorChanger(0);
        delay(3);
        return;
    }

    fwrite(Owner,sizeof(struct OwnerInfo),1,settingsF);

    fclose(databaseF);

}

void SetupSettings()
{
    char InputUsername[20];
    char InputPass[20];

    printf("Let's set up your Owner Profile\n");

    printf("Please Enter Username\n");
    fgets(InputUsername,20,stdin);
    fflush(stdin);

    printf("Please Enter Password\n");
    fgets(InputPass,20,stdin);
    fflush(stdin);

    InputUsername[strcspn(InputUsername, "\n")] = 0;
    InputPass[strcspn(InputPass, "\n")] = 0;

    strcpy(Owner->UserName,InputUsername);
    strcpy(Owner->Password,InputPass);

    printf("Creating your profile\tPlease Wait!\n");

    SaveSettings();
}

void LoadSettings()
{
    settingsF = fopen("Settings.bin", "rb");
    if(settingsF == NULL)
    {
        printf("\nLooks like you are entering our system for the first time\n");
        delay(3);
        system("cls");
        SetupSettings();
        //For Creating Empty Database
        databaseF = fopen("Database.bin", "wb");
        fclose(databaseF);
        return;
    }

    fread(Owner,sizeof(struct OwnerInfo),1,settingsF);
    fclose(settingsF);

    delay(3);
}

// OWNER Functions

int OwnerLogin()
{
    char OwnerNameInput[20], OwnerPassInput[20], c = ' ';
    int NameAuth = 0, PassAuth = 0, PassRetry = 5,i = 0;

    //		NAME
    do
    {
        printf("Please Enter Your Name:");
        fgets(OwnerNameInput, 20, stdin);

        printf("Press  Enter To Confirm");
        while ((discard = getchar()) != '\n' && discard != EOF)
            ; //clears buffer so it doesn't write on other variables

        OwnerNameInput[strcspn(OwnerNameInput, "\n")] = 0; //Deletes \n newline that added by fgets

        NameAuth = strcmp(Owner->UserName, OwnerNameInput); //Checks if the both strings identical

        if (NameAuth != 0)
        {
            system("cls");
            ColorChanger(1);
            printf("\tWrong Name\n");
            ColorChanger(0);
            continue;
        }

    } while (NameAuth != 0);
    system("cls");
    ColorChanger(2);
    printf("\tCorrect Name\n");
    ColorChanger(0);

    //		PASSWORD
    do
    {
        printf("Please Enter Your Password:");
        //scanf(" %d", &OwnerPassInput);

        while (i < 20)
        {
            OwnerPassInput[i] = getch();
            c = OwnerPassInput[i];
            if (c == 13)
                break;
            else
                printf("*");
            i++;
        }
        OwnerPassInput[i] = '\0';
        i = 0;
        PassAuth = strcmp(Owner->Password, OwnerPassInput); //Checks if the both strings identical

        if (PassAuth != 0)
        {
            system("cls");
            ColorChanger(1);
            printf("\tWrong Password\n");
            ColorChanger(0);
            printf("\t%d Retries Left!\n", --PassRetry);
            if (PassRetry <= 0)
            {
                printf("   Too Many Wrong Passwords\n");
                delay(2);
                return 0;
            }
            continue;
        }

    } while (PassAuth != 0);
    system("cls");
    ColorChanger(2);
    printf("\tCorrect Password\n");
    ColorChanger(0);
    printf("      Welcome Back, %s!\n", Owner->UserName);
    delay(3);
    return 1;
}

void OwnerShowRoom()
{
    RoomNo = &Room[1];
    printf("\t    LOCK - NAME - SURNAME - MAIL - PHONE NUMBER\n");
    for (int i = 1; i < 9; i++)
    {
        printf("\t _\n");

        if(RoomNo->Status == 0)
        {
            
            printf("Room %d: |", i);
            ColorChanger(2);
            printf("O");
            ColorChanger(0);
            printf("|");
            printf(" Room is Empty\n");
        }
        else if(RoomNo->Status == 1)
        {
            printf("Room %d: |", i);
            ColorChanger(1);
            printf("X");
            ColorChanger(0);
            printf("|");
            printf(" %d %s %s %s %s\n",RoomNo->Lock, RoomNo->Name, RoomNo->Surname, RoomNo->Mail, RoomNo->PhoneNum);
        }

        RoomNo++;
    }
    delay(3);
    RoomNo = &Room[1];
}

void OwnerEditRooms()
{
    int RoomChoice,EditChoice;
    int InputInt;
    char Input20[20],Input30[30];
    system("cls");
    printf("Which room you want to Edit?\n");
    RoomChoice = GetAnswerInt();
    RoomSwitcher(RoomChoice);
    printf("What do you want to edit?\n0.GO BACK!\n1.Lock\n2.Name\n3.Surname\n4.Mail\n5.Phone Number\n");
    EditChoice = GetAnswerInt();
    if(EditChoice == -1)
    {
        RoomNo = Room;
        return;
    }
    else if(EditChoice == 1)
    {
        printf("Please Enter New Lock: ");
        do
        {
            scanf("%d", &InputInt);
            fflush(stdin);
        }
        while(!(1000 < InputInt && InputInt < 10001));
        RoomNo->Lock = InputInt;
    }
    else if(EditChoice == 2)
    {
        printf("Please Enter New Name: ");
        fgets(Input20,20,stdin);
        fflush(stdin);
        Input20[strcspn(Input20, "\n")] = 0;
        strcpy(RoomNo->Name,Input20);
    }
    else if(EditChoice == 3)
    {
        printf("Please Enter New Surname: ");
        fgets(Input20,20,stdin);
        fflush(stdin);
        Input20[strcspn(Input20, "\n")] = 0;
        strcpy(RoomNo->Surname,Input20);
    }
    else if(EditChoice == 4)
    {
        printf("Please Enter New Mail: ");
        fgets(Input30,20,stdin);
        fflush(stdin);
        Input30[strcspn(Input30, "\n")] = 0;
        strcpy(RoomNo->Mail,Input30);
    }
    else if(EditChoice == 5)
    {
        printf("Please Enter New Phone Number: ");
        fgets(Input20,20,stdin);
        fflush(stdin);
        Input20[strcspn(Input20, "\n")] = 0;
        strcpy(RoomNo->PhoneNum,Input20);
    }
    
    RoomNo = Room;
    WriteToDatabase();
}

void OwnerDeleteRooms()
{
    int RoomChoice;
    system("cls");
    printf("Which room you want to Delete?\n");
    RoomChoice = GetAnswerInt();
    RoomSwitcher(RoomChoice);
    RoomNo->Status = 0;
    RoomNo->Lock = 0000;

    memset(RoomNo->Name,0,sizeof(RoomNo->Name));
    memset(RoomNo->Surname,0,sizeof(RoomNo->Surname));
    memset(RoomNo->Mail,0,sizeof(RoomNo->Mail));
    memset(RoomNo->PhoneNum,0,sizeof(RoomNo->PhoneNum));

    RoomNo = Room;
    WriteToDatabase();
}

void OwnerSelector(int OwnerChoice)
{
    if (OwnerChoice == 1)
        OwnerShowRoom();
    else if (OwnerChoice == 2)
        OwnerEditRooms();
    else if (OwnerChoice == 3)
        OwnerDeleteRooms();
    else
        printf("please answer with corresponding numbers(do not enter other numbers)\n");
}

// GUEST Functions

void GuestCreate(int RoomChoice)
{
    char InputName[20];
    char InputSurname[20];
    char InputPhoneNum[20] = "";
    char InputMail[30];
    int PhoneNumLength;
    int RoomKeyLock = RandomNumber(1001, 10000);
    system("cls");

    printf("Please Enter Your Name:");
    fgets(InputName,20,stdin);
    fflush(stdin);
    printf("Please Enter Your Surname:");
    fgets(InputSurname,20,stdin);
    fflush(stdin);
    printf("Please Enter Your Mail:");
    fgets(InputMail,20,stdin);
    fflush(stdin);

    //Cleaning the \n newlines that added by fgets
    InputName[strcspn(InputName, "\n")] = 0;
    InputSurname[strcspn(InputSurname, "\n")] = 0;
    InputMail[strcspn(InputMail, "\n")] = 0;

    do
    {
        printf("Please Enter Your Phone Number:");
        scanf(" %s", &InputPhoneNum);
        printf("\n");
        PhoneNumLength = strlen(InputPhoneNum);
        if(PhoneNumLength <= 15)
            break;
        system("cls");
        ColorChanger(1);
        printf("Please Enter Max 15 Digits\n");
        ColorChanger(0);
        
    } while (PhoneNumLength > 15);
    system("cls");

    RoomSwitcher(RoomChoice);
    RoomNo->Lock = RoomKeyLock;
    strcpy(RoomNo->PhoneNum, InputPhoneNum);
    strcpy(RoomNo->Name, InputName);
    strcpy(RoomNo->Surname, InputSurname);
    strcpy(RoomNo->Mail, InputMail);
    RoomNo->Status = 1;

    printf("You've booked room %d\nYour Pass for the Lock is: %d (Remember this to enter your room)\n", RoomChoice, RoomNo->Lock);

    // FILE HANDLING
    WriteToDatabase();

    delay(5);
}

void GuestShowRoom()
{
    RoomNo = &Room[1];
    for (int i = 1; i < 9; i++)
    {
        printf("\t _\n");
        printf("Room %d: |", i);

        if(RoomNo->Status == 0)
        {
            ColorChanger(2);
            printf("O");
            ColorChanger(0);
            printf("|");
            printf(" Room is Empty\n");
        }
        else if(RoomNo->Status == 1)
        {
            ColorChanger(1);
            printf("X");
            ColorChanger(0);
            printf("|");
            printf(" Room is Full\n");
        }
        RoomNo++;
    }
    delay(3);
    RoomNo = &Room[1];
}

void BookRoom()
{
    int RoomChoice = 0,Booking = 0;
    while(RoomChoice<1 || 8<RoomChoice)
    {
        system("cls");
        printf("Please enter the room number you want to book\n");
        RoomChoice = GetAnswerInt();
    }

    RoomSwitcher(RoomChoice);

    if (RoomNo->Status == 0)
    {
        printf("\tRoom %d is empty\n", RoomChoice);
        delay(1);
        printf("Do you want to book it ? [Yes or No]\n");
        Booking = GetAnswerChar();
        if(Booking == 1)
            GuestCreate(RoomChoice);
        else
            printf("You've decided not to book room %d\n", RoomChoice);
        
    }
    else if (RoomNo->Status == 1)
    {
        printf("\tRoom %d is full\n", RoomChoice);
        printf("You can't book it.please select an empty room\n");
        delay(3);
    }
    else
        printf("ERROR 3");
    
    RoomNo = &Room[1];
}

void GoToRoom()
{
    system("cls");
    RoomNo = &Room[1];
    int InputLock = 1000;
    printf("Which room you want to go\n");
    int RoomChoice = GetAnswerInt();
    if(RoomChoice<1 || 8<RoomChoice)
    {
        printf("Please enter the Correct Room Number next time\nReturning to lobby...");
        delay(3);
        return;
    }
    
    RoomSwitcher(RoomChoice);

    if(RoomNo->Status == 1)
    {
        printf("Please Enter Your Key For the Lock\n");
        scanf(" %d", &InputLock);
        
        if(RoomNo->Lock == InputLock)
        {
            printf("Welcome to your room, %s %s\n\n", RoomNo->Name, RoomNo->Surname);
            delay(1);
            printf("Lock: %d\n", RoomNo->Lock);
            printf("Mail: %s\n", RoomNo->Mail);
            printf("Phone Number: %s\n", RoomNo->PhoneNum);
            delay(5);
            RoomNo = &Room[1];
        }
        else
        {
            ColorChanger(1);
            printf("Wrong Key\nReturning to lobby...");
            ColorChanger(0);
            delay(3);
        }
    }
    else
    {
        printf("You can't enter empty rooms\tPlease select your room\n");
        delay(3);
    }

    return;
}

void GuestSelector(int GuestChoice)
{
    if (GuestChoice == 1)
        GuestShowRoom();
    else if (GuestChoice == 2)
        BookRoom();
    else if (GuestChoice == 3)
        GoToRoom();
    else
        printf("please answer with corresponding numbers(do not enter other numbers)\n");
}

int main()
{
    srand(time(0));
    int LanguageSelection = 1,LoginSuccess;
    printf("\t\t  Hotel Management System\n\t\tCem Kaan Ugur - Utku Yildiz\n");
    LoadSettings();
    LoadFromDatabase();

    LanguageSelector:
    system("cls");
    printf("Please Select Your Language\tLutfen Dilinizi Seciniz\n");
    printf("\t\t0.Exit Program\n\t\t1.English\tEN\n\t\t2.Turkce\tTR\n");
    LanguageSelection = GetAnswerInt();
    
    if(LanguageSelection == -1)
        goto EmergencyExit;
    //ENGLISH
    else if (LanguageSelection == 1)
    {
        while (1)
        {
            UserTypeSelection:
            system("cls");
            printf("\t0.GO BACK!\n\t1.Hotel Owner\n\t2.Hotel Guest\n");
            UserType = GetAnswerInt();
            if(UserType == -1)
                goto LanguageSelector;
            else if(UserType == 1)
            {
                system("cls");
                LoginSuccess = OwnerLogin();
                if (LoginSuccess == 1)
                {
                    while(1)
                    {
                        system("cls");
                        printf("0.GO BACK!\n");
                        printf("1.Show Rooms\n");
                        printf("2.Edit Rooms\n");
                        printf("3.Delete Rooms\n\n");
                        OwnerChoice = GetAnswerInt();
                        if(OwnerChoice == -1)
                            goto UserTypeSelection;
                        system("cls");
                        OwnerSelector(OwnerChoice);
                        delay(3);
                    }
                }
                else if (LoginSuccess == 0)
                {
                    printf("\tPlease Exit HMS\n");
                    delay(1);
                    goto EmergencyExit;
                }
            }
            else if(UserType == 2)
            {
                while(1)
                {
                    system("cls");
                    printf("Welcome To Our Hotel, Guest!\n\n");
                    printf("0.GO BACK!\n");
                    printf("1.See Room Status\n");
                    printf("2.Book a Room\n");
                    printf("3.Go to Your Room\n\n");
                    GuestChoice = GetAnswerInt();
                    if(GuestChoice == -1)
                        goto UserTypeSelection;
                    system("cls");
                    GuestSelector(GuestChoice);
                    delay(3);
                }
            }
            else
            {
                printf("please answer with corresponding numbers(do not enter other numbers)\n");
                delay(1);
            }
        }
    }
    //TURKCE
    else if (LanguageSelection == 2)
    {
        printf("Turkce dili hala yapim asamasindadir\tLutfen baska bir dil seciniz\n");
        delay(3);
        goto LanguageSelector;
    }
    else
    {
        printf("Please pick only available languages\tLutfen uygun bir dil seciniz\n");
        delay(3);
        goto LanguageSelector;
    }

    EmergencyExit:
    return 0;
}
