#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <iomanip>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#define PORT 8080

using namespace std;

class USER
{
	private:
		char name[30];
		char email[50];
		double phone;
	public:
		friend class USER_TAG;
};

class USER_TAG : protected USER
{
	private:
		char username[20];
		char password[16];

	public:
	  	void main_menu(int argc, char *argv[]);
	  	void add();
	  	void chat(int argc, char *argv[]);
	  	void display();
	  	int check(char*);
	  	void modify();
	  	void delete_user();              
	  	void modify_name(char*);      
	  	void modify_email(char*);     
	  	void modify_phone(char*);     
	  	void modify_username(char*);
	  	void modify_password(char*);
}u_t;

void USER_TAG::main_menu(int argc, char *argv[])
{
	int choice;
	while(choice!=6)
	{
		system("clear");
		cout<<"\n\t\t\t\t\t *WELCOME TO THE CHATROOM*";
		cout<<"\n\t\t\t\t\t *************************";
		cout<<"\n\t\t\t\t\t ******* MAIN MENU *******";
		cout<<"\n\t\t\t\t\t *************************";
		cout<<"\n\n\n\t\t\t\t\t1. Create New User";
		cout<<"\n\t\t\t\t\t2. Login and Chat";
		cout<<"\n\t\t\t\t\t3. User List";
		cout<<"\n\t\t\t\t\t4. Modify User Details";
		cout<<"\n\t\t\t\t\t5. Delete User Account";
		cout<<"\n\t\t\t\t\t6. Exit";
		cout<<"\n\n\t\t\t\t\tEnter Your Choice: ";
		cin>>choice;
		switch(choice)
		{
			case 1:	add();
				break;
			case 2: chat(argc,argv);
				break;
			case 3: display();
				break;
			case 4:	modify();
				break;
			case 5: delete_user();
				break;
			case 6: return;
			default:
				{
					cout<<"\n\n\t\t\tWrong choice.";
					cout<<"\n\t\t\tPress any key to continue.";
					getchar();
				}
		}
	}
}

void USER_TAG::add()
{
	system("clear");
	int flag;
	char user_name[20];
	char ch;
	ofstream fout("Record.dat",ios::app|ios::binary);
	cout<<"\n\n ENTER USER DETAILS";
	cout<<"\n ------------------";
	cout<<"\n\n Username: ";
	cin>>user_name;
	flag=check(user_name);
	if(flag==1)
		cout<<"\n Sorry, this Username has been taken.\n";
	else
	{
		cout<<" Name: ";
		cin>>name;
		cout<<" Email ID: ";
		cin>>email;
		cout<<" Phone Number: ";
		cin>>phone;
		strcpy(username,user_name);
		cout<<" Password (8-16 characters): ";
		cin>>password;
		fout.write((char*)this, sizeof(USER_TAG));
		cout<<"\n User Account Created.";
	}
	cout<<"\n Press any key to continue.";
	getchar();
	getchar();
	fout.close();
}


