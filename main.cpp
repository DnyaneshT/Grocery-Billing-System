
#include<iostream>
#include<conio.h>
#include<sstream>
#include<stdio.h>
#include<process.h>
#include<fstream>
#include <bits/stdc++.h>
using namespace std;


class product
{
    int productNumber;
    string name;
    float price,quantity,tax,discount,stock;
    public:
        void create_product()                       //Add a new product in the shop
        {
            cout<<"Enter The Name of The Product : ";
            cin>>name;
            cout<<"Enter The Product No. :";
            cin>>productNumber;
            cout<<"Enter The Price of The Product : ";
            cin>>price;
            cout<<"Enter The Quantity in Stocks : ";
            cin>>stock;
            cout<<"Enter The Discount (%) :  ";
            cin>>discount;
        }
        void create_specific_product(string productName, int productNumber, int productPrice, int productQuantity, float productDiscount)    //Create a product with given data
        {
            name = productName;
            productNumber = productNumber;
            price = productPrice;
            stock = productQuantity;
            discount = productDiscount;
        }


        void show_product()                         //Display all the products
        {
            cout<<"_____________________________________________________\n";
            cout<<"P.NO.\tNAME\tPRICE\tQUANTITY\tDISCOUNT\n";
            cout<<"_____________________________________________________\n";

            cout<<"\n"<<productNumber<<"\t"<<name<<"\t"<<price<<"\t"<<quantity<<"\t"<<discount<<endl;
        }

        int ret_product_number()
        {
            return productNumber;
        }

        float ret_price()
        {
            return price;
        }

        string ret_name()
        {
            return name;
        }

        int ret_stock()
        {
            return stock;
        }

        int ret_discount()
        {
            return discount;
        }

};

fstream fp;             // File pointer
product pr;             // object of the class


void write_product()           //Writes a product to the file
{
    fp.open("Shop.txt",ios::out|ios::app);
    pr.create_product();
    fp.write((char*)&pr,sizeof(product));
    fp.close();
    cout<<"\nPRODUCT CREATED! "<<endl;
    getch();
}

void display_specific(int productNumber)        //Displays specific product with given productNumber
{
    int flag=0;
    fp.open("Shop.txt",ios::in);
    while(fp.read((char*)&pr,sizeof(product)))
    {
        if(pr.ret_product_number()==productNumber)
        {
            pr.show_product();
            flag=1;
        }
    }
    fp.close();
    if(flag==0)
        cout<<"\nWRONG PRODUCT CODE!";
    getch();
}

int ret_specific_quantity(int productNumber)        //Searches and returns the available quantity in the shop of a specific product
{
    int flag=0;
    fp.open("Shop.txt",ios::in);
    while(fp.read((char*)&pr,sizeof(product)))
    {
        if(pr.ret_product_number()==productNumber)
        {
            flag=1;
            fp.close();
            return pr.ret_stock();  //returning the availbale quantitu
        }
    }
    fp.close();
    if(flag==0)
    {
        cout<<"\nWRONG PRODUCT CODE!";
        return INT_MAX;     //max int value to dissatisfy (desired > available)
    }
    return INT_MAX;   //max int value to dissatisfy (desired > available)
    getch();
}

void modify_product()
{
    int productNumber,found=0;
    cout<<"\nTo Modify ";
    cout<<"\nEnter The Product Number : ";
    cin>>productNumber;
    fp.open("Shop.txt",ios::in|ios::out);
    while(fp.read((char*)&pr,sizeof(product)) && found==0)
    {
        if(pr.ret_product_number()==productNumber)
        {
            pr.show_product();
            cout<<"\nEnter The New Details of Product : "<<endl;
            pr.create_product();
            int pos=-1*sizeof(pr);          //calulating the negative size of object pr
            fp.seekp(pos,ios::cur);         //go back to the start of the product entry
            fp.write((char*)&pr,sizeof(product));
            cout<<"\nRECORD UPDATED!"<<endl;           ;
            found=1;
        }
    }
    fp.close();
    if(found==0)
        cout<<"\nWRONG PRODUCT CODE!";
    getch();
}


void delete_product()           //Copy all the contents of the file to a new file except the entry to be deleted. Now rename the new file. Remove the older file.
{
    int no;
    cout<<"\nDELETE PRODUCT";
    cout<<"\nEnter The Product Number You Want To Delete : ";
    cin>>no;
    fp.open("Shop.txt",ios::in|ios::out);
    fstream fp2;
    fp2.open("Temp.txt",ios::out);
    fp.seekg(0,ios::beg);
    while(fp.read((char*)&pr,sizeof(product)))
    {
        if(pr.ret_product_number()!=no)
        {
            fp2.write((char*)&pr,sizeof(product));
        }
    }
    fp2.close();
    fp.close();
    remove("Shop.txt");
    rename("Temp.txt","Shop.txt");
    cout<<"\nPRODUCT DELETED!";
    getch();
}

