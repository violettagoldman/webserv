/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestCGIRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:28:31 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/04 17:55:24 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTCGIREQUEST_HPP
# define TESTCGIREQUEST_HPP

# include "ICGIRequest.hpp"

class TestCGIRequest : public ICGIRequest {

public:

	TestCGIRequest();
	~TestCGIRequest();
	// TestCGIRequest(const TestCGIRequest &copy);
	// TestCGIRequest &operator= (const TestCGIRequest &operand);

private:

};

#endif