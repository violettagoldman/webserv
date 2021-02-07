/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICGIRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:04:14 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/04 17:26:33 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ICGIRequest.hpp"

ICGIRequest::ICGIRequest(CGIRequestPrototype requestProto) :
	_requestProto(requestProto)
{
}

CGIRequestPrototype ICGIRequest::getRequestProto() const
{
	return _requestProto;
}