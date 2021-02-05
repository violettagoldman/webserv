/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:18:35 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/04 17:27:55 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIREQUEST_HPP
# define CGIREQUEST_HPP

# include "ICGIRequest.hpp"

class CGIRequest : public ICGIRequest {

public:
	CGIRequest();
	~CGIRequest();
	// CGIRequest(const CGIRequest &copy);
	// CGIRequest &operator= (const CGIRequest &operand);

private:

};

#endif