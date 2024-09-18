#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
FILE *fptr; //much easier to use a ptr to a function as a global variable
int count = 0; // to get the total number of accounts
char *MonthPrint(int n)
{
    switch (n)
    {
    case 1:
        return "January";
        break;
    case 2:
        return "February";
        break;
    case 3:
        return "March";
        break;
    case 4:
        return "April";
        break;
    case 5:
        return "May";
        break;
    case 6:
        return "June";
        break;
    case 7:
        return "July";
        break;
    case 8:
        return "August";
        break;
    case 9:
        return "September";
        break;
    case 10:
        return "October";
        break;
    case 11:
        return "November";
        break;
    case 12:
        return "December";
        break;
    default:
        return "Invalid month number";
    }
}
int validate_balance(char *check)
{
    int j,flag=0;
    for (j = 0; check[j] != '\0'; j++)

    {
        if ((!isdigit(check[j]) && check[j]!='.'))
            return 0;
        if(check[j]=='.')
            flag++;
    }
    if(j-1>5 || flag>1)//j-1 because j is incremented before it's exit from the for loop
        return 0;
    if(atof(check) < 0)
        return 0;
    return 1;
}
int validate_name(char *name)
{
    int j,flag = 0;
    for(j=0; name[j]!='\0'; j++)
    {
        if(name[j] == ' ')
            flag++;
        if(!isalpha(name[j]) && name[j]!=' ')
            return 0;
    }
    if(flag != 1 || j>25 || name[0] == ' ' || name[j-1] == ' ')
        return 0;
    return 1;
}
int validate_accNum(char *num)
{
    int j;
    for(j=0; num[j]!='\0'; j++)
        if(!isdigit(num[j]))
            return 0;
    if(j != 10)
        return 0;
    return 1;
}
int validate_email(char *mail)
{
    int i,j,flag = 0;
    for(j=0; mail[j]!='\0'; j++)
    {
        if(mail[j] == ',' || mail[j] == ' ')
            return  0;
        if(mail[j] == '@')
        {
            flag++;
            i=j;
        }
    }
    if(flag != 1 || mail[++i] == '.' || j > 50)
        return 0;
    for(i++; mail[i]!='\0'; i++)
        if(mail[i] == '.' && mail[i+1] != '\0')
            return 1;
    return 0;
}
int validate_mobile(char *mob)
{
    int j;
    for(j=0; mob[j]!='\0'; j++)
        if(!isdigit(mob[j])) // 48 to 57 is equivalent to 0 to 9 in ascii
            return 0;
    if(j != 11 || mob[0] != '0' || mob[1] != '1' || !(mob[2] == '0' || mob[2] == '1' || mob[2] == '2' || mob[2] == '5')) // number must consist of 11 digits and first two digits are 0 and third digit is 0,1,2 or 5.
        return 0;
    return 1;
}
typedef struct
{
    char username[30],pass[20];
} User;

void getcurrentdate(int *month, int *year)
{
    time_t t = time(NULL);
    struct tm* local = localtime(&t);
    *month = local->tm_mon + 1;
    *year = local->tm_year + 1900;
}
typedef struct
{
    int month, year;
} date_open;

