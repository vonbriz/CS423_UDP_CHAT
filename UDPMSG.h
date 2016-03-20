/*
 * UDPMSG.h
 *
 *  Created on: Mar 19, 2016
 *      Author: erik
 */
#pragma once
#include "IMENCR.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#ifndef UDPMSG_H_
#define UDPMSG_H_
using namespace std;
class UDPMSG
{
public:

	UDPMSG()
	{
		serverport = 0;
		serverip = "192.168.0.1";
		destinationCallSign = "PLACEHOLDER";
		callsign = "PLACEHOLDER";
		msgType = "1";
		msgNumCh = "20000";
		msgNum = 20000;
	}

	void setPortAndIP4byIO() //get server and port from user
	{
		cout << "please enter ipv4 address in XXX.XXX.XXX.XXX notation \n";
		getline(cin, serverip);
		cout << "enter 5 digit port number and press enter \n";
		cin >> serverport;
		cin.ignore(1, '`');
		cin.clear();

	}

	int getServerPortNum() // return server port as int
	{
		return serverport;
	}

	string getServerIpAdr() // return server ip address as a string
	{
		return serverip;
	}


	void setCallsignIO() // gets user name
	{
		cout << "Please Enter your username \n";
		getline(cin, callsign);
	}

	string getFullMsgForSvr() // formats and encrypts message to be sent to user
	{
		msgNum++;
		string result;
		result = to_string(msgNum) + ";2;" + encryptMsg(callsign + "\n" + destinationCallSign + "\n" + msgBody);
		return result;
	}
/*
	string getKeepAliveForSvr() // formats and encrypts message to be sent to user
		{
			msgNum++;
			string result = "ack;"+to_string(msgNum)+";"+encryptMsg("Keep-Alive");
			return result;
		}
*/
	string getSignOnForSvr() // formats and encrypts sign off message
	{
		msgNum++;
		string signOnResult;
		signOnResult = to_string(msgNum) + ";1;" + encryptMsg(callsign);
		return signOnResult;
	}

	string getSignOffForSvr() // formats and encrypts
	{
		msgNum++;
		string signOffResult;
		signOffResult = to_string(msgNum) + ";3;" + encryptMsg(callsign);
		return signOffResult;
	}

	void setMsgNumInt(int passone)// sets message number from int
	{
		msgNum = passone;
	}


	void setMsgFromIO() // gets destination username and
	{
		cout << "Enter destination user's callsign \n";
		getline(cin, destinationCallSign);
		cout << "enter the message to be sent to that user \n";
		getline(cin, msgBody);
	}


private:
	string msgType, msgNumCh, msgBody, encMsgBody, callsign;
	string destinationCallSign, serverip;
	int msgNum , serverport;


	string encryptMsg(string TBE) // encrypt a string with el cipher
	{
		string result;
		string::iterator it;
		for (it = TBE.begin(); it != TBE.end(); it++)
		{
			result.push_back(encrypt(*it));
		}
		return result;
	}

	string decryptMsg(string TBE) // decrypt a string with el cipher
	{
		string result;
		string::iterator it;
		for (it = TBE.begin(); it != TBE.end(); it++)
		{
			result.push_back(decrypt(*it));
		}
		return result;
	}

};

#endif /* UDPMSG_H_ */
