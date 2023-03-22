#include "Bot.hpp"


Bot::Bot(char *dest_ip, int dest_port, char *dict_file, char *server_password) : sockATTR(), ircServATTR(), destIp(dest_ip), destPort(dest_port), dictionary(dict_file)
{
	if ((sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) // IPV4 TCP özelliklerine sahip socket oluşturuldu.
		throw std::runtime_error(strerror(errno));

	sockATTR.sin_family = AF_INET; //IPV4
	sockATTR.sin_addr.s_addr = htons(INADDR_ANY);	//bu tanım bulunduğu bilgisayarın ip adresini almasını söyler //inet_addr("127.0.0.1"); // htonl(INADDR_ANY)  INADDR_ANY sabitinin mesela 12 olarak belirlendiği durumlarda işe yarar.
	sockATTR.sin_port = 0;							// İşletim sistemi boşta olan bir portu atayacak.
	memset(sockATTR.sin_zero, 0, 8);

	// I am trying to use connect().
	ircServATTR.sin_family = AF_INET;
	ircServATTR.sin_addr.s_addr = inet_addr(dest_ip);
	ircServATTR.sin_port = htons(destPort);
	memset(ircServATTR.sin_zero, 0, 8);

	if (connect(sockFD, reinterpret_cast<sockaddr *>(&ircServATTR), sizeof(ircServATTR)) < 0)
		throw std::runtime_error(strerror(errno));
	if (send(sockFD, server_password, ::strlen(server_password), 0) < 0)
		throw std::runtime_error("send error");
}

void Bot::catchBadWords()
{
	char *buff = (char *)malloc(512);
	string badWord;
	while (1)
	{
		::memset(buff, 0, ::strlen(buff));
		if (recv(sockFD, buff, 512, 0) < 0)
			throw std::runtime_error("recv error");
		std::cout << "Data from SERVER:" << buff;
		if ((badWord = checkWordsInFile(dictionary, buff)).length() > 1)
		{
			string banMessage = "\tBOT: BAN <USER>.\n\tREASON: USING BAD WORD : ";
			banMessage.append(badWord + "\n\r");
			if (send(sockFD, banMessage.c_str(), banMessage.length(), 0) < 0)
				throw std::runtime_error("send error");
			std::cout << "Data send from socket:\n" << banMessage;
			badWord.erase(badWord.begin(), badWord.end());
			banMessage.clear();
		}
	}
}

std::string Bot::checkWordsInFile(string file_name, string sentence)
{
	string word;
	ifstream file(file_name);

	if (!file)
	{
		(ofstream (file_name)).close();
		file.close();
		file.open(file_name);
	}

	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file,word);
			if (sentence.find(word, 0) != string::npos)
			{
				file.close();
				return word;
			}
		}
		file.close();
		return string("");
	}
	else
		throw std::runtime_error("Can't open ");
}