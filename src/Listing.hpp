/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-tell <ale-tell@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 09:33:23 by ale-tell          #+#    #+#             */
/*   Updated: 2025/02/14 09:33:23 by ale-tell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>

class Listing
{

  public:
	static std::string makeListing(const std::string &serverPath, const std::string &uriPath);

  private:
	static std::vector<std::string> makeDirList(const std::string &serverPath, const std::string &uriPath);

	Listing() {}

	~Listing() {}
};
