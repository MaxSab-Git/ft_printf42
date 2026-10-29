NAME=libftprintf.a
LIBFT=./libft/libft.a
COMPILER=cc
FLAGS=-Wall -Wextra -Werror
SOURCES=ft_printf.c ./libft/ft_putchar_fd.o ./libft/ft_putstr_fd.o ./libft/ft_strlen.o

TESTSOURCES=main.c

OBJS=$(SOURCES:%.c=%.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C ./libft

$(NAME): $(OBJS)
	ar -rc $@ $(OBJS)

$(OBJS):%.o:%.c
	$(COMPILER) $(FLAGS) -c $< -o $@ -I./libft 

.PHONY: all clean fclean re test

clean:
	$(MAKE) clean -C ./libft
	rm -f $(OBJS)

fclean: clean
	$(MAKE) fclean -C ./libft
	rm -f $(NAME)

re: fclean all

test: all
	norminette --use-gitignore
	$(COMPILER) $(FLAGS) $(TESTSOURCES) -L. -lftprintf -lbsd
	./a.out