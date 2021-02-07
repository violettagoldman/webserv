// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.cpp                                           :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2021/01/27 11:19:47 by ashishae          #+#    #+#             */
// /*   Updated: 2021/01/31 13:01:03 by ashishae         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */


// #include <unistd.h>
// #include <vector>
// #include <string>
// #include <iostream>
// #include <sys/errno.h>
// #include <sys/wait.h>
// #include "../config/get_next_line/get_next_line.hpp"

// // dup stdout to some pipe
// // close real stdout
// // run program
// // read from pipe

// size_t	ft_strlen(const char *str)
// {
// 	char *start;

// 	start = (char *)str;
// 	while (*str)
// 		str++;
// 	return ((size_t)(str - start));
// }

// size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
// {
// 	unsigned int i;
// 	unsigned int limit;

// 	i = 0;
// 	limit = (unsigned int)(dstsize - 1);
// 	if (!dst || !src)
// 		return (0);
// 	if (dstsize == 0)
// 		return (ft_strlen(src));
// 	while (src[i] != '\0' && i < limit)
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	dst[i] = '\0';
// 	return (ft_strlen(src));
// }

// char *cppalloc(size_t size)
// {
// 	char *ret = new char[size];
// 	return ret;
// }

// char	*ft_strdup(const char *s1)
// {
// 	char	*p;
// 	size_t	len;

// 	len = ft_strlen(s1);
// 	if ((p = cppalloc(sizeof(char) * (len + 1))) == NULL)
// 		return (NULL);
// 	ft_strlcpy(p, s1, len + 1);
// 	p[len] = '\0';
// 	return (p);
// }

// char **create_envp(std::vector<std::string> mvars)
// {
// 	char **ret = new char*[mvars.size()+1];
// 	for (int i = 0; i < mvars.size(); i++)
// 	{
// 		ret[i] = ft_strdup(mvars[i].c_str());
// 	}
// 	ret[mvars.size()] = NULL;
// 	return ret;
// }

// // launch cgi
// // specify file
// // write something to the stdin of a running process

// void execute_cgi(const char *cgi_path, const char *requested_file, char **envp)
// {
// 	//"/Users/ae/spacial-steel/cdng/webserv/cgi/tiny.php"
// 	// const char *path_to_cgi = "/usr/local/bin/php-cgi";
// 		// const char *path_to_cgi = "/Users/ae/spacial-steel/cdng/webserv/cgi/reader";
// 	char * const argv[] = {const_cast<char *>(cgi_path),
// 				const_cast<char *>(requested_file), NULL};

// 	execve(cgi_path, argv, envp);
// }


// void launch_cgi(int *pipe_in, int *pipe_out, const char *cgi_path,
// 				const char *requested_file, char **envp)
// {
// 	// close(STDIN_FILENO);
// 	// dup(pipe_in[0]);
// 	// close(STDOUT_FILENO);
// 	// dup(pipe_out[1]);
// 	execute_cgi(cgi_path, requested_file, envp);
// }



// int main(void)
// {
// 	pid_t pid;
// 	extern int errno;
// 	// const char *path_to_cgi = "/usr/local/bin/php-cgi";
	

// 	// Pipe
// 	// int pipe_res[2];
// 	// pipe(pipe_res);

// 	int pipe_in[2];
// 	int pipe_out[2];
// 	// pipe(pipe_in);
// 	// pipe(pipe_out);

// 	// std::vector<std::string> v;

// 	// v.push_back("SERVER_SOFTWARE=webserv");
// 	// v.push_back("GATEWAY_INTERFACE=CGI/1.1");
// 	// // v.push_back("pipiska2");

// 	// char **envp = create_envp(v);


// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		// child
// 		// execute_to_pipe(pipe_res);
// 		const char *path_to_cgi = "/Users/ae/spacial-steel/cdng/webserv/cgi/test";
// 		const char *requested_file = "/Users/ae/spacial-steel/cdng/webserv/cgi/tiny.php";
// 		// int s = dup(STDIN_FILENO);
		
// 		// close(pipe_res[0]);
// 		// close(pipe_res[1]);c
// 		launch_cgi(pipe_in, pipe_out, path_to_cgi, requested_file, NULL);

		
// 	}
// 	std::cout << "Writing" << std::endl;
// 	// const char *s = "pipiska\n";
// 	// write(pipe_res[1], s, strlen(s));
// 	waitpid(pid, NULL, 0);
// 	// close(pipe_res[1]);
// 	// int ret;
// 	// char *line;
// 	// while ((ret = get_next_line(pipe_res[0], &line)))
// 	// {
// 	// 	std::cout << "Ret: " << ret << "Line: |" << line << "|" << std::endl;
// 	// }

// 	/* code */
// 	return 0;
// }