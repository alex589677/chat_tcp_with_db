Здравстуйте уважаемый пользователь.
Операционная система windows 8.1 x64.
IDLE Visual Studio 2019, MySQL 8.0 Command Line Client.
У вас на компьютере должна быть установлена MySQL 8.0 Command Line Client.

Ключ компиляции client.cpp: g++ -o client client.cpp -lws2_32
Компиляция файла server.cpp применялась с помощью IDLE Visual Studio 2019,
	иначе способ компиляции ошибка поиска "mysql.h".

Запуск сначала server.exe, затем client.exe, client2.exe.
//mysql_real_connect(&mysql_obj, "localhost", "alexandr", "1234", "testdb", NULL, NULL, 0);

!!!База данных testdb уже должна быть создана. (запрос mysql: "create database testdb;")

Создаются 2 табилцы users, msg.
Сначала вводятся имена в обоих клиентах,
затем сообщения состоящие из одного слова через знак & и текст кому (см. вывод таблицы users(id)).
Посмотреть таблицу msg, есть возможность через консоль MySQL 8.0 Command Line Client.
Прекращение работающих программ ctrl+c. 