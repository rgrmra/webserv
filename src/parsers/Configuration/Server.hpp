/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:51:41 by rde-mour          #+#    #+#             */
/*   Updated: 2025/01/04 20:36:05 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Configuration.hpp"
#include <ostream>
#include <set>

using namespace std;

class Server {

	private:
		set<Configuration> _servers;

		Server(void);

	public:
		Server(string configuration_file);
		Server(const Server &src);
		Server &operator=(const Server &src);
		~Server(void);

		Configuration getServer(string host, string port);
		set<Configuration> getServers(void) const;
};

ostream &operator<<(ostream &os, const Server &src);

#endif /* SERVER_HPP */
