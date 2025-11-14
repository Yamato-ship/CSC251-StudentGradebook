#include<stdio.h>
#include <string.h>
#include "sqlite3.h"
#include<stdbool.h>

//int displayRecords(sqlite3 * db); should display all relevent data on based on ID
int addStudent(sqlite3 * db);       //functions take user input, problem when
int deleteStudent(sqlite3 * db);    //wrong data type is taken in, char into
                                    //studentID input

int main(int argc, char const *argv[]){

    sqlite3 *db;
    char *err_msg = 0;
    int conn = sqlite3_open("students.db", &db);

    //Check #1
    if(conn != SQLITE_OK){
        fprintf(stderr, "Connection Failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_free(err_msg);
        return 1;
    }

    //Can add more columns for data
    char *sql_qry = "CREATE TABLE IF NOT EXISTS students"
    "(studentID INTEGER PRIMARY KEY, firstName TEXT, lastName TEXT);";
    //"INSERT INTO students VALUES (222333444, 'See', 'Sim');";

    conn = sqlite3_exec(db, sql_qry, 0, 0, &err_msg);

    //Check #2
    if(conn != SQLITE_OK)
    {
        fprintf(stderr, "Table Creation Failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_free(err_msg);
        return 1;
    }   else
    {
        fprintf(stderr, "Table Created Successfully\n");
    }

    //FUNCTIONS START HERE    
    //probably have interactive switch here to select functions
    int option;
    printf("What would you like to do today:\n\n1) Add a student\n2) Remove a student\nEnter your option: ");
    scanf("%d", &option);

    switch(option)
    {
        case 1:
            addStudent(db);
            break;
        case 2:
            deleteStudent(db);
            break;
        default:
            printf("Please enter a valid option.");
            break;
    }

    //FUNCTIONS END HERE

    sqlite3_close(db);

    return 0;
}

int addStudent(sqlite3 * db){
    char *err_msg = NULL;
    char firstName[30], lastName[30], sql_qry[256];
    char inputID[20];
    int digit;

    //Should take max of 30 characters
    fprintf(stderr, "Enter student first name (Max 30 Char): ");
    scanf("%29s", firstName);
    fprintf(stderr, "Enter student last name (Max 30 Char): ");
    scanf("%29s", lastName);

    //checks id is 9 digits, problem when ID starts with 0 (000001234)
    printf("Enter 9-digit student ID: ");
    scanf("%19s", inputID);
    //Optimized for rejecting numbers less than 9 digits and allows leading 0's (Moises)
    while (1) //while true
    {
        digit = 0; //Digit count
        while(inputID[digit] != '\0')
        {
            digit++;
        }
        if(digit == 9 && strspn(inputID, "0123456789") == 9)
        {
            break;
        }
        else
        {
            printf("Please enter a valid 9-digit ID: ");
            scanf("%19s", inputID);
        }
    }
    
        /*do{
    fprintf(stderr, "Enter studentID (9 digits): ");
    scanf("%d", &inputID);
    } while (inputID < 100000000 || inputID > 999999999);*/
    
    //snprintf for strings
    int numericID = atoi(inputID);
    snprintf(sql_qry, sizeof(sql_qry), "INSERT INTO students (studentID, firstName,lastName) VALUES"
                "(%d, '%s', '%s');", numericID, firstName, lastName);
    

    int conn = sqlite3_exec(db, sql_qry, 0, 0, &err_msg);
    if(conn != SQLITE_OK){
        fprintf(stderr, "Failed to Add Student: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_free(err_msg);
        return 1;
    }else{
        fprintf(stderr, "Student Added Successfully\n" );
    }

    return 0;
}

int deleteStudent(sqlite3 *db){
    char *err_msg = NULL;
    char sql_qry[256];
    int deleteID = 0, exists = 0;

    int callBack(void *data, int argc, char **argv, char **colNames)
    {
        int *existsPtr = (int *) data;
        *existsPtr = atoi(argv[0]);
        return 0;
    }

    //checks if ID exists in database
    do
    {
        fprintf(stderr, "Enter existing studentID to delete (9 digits): ");
        scanf("%d", &deleteID);

        snprintf
    } while (exists == ("SELECT exists(SELECT 1 FROM students"
                "WHERE studentID = %d AS row_exists;", deleteID));

    //only integers so sprintf is fine
    sprintf(sql_qry, "DELETE FROM students WHERE studentID = %d;", deleteID);

    int conn = sqlite3_exec(db, sql_qry, 0, 0, &err_msg);
    if(conn != SQLITE_OK){
        fprintf(stderr, "Failed to Remove Student Record: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_free(err_msg);
        return 1;
    }else{
        fprintf(stderr, "Student Record Removed\n" );
    }

    return 0;
}


/*int main() 
{
    printf("Testing AddStudent function...\n");
    int option;
    printf("What would you like to do today:\n\n1) Add a student\n2) Remove a student\nEnter your option: ");
    scanf("%d", &option);

    switch(option)
    {
        case 1:
            addStudent();
            break;
        case 2:
            deleteStudent();
            break;
        default:
            printf("Please enter a valid option.");
            break;
    }

    return 0;
} */