void USER_TAG::chat(int argc, char *argv[])
{
	system("clear");
	ifstream fin("Record.dat",ios::in|ios::binary);
	int flag=0;
	char user_name[20], pass_word[16];
	cout<<"\n Enter Username: ";
	cin>>user_name;
	cout<<" Enter Password: ";
	cin>>pass_word;
	while(fin.read((char*)this,sizeof(USER_TAG)))
	{
		if((strcmp(user_name,username)==0) && (strcmp(pass_word,password)==0))
		{
			//grab the IP address and port number 
			char *serverIp = argv[1]; int port = atoi(argv[2]); 
    			//create a message buffer 
			char msg[100]; 
			//setup a socket and connection tools 
			struct hostent* host = gethostbyname(serverIp); 
			sockaddr_in sendSockAddr;   
			bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
			sendSockAddr.sin_family = AF_INET; 
			sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
			sendSockAddr.sin_port = htons(port);
			int clientSd = socket(AF_INET, SOCK_STREAM, 0);
			//try to connect...
			int status = connect(clientSd,(sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
			if(status < 0)
			{
				cout<<" ##### Error connecting to socket."<<endl; 
				exit(0);
			}
			cout <<" ##### Connected to the server." << endl;
			int bytesRead, bytesWritten=0,n = 0;
			char buff[80];
			struct timeval start1, end1;
			gettimeofday(&start1, NULL);
			int c=1;
			while(1)
			{
				fflush(stdin); 
				c++;
				cout <<"\n\t\t Client: ";
				//string data;
				//getline(cin, data);
				n = 0; 
				fflush(stdin);
				while ((buff[n++] = getchar()) != '\n');
				buff[n]='\0';
				memset(&msg, 0, sizeof(msg));//clear the buffer
				strcpy(msg, buff);
				if(strcmp(buff,"exit")==0)
				{
					send(clientSd, (char*)&msg, strlen(msg), 0);
					break;
				}
				bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
				//cout <<"\n\n Awaiting Server response." << endl;
				memset(&msg, 0, sizeof(msg));//clear the buffer
				bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
				if(!strcmp(msg, "exit"))
				{
					cout <<"##### Server has quit the session" << endl;
					break;
				}
				cout <<"\n Server: " << msg << endl;
			}
			gettimeofday(&end1, NULL);
			close(clientSd);
			cout << "********Session Ended********" << endl;
			//cout << "Bytes Written: " << bytesWritten <<" Bytes Read: " << bytesRead-c << endl;
			cout << "Elapsed Time: " << (end1.tv_sec- start1.tv_sec)<<" seconds" << endl;
			cout << "Connection Closed" << endl;
			break;
		}
	}
	if(flag==0)
		cout<<"\n Invalid Username or Password.";
	cout<<"\n\n Press any key to continue.";
	getchar();
	getchar();
	fin.close();
}


void USER_TAG::display()
{
	system("clear");
	int i=1;
	const char separator=' ';
	const int NoWidth=10;
	const int NameWidth=11;
	const int EmailWidth=39;
	const int UsernameWidth=16;
	const int ContactNoWidth=13;
	ifstream fin("Record.dat",ios::in|ios::binary);
	cout<<"\n\t\t\t\t\t    LIST OF USER ACCOUNTS";
	cout<<"\n\t\t\t\t\t    ---------------------";
	cout<<"\n\n\t +-----------+------------+----------------------------------------+-----------------+--------------+";
	cout<<"\n\t | Serial No.|    Name    |                Email ID                |     Username    |  Contact No. |";
	cout<<"\n\t +-----------+------------+----------------------------------------+-----------------+--------------+";
	while(fin.read((char*)this,sizeof(USER_TAG)))
	{
		cout<<"\n\t |"<<setw(NoWidth)<<setfill(separator)<<i++<<" |";
		cout<<setw(NameWidth)<<setfill(separator)<<name<<" |";
		cout<<setw(EmailWidth)<<setfill(separator)<<email<<" |";
		cout<<setw(UsernameWidth)<<setfill(separator)<<username<<" |";
		cout.precision(0);
		cout<<setw(ContactNoWidth)<<setfill(separator)<<fixed<<phone<<" |";
	}
	cout<<"\n\t +-----------+------------+----------------------------------------+-----------------+--------------+";
	cout<<"\n\n\n\t\t\t\t\tPress any key to continue.";
	getchar();
	getchar();
	fin.close();
}

int USER_TAG::check(char user_name[20])
{
	int flag=0;
	ifstream fin("Record.dat",ios::in|ios::binary);
	while(fin.read((char*)this,sizeof(USER_TAG)))
	{
		if(strcmp(user_name,username)==0)
		{
			flag=1;
			break;
		}
	}
	fin.close();
	return(flag);
}


void USER_TAG::modify()
{
	system("clear");
	int ch;
	char user_name[20];
	cout<<"\n MODIFY MENU";
	cout<<"\n -----------";
	cout<<"\n\n\n 1. Modify Name";
	cout<<"\n 2. Modify Email ID";
	cout<<"\n 3. Modify Username";
	cout<<"\n 4. Modify Password";
	cout<<"\n 5. Modify Phone Number";
	cout<<"\n Enter Your Choice: ";
	cin>>ch;
	system("clear");
	cout<<"\n Enter Username: ";
	cin>>user_name;
	switch(ch)
	{
		case 1: modify_name(user_name);
			break;
		case 2: modify_email(user_name);
			break;
		case 3: modify_username(user_name);
			break;
		case 4: modify_password(user_name);
			break;
		case 5: modify_phone(user_name);	
			break;
		default: cout<<"\n Wrong Choice";
				 getchar();
				 getchar();
	}
}

void USER_TAG::modify_name(char user_name[20])
{
	long pos,flag=0;
	char pass_word[16];
	fstream file("Record.dat",ios::in|ios::out|ios::binary);
	cout<<" Enter Password: ";
	cin>>pass_word;
	while(!file.eof())
	{
		pos=file.tellg();
		file.read((char*)this,sizeof(USER_TAG));
		if((strcmp(user_name,username)==0) && (strcmp(pass_word,password)==0))
		{
			cout<<"\n Enter New Name: ";
			cin>>name;
			file.seekg(pos);
			file.write((char*)this,sizeof(USER_TAG));
			cout<<"\n User Account Name has been modified.";
			flag=1;
			break;
		}
	}
	if(flag==0)
		cout<<"\n Invalid Username or Password.";
	getchar();
	getchar();
	file.close();
}

void USER_TAG::modify_email(char user_name[20])
{
	long pos,flag=0;
	char pass_word[16];
	fstream file("Record.dat",ios::in|ios::out|ios::binary);
	cout<<" Enter Password: ";
	cin>>pass_word;
	while(!file.eof())
	{
		pos=file.tellg();
		file.read((char*)this,sizeof(USER_TAG));
		if((strcmp(user_name,username)==0) && (strcmp(pass_word,password)==0))
		{
			cout<<"\n Enter New Email ID: ";
			cin>>email;
			file.seekg(pos);
			file.write((char*)this,sizeof(USER_TAG));
			cout<<"\n User Email ID has been modified.";
			flag=1;
			break;
		}
	}
	if(flag==0)
		cout<<"\n Ivalid Username or Password.";
	getchar();
	getchar();
	file.close();
}

void USER_TAG::modify_phone(char user_name[20])
{
	long pos,flag=0;
	char pass_word[16];
	fstream file("Record.dat",ios::in|ios::out|ios::binary);
	cout<<" Enter Password: ";
	cin>>pass_word;
	while(!file.eof())
	{
		pos=file.tellg();
		file.read((char*)this,sizeof(USER_TAG));
		if((strcmp(user_name,username)==0) && (strcmp(pass_word,password)==0))
		{
			cout<<"\n Enter New Phone Number: ";
			cin>>phone;
			file.seekg(pos);
			file.write((char*)this,sizeof(USER_TAG));
			cout<<"\n User Phone Number has been modified.";
			flag=1;
			break;
		}
	}
	if(flag==0)
		cout<<"\n Invalid Username or Password.";
	getchar();
	getchar();
	file.close();
}

void USER_TAG::modify_username(char user_name[20])
{
	long pos,flag=0;
	char pass_word[16];
	fstream file("Record.dat",ios::in|ios::out|ios::binary);
	cout<<" Enter Password: ";
	cin>>pass_word;
	while(!file.eof())
	{
		pos=file.tellg();
		file.read((char*)this,sizeof(USER_TAG));
		if((strcmp(user_name,username)==0) && (strcmp(pass_word,password)==0))
		{
			cout<<"\n Enter New Username: ";
			cin>>username;
			file.seekg(pos);
			file.write((char*)this,sizeof(USER_TAG));
			cout<<"\n Account Username has been modified.";
			flag=1;
			break;
		}
	}
	if(flag==0)
		cout<<"\n Invalid Username or Password.";
	getchar();
	getchar();
	file.close();
}

void USER_TAG::modify_password(char user_name[20])
{
	long pos,flag=0;
	char pass_word[16];
	fstream file("Record.dat",ios::in|ios::out|ios::binary);
	cout<<" Enter Password: ";
	cin>>pass_word;
	while(!file.eof())
	{
		pos=file.tellg();
		file.read((char*)this,sizeof(USER_TAG));
		if((strcmp(user_name,username)==0) && (strcmp(pass_word,password)==0))
		{
			cout<<"\n Enter New Password: ";
			cin>>password;
			file.seekg(pos);
			file.write((char*)this,sizeof(USER_TAG));
			cout<<"\n Account Password has been modified.";
			flag=1;
			break;
		}
	}
	if(flag==0)
		cout<<"\n Invalid Username or Password.";
	getchar();
	getchar();
	file.close();
}

void USER_TAG::delete_user()
{
	int flag=0;
	char ch, user_name[20], pass_word[16];
	cout<<"\n Enter Username: ";
	cin>>user_name;
	cout<<" Enter Password: ";
	cin>>pass_word;
	ifstream fin("Record.dat",ios::in|ios::binary);
	ofstream fout("temp.dat",ios::out|ios::binary);
	while(fin.read((char*)this,sizeof(USER_TAG)))
	{
		if((strcmp(user_name,username)==0) && (strcmp(pass_word,password)==0))
		{
			cout<<"\n Name: "<<name;
			cout<<"\n Email ID: "<<email;
			cout<<"\n Phone Number: "<<phone;
			cout<<"\n Username: "<<username;
			cout<<"\n\n Do you want to delete this user account?(y/n): ";
			cin>>ch;
			if(ch=='n')
			   fout.write((char*)this,sizeof(USER_TAG));
			else
			   cout<<"\n User Account Deleted.";
			flag=1;
		}
		else
			fout.write((char*)this,sizeof(USER_TAG));
	}
	fin.close();
	fout.close();
	if(flag==0)
		cout<<"\n Invalid Username or Password.";
	else
	{
		remove("Record.dat");
		rename("temp.dat","Record.dat");
	}
	getchar();
	getchar();
}

int main(int argc, char *argv[])
{
	system("clear");
	u_t.main_menu(argc,argv);
	return 0;
}

