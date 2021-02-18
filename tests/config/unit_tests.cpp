/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_tests.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 17:45:02 by ashishae          #+#    #+#             */
/*   Updated: 2021/02/18 12:49:56 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Test-related stuff below

// Needed to automatically fail tests;
#include <assert.h>
#include <string>

#include <iostream>

bool exception_thrown = false;

void check(int expression);

#define TEST_EXCEPTION(expression, exceptionType, exceptionString) { \
	exception_thrown = false; \
	try \
	{ \
		expression; \
	} \
	catch (const exceptionType &e) \
	{ \
		exception_thrown = true; \
		check((!strcmp(e.what(), exceptionString))); \
		std::cout << e.what() << std::endl;\
	} \
	if (!exception_thrown)\
		std::cout << "Exception NOT thrown" << std::endl; \
	check(exception_thrown == true); \
}

int test_no = 1;

void out(std::string s)
{
	std::cout << std::endl;
	std::cout << "\033[0;34m" << "Test " << test_no << " | " << s << "\033[0m" << std::endl;
	test_no += 1;
}

void check(int expression)
{
	// If expression doesn't evaluate to 1, the program will abort
	// assert(expression == 1);
	if (expression == 1)
	{
		std::cout << "\033[92m✓ PASS\033[0m" << std::endl;
	}
	else 
	{
		std::cout << "\033[91m✓ FAIL\033[0m" << std::endl;
	}
	
}

#include "VirtualHost.class.hpp"
#include "ConfigReader.class.hpp"

int main(void)
{
	// out("virtualHost | Default virtualHost");
	// virtualHost default_virtualHost;
	out("Simple config");
	ConfigReader r("./config/test_configs/nginx.conf");

	// Config *conf = r.createConfig();
	Config conf = r.getConfig();

	check(conf.getAutoindex() == true);
	check(conf.getClientMaxBodySize() == 32);
	check(conf.getIndex().size() == 1);
	check(conf.getIndex()[0] == "index.html");

	Config *_conf = r.createConfig();

	check(_conf->getAutoindex() == true);
	check(_conf->getClientMaxBodySize() == 32);
	check(_conf->getIndex().size() == 1);
	check(_conf->getIndex()[0] == "index.html");


	std::vector<VirtualHost> virtualHostVector = conf.getVirtualHostVector();
	check(virtualHostVector.size() == 4);

	out("Config | Root");
	check(conf.getRoot() == "/var/www/");

	out("Host 0 | Listen");
	check(virtualHostVector[0].getListenIp() == 80);
	check(virtualHostVector[0].getListenHost() == "");



	check(virtualHostVector[0].getServerName().size() == 2);
	check(virtualHostVector[0].getServerName()[0] == "domain1.com");
	check(virtualHostVector[0].getServerName()[1] == "www.domain1.com");
	check(virtualHostVector[0].getAutoindex() == true);

	out("Host 0 | Index");
	check(virtualHostVector[0].getIndex().size() == 2);
	check(virtualHostVector[0].getIndex()[0] == "index.php");
	check(virtualHostVector[0].getIndex()[1] == "index2.php");

	check(virtualHostVector[0].getLocations().size() == 1);

	check(virtualHostVector[0].getLocations()[0].getPattern() == "/");
	check(virtualHostVector[0].getLocations()[0].getRoot() == "/var/www/");

	check(virtualHostVector[0].getClientMaxBodySize() == 32);

	out("Host 0 | Root inherited from config");
	check(virtualHostVector[0].getRoot() == "/var/www/");


	check(virtualHostVector[1].getListenHost() == "127.0.0.1");
	check(virtualHostVector[1].getListenIp() == 443);
	check(virtualHostVector[1].getServerName().size() == 2);
	check(virtualHostVector[1].getServerName()[0] == "domain2.com");
	check(virtualHostVector[1].getServerName()[1] == "www.domain2.com");
	check(virtualHostVector[1].getClientMaxBodySize() == 1000);
	check(virtualHostVector[1].getAutoindex() == true);


	
	check(virtualHostVector[1].getLocations().size() == 2);

	check(virtualHostVector[1].getLocations()[0].getPattern() == "/app/");
	check(virtualHostVector[1].getLocations()[0].getAutoindex() == true);
	check(virtualHostVector[1].getLocations()[0].getClientMaxBodySize() == 1000);

	check(virtualHostVector[1].getLocations()[0].getRoot() == "/var/www/");
	out("Host 1 | Location 0 | limit_except ");
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getMethods().size() == 1);
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getMethods()[0] == "GET");
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getAllow().size() == 2);
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getAllow()[0] == "127.0.0.1");
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getAllow()[1] == "127.0.0.2");
	check(virtualHostVector[1].getLocations()[0].getLimitExcept().getDeny().size() == 1);

	out("Host 1 | Root on server level");
	check(virtualHostVector[1].getRoot() == "/var/www2/");
	// check(virtualHostVector[1].getLocations()[0].getLimitExcept()().getDeny()[0] == "all");

	out("Host 1 | Location 0 | upload_store on Server, inherited to Location");
	check(virtualHostVector[1].getUploadStore() == "/toto/lol/");
	check(virtualHostVector[1].getLocations()[0].getUploadStore() == "/toto/lol/");

	out("Host 1 | Location 0 | Index");
	check(virtualHostVector[1].getLocations()[0].getIndex().size() == 1);
	check(virtualHostVector[1].getLocations()[0].getIndex()[0] == "index3.html");

	check(virtualHostVector[2].getClientMaxBodySize() == 1024);
	check(virtualHostVector[2].getAutoindex() == true);
	check(virtualHostVector[2].getLocations()[0].getAutoindex() == true);

	out("Host 1 | Location 1 | Root inherited from server");
	check(virtualHostVector[1].getLocations()[1].getRoot() == "/var/www2/");

	out("Host 2 | Location 0 | getClientMaxBodySize in megabytes");
	check(virtualHostVector[2].getLocations()[0].getClientMaxBodySize() == 42000000);
	
	out("Host 2 | Location 0 | upload_store on Location");
	check(virtualHostVector[2].getLocations()[0].getUploadStore() == "/toto/lol2/");

	out("Host 2 | Location 1 | fastcgi_pass");
	check(virtualHostVector[2].getLocations()[1].getFcgiPass() == "127.0.0.1:9000");
	check(virtualHostVector[2].getLocations()[1].getFcgiParams()["TEST_PARAM"] == "test_val");

	out("Host 2 | Location 1 | autoindex off");
	check(virtualHostVector[2].getLocations()[1].getAutoindex() == false);

	out("Host 3 | Location 0 | limit_except with multiple methods");
	check(virtualHostVector[3].getLocations()[0].getLimitExcept().getMethods().size() == 2);
	check(virtualHostVector[3].getLocations()[0].getLimitExcept().getMethods()[0] == "GET");
	check(virtualHostVector[3].getLocations()[0].getLimitExcept().getMethods()[1] == "POST");

	out("Exception | Missing semicolon");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/unfinished_directive.conf"), Exception, "A semicolon is missing");

	out("Exception | Block not closed");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/block_not_closed.conf"), Exception, "A block wasn't closed");

	out("Exception | Two braces on one line");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/two_braces_on_line.conf"), Exception, "Please close each block on a new line.");

	out("Exception | Two directives");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/two_directives_on_one_line.conf"), Exception, "Please only put one directive per line.");

	out("Exception | Two directives (text after semicolon)");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/text_after_semicolon.conf"), Exception, "Please only put one directive per line.");

	out("Exception | limit_except not closed");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/limit_except_not_closed.conf"), Exception, "A block wasn't closed");

	out("Exception | location not closed");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/location_not_closed.conf"), Exception, "A block wasn't closed");

	out("Exception | file errors");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/nonexistent_config"), Exception, "Couldn't open file");

	out("Exception | overflow");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/overflow1.conf"), Exception, "clientMaxBodySize too large");

	out("Exception | overflow");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/overflow2.conf"), Exception, "clientMaxBodySize too large");

	out("Exception | two servers with one name");
	TEST_EXCEPTION(ConfigReader r2("./config/test_configs/two_servers_with_one_name.conf"), Exception, "Two servers with one server_name and listen");

	
	out("Exception | root and fcgi on same location");
	TEST_EXCEPTION(ConfigReader r3("./config/test_configs/location_with_multiple_actions.conf"), Exception, "Root and fcgi_pass on the same location.");

	out("Exception | upload and fcgi on same location");
	TEST_EXCEPTION(ConfigReader r3("./config/test_configs/upload_and_fcgi.conf"), Exception, "Upload_store and fcgi_pass on the same location.");

	// TEST_EXCEPTION(ConfigReader r2("missing_listen.conf"), virtualHost::DirectiveNotFound,\
	// 				"A required directive wasn't found in a context.");

	// TEST_EXCEPTION(ConfigReader r2("missing_root.conf"), Location::DirectiveNotFound,\
	// 				"A required directive wasn't found in a context.");
	
	// TEST_EXCEPTION(ConfigReader r2("syntaxerror.conf"), Reader::SyntaxError,\
	// 				"The virtualHost file contains a syntax error.");

					
	


	// check(default_virtualHost.getListenIp() == 80);
	// check(default_virtualHost.getServerName().size() == 1);
	// check(default_virtualHost.getServerName()[0] == "localhost");
	// check(default_virtualHost.getLocations().size() == 1);
	// check(default_virtualHost.getLocations()[0].getPattern() == "/");
	// check(default_virtualHost.getLocations()[0].getRoot() == "/var/www/");

	
	
}