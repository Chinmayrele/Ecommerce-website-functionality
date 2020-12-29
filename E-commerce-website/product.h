//STRUCTURE DECLARATION
struct Product
{
    int ID;
    char name[100];
    char description[100];
    int price;
    char category[100];
    struct Product *next;
}*product_head=NULL;



//DECLARATION OF FUNCTION
void extract_product_name_from_file();
void add_product(struct Product*);
void create_product(char[100]);
void print_product_according_category(char[100]);
void remove_product(int);
int product_available(char[100]);
struct Product* find_product_by_id(int);




//DEFINITION OF FUNCTION
void create_product(char category_name[100])
{
    FILE *product_ptr;
    struct Product*p;
    char product_name[100];
    char product_description[100];
    int product_price;
    int product_ID;

    product_ptr=fopen("products.txt","a");
    if(product_ptr==NULL)
    {
        printf("ERROR! File opening\n");
        exit(1);
    }
    printf("Enter the product Detail->\n");
    printf("Enter the product name:");
    scanf(" %[^\n]",product_name);
    printf("Enter the product description:");
    scanf(" %[^\n]",product_description);
    printf("Enter the product price:");
    scanf("%d",&product_price);
    product_ID=generate_uuid();
    fprintf(product_ptr,"%d=%s=%s=%d=%s\n",product_ID,product_name,product_description,product_price,category_name);
    fclose(product_ptr);

    p=(struct Product*)malloc(sizeof(struct Product));
    if(p==NULL)
    {
        printf("Heap overflow!");
        exit(1);
    }
    p->ID=product_ID;
    copy_item(p->name,product_name);
    copy_item(p->description,product_description);
    p->price=product_price;
    copy_item(p->category,category_name);
    p->next=NULL;
    add_product(p);

    printf("Product created successfully\n");


}


void extract_product_name_from_file()
{
    FILE *product_ptr;
    struct Product *p=NULL;
    char product_name[100];
    char product_description[100];
    char product_category[100];
    char product_price[100];
    char product_ID[100];
    char read;
    int temp=0;
    product_ptr=fopen("products.txt","r");
    if(product_ptr==NULL)
    {
        printf("ERROR! File opening\n");
        exit(1);
    }
    product_ID[0]='\0';
    product_name[0]='\0';
    product_description[0]='\0';
    product_price[0]='\0';
    product_category[0]='\0';
    do
    {
        read=fgetc(product_ptr);
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
                    product_ID[strlen(product_ID)+1]='\0';
                    product_ID[strlen(product_ID)]=read;
                }
                else if(temp==1)
                {
                    product_name[strlen(product_name)+1]='\0';
                    product_name[strlen(product_name)]=read;
                }
                else if(temp==2)
                {
                    product_description[strlen(product_description)+1]='\0';
                    product_description[strlen(product_description)]=read;
                }
                else if(temp==3)
                {
                    product_price[strlen(product_price)+1]='\0';
                    product_price[strlen(product_price)]=read;
                }
                else
                {
                    product_category[strlen(product_category)+1]='\0';
                    product_category[strlen(product_category)]=read;
                }
            }
            else
            {
                p=(struct Product*)malloc(sizeof(struct Product));
                if(p==NULL)
                {
                    printf("Heap overflow!");
                    exit(1);
                }
                p->ID=atoi(product_ID);
                copy_item(p->name,product_name);
                copy_item(p->description,product_description);
                p->price=atoi(product_price);
                copy_item(p->category,product_category);
                p->next=NULL;
                add_product(p);
                product_ID[0]='\0';
                product_name[0]='\0';
                product_description[0]='\0';
                product_price[0]='\0';
                product_category[0]='\0';
                temp=0;
            }
        }
    }
    while(!feof(product_ptr));
}



void add_product(struct Product *p)
{
    struct Product*q=product_head;
    if(product_head==NULL)
    {
        product_head=p;
    }
    else
    {
        while(q->next!=NULL)
        {
            q=q->next;
        }
        q->next=p;
    }
}


void remove_product(int id)
{
    struct Product*p=product_head,*q=NULL;
    FILE *product_ptr;
    if(product_head==NULL)
    {
        printf("No product found with that ID\n");
        return;
    }
    else if(product_head->ID==id)
    {
        product_head=p->next;
        free(p);
    }
    else
    {
        while(p!=NULL)
        {
            if(p->ID==id)
            {
                q->next=p->next;
                free(p);
                break;
            }
            q=p;
            p=p->next;
        }
        if(q->next==NULL)
        {
            printf("No product found with that ID\n");
            return;
        }
    }
    product_ptr=fopen("products.txt","w");
    if(product_ptr==NULL)
    {
        printf("ERROR! File opening\n");
        exit(1);
    }
    p=product_head;
    while(p!=NULL)
    {
        fprintf(product_ptr,"%d=%s=%s=%d=%s\n",p->ID,p->name,p->description,p->price,p->category);
        p=p->next;
    }
    fclose(product_ptr);
    printf("Product remove successfully\n");

}


void print_product_according_category(char category_name[100])
{
    struct Product*p=product_head;
    while(p!=NULL)
    {
        if(strcmp(p->category,category_name)==0)
        {
            printf("-> %d=%s=%d\n",p->ID,p->name,p->price);
        }
        p=p->next;
    }

}

int product_available(char category_name[100])
{
    struct Product*p=product_head;
    if(product_head==NULL)
    {
        return 0;
    }
    else
    {
        while(p!=NULL)
        {
            if(strcmp(p->category,category_name)==0)
            {
                return 1;
            }
            p=p->next;
        }
        return 0;
    }
}


struct Product* find_product_by_id(int id)
{
    struct Product*p=product_head;
    if(product_head==NULL)
    {
        return NULL;
    }
    else
    {
        while(p!=NULL)
        {
            if(p->ID==id)
            {
                return p;
            }
        }
        return NULL;
    }
}
