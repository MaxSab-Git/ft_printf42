NAME=libftprintf.a
LIBFT=./libft/libft.a
COMPILER=cc
FLAGS=-Wall -Wextra -Werror -g3
SOURCES=ft_printf.c

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
	$(COMPILER) $(FLAGS) $(TESTSOURCES) -I./libft -I./ -L./libft -L./ -lftprintf -lft -lbsd
	./a.out