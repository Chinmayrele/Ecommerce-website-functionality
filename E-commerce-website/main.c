#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<conio.h>
#include<ctype.h>
#include<time.h>
#include "helper.h"
#include "category.h"
#include "product.h"


struct User
{
    int ID;
    char name[100];
    char email[100];
    char password[100];
    struct User*next;
}*user_head=NULL;

struct User*find_user_by_email(char[100]);
void extract_users_from_file();
void add_user(struct User*);
void sign_up();
struct User*login();
int email_verify(char[100]);
int password_verify(char[100]);
void update_user_file();

void sign_up()
{
    FILE *user_ptr;
    struct User*u;
    char user_name[100];
    char user_email[100];
    char user_password[100];
    char confirm_password[100];
    int user_ID,i;

    printf("Enter your Details->\n");
    printf("Enter the name:");
    scanf(" %[^\n]",user_name);
    do
    {
        printf("Enter the email:");
        scanf(" %[^\n]",user_email);
    }
    while(!email_verify(user_email));
    do
    {
        do
        {
            i=0;
            printf("Enter the password:");
            do
            {
                user_password[i]=getch();
                if(user_password[i]==13)
                {
                    break;
                }
                printf("*");
                i++;
            }
            while(1);
            printf("\n");
            user_password[i]='\0';
        }
        while(!password_verify(user_password));
        i=0;
        printf("Enter password again to confirm:");
        do
        {
            confirm_password[i]=getch();
            if(confirm_password[i]==13)
            {
                break;
            }
            printf("*");
            i++;
        }
        while(1);
        printf("\n");
        confirm_password[i]='\0';
        if(strcmp(user_password,confirm_password)!=0)
        {
            printf("Password is not matched, Try again!\n");
        }
    }
    while(strcmp(user_password,confirm_password)!=0);
    user_ID=generate_uuid();
    if(find_user_by_email(user_email)==NULL)
    {
        user_ptr=fopen("users.txt","a");
        if(user_ptr==NULL)
        {
            printf("ERROR! File opening\n");
            exit(1);
        }
        fprintf(user_ptr,"%d=%s=%s=%s\n",user_ID,user_name,user_email,encrypt(user_password));
        fclose(user_ptr);

        u=(struct User*)malloc(sizeof(struct User));
        if(u==NULL)
        {
            printf("Heap overflow!");
            exit(1);
        }
        u->ID=user_ID;
        copy_item(u->name,user_name);
        copy_item(u->email,user_email);
        copy_item(u->password,encrypt(user_password));
        u->next=NULL;
        add_user(u);

        printf("User created successfully, Now you can login\n");
    }
    else
    {
        printf("User with email %s is already sign up, you can login\n",user_email);
    }
}

struct User*login()
{
    struct User*u=NULL;
    char user_email[100];
    char user_password[100];
    char confirm_password[100];
    int reset,i;
    printf("Enter the login details->\n");
    do
    {
        printf("Enter the email:");
        scanf(" %[^\n]",user_email);
    }
    while(!email_verify(user_email));
    printf("Enter the password:");
    i=0;
    do
    {
        user_password[i]=getch();
        if(user_password[i]==13)
        {
            break;
        }
        printf("*");
        i++;
    }
    while(1);
    printf("\n");
    user_password[i]='\0';
    u=find_user_by_email(user_email);
    if(u!=NULL)
    {
        if(strcmp(decrypt(u->password),user_password)==0)
        {
            return u;
        }
        else
        {
            printf("Password is wrong\n");
            printf("Want to reset password YES[1]/NO[0]:");
            scanf("%d",&reset);
            if(reset==1)
            {
                do
                {
                    do
                    {
                        printf("Enter the password:");
                        i=0;
                        do
                        {
                            user_password[i]=getch();
                            if(user_password[i]==13)
                            {
                                break;
                            }
                            printf("*");
                            i++;
                        }
                        while(1);
                        printf("\n");
                        user_password[i]='\0';
                    }
                    while(!password_verify(user_password));
                    printf("Enter password again to confirm:");
                    i=0;
                    do
                    {
                        confirm_password[i]=getch();
                        if(confirm_password[i]==13)
                        {
                            break;
                        }
                        printf("*");
                        i++;
                    }
                    while(1);
                    printf("\n");
                    confirm_password[i]='\0';
                    if(strcmp(user_password,confirm_password)!=0)
                    {
                        printf("Password is not matched, Try again!\n");
                    }
                }
                while(strcmp(user_password,confirm_password)!=0);
                copy_item(u->password,encrypt(user_password));
                update_user_file();
                printf("Password reset successfully, Now you can login\n");
            }
            return NULL;
        }
    }
    else
    {
        printf("email not registered, first you have to sign up\n");
        return NULL;
    }
}


