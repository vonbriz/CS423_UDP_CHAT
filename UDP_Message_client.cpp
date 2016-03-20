/*
UDP IM CLIENT PROJECT
ERIK BRIZENDINE CS423
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "IMENCR.h"
#include "UDPMSG.h"

#define SERVERPORT "23456"
#define SERVERIP "204.76.188.23"
using namespace std;

int sockfd;
	    struct addrinfo server_struct, *servinfo, *p;
	    int return_value;
	    int recv_byte;
	    char buf[100];
	    char sendStr[] = "12345;1;I1@V";
	    std::string msgNumber = "12345";
	    //std::string getName;
	    int recvThreadReturn;
	    pthread_t recvThread;
	    char menuOption;



int connectSockets(); // prepares POSIX sockets for action
int logOn(); // sends a thing to a place
string decrsvrMsg(char *svrMs); // decrypts const char server message
int logOff(); // sends a logoff message
void sendMsg(); // gets a user to user message from IO and sends to server
//void beginCheckThread(void * start); // begins message checking thread
void *recvMessage(void *sockfd);
//int sendKeepAlive(); // sends a keepalive message to the server
UDPMSG control;


int main(int argc, char *argv[])
{
	logOn();
	char menu = 'z'; // makin stuff happen

	while (menu != 'q' && menu != 'Q') // quits
	{
		cout << "please choose an option, enter s to send a message, c to check messages, q to quit the program \n"; // tells dumb user how to use dumb program
		cin >> menu; // take in user's choice
		cin.ignore(1, '`'); // clear cin for getline
		cin.clear(); // clear cin for getline

		if (menu == 's' || menu == 'S') //
		{
			sendMsg();
		}
		if (menu == 'c' || menu == 'C') //
		{
			sendMsg();
		}
		menu = 'z';
	}
	logOff(); // logs off of server


	return 0;
}


int logOn()
{
	connectSockets(); // preps sockets
	control.setCallsignIO(); // gets username that user desires to use

	string message = control.getSignOnForSvr(); // gives us a copy we can use
	if ((recv_byte = sendto(sockfd, message.c_str(), message.length(), 0, servinfo->ai_addr, servinfo->ai_addrlen)) == -1) // attempts to send message
	{
	        perror("UDP_Client: sento error");
	        exit(1);
	}
	cout << "Data Sent" << endl; // confirms to user data was sent
	return 1;
}
/*
int sendKeepAlive()
{
	string message = control.getKeepAliveForSvr();
		if ((recv_byte = sendto(sockfd, message.c_str(), message.length(), 0, servinfo->ai_addr, servinfo->ai_addrlen)) == -1)
		{
		        perror("UDP_Client: sento error");
		        exit(1);
		}
		cout << "Data Sent" << endl;
		return 1;
}
*/
int logOff()
{
	control.setCallsignIO();
	//Send some data
	string message = control.getSignOffForSvr();
	if ((recv_byte = sendto(sockfd, message.c_str(), message.length(), 0, servinfo->ai_addr, servinfo->ai_addrlen)) == -1)
	{
	        perror("UDP_Client: sento error");
	        exit(1);
	}
	cout << "Data Sent" << endl;
	return 1;
}


void sendMsg()
{
	control.setMsgFromIO();
	string message = control.getFullMsgForSvr();
		if ((recv_byte = sendto(sockfd, message.c_str(), message.length(), 0, servinfo->ai_addr, servinfo->ai_addrlen)) == -1)
		{
		        perror("UDP_Client: sento error");
		        exit(1);
		}
		cout << "Data Sent" << endl;
}

/*
void beginCheckThread(void * start)
{

	cout << "starting to listen \n";
	while (2 + 2 == 4)
	{
		if ((recv_size = recvfrom(s, server_reply, 500, 0, NULL, NULL)) == SOCKET_ERROR) {
			cout << "recv failed" << endl;
		}
		cout << "reply Received" << endl;

		server_reply[recv_size] = '\0';//Add \0 at the end of received string string before printing

		string dummy;
		dummy = decrsvrMsg(server_reply);
		cout << dummy << endl;
	}
}
*/

string decrsvrMsg(char *svrMs)
{
	string result;
	int i = 0, sc = 0;
	while (svrMs[i] != '\0' && sc < 2)
	{
		result.push_back(svrMs[i]);
		if (svrMs[i] == ';')
		{
			sc++;
		}
		i++;
	}
	while (svrMs[i] != '\0')
		{
			result.push_back(decrypt(svrMs[i]));
			i++;
		}
	return result;
}

int connectSockets()
{

	    memset(&server_struct, 0, sizeof server_struct);
	    server_struct.ai_family = AF_UNSPEC;
	    server_struct.ai_socktype = SOCK_DGRAM;

	    if ((return_value = getaddrinfo(SERVERIP, SERVERPORT, &server_struct, &servinfo)) != 0){
	        fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(return_value));
	        return 1;
	    }

	    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);


	    return 1;
}



void *recvMessage(void *sockfd)
{
    int recv_byte;
    char buf[100];
    //TEST
    struct addrinfo server_struct, *servinfo;
    int(return_value);

	memset(&server_struct, 0, sizeof server_struct);
    server_struct.ai_family = AF_UNSPEC; server_struct.ai_socktype = SOCK_DGRAM;

    if ((return_value = getaddrinfo(SERVERIP, SERVERPORT, &server_struct, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(return_value));
        exit(1);
    }

    int sock = *(int*)sockfd;


    while(1)
    {
        std::cin.clear();
        std::cout.clear();
        memset(buf ,0 ,sizeof(buf));
        std::cout << "Listening for incoming messages...\n";
        //wait for messages


        if ((recv_byte = recvfrom(sock, buf, 100-1 , 0, NULL, NULL)) == -1)
        {
            perror("recvfrom");
            exit(1);
        }
        std::string message = buf;
        //notify user of recvd message

        //get message type
//        char msgtype = message.at(message.find_first_of(';')+1);
        std::string msgnum = message.substr(0,message.find_first_of(';'));

/*
        //handle "keep alive" messages
        if(msgtype == '4')
        {
            cout << "Sending ack to server...\n";
           sendKeepAlive();
        }


        else if (msgtype == '1' || msgtype == '3'){
            //output in GREEN
//            std::cout << "\033[1;32m " + decrsvrMsg(message) + "\033[0m";
        }


        else if (msgtype == '2'){
            //output in BLUE
 //           std::cout << "\033[1;34m " + decrsvrMsg(message) + "\033[0m\n";
        }



        else if (msgnum == "Error"){
            //output in RED
           std::cout << "\033[1;31m " + decrsvrMsg(message) + "\033[0m\n";

        }
*/
    }
}



