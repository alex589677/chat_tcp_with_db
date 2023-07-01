// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// author A.C.Gadiatulin group CPLUS-1006 Block-4(Databases)

#include <iostream>
#include <string>
#include <winsock2.h>
#include <mysql.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

void ClientHandlerAddData(int index);
void createTableUsers(MYSQL mysql_obj);
void createTableMessages(MYSQL mysql_obj);
void insertToTableUsers(MYSQL mysql_obj, std::string& str);
void insertToTableMsg(MYSQL mysql_obj, std::string& temp_msg, std::string& to);
void sending(SOCKET connections[], int index);

static SOCKET connections[100];	// container clients
int counter = 0;				// index connection

int handler_count = 1;			// counter fill tables users and msg

MYSQL mysql_obj;
MYSQL_RES* res;
MYSQL_ROW row;

int main(int argc, char* argv[]) {
	setlocale(0, "");
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}
	
	SOCKADDR_IN server_address;
	int sizeofaddr = sizeof(server_address);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(50000);
	server_address.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	bind(sListen, (SOCKADDR*)&server_address, sizeof(server_address));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for(int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&server_address, &sizeofaddr);

		if(newConnection == 0) {
			std::cout << "Error #2\n";
		} else {
			std::cout << "Client Connected!\n";
			// send data to client
			char str[255] = "\n1. Input you name and click enter\n"\
							"2. Input you message(only one word) example text&number_client\n"\
							"3. Input 'exit' to quit.";
			send(newConnection, str, sizeof(str), 0);

			connections[i] = newConnection;		// write connection is array connections[]
			counter++;
			
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClientHandlerAddData, (LPVOID)(i), 0, 0);
		}
	}

	system("pause");
	return 0;
}

void ClientHandlerAddData(int index) {
	// get discriptor connection
	mysql_init(&mysql_obj);
	if (&mysql_obj == nullptr) {
		std::cout << "Error: can't create MySQL-descriptor\n";
	}
	std::cout << "Init mysql succes.\n";

	// connection to server
	if (!mysql_real_connect(&mysql_obj, "localhost", "alexandr", "1234", "testdb", NULL, NULL, 0)) {
		std::cout << "Error: can't connect to database " << mysql_error(&mysql_obj) << std::endl;
	}
	std::cout << "Connect to database succes.\n";
	
	mysql_query(&mysql_obj, "USE testdb");
	std::cout << "Use database testdb\n";

	createTableUsers(mysql_obj);			// call function
	createTableMessages(mysql_obj);			// call function

	char user_name1[256];		// name client1
	char user_name2[256];		// name client2
	char buf[256];
	char text[256];

	while (handler_count < 5) {
		// receive message from client on server
		recv(connections[index], buf, sizeof(buf), 0);				

		// display message from client on server
		std::cout << "[Server][ClientHandlerAddData] " << buf << std::endl;
		
		// Fill string table users where id = 1, 2 (name client)
		if (handler_count == 1) {
			handler_count++;
			strcpy(user_name1, buf);
			std::cout << "[Server][Coping user_name1] " << user_name1 << std::endl;
			std::string m_name1 = user_name1;
			insertToTableUsers(mysql_obj, m_name1);		// call function
		}
		else if (handler_count == 2) {
			handler_count++;
			strcpy(user_name2, buf);
			std::cout << "[Server][Coping user_name2] " << user_name2 << std::endl;
			std::string m_name2 = user_name2;
			insertToTableUsers(mysql_obj, m_name2);		// call function

			// Display table users
			mysql_query(&mysql_obj, "SELECT * FROM users");
			if (res = mysql_store_result(&mysql_obj)) {
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < mysql_num_fields(res); i++) {
						std::cout << row[i] << "  ";
					}
					std::cout << std::endl;
				}
			}
			else {
				std::cout << "Error MySQL " << mysql_error(&mysql_obj);
			}
		}
		else if (handler_count == 3) {
			// receive message from clients and fill table msg
			handler_count++;

			//-----------------begin split string example 'John@text'
			std::string key;
			std::string value;
			int split_counter = 0;
			std::string s = buf;
			for (int i = 0; i < s.length(); i++) {
				if (buf[i] == '@') {					// '@'- symbol delimeter message and 'to'
					split_counter = i;
				}
			}

			key = s.substr(0, split_counter);					// message
			value = s.substr(split_counter + 1, s.length());	// to
			//------------------end split

			strcpy(text, key.c_str());
			std::string to = value;
			std::cout << "[Server][Added message] " << text << std::endl;
			std::string temp_msg = text;
			insertToTableMsg(mysql_obj, temp_msg, to);
		}
		else if (handler_count >= 4) {
			// receive message from clients and fill table msg
			handler_count++;

			//-----------------begin split string example 'John@text'
			std::string key;
			std::string value;
			int split_counter = 0;
			std::string s = buf;
			for (int i = 0; i < s.length(); i++) {
				if (buf[i] == '&') {					// '&'- symbol delimeter message and 'to'
					split_counter = i;
				}
			}

			key = s.substr(0, split_counter);					// message
			value = s.substr(split_counter + 1, s.length());	// to
			//------------------end split

			strcpy(text, key.c_str());
			std::string to = value;
			std::cout << "[Server][Added message] " << text << std::endl;
			std::string temp_msg = text;
			insertToTableMsg(mysql_obj, temp_msg, to);
		}
	}

	std::cout << "Work for fill table finished.\n";
	
	sending(connections, index);						// call function

	// Close connection
	mysql_close(&mysql_obj);
}

