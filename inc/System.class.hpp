#ifndef SYSTEM_CLASS_HPP
# define SYSTEM_CLASS_HPP

# include <iostream>

class System
{
	public:
		System(void);
		System(System const &src);
		~System(void);

		System	&operator=(System const &src);
        void    clearSocketSet(void);
        void    addMasterSocket(int masterSocket);

        fd_set  *getFdSet() const;
        void    setFdSet(fd_set *fd_set);

		
	private:
        fd_set *_fd_set;

};

#endif