#include<unistd.h>
#include<iostream>
#include<conio.h>
#include<fstream>
#include<iomanip>
#include<windows.h>


void add_record();
void search_record();
void delete_record();
void view_record();
void update_record();

struct phone{
char name[15],phone[15];
}rec;
using namespace std;

void add_record(){
    char op;
    ofstream addrec;
    addrec.open("phone.dat",ios::binary|ios::app);
    //cout<<"testing"<<endl;
    if(!addrec){
        cout<<"file opening error"<<endl;
    }
    do{
        cout<<"enter name:"<<endl;
        cin>>rec.name;
        cout<<"enter number"<<endl;
        cin>>rec.phone;
        addrec.write((char*)&rec,sizeof(rec));
        cout<<"More record[y/n]:"<<endl;
        op=getch();
    }
    while(op=='y'||op=='Y');
    addrec.close();
}
    void view_record(){
    ifstream show;
    show.open("phone.dat",ios::binary);
    if(!show){
        cout<<"file opening error:"<<endl;
    }
    cout<<"\n------------------------------\n";
    cout<<"|Name                  |Number              |\n";
    cout<<"\n------------------------------\n";
    while(show){
        show.read((char*)(&rec),sizeof(rec));
        if(!show.eof()){
            cout<<endl;
            cout<<rec.name;
            cout<<setw(25)<<rec.phone<<endl;
        }
    }
    show.close();
    getch();
    }

    void update_record()
{
    char num[15];
    int rn=0,t=0;
    fstream rrr("phone.dat",ios::binary|ios::in|ios::out);
    if(!rrr){
        cout<<"file opening error";
    }
    cout<<"enter number to update:"<<endl;
    cin>>num;
    while(rrr.read((char*)&rec,sizeof(rec))){
        if(strcmp(num,rec.phone)==0){
            cout<<"\nName and Number is:"<<endl;
            cout<<"\n"<<rec.name<<"\t"<<rec.phone<<endl;
            rn=(rn)*sizeof(rec);
            rrr.seekp(rn);
            cout<<"\nEnter new name and number"<<endl;
            cin>>rec.name>>rec.phone;
            rrr.write((char*)&rec,sizeof(rec));
            rrr.close();
            t=1;
        }
        rn++;
    }
    if(t==0)
        cout<<"Record not found:";
}

void search_record(){
char num[15];
int found=0;
ifstream search;
search.open("phone.dat",ios::binary|ios::in|ios::out);
cout<<"\nEnter number to search:\n";
cin>>num;
cout<<"Please wait,we are searching.....";
search.seekg(0,ios::beg);
while(search.read((char*)&rec,sizeof(rec))){
    if(strcmp(num,rec.phone)==0){
    sleep(5);
    found=1;
    cout<<"\nName:"<<rec.name<<"\t"<<"Number:"<<rec.phone;
    break;
    }
}
if(found==0)
    cout<<"\nrecord not found:";
getch();
}

void delete_record(){
char num[15];
int found=0;
ifstream delet("phone.dat",ios::binary);
ofstream ooo("temp.dat",ios::binary);
cout<<"Enter number to delete:";
cin>>num;
delet.seekg(0,ios::beg);
while(delet.read((char*)&rec,sizeof(rec))){
        found=0;
if(strcmp(num,rec.phone)==0){
    found=1;
    cout<<"Name:"<<rec.name<<"\t"<<"Number:"<<rec.phone<<endl;
    sleep(5);
    cout<<"\nNumber Deleted:";
}
if(strcmp(num,rec.phone)!=0)
    ooo.write((char*)&rec,sizeof(rec));
if(found==0)
    cout<<"Record not found:";
}
ooo.close();
delet.close();
remove("phone.dat");
rename("temp.dat","phone.dat");
getch();
}





int main()
{
    char ch;
    while(1){
        system("cls");
        cout<<"\t\t*********************\n";
        cout<<"\t\tChoose your option"<<endl;
        cout<<"\t\t*********************\n";
        cout<<endl;
        cout<<"\t\t1:Add a Record\n";
        cout<<"\t\t2:Update a Record\n";
        cout<<"\t\t3:Delete a Record\n";
        cout<<"\t\t4:View a Record\n";
        cout<<"\t\t5:Search a Record\n";
        cout<<"\t\t6:Exit"<<endl;

        ch=getch();
        switch(ch){
            case '1':add_record();break;
            case '2':update_record();break;
            case '3':delete_record();break;
            case '4':view_record();break;
            case '5':search_record();break;
            case '6':exit(0);break;
            //default:cout<<"enter a valid number,from 1 and 6"<<endl;break;
            }
    }
    return 0;
}