void sending(SOCKET connections[], int index) {
	char buf[256];
	while (true) {
		// receive message from client on server
		recv(connections[index], buf, sizeof(buf), 0);	
		
		// display message from client on server
		std::cout << "[Server][sending()] " << buf << std::endl;
		// speak client loop for()
		for (int i = 0; i < counter; i++) {
			if (i == index) {
				continue;
			}
			send(connections[i], buf, sizeof(buf), 0);
		}
	}
}

void createTableUsers(MYSQL mysql_obj) {
	int qstate = 0;
	std::string temp("CREATE TABLE IF NOT EXISTS users (id INTEGER AUTO_INCREMENT PRIMARY KEY, name VARCHAR(20) NOT NULL);");
	qstate = mysql_query(&mysql_obj, temp.c_str());
	if (qstate == 0) {
		std::cout << "Created table users..." << std::endl;
	}
	else {
		std::cout << "Failed creating table users..." << std::endl;
	}
}

void insertToTableUsers(MYSQL mysql_obj, std::string& str) {
	int qstate = 0;
	std::string temp("INSERT users (name) VALUES ('" + str + "');");
	qstate = mysql_query(&mysql_obj, temp.c_str());
	if (qstate == 0) {
		std::cout << "Record data ..." << std::endl;
	}
	else {
		std::cout << "Failed to insert..." << std::endl;
	}
}

void createTableMessages(MYSQL mysql_obj) {
	int qstate = 0;
	std::string temp("CREATE TABLE IF NOT EXISTS msg (id INTEGER AUTO_INCREMENT PRIMARY KEY, message VARCHAR(20) NOT NULL, user_to VARCHAR(10) NOT NULL);");
	qstate = mysql_query(&mysql_obj, temp.c_str());
	if (qstate == 0) {
		std::cout << "Created table msg..." << std::endl;
	}
	else {
		std::cout << "Failed creating table msg..." << std::endl;
	}
}

void insertToTableMsg(MYSQL mysql_obj, std::string& temp_msg, std::string& to) {
	int qstate = 0;
	std::string temp("INSERT msg (message, user_to) VALUES ('" + temp_msg + "', '" + to + "'); ");
	qstate = mysql_query(&mysql_obj, temp.c_str());
	if (qstate == 0) {
		std::cout << "Record data ..." << std::endl;
	}
	else {
		std::cout << "Failed to insert..." << std::endl;
	}
}