typedef struct
{
    char name[30],acc_num[15],email[60],mobile[20]; //mobile is char to prevent skipping 0 if it is the first element of the number, ex(01265418721)
    double balance;
    date_open d;
} Account;
Account A[100];
void append_transaction(Account x,double cash)
{
    char filename[50];
    sprintf(filename,"%s.txt",x.acc_num);
    fptr = fopen(filename,"a");
    if(fptr == NULL)
    {
        printf("Error creating the file\n\n");
        exit(-1);
    }
    fprintf(fptr,"Transaction: %0.1lf, your new balance in the account is %0.1lf\n",cash,x.balance);
    fclose(fptr);
}
int found_acc(char*accountnumber, int*i)
{
    for ( *i = 0; *i < count; (*i)++)
    {
        if (strcmp(A[*i].acc_num, accountnumber) == 0)
            return 1;
    }
    return 0;
}
int LogIn()
{
    int i;
    User UserArray[100];
    int UsersNo = 0;
    char CurrentUsername[30],CurrentPassword[30];
    fptr=fopen("users.txt","r");
    if(fptr == NULL)
    {
        printf("Can not read the file\n");
        exit(-1);
    }
    while(!feof(fptr))
    {
        fscanf(fptr,"%s %s",UserArray[UsersNo].username,UserArray[UsersNo].pass);
        UsersNo++;
    }
    printf("Enter your username: ");
    scanf("%s",CurrentUsername);
    printf("Enter your password: ");
    scanf("%s",CurrentPassword);

    for(i=0; i<UsersNo; i++)
    {
        if(strcmp(CurrentUsername,UserArray[i].username) == 0 && strcmp(CurrentPassword,UserArray[i].pass) == 0)
            return 1;
    }
    printf("Wrong user name or password, please try again.\n\n");
    return 0;
}
void load()
{
    int i;
    fptr=fopen("accounts.txt","r");
    if(fptr == NULL)
    {
        printf("Can not read the file\n");
        exit(-1);
    }
    for(i=0; !feof(fptr); i++)
        fscanf(fptr,"%14[^,],%29[^,],%59[^,],%lf,%19[^,],%d-%d\n",A[i].acc_num, A[i].name, A[i].email, &A[i].balance, A[i].mobile, &A[i].d.month, &A[i].d.year);
    count=i;
    fclose(fptr);
}
int save()
{
    int x,i;
    printf("Do you want to save the changes? enter 1 for yes or 2 for no\n");
    scanf("%d",&x);
    if(x==1)
    {
        fptr = fopen("accounts.txt", "w");
        if (fptr == NULL)
        {
            printf("Error opening file.\n");
            exit(EXIT_FAILURE);
        }
        for(i=0; i<count; i++)
        {
            fprintf(fptr,"%s,%s,%s,%0.1lf,%s,%d-%d\n",A[i].acc_num,A[i].name,A[i].email,A[i].balance,A[i].mobile,A[i].d.month,A[i].d.year);
        }
        fclose(fptr);
        printf("Changes are saved successfully\n\n");
        return 1;
    }
    else if(x==2)
    {
        printf("Changes are not saved\n\n");
        return 0;
    }

    else
        printf("invalid entry, choose only either 1 or 2\n\n");
    return 0;
}
void Add()
{
    int i;
    char fNAME[20];
    printf("Enter an account number containing 10 digits: ");
    scanf("%s", A[count].acc_num);
    if((validate_accNum(A[count].acc_num))==0)
    {
        printf("Account number invalid\n\n");
        return;
    }
    if(found_acc(A[count].acc_num,&i))////
    {
        printf("Account number already taken\n\n");
        return;
    }
    printf("Enter your first and last name [max 25 charachters]: ");
    scanf(" %49[^\n]",A[count].name);
    if(validate_name(A[count].name)==0)
    {
        printf("Invalid entry\n\n");
        return;

    }
    printf("Enter your mobile number: ");
    scanf("%s",A[count].mobile);
    if((validate_mobile(A[count].mobile))==0)
    {
        printf("Invalid entry, mobile number should consist of 11 digits and the first 2 digits must be 0 and 1\n\n");
        return;
    }
    printf("Enter your email address: ");
    scanf(" %49[^\n]",A[count].email);
    if((validate_email(A[count].email))==0)
    {
        printf("Invalid email\n\n");
        return;
    }
    getcurrentdate(&A[count].d.month, &A[count].d.year);
    A[count].balance=0;
    count++;
    if(!save())
    {
        count--;
        return;
    }
    sprintf(fNAME, "%s.txt",A[count-1].acc_num);
    fptr=fopen(fNAME, "w");
    if(fptr==NULL)
    {
        printf("Error opening file for reading.\n");
        exit(EXIT_FAILURE);
    }
    fclose(fptr);
}

