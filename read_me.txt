����������� ��������� ������������.
������������ ������� windows 8.1 x64.
IDLE Visual Studio 2019, MySQL 8.0 Command Line Client.
� ��� �� ���������� ������ ���� ����������� MySQL 8.0 Command Line Client.

���� ���������� client.cpp: g++ -o client client.cpp -lws2_32
���������� ����� server.cpp ����������� � ������� IDLE Visual Studio 2019,
	����� ������ ���������� ������ ������ "mysql.h".

������ ������� server.exe, ����� client.exe, client2.exe.
//mysql_real_connect(&mysql_obj, "localhost", "alexandr", "1234", "testdb", NULL, NULL, 0);

!!!���� ������ testdb ��� ������ ���� �������. (������ mysql: "create database testdb;")

��������� 2 ������� users, msg.
������� �������� ����� � ����� ��������,
����� ��������� ��������� �� ������ ����� ����� ���� & � ����� ���� (��. ����� ������� users(id)).
���������� ������� msg, ���� ����������� ����� ������� MySQL 8.0 Command Line Client.
����������� ���������� �������� ctrl+c. 