void extract_users_from_file()
{
    FILE *user_ptr;
    struct User *u=NULL;
    char user_name[100];
    char user_email[100];
    char user_password[100];
    char user_ID[100];
    char read;
    int temp=0;
    user_ptr=fopen("users.txt","r");
    if(user_ptr==NULL)
    {
        printf("ERROR! File opening\n");
        exit(1);
    }
    user_ID[0]='\0';
    user_name[0]='\0';
    user_email[0]='\0';
    user_password[0]='\0';
    do
    {
        read=fgetc(user_ptr);
        if(read=='=')
        {
            temp++;
        }
        else
        {
            if(read!='\n')
            {

                if(temp==0)
                {
                    user_ID[strlen(user_ID)+1]='\0';
                    user_ID[strlen(user_ID)]=read;
                }
                else if(temp==1)
                {
                    user_name[strlen(user_name)+1]='\0';
                    user_name[strlen(user_name)]=read;
                }
                else if(temp==2)
                {
                    user_email[strlen(user_email)+1]='\0';
                    user_email[strlen(user_email)]=read;
                }
                else
                {
                    user_password[strlen(user_password)+1]='\0';
                    user_password[strlen(user_password)]=read;
                }
            }
            else
            {
                u=(struct User*)malloc(sizeof(struct User));
                if(u==NULL)
                {
                    printf("Heap overflow!");
                    exit(1);
                }
                u->ID=atoi(user_ID);
                copy_item(u->name,user_name);
                copy_item(u->email,user_email);
                copy_item(u->password,user_password);
                u->next=NULL;
                add_user(u);
                user_ID[0]='\0';
                user_name[0]='\0';
                user_email[0]='\0';
                user_password[0]='\0';
                temp=0;
            }
        }
    }
    while(!feof(user_ptr));
}



void add_user(struct User *u)
{
    struct User*q=user_head;
    if(user_head==NULL)
    {
        user_head=u;
    }
    else
    {
        while(q->next!=NULL)
        {
            q=q->next;
        }
        q->next=u;
    }
}

struct User*find_user_by_email(char email[100])
{
    struct User*u=user_head;
    if(user_head==NULL)
    {
        return NULL;
    }
    else
    {
        while(u!=NULL)
        {
            if(strcmp(u->email,email)==0)
            {
                return u;
            }
            u=u->next;
        }
        return NULL;
    }
}

int email_verify(char email[100])
{
    int count1=0;
    int count2=0;
    for(int i=0; i<strlen(email); i++)
    {
        if(email[i]=='@')
        {
            count1++;
        }
        else if(email[i]=='.')
        {
            count2++;
        }
    }

    if(count1==1 && count2==1)
    {
        return 1;
    }
    else
    {
        printf("Email is invalid!\n");
        return 0;
    }
}


int password_verify(char password[100])
{
    if(strlen(password)>=6)
    {
        return 1;
    }
    else
    {
        printf("Password should contain atleast 6 character\n");
        return 0;
    }
}

void update_user_file()
{
    FILE *user_ptr;
    struct User*u=user_head;
    user_ptr=fopen("users.txt","w");
    if(user_ptr==NULL)
    {
        printf("ERROR! File opening\n");
        exit(1);
    }
    while(u!=NULL)
    {
        fprintf(user_ptr,"%d=%s=%s=%s\n",u->ID,u->name,u->email,u->password);
        u=u->next;
    }
    fclose(user_ptr);
}


