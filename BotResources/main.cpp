//
// Created by Kerim Sancakoglu on 3/20/23.
//

#include "Bot.hpp"
#include <stdexcept>
#include <iostream>


// args = [dest_ip] [dest_port] [dict_file]
int	main(int argc, char **argv)
{
	try
	{
		Bot badWordBot(argv[1], atoi(argv[2]), argv[3]);

		badWordBot.catchBadWords();

		//badWordBot.binding();
		//badWordBot.listenSock(4);
		//badWordBot.acceptIrcServer();


	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}


}