void Modify()
{
    char number[20];
    int flag = 0,i;
    printf("\nEnter account number:");
    scanf("%s",number);
    if(!validate_accNum(number))
    {
        printf("Account number invalid\n\n");
        return;
    }
    printf("\n");
    for(i=0; i<count; i++)
        if(atoi(number) == atoi(A[i].acc_num))
        {
            flag = 1;
            break;
        }
    if(!flag)
    {
        printf("Account required to be modified does not exist\n\n");
    }
    else
    {
        printf("Enter the new name: ");
        getchar();
        gets(A[i].name);
        if(!validate_name(A[i].name))
        {
            printf("name should contain first and last name with a space in between, Please try again\n\n");
            return;
        }
        printf("Enter the new Email: ");
        scanf("%s",A[i].email);
        if(!validate_email(A[i].email))
        {
            printf("Invalid email\n\n");
            return;
        }
        printf("Enter the new mobile number: ");
        scanf("%s",A[i].mobile);
        if(!validate_mobile(A[i].mobile))
        {
            printf("Invalid mobile number\n\n");
            return;
        }
        if(!save())
        {
            return;
        }
    }
}
void search()
{
    char an[20];
    int flag = 0;
    int i;
    printf("Enter account number: ");
    scanf("%s", an);
    if (validate_accNum(an) == 0)
    {
        printf("Invalid account number\n\n");
        return;
    }

    for (i=0; i<count; i++)
        if (strcmp(A[i].acc_num,an) == 0)
        {
            flag = 1;
            break;
        }
    if (flag)
    {
        printf("Account number: %s\n", A[i].acc_num);
        printf("Name: %s\n", A[i].name);
        printf("Email: %s\n", A[i].email);
        printf("Balance: %0.1lf $\n", A[i].balance);
        printf("Mobile: %s\n", A[i].mobile);
        printf("Date opened: %s %d\n\n",MonthPrint(A[i].d.month),A[i].d.year);
    }
    else
        printf("Account not found\n\n");
}
void Advanced_Search()
{
    char keyword[50];
    int i;
    int flag=0;
    printf("Enter keyword: ");
    scanf("%s", keyword);
    for (i=0; i<count; i++)
    {
        if (strstr(A[i].name,keyword) != NULL)
        {
            flag=1;
            printf("Account number: %s\n", A[i].acc_num);
            printf("Name: %s\n", A[i].name);
            printf("Email: %s\n", A[i].email);
            printf("Balance: %0.1lf $\n", A[i].balance);
            printf("Mobile: %s\n", A[i].mobile);
            printf("Date opened: %s %d\n\n",MonthPrint(A[i].d.month),A[i].d.year);
        }
    }
    if (!flag)
        printf("Keyword not found!\n\n");
}
void Withdraw()
{
    int i = 0;
    char accountNumber[11];
    char amount[6];
    printf("Enter account number: ");
    scanf("%10s", accountNumber);
    if (validate_accNum(accountNumber) == 0)
    {
        printf("Invalid account number\n\n");
        return;
    }

    if (found_acc(accountNumber, &i))
    {
        printf("Enter amount to withdraw [maximum 10000]: ");
        scanf("%s", amount);
        if(validate_balance(amount))
        {
            double wd = atof(amount);
            if (wd <= 10000 && wd <= A[i].balance)
            {
                A[i].balance -= wd;
                printf("New balance: %.1lf\n", A[i].balance);
                if (save())
                    append_transaction(A[i],-1*wd);
            }
            else printf("Withdrawal exceeding limit or not enough balance\n\n");

        }
        else
            printf("Invalid withdrawal amount.\n\n");
    }
    else
    {
        printf("Account not found.\n\n");
    }
}

void Deposit()
{
    char accountNumber[11];
    char amount[6];
    int i = 0;

    printf("Enter account number: ");
    scanf("%10s", accountNumber);

    if (validate_accNum(accountNumber) == 0)
    {
        printf("Invalid account number\n\n");
        return;
    }

    if (found_acc(accountNumber, &i))
    {
        printf("Enter amount to deposit: ");
        scanf("%s", amount);
        if(validate_balance(amount))
        {
            double dep = atof(amount);
            if (dep <= 10000)
            {
                A[i].balance += dep;
                printf("New balance: %.1lf\n", A[i].balance);
                if (save())
                    append_transaction(A[i],dep);
            }
            else
                printf("deposit exceeding limit.\n\n");
        }
        else
            printf("Invalid deposit amount\n\n");
    }
    else
        printf("Account not found.\n\n");
}

void Transfer()
{
    int i, j;
    char sourceAccount[11], destinationAccount[11];
    char amount[6];

    printf("Enter source account number: ");
    scanf("%10s", sourceAccount);
    if (validate_accNum(sourceAccount) == 0)
    {
        printf("Invalid account number\n\n");
        return;
    }

    if (found_acc(sourceAccount, &i))
    {
        printf("Enter destination account number: ");
        scanf("%10s", destinationAccount);

        if (strcmp(sourceAccount, destinationAccount) != 0)
        {
            if (found_acc(destinationAccount, &j))
            {
                printf("Enter amount to transfer: ");
                scanf("%s", amount);

                if (validate_balance(amount))
                {
                    double transfer = atof(amount);
                    if (transfer <= 10000 && transfer <= A[i].balance)
                    {
                        A[j].balance += transfer;
                        A[i].balance -= transfer;

                        printf("New balance of source account: %.1lf\n", A[i].balance);
                        printf("New balance of destination account: %.1lf\n", A[j].balance);


                        if (save())
                        {
                            append_transaction(A[i],-1*transfer);
                            append_transaction(A[j],transfer);
                        }
                    }
                    else
                        printf("Transfer amount exceeding account balance or exceeding maximum limit.\n\n");
                }
                else
                {
                    printf("Invalid Balance.\n\n");
                }
            }
            else
                printf("Destination account not found.\n\n");
        }
        else
            printf("Error: Source account is the same as the destination account.\n\n");
    }
    else
        printf("Source account not found.\n\n");
}

