#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <ctype.h>
#define smax 1000

struct customer
    {
        int customer_identifier;
        char customer_name[100];
        int contact;
        char address[200];
        char city[99];
        char date[15];

    };
struct product_detail
    {
        int product_identifier;
        char product_name[100];
        char product_model[100];
        int quantity;
        double unit_price;
    };
struct transaction
{
    int transaction_identifier;
    char type[6];
    double value;
    char processing_date[15];
    customer c;
    product_detail p;
};
struct List{
    int n;
    transaction nodes[smax];
};
int input(List &head);
void Initialize(List &head){
    (head).n= 0;
}
bool EmptyList(List head){
    return (head.n==0);
}
bool FullList(List head){
    return (head.n==smax);
}
void Insert(List &head,int pos,transaction info){
    if (pos<0||pos>head.n) return;
    if (FullList(head)) return;
    for (int i=head.n-1;i>=pos;i--) head.nodes[i+1]=head.nodes[i];
    head.nodes[pos]=info;(head.n)++;return;
}
void PrintTrans(List &head){
    if (EmptyList(head)) return;
    printf("\n");
    for (int i=0;i<head.n;i++){
        printf("\n%s%20d",head.nodes[i].c.customer_name,head.nodes[i].c.customer_identifier);
    }
}
int check_8digits(char *s){
    if (strlen(s)>8||strlen(s)<8)
    {
        printf("Must be 8 digits");
        return 0;
    }

    for (int i=0;i<(int)strlen(s);i++){
        if (s[i]>=58 || s[i]<=47)
        {
            printf("Identifier must be a digit");
            return 0;
        }
    }
    return 1;
}
int check_12digits(char *s){
    if (strlen(s)>12||strlen(s)<12)
    {
        printf("Must be 12 digits");
        return 0;
    }
    for (int i=0;i<(int)strlen(s);i++){
        if (s[i]>=58 || s[i]<=47)
        {
            printf("Contact number must be digit");
            return 0;
        }
    }
    return 1;
}
bool check_day(int d,int m,int y){
    if (d<=0||d>31||m<=0||m>12||y<=0) return false;
    int ngaythang[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if ((y%4==0 && y%100!= 0) || y%400==0) ngaythang[1]+=1;
    if (d<=ngaythang[m-1]) return true;
}
int check_date_format(char *s){
    char *token;token=strtok(s,"/");
    int day,month,year; int i=0;
    while (token!=NULL){
        switch(i){
            case 0:day=atoi(token);break;
            case 1:month=atoi(token);break;
            case 2:year=atoi(token);break;
        }
        i++;token=strtok(NULL,"/");
    }
    if (check_day(day,month,year)) return 1;
    else return 0;
}
int check_only_name(char *s){
    for (int i=0;i<strlen(s);i++){
        if ((s[i]>=65&&s[i]<=90)||(s[i]>=97&&s[i]<=122)||s[i]==32) continue;
        else {printf("Must only contain character");return 0;}
    }
    return 1;
}
void delete_redundant_space(char *s){
    for (int i=0;i<strlen(s);i++){
        while ((s[i]==32 && s[i+1]==32)){
            for (int j=i+1;j<strlen(s);j++){
                if (j==strlen(s)-1) {s[j]='\0'; break;}
                s[j]=s[j+1];
            }
        }
    }
}
void lowercase(char *s){
    for (int i=0;i<strlen(s);i++){
        if (s[i]>=97 && s[i]<=122 || s[i]==32) continue;
        s[i]+=32;
    }
}
int check_type(char *s){
    lowercase(s);
    if (strcmp(s,"whole")==0||strcmp(s,"retail")==0||strcmp(s,"other")==0) return 1;
    return 0;
}
bool readfile(char *filename, List &head){
    FILE *fp; bool ret=true;
    fp=fopen(filename,"r+");
    if (fp!=NULL){
        while (!feof(fp)){
            transaction listt;
            char data[1000];
            fgets(data,1000,fp);
            char *token=strtok(data,"-");
            int i=1,success=1;
            while (token!=NULL){
                delete_redundant_space(token);
                switch (i){
                    case 1:{
                        if (check_8digits(token)==0) success=0;
                        else listt.transaction_identifier=atoi(token);
                        break;
                    }
                    case 2: {
                        if (check_8digits(token)==0) success=0;
                        else listt.c.customer_identifier=atoi(token);
                        break;
                    }
                    case 3: {
                        char temp[50]; strcpy(temp,token);
                        if (check_only_name(temp)==0) success=0;
                        else strcpy(listt.c.customer_name,token);
                        break;
                    }
                    case 4:{
                        if (check_12digits(token)==0) success=0;
                        else listt.c.contact=atoi(token);
                        break;
                    }
                    case 5: strcpy(listt.c.address,token);break;
                    case 6: strcpy(listt.c.city,token);break;
                    case 7: {
                        if (check_type(token)==0) success=0;
                        else {lowercase(token);strcpy(listt.type,token);}
                        break;}
                    case 8: listt.value=atof(token);break;
                    case 9:{
                        if (check_8digits(token)==0) success=0;
                        else listt.p.product_identifier=atoi(token);
                        break;
                    }
                    case 10: strcpy(listt.p.product_name,token);break;
                    case 11: strcpy(listt.p.product_model,token);break;
                    case 12: listt.p.quantity=atoi(token);break;
                    case 13: listt.p.unit_price=atof(token);break;
                    case 14:{
                        char temp[15]; strcpy(temp,token);
                        if (check_date_format(temp)==0) success=0;
                        else strcpy(listt.processing_date,token);
                        break;
                    }
                }
                if (success==0) break;
                i++; token=strtok(NULL,"-");
            }
            if (success==1) Insert(head,head.n,listt);
        }
        fclose(fp);ret=true;
    }
    else ret=false;
    return ret;
}
int Execute_other_command ()
{
    int ask;
    printf("=======================================================================================\n");
    printf("||  To execute others commands: Press 1                                              ||\n");
    printf("||  To quit: Press 2                                                                 ||\n");
    printf("=======================================================================================\n");
    printf("||  Press 1 or 2:                                                                     ");
    scanf("%d",&ask);
}
void printthemostvaluedCustomer(List head){
    int maxtrans=0;
    printf("================================Most Valued Customers==================================\n");
    printf("||  Name\t\tNumbers of Transactions\t\tTotalValue                   ||\n");
    for (int i=0;i<head.n;i++){
        int maxcount=1;
        for (int j=i+1;j<head.n;j++){
            char temp1[50],temp2[50];
            strcpy(temp1,head.nodes[i].c.customer_name);strcpy(temp2,head.nodes[j].c.customer_name);
            lowercase(temp1);lowercase(temp2);
            if (strcmp(temp1,temp2)==0) maxcount++;
        }
        if (maxcount>maxtrans) maxtrans=maxcount;
    }
    for (int i=0;i<head.n;i++){
        int maxcount=1; double totalvalue=head.nodes[i].value;
        for (int j=i+1;j<head.n;j++){
            char temp1[50],temp2[50];
            strcpy(temp1,head.nodes[i].c.customer_name);strcpy(temp2,head.nodes[j].c.customer_name);
            lowercase(temp1);lowercase(temp2);
            if (strcmp(temp1,temp2)==0) {maxcount++;totalvalue+=head.nodes[j].value;}
        }
        if (maxcount==maxtrans) printf("||  %s\t%10d\t%25.2lf\n",head.nodes[i].c.customer_name,maxtrans,totalvalue);
    }


}
void printthemostvaluedProduct(List head){
    int maxtrans=0;
    printf("=================================Most Valued Product===================================\n");
    printf("||  Name\t\tNumbers of Transactions\t\tTotalValue                   ||\n");
    for (int i=0;i<head.n;i++){
        int maxcount=1;
        for (int j=i+1;j<head.n;j++){
            char temp1[50],temp2[50];
            strcpy(temp1,head.nodes[i].p.product_name);strcpy(temp2,head.nodes[j].p.product_name);
            lowercase(temp1);lowercase(temp2);
            if (strcmp(temp1,temp2)==0) maxcount++;
        }
        if (maxcount>maxtrans) maxtrans=maxcount;
    }
    for (int i=0;i<head.n;i++){
        int maxcount=1; double totalvalue=head.nodes[i].value;
        for (int j=i+1;j<head.n;j++){
            char temp1[50],temp2[50];
            strcpy(temp1,head.nodes[i].p.product_name);strcpy(temp2,head.nodes[j].p.product_name);
            lowercase(temp1);lowercase(temp2);
            if (strcmp(temp1,temp2)==0) {maxcount++;totalvalue+=head.nodes[j].value;}
        }
        if (maxcount==maxtrans) printf("||  %s\t\t%10d\t%25.2lf\n",head.nodes[i].p.product_name,maxtrans,totalvalue);
    }


}
void quantityproduct(List head){
    int maxquantity=0;
    printf("============================Most Sold Product in Quantity==============================\n");
    printf("||  Name\t\tQuantity                                                        ||\n");
    for (int i=0;i<head.n;i++){
        int maxquantitycount=head.nodes[i].p.quantity;
        for (int j=i+1;j<head.n;j++){
            char temp1[50],temp2[50];
            strcpy(temp1,head.nodes[i].p.product_name);strcpy(temp2,head.nodes[j].p.product_name);
            lowercase(temp1);lowercase(temp2);
            if (strcmp(temp1,temp2)==0) {maxquantitycount+=head.nodes[j].p.quantity;}
        }
        if (maxquantitycount>maxquantity) maxquantity=maxquantitycount;
    }
    for (int i=0;i<head.n;i++){
        int maxquantitycount=head.nodes[i].p.quantity;
        for (int j=i+1;j<head.n;j++){
            char temp1[50],temp2[50];
            strcpy(temp1,head.nodes[i].p.product_name);strcpy(temp2,head.nodes[j].p.product_name);
            lowercase(temp1);lowercase(temp2);
            if (strcmp(temp1,temp2)==0) {maxquantitycount+=head.nodes[j].p.quantity;}
        }
        if (maxquantitycount==maxquantity) printf("||   %s\t%5d\n",head.nodes[i].p.product_name,maxquantity);

    }

}
void addelementincharmatrix(char (*a)[20],char *s, int &sizee){
    for (int i=0;i<sizee;i++){
        if (strcmp(a[i],s)==0) return;
    }
    sizee++; strcpy(a[sizee-1],s);
}
void printcities(List head){
    int maxtrans=0;
    printf("==================================Most Valued Cities===================================\n");
    printf("||  Cities                                                                           ||\n");
    for (int i=0;i<head.n;i++){
        int maxcount=1;
        for (int j=i+1;j<head.n;j++){
            char temp1[50],temp2[50];
            strcpy(temp1,head.nodes[i].c.customer_name);strcpy(temp2,head.nodes[j].c.customer_name);
            lowercase(temp1);lowercase(temp2);
            if (strcmp(temp1,temp2)==0) maxcount++;
        }
        if (maxcount>maxtrans) maxtrans=maxcount;
    }
    char mostvaluecity[100][20];
    int sizex=1;
    for (int i=0;i<head.n;i++){
        int maxcount=1;
        for (int j=i+1;j<head.n;j++){
            char temp1[50],temp2[50];
            strcpy(temp1,head.nodes[i].c.customer_name);strcpy(temp2,head.nodes[j].c.customer_name);
            lowercase(temp1);lowercase(temp2);
            if (strcmp(temp1,temp2)==0) maxcount++;
        }
        if (maxcount==maxtrans) addelementincharmatrix(mostvaluecity,head.nodes[i].c.city,sizex);
    }
    for (int i=0;i<sizex;i++) printf("||    %s\n",mostvaluecity[i]);

}
void swapp(transaction &a, transaction &b){
    transaction temp=a;
    a=b;b=temp;
}
void sortproductname(List &head){
    for (int i=0;i<head.n-1;i++){
        for (int j=i+1;j<head.n;j++){
            char temp1,temp2;
            temp1=head.nodes[i].p.product_name[0];temp2=head.nodes[j].p.product_name[0];
            temp1=tolower(temp1);temp2=tolower(temp2);
            if (temp2<temp1) swapp(head.nodes[i],head.nodes[j]);
        }
    }
}
void printtransactionsontype(List &head){
    printf("\n\n\t\t__Transaction List follow by type__");
    char typeinput[10]; int check=0;
    while (check==0){
        printf("\nInput the transaction type: ");fflush(stdin);gets(typeinput);fflush(stdin);
        lowercase(typeinput); delete_redundant_space(typeinput);
        if (strcmp(typeinput,"whole")==0 || strcmp(typeinput,"retail")==0 || strcmp(typeinput,"other")==0) check=1;
    }
    sortproductname(head);
    int i=1;
    for (int j=0;j<head.n;j++){
        if (strcmp(typeinput,head.nodes[j].type)==0){
            printf("==========================Transaction List according to Type===========================\n");
            printf("\nTransaction %d Detail",i);
            printf("\nTransaction Identifier: %d",head.nodes[j].transaction_identifier);
            printf("\nProduct Name: %s",head.nodes[j].p.product_name);
            printf("\nProduct Identifier: %d",head.nodes[j].p.product_identifier);
            printf("\nProduct Model: %s",head.nodes[j].p.product_model);
            printf("\nQuantity: %d",head.nodes[j].p.quantity);
            printf("\nUnit Price: %.2lf",head.nodes[j].p.unit_price);
            printf("\nCustomer Identifier: %d",head.nodes[j].c.customer_identifier);
            printf("\nCustomer Name: %s",head.nodes[j].c.customer_name);
            printf("\nContact Phone: %d",head.nodes[j].c.contact);
            printf("\nAddress: %s",head.nodes[j].c.address);
            printf("\nCity: %s",head.nodes[j].c.city);
            printf("\nType: %d",head.nodes[j].type); printf("\nValue %.2lf",head.nodes[j].value);
            printf("\nProcess Date: %s",head.nodes[j].processing_date);
        }
        i++;
    }
    printf("\n");

}
void data_selection(List &head){
    int choicee;
    char filename[255];
    printf("=======================================================================================\n");
    printf("||                                                                                   ||\n");
    printf("||  Input data from a file:   Press 1                                                ||\n");
    printf("||  Input data by hand:   Press 2                                                    ||\n");
    printf("||                                                                                   ||\n");
    printf("=======================================================================================\n");
    printf("||  Press a number:                                                                   ");
    scanf("%d",&choicee);
    switch (choicee){
    case 1:
        printf("=======================================================================================\n");
        printf("||  Enter your file name: ");
        scanf("%s",&filename);
        //printf("\n%s",filename);
        //readfile(filename,head);
        break;
    /*case 2:
        int n;
        List head;
        printf("=======================================================================================\n");
        printf("||  Transaction Amount: ");

        scanf("%d",&n);
        printf("=======================================================================================\n");
        for (int i=0;i<=n;i++)
        {
            printf("||    Transaction NO.%d",i);
            fflush(stdin);
            printf("\n||    Transaction Identifier: ");
            gets(head.nodes[i].transaction_identifier);
            check_8digits(head.nodes[i].transaction_identifier);
            printf("||    Product Name: ");
            gets(head.nodes[i].p.product_name);
            check_only_name(head.nodes[i].p.product_name);

            printf("||    Product Identifier: ");
            scanf("%d",head.nodes[i].p.product_identifier);
            check_8digits(head.nodes[i].p.product_identifier);

            printf("||    Product Model: ");
            gets(head.nodes[i].p.product_model)

            printf("||    Quantity: ");
            scanf("%d",head.nodes[i].p.quantity)

            printf("||    Unit Price: ");
            scanf("%d",head.nodes[i].p.unit_price);

            printf("||    Customer Identifier: ");
            scanf("%d",head.nodes[i].c.customer_identifier);
            check_8digits(head.nodes[i].c.customer_identifier);

            printf("||    Customer Name: ");
            gets(head.nodes[i].c.customer_name);
            check_only_name(head.nodes[i].c.customer_name);

            printf("||    Customer Contact: ");
            scanf("%d",head.nodes[i].c.contact);
            check_12digits(head.nodes[i].c.contact);

            printf("||    Customer Address: ");
            gets(head.nodes[i].c.address);

            printf("||    Customer Date:");
            gets(head.nodes[i].c.date);
            check_date_format(head.nodes[i].c.date);
            printf("=======================================================================================\n");
            }
            printf("=======================================================================================\n");
            printf("||  To execute others commands: Press 1                                              ||\n");
            printf("||  To quit: Press 2                                                                 ||\n");
            printf("=======================================================================================\n");
            printf("||  Press 1 or 2:                                                                     ");
            scanf("%d",&choice);
            switch (choice){
                case 1: input(); break;
                case 2: return ;}
        break;*/

}
}
int input(List &head){
    printf("===================Welcome to Enterprise Customer Management Service===================\n");
    printf("||                                                                                   ||\n");
    //int ask=1;
    //while (ask==1){
    printf("||  Input data: Press 1                                                              ||\n");
    printf("||  List the most value customers: Press 2                                           ||\n");
    printf("||  List the most value products: Press 3                                            ||\n");
    printf("||  List the products which were purchased the most in terms of quantity: Press 4    ||\n");
    printf("||  List the most valued cities: Press 5                                             ||\n");
    printf("||  Given a transaction type, list the details of all the transactions: Press 6      ||\n");
    printf("||                                                                                   ||\n");
    printf("=======================================================================================\n");
    int choice;
    printf("||  Press a number:                                                                   ");
    scanf("%d",&choice);
    switch (choice){
    case 1:
        int choicee;
        char filename[255];
        printf("=======================================================================================\n");
        printf("||                                                                                   ||\n");
        printf("||  Input data from a file:   Press 1                                                ||\n");
        printf("||  Input data by hand:   Press 2                                                    ||\n");
        printf("||                                                                                   ||\n");
        printf("=======================================================================================\n");
        printf("||  Press a number:                                                                   ");
        scanf("%d",&choicee);
        switch (choicee){
        case 1:
            printf("=======================================================================================\n");
            printf("||  Enter your file name: ");fflush(stdin);
            gets(filename); fflush(stdin);
            readfile(filename,head); printf("\nDMM");
            break;
        }
        //ask=Execute_other_command();
        break;
    case 2:
        printthemostvaluedCustomer(head);
        //ask=Execute_other_command();
        break;
    case 3:
        printthemostvaluedProduct(head);
        //ask=Execute_other_command();
        break;
    case 4:
        quantityproduct(head);
        //ask=Execute_other_command();
        break;
    case 5:
        printcities(head);
        //ask=Execute_other_command();
        break;
    case 6:
        printtransactionsontype(head);
        //ask=Execute_other_command();
        break;
    }
    //}
}
int main(){
    List head;
    Initialize(head);
    input(head);
}
