NAME = minishell

SRC = minishell.c main.c echo.c env.c struct.c cd.c lib_function.c setenv.c \
	  unsetenv.c path.c string.c dir.c execve.c

HEADER = .

all : $(NAME)

$(NAME) : 
	gcc -ggdb3 -Wall -Wextra -I$(HEADER) -c $(SRC)
	gcc -o $(NAME)   *.o 

clean : 
	rm -f *.o

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re ft_ls