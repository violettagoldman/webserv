#include "../inc/Server.class.hpp"

#include <sys/time.h>
#include <unistd.h>
#include "../inc/Request.class.hpp"
// #include "../inc/Header.class.hpp"
// #include "./read_request.cpp"

Request *read_request(int sd, Request *req);

int main(void)
{
    int     i;
    Server  s;
    int	    clients[30]; //change to vector later
    fd_set  fds;
    // char    buffer[1024];
    int     new_socket;
	Request *request = new Request;
	// int valread;
	int sd;
	int max_sd;

    s = Server();
    for (i = 0; i < 30; i++)
		clients[i] = 0;

//     s.setup();
//     s.listen();

	std::cout << "Waiting for connections ...\n";

	while(1)
	{
		//clear the socket set
		FD_ZERO(&fds);

		//add master socket to set
		FD_SET(s.getFd(), &fds);
		max_sd = s.getFd();

		//add child sockets to set
		for (i = 0 ; i < 30; i++)
		{
			//socket descriptor
			sd = clients[i];
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &fds);
			// highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
		// wait for an activity on one of the sockets
		select( max_sd + 1 , &fds , NULL , NULL , NULL);
		if (FD_ISSET(s.getFd(), &fds))
		{
			new_socket = s.accept();
			s.send(new_socket, "Hello from the server\n");
			for (i = 0; i < 30; i++)
			{
				//if position is empty
				if( clients[i] == 0 )
				{
					clients[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
					break;
				}
			}
		}
		//else its some IO operation on some other socket
		for (i = 0; i < 30; i++)
		{
			sd = clients[i];

			if (FD_ISSET( sd , &fds))
			{
				(void)request;
				// request = read_request(sd, request);
				// if (request->getState() == "end")
				// // if ((valread = read( sd , buffer, 1024)) == 0)
				// {
                //     s.close();
				// 	printf("Host disconnected\n");
				// 	close(sd);
				// 	clients[i] = 0;
				// }
				// else if (request->getState() == "read")
				// {
				// 	// request->print_headers();
				// 	// cgi_dostuff(request);
				// 	std::cout << "Success";
				// 	// std::cout << "Request method is " << request.getMethod() << std::endl;
				// // 	buffer[valread] = '\0';
				// 	// std::cout << "I just got your message: " << buffer << std::endl;
				// //
				// }
			}
		}
	}

    return (0);
}