void Delete()
{
    char number[20];
    int flag = 0,i;
    printf("\nEnter account number:");
    scanf("%s",number);
    if(!validate_accNum(number))
    {
        printf("Invalid account number\n\n");
        return;
    }
    printf("\n");
    for(i=0; i<count; i++)
        if(atoi(number) == atoi(A[i].acc_num))
        {
            flag = 1;
            break;
        }
    if(!flag)
    {
        printf("Account required to be deleted does not exist\n\n");
    }
    else
    {
        if(!A[i].balance)
        {
            Account hold = A[i];
            A[i]=A[count-1];
            A[count-1]=hold;
            count--;
            if(!save())
            {
                count++;
                return;
            }

        }
        else
            printf("you should withdraw all the money to be able to delete the account\n\n");
    }

}
void printAllArray()
{
    int i;
    for(i=0; i<count; i++)
    {
        printf("ACCOUNT %d\n",i+1);
        printf("Account Number: %s\n",A[i].acc_num);
        printf("Name: %s\n",A[i].name);
        printf("E-Mail: %s\n",A[i].email);
        printf("Balance: %0.1lf $\n",A[i].balance);
        printf("Mobile: %s\n",A[i].mobile);
        printf("Opened: %s %d\n\n",MonthPrint(A[i].d.month),A[i].d.year);
    }
}
void SortByName()
{
    int i, j;
    for(i=0; i<count; i++)
    {
        for(j=0; j<count-i-1; j++)
        {
            if(strcmp(A[j].name,A[j+1].name)==1)
            {
                Account tempName = A[j];
                A[j] = A[j+1];
                A[j+1] = tempName;
            }
        }
    }
    printAllArray();
}
void SortByDate()
{
    int i,j;
    for(i=0; i<count; i++)
    {
        for(j=0; j<count-i-1; j++)
            if(A[j].d.year>A[j+1].d.year)
            {
                Account tempdYear = A[j];
                A[j] = A[j+1];
                A[j+1] = tempdYear;
            }
            else if(A[j].d.year == A[j+1].d.year)
                if(A[j].d.month>A[j+1].d.month)
                {
                    Account tempdYear = A[j];
                    A[j] = A[j+1];
                    A[j+1] = tempdYear;
                }
    }
    printAllArray();
}
void SortByBalance()
{
    int i,j;
    for(i=0; i<count; i++)
    {
        for(j=0; j<count-i-1; j++)
        {
            if(A[j].balance>A[j+1].balance)
            {
                Account tempBalance = A[j];
                A[j] = A[j+1];
                A[j+1] = tempBalance;
            }
        }
    }
    printAllArray();
}
void PrintSort()
{
    int sortWay;
    printf("ChOOSE A SORTING METHOD\n");
    printf("1-SORT BY NAME\n");
    printf("2-SORT BY DATE\n");
    printf("3-SORT BY BALANCE\n");
    scanf("%d",&sortWay);
    switch(sortWay)
    {
    case 1:
        SortByName();
        break;
    case 2:
        SortByDate();
        break;
    case 3:
        SortByBalance();
        break;
    default:
        printf("Wrong input\n\n");
    }
}
void Report()
{
    char lines[100][100],acc_num[11],fNAME[20];
    int numLines=0,j=0;
    printf("Enter account number: ");
    scanf("%s",acc_num);
    if(found_acc(acc_num,&j))
    {
        sprintf(fNAME, "%s.txt",acc_num );

    }
    else
    {
        printf("Error\n\n");
        return;
    }
    fptr=fopen(fNAME, "r");
    if(fptr==NULL)
    {
        printf("Error opening file for reading.\n\n");
        return;
    }
    while (fgets(lines[numLines], 100, fptr) != NULL)
    {
        numLines++;
    }
    int i;
    if(numLines>=5)
    {
        i=numLines-5;
    }
    else  i=0;
    for(; i<numLines; i++)
        printf("%s",lines[i]);
    printf("\n");
    fclose(fptr);
}
void quit()
{
    exit(1);
}
void menu()
{
    while (1)
    {
        char x[5],y[5]; //not int to prevent the user from entering a false characters, when using int it makes an infinite loop and can't be well controlled
        printf("1-LOGIN\n");
        printf("2-QUIT\n");
        scanf("%s",x);
        printf("\n");
        if(atoi(x) == 1)
        {
            if(LogIn())
            {
                printf("\n");
                while(1)
                {
                    load();
                    //printf("%d\n",count);
                    printf("1-ADD\n");
                    printf("2-DELETE\n");
                    printf("3-MODIFY\n");
                    printf("4-SEARCH\n");
                    printf("5-ADVANCED SEARCH\n");
                    printf("6-WITHDRAW\n");
                    printf("7-DEPOSIT\n");
                    printf("8-TRANSFER\n");
                    printf("9-REPORT\n");
                    printf("10-PRINT\n");
                    printf("11-QUIT\n");
                    scanf("%s",y);
                    printf("\n");
                    switch(atoi(y))
                    {
                    case 1:
                        Add();
                        break;
                    case 2:
                        Delete();
                        break;
                    case 3:
                        Modify();
                        break;
                    case 4:
                        search();
                        break;
                    case 5:
                        Advanced_Search();
                        break;
                    case 6:
                        Withdraw();
                        break;
                    case 7:
                        Deposit();
                        break;
                    case 8:
                        Transfer();
                        break;
                    case 9:
                        Report();
                        break;
                    case 10:
                        PrintSort();
                        break;
                    case 11:
                        quit();
                        break;
                    default:
                        printf("Wrong input\n\n");
                    }
                }
            }
        }
        else if(atoi(x) == 2)
            quit();
        else
            printf("Invalid input\n\n");
    }

}
int main()
{
    menu();
    return 0;
}