int main()
{
    struct User*customer;
    char categories_name[100][100];
    int total_category;
    int choice1,choice2,choice3,choice4,choice5;
    int i;
    char read;
    char category_name[100];
    char secret[100];
    int id;
    FILE *secret_ptr,*category_ptr;

    extract_category_name_from_file(categories_name,&total_category);
    extract_product_name_from_file();
    extract_users_from_file();

    do
    {
        printf("1.Enter as an admin\n2.Enter as a customer\n3.EXIT\n");
        printf("Enter the option:");
        scanf("%d",&choice1);
        if(choice1==1)
        {
            printf("Enter the password to enter:");
            i=0;
            do
            {
                secret[i]=getch();
                if(secret[i]==13)
                {
                    break;
                }
                printf("*");
                i++;
            }
            while(1);
            printf("\n");
            secret[i]='\0';
            secret_ptr=fopen("secret.txt","r");
            if(secret_ptr==NULL)
            {
                printf("ERROR! File opening\n");
                exit(1);
            }
            i=0;
            do
            {
                read=fgetc(secret_ptr);
                if(read!=secret[i] )
                {
                    break;
                }
                i++;
            }
            while(!feof(secret_ptr) && i<strlen(secret));

            if(feof(secret_ptr)|| i>=strlen(secret))
            {
                fclose(secret_ptr);
                printf("HELLO ADMIN!\n");
                do
                {
                    printf("1.Add the product\n2.Remove the product\n3.BACK\n");
                    printf("Enter the option:");
                    scanf("%d",&choice2);
                    if(choice2==1)
                    {
                        do
                        {
                            printf("1.Add product in the existing category\n2.Add product in the new category\n3.BACK\n");
                            printf("Enter the option:");
                            scanf("%d",&choice3);
                            if(choice3==1)
                            {
                                if(total_category!=0)
                                {
                                    printf("Categories->\n");
                                    print_categories(categories_name,total_category);
                                    printf("Enter the name of the category:");
                                    scanf(" %[^\n]",category_name);
                                    if(search_category(categories_name,total_category,category_name))
                                    {
                                        create_product(category_name);
                                    }
                                    else
                                    {
                                        printf("No such category found , create new category\n");
                                    }

                                }
                                else
                                {
                                    printf("No category available, create a new category\n");
                                }
                            }
                            else if(choice3==2)
                            {
                                printf("Enter the name of the new category:");
                                scanf(" %[^\n]",category_name);
                                category_ptr=fopen("categories.txt","a");
                                if(category_ptr==NULL)
                                {
                                    printf("ERROR! File opening\n");
                                    exit(1);
                                }
                                fprintf(category_ptr,"%s\n",category_name);
                                fclose(category_ptr);
                                add_category(categories_name,category_name,&total_category);
                                create_product(category_name);
                            }
                            else
                            {
                                if(choice3!=3)
                                {
                                    printf("Invalid operation\n");
                                }
                            }
                        }
                        while(choice3!=3);
                    }
                    else if(choice2==2)
                    {
                        printf("Categories->\n");
                        print_categories(categories_name,total_category);
                        printf("Enter the name of the category:");
                        scanf(" %[^\n]",category_name);
                        if(search_category(categories_name,total_category,category_name))
                        {
                            if(product_available(category_name))
                            {
                                printf("Products according to the selected category->\n\n");
                                printf("Products are display as [ID=name=price]\n");
                                print_product_according_category(category_name);
                                printf("Enter the ID of the product you want to remove:");
                                scanf("%d",&id);
                                remove_product(id);

                            }
                            else
                            {
                                printf("No product available of that category\n");
                            }
                        }
                        else
                        {
                            printf("No such category found , create new category\n");
                        }
                    }
                    else
                    {
                        if(choice2!=3)
                        {
                            printf("Invalid operation\n");
                        }
                    }
                }
                while(choice2!=3);
            }
            else
            {
                printf("Incorrect password\n");
            }
        }
        else if(choice1==2)
        {
            do
            {
                printf("1.Sign Up\n2.Login\n3.Back\n");
                printf("Enter the option:");
                scanf("%d",&choice4);
                if(choice4==1)
                {

                    sign_up();
                }
                else if(choice4==2)
                {
                    customer=login();
                    if(customer!=NULL)
                    {
                        printf("HELLO %s! WELCOME TO THE APNI DUKAN!\n\n",customer->name);
                        do
                        {
                            printf("1.Shopping\n2.My carts\n3.My orders\n4.My wishlist\n5.BACK\n");
                            printf("Enter the option:");
                            scanf("%d",&choice5);
                            if(choice5==1)
                            {

                            }
                            else if(choice5==2)
                            {

                            }
                            else if(choice5==3)
                            {

                            }
                            else if(choice5==4)
                            {

                            }
                            else
                            {
                                if(choice5!=5)
                                {
                                    printf("Invalid option\n");
                                }
                            }
                        }
                        while(choice5!=5);
                    }
                }
                else
                {
                    if(choice4!=3)
                    {
                        printf("Invalid option\n");
                    }
                }
            }
            while(choice4!=3);

        }
        else
        {
            if(choice1!=3)
            {
                printf("Invalid operation\n");
            }
        }
    }
    while(choice1!=3);
    return 0;
}