bool menu()
{

    fp.open("Shop.txt",ios::in);
    if(!fp)
    {
        cout<<"ERROR!!! FILE COULD NOT BE OPENED!";
        cout<<"\n\n\n Go To Admin Menu to create File First.";
        getch();
        return false;
    }

    cout<<"\n\n\t\tProduct MENU\n";
    cout<<"_____________________________________________________\n";
    cout<<"P.NO.\t\tNAME\t\tPRICE\t\tQUANTITY\n";
    cout<<"_____________________________________________________\n";

    while(fp.read((char*)&pr,sizeof(product)))
    {
        cout<<pr.ret_product_number()<<"\t\t"<<pr.ret_name()<<"\t\t"<<pr.ret_price()<<"\t\t"<<pr.ret_stock()<<endl;
    }
    fp.close();
    return true;
}


void place_order()
{
    int order[50],purchasedQuantity[50],count=0;
    float amount,discountedAmount,total=0;
    char ch='Y';
    if(!menu())
        return;
    cout<<"\n_____________________________________________________";
    cout<<"\n PLACE YOUR ORDER";
    cout<<"\n_____________________________________________________\n";
    do{
        cout<<"\nEnter The Product Number : ";
        cin>>order[count];
        cout<<"\nQuantity in number : ";
        cin>>purchasedQuantity[count];

        int availableQuantity = ret_specific_quantity(order[count]);
        if(purchasedQuantity[count] > availableQuantity)
        {
            cout<<"\nNot enough quantity in stock!";
            return;
        }

        count++;
        cout<<"\nDo You Want To Buy Another Product ? (y/n)";
        cin>>ch;
    }while(ch=='y' ||ch=='Y');
    getch();
    cout<<"\n\n***************INVOICE*********************\n";
    cout<<"\nProduct Number.\t| Prpduct Name\t| Quantity\t| Price\t| Amount\t| Amount After Discount\n";
    for(int x=0;x<=count;x++)
    {
        fp.open("Shop.txt",ios::in|ios::out);
        fp.read((char*)&pr,sizeof(product));
        while(!fp.eof())
        {
            if(pr.ret_product_number()==order[x])
            {
                amount=pr.ret_price()*purchasedQuantity[x];
                discountedAmount=amount-(amount*pr.ret_discount()/100);
                cout<<"\n"<<order[x]<<"\t"<<pr.ret_name()<<"\t"<<purchasedQuantity[x]<<"\t\t"<<pr.ret_price()<<"\t"<<amount<<"\t\t"<<discountedAmount;
                total+=discountedAmount;

                string productName = pr.ret_name();
                int productNumber = pr.ret_product_number();
                int productPrice = pr.ret_price();
                int productQuantity = pr.ret_stock() - purchasedQuantity[x];
                float productDiscount = pr.ret_discount();

                pr.create_specific_product(productName,productNumber,productPrice,productQuantity,productDiscount);
                int pos=-1*sizeof(pr);
                fp.seekp(pos,ios::cur);
                fp.write((char*)&pr,sizeof(product));

            }
            fp.read((char*)&pr,sizeof(product));
        }

        fp.close();
    }
    cout<<"\n\nThank You For Shopping!";
    cout<<"\n\nTOTAL = "<<total;
    getch();
}


void admin_menu()
{
    char ch2;
    cout<<"\n\n************WELCOME TO THE ADMIN MENU**********";

    cout<<"\n(1) ADD A PRODUCT IN THE SHOP";
    cout<<"\n(2) DISPLAY ALL PRODUCTS";
    cout<<"\n(3) SEARCH FOR A PRODUCT";
    cout<<"\n(4) MODIFY A PRODUCT";
    cout<<"\n(5) DELETE A PRODUCT";
    cout<<"\n(6) BACK TO MAIN MENU";
    cout<<"\nEnter Your Choice (1-7) "<<endl;
    ch2=getch();
    switch(ch2)
    {
        case '1':
            write_product();
        break;

        case '2':
            menu();
        break;

        case '3':
            int num;
            cout<<"\nEnter The Product Number: ";
            cin>>num;
            display_specific(num);
        break;

        case '4':
            modify_product();
        break;

        case '5':
            delete_product();
        break;

        case '6':
            break;

        default:cout<<"\a";admin_menu();
    }
}



int main()
{
    char ch;
    do
    {
        cout<<"\n*******MAIN MENU***********";
        cout<<"\n(1) CUSTOMER";
        cout<<"\n(2) ADMINISTRATOR";
        cout<<"\n(3) EXIT";
        cout<<"\nPlease Select Your Option (1-3) ";
        ch = getche();
        switch(ch)
        {
            case '1':
                place_order();
                getch();
            break;

            case '2':
                admin_menu();
            break;

            case '3':
            default :cout<<"\a";
        }
    }while(ch!='3');
}

