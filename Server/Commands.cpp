#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include "Commands.h"

Commands::Commands()
{
	//
}

void Commands::ParseCommand(Database& Db, std::string Command)
{
	// Split the command into its components.
	std::istringstream Stream(Command);

	std::string Function;
	Stream >> Function;

	if(Function == "login")
	{
		std::string Username;
		std::string Password;
		Stream >> Username;
		Stream >> Password;
		Login(Db, Username, Password);
	}
	else if(Function == "logout")
	{
		Logout(Db);
	}
	else if(Function == "send")
	{
		std::string Recipient;
		std::string Word;
		std::string Contents = "";
		Stream >> Recipient;
		Stream >> Contents;
		while(Stream >> Word)
		{
			Contents = Contents + " " + Word;
		}
		SendEmail(Db, CurrentUser, Recipient, Contents);
	}
	else if(Function == "check")
	{
		Check(Db);
	}
	else if(Function == "stat")
	{
		Stats(Db);
	}
	else if(Function == "delete")
	{
		std::string Temp;
		Stream >> Temp;
		Delete(Db, std::stoi(Temp));
	}
	else if(Function == "clean")
	{
		Clean(Db);
	}
	else if(Function == "receive")
	{
		std::string Temp;
		Stream >> Temp;
		Receive(Db, std::stoi(Temp));
	}
	else
	{
		std::string Message = "Unknown command.";
		send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);
	}
}

void Commands::Login(Database& Db, std::string Username, std::string Password)
{
	std::cout << "Login attempt from " << Username << "..." << std::endl;

	// Check login credentials.
	for(auto it = Db.Users.begin(); it != Db.Users.end(); ++it)
	{
		if(it->Username == Username && it->Password == Password)
		{
			std::string Message = "Login successful.";

			std::cout << Message << std::endl;
			CurrentUser = Username;
			CurrentUserId = it - Db.Users.begin();

			send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);

			return;
		}
	}

	std::string Message = "Login failed.";

	std::cout << Message << std::endl;

	send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);
}

void Commands::Logout(Database& Db)
{
	std::cout << "User " << CurrentUser << " is logging out..." << std::endl;

	std::string Message = "Logged out successfuly.";
	send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);

	std::cout << "User " << CurrentUser << " logged out." << std::endl;

	CurrentUser = "";
}

void Commands::SendEmail(Database& Db, std::string Who, std::string Where, std::string What)
{
	// Track stats.
	Db.Users[CurrentUserId].Stats.SentEmails++;
	Db.UpdateStats();

	// Write into variable and database.
	Email TemporaryEmail;
	TemporaryEmail.Sender = Who;
	TemporaryEmail.Recipient = Where;
	TemporaryEmail.Contents = What;
	Db.AddEmail(TemporaryEmail);

	std::string Message = "Email sent.";
	send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);
}

void Commands::Check(Database& Db)
{
	// Check if any emails are for the user and send them.
	Db.Users[CurrentUserId].Stats.InboxEmails = 0;
	bool Check = true;
	int i = 0;
	std::string ToSend = "";
	for(auto it = Db.Emails.begin(); it != Db.Emails.end(); ++it)
	{
		if(it->Recipient == CurrentUser)
		{
			std::string Message = std::to_string(i) + ". from " + it->Sender;
			ToSend = ToSend + Message + "\n";

			std::cout << Message  << std::endl;

			Check = false;

			// Track stats.
			Db.Users[CurrentUserId].Stats.InboxEmails++;
		}

		i++;
	}
	send(ServerId, ToSend.c_str(), strlen(ToSend.c_str()), 0);
	Db.UpdateStats();

	if(Check)
	{
		ToSend = "There are no emails in your inbox.";
		std::cout << ToSend << std::endl;
		send(ServerId, ToSend.c_str(), strlen(ToSend.c_str()), 0);
	}
}

void Commands::Stats(Database& Db)
{
	for(auto it = Db.Users.begin(); it != Db.Users.end(); ++it)
	{
		if(it->Username == CurrentUser)
		{
			std::cout << "Sent Emails: " <<  it->Stats.SentEmails << std::endl;
			std::cout << "Emails in inbox: " << it->Stats.SentEmails << std::endl;
			std::cout << "Deleted emails:" << it->Stats.SentEmails << std::endl;

			std::string Message = "Sent Emails: " + std::to_string(it->Stats.SentEmails) + "\n";
			Message = Message + "Emails in inbox: " + std::to_string(it->Stats.InboxEmails) + "\n";
			Message = Message + "Deleted emails: " + std::to_string(it->Stats.DeletedEmails);
			send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);

			return;
		}
	}
}

void Commands::Delete(Database& Db, int EmailId)
{
	// Track stats.
	Db.Users[CurrentUserId].Stats.DeletedEmails++;
	Db.Users[CurrentUserId].Stats.InboxEmails--;
	Db.UpdateStats();

	// Delete from variable and database.
	Db.DeleteEmail(EmailId);

	std::string Message = "Email deleted.";
	send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);
}

void Commands::Clean(Database& Db)
{
	for(int i = 0; i < Db.Emails.size(); ++i)
	{
		if(Db.Emails[i].Recipient == CurrentUser)
		{
			Commands::Delete(Db, i);
		}
	}

	std::string Message = "Emails cleared.";
	send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);
}

void Commands::Receive(Database& Db, int EmailId)
{
	std::cout << "From: " << Db.Emails[EmailId].Sender << std::endl;
	std::cout << Db.Emails[EmailId].Contents << std::endl;

	std::string Message = "From: " + Db.Emails[EmailId].Sender + "\n" + Db.Emails[EmailId].Contents;
	send(ServerId, Message.c_str(), strlen(Message.c_str()), 0);
}

Commands::~Commands()
{
	//
}
