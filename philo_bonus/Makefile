NAME = philo_bonus

SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)

GCC = cc
FLAGS = -g -ggdb3 -Wall -Werror -Wextra

$(NAME) : $(OBJS)
	$(GCC) $(FLAGS) $(OBJS) -o $(NAME) -I ./

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re
