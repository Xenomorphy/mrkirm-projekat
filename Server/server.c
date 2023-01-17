#include <stdio.h>
#include <stdlib.h>

#define USERS_DATABASE_PATH "../Database/users.db"
#define EMAILS_DATABASE_PATH "../Database/emails.db"

#define BUFF_SIZE 256

typedef enum {PROCESSING_ERR = 0, PROCESSING_OK} processing_enum;
typedef enum {ERR_CMD = 0, LOGIN_CMD, LOGOUT_CMD, SEND_CMD, STAT_CMD, CHECK_CMD, CLEAN_CMD, DELETE_CMD, RECEIVE_CMD} commands_enum;

int
ParseCommand(const char* message)
{
	switch(message[0])
	{
		case 'l':
			if(message[3] == 'i') // Login
			{
				return LOGIN_CMD;
			}
			else // Logout
			{
				return LOGOUT_CMD;
			}
		break;
		case 's':
			if(message[1] == 'e') // Send
			{
				return SEND_CMD;
			}
			else // Stat
			{
				return STAT_CMD;
			}
		break;
		case 'c':
			if(message[1] == 'h') // Check
			{
				return CHECK_CMD;
			}
			else // Clean
			{
				return CLEAN_CMD;
			}
		break;
		case 'd':	// Delete
			return DELETE_CMD;
		break;
		case 'r':	// Receive
			return RECEIVE_CMD;
		break;
		default:
			return ERR_CMD;
		break;
	}
}

int
ProcessMessage(const char* message)
{
	// Quick check for command type.
	int command_id = ParseCommand(message);
	if(command_id == ERR_CMD)
	{
		return PROCESSING_ERR;
	}

	// Call the appropriate function for each command type.
	char arguments[BUFF_SIZE];
	switch(command_id)
	{
		case LOGIN_CMD:
		break;
		//TODO HERE
	}

	return PROCESSING_OK;
}

int
main(void)
{
	char* message1 = "login mane 123";
	char* message2 = "send ala hello";
	char* message3 = "logout";

	char* message4 = "login ala 123";
	char* message5 = "check";
	char* message6 = "receive 1";
	char* message7 = "logout";

	int ret_check;
	char message[BUFF_SIZE];

	// Connect to the database.

	FILE* users_fp;
	FILE* emails_fp;

	users_fp = fopen(USERS_DATABASE_PATH, "rw+");
	emails_fp = fopen(EMAILS_DATABASE_PATH, "rw+");

	// Wait for message.

	// Parse and process message.

	ret_check = ProcessMessage(message);
	if(ret_check == PROCESSING_ERR)
	{
		fclose(users_fp);
		fclose(emails_fp);
		exit(EXIT_FAILURE);
	}

	// Close database connection.

	fclose(users_fp);
	fclose(emails_fp);
}
