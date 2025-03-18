/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 13:54:20 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/26 15:41:29 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "SuperServer.hpp"
#include "color.hpp"
#include "webserv.hpp"

volatile sig_atomic_t gSignalStatus = 0;
std::string			  whitespace = " \t\n";

void printBanner();

void handleSignal(int signal)
{
	gSignalStatus = signal;
}

int main(int argc, char **argv, char **envp)
{
	try
	{
		if (argc < 2)
			throw(std::invalid_argument("server configuration file required"));
		if (argc > 2)
			throw(std::invalid_argument("too many arguments"));
		printBanner();

		initMimeTypes();
		initHTTPIdentifiers();
		initURLCodes();

		if (std::signal(SIGINT, handleSignal) == SIG_ERR)
			throw(std::runtime_error("signal binding failed"));

		superServerConfig config = ConfigParser::parse(argv[1]);
		SuperServer		  super(config, envp);

		std::cout << super << std::endl;

		super.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << color::red << "Error: " << e.what() << color::reset << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void printBanner()
{
	std::cout << std::endl;
	std::cout << color::white << "███████╗" << color::green << "██╗   ██╗" << color::white << "██████╗ " << color::green << "███████╗" << color::white << "██████╗ " << color::green << "███████╗" << color::white << "███████╗" << color::green << "██████╗ " << color::white << "██╗   ██╗" << color::green << "███████╗" << color::white << "██████╗ " << std::endl;
	std::cout << color::white << "██╔════╝" << color::green << "██║   ██║" << color::white << "██╔══██╗" << color::green << "██╔════╝" << color::white << "██╔══██╗" << color::green << "██╔════╝" << color::white << "██╔════╝" << color::green << "██╔══██╗" << color::white << "██║   ██║" << color::green << "██╔════╝" << color::white << "██╔══██╗" << std::endl;
	std::cout << color::white << "███████╗" << color::green << "██║   ██║" << color::white << "██████╔╝" << color::green << "█████╗  " << color::white << "██████╔╝" << color::green << "███████╗" << color::white << "█████╗  " << color::green << "██████╔╝" << color::white << "██║   ██║" << color::green << "█████╗  " << color::white << "██████╔╝" << std::endl;
	std::cout << color::white << "╚════██║" << color::green << "██║   ██║" << color::white << "██╔═══╝ " << color::green << "██╔══╝  " << color::white << "██╔══██╗" << color::green << "╚════██║" << color::white << "██╔══╝  " << color::green << "██╔══██╗" << color::white << "╚██╗ ██╔╝" << color::green << "██╔══╝  " << color::white << "██╔══██╗" << std::endl;
	std::cout << color::white << "███████║" << color::green << "╚██████╔╝" << color::white << "██║     " << color::green << "███████╗" << color::white << "██║  ██║" << color::green << "███████║" << color::white << "███████╗" << color::green << "██║  ██║" << color::white << " ╚████╔╝ " << color::green << "███████╗" << color::white << "██║  ██║" << std::endl;
	std::cout << color::white << "╚══════╝" << color::green << " ╚═════╝ " << color::white << "╚═╝     " << color::green << "╚══════╝" << color::white << "╚═╝  ╚═╝" << color::green << "╚══════╝" << color::white << "╚══════╝" << color::green << "╚═╝  ╚═╝" << color::white << "  ╚═══╝  " << color::green << "╚══════╝" << color::white << "╚═╝  ╚═╝" << std::endl;
	std::cout << color::reset << std::endl;
}
