CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = main.c\
	   redirection.c\
	   utils.c\

NAME = pipex
OBJ_DIR = OBJS
SRC_DIR = ./SRC
LIBFT = ./libft
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all : $(NAME)

$(NAME): $(OBJS) ./include/pipex.h
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT) -lft -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(MAKE) clean -C $(LIBFT)
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) fclean -C $(LIBFT)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
