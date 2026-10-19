NAME        = codexion

CC          = cc
INCLUDES    = -Isrc
CFLAGS      = -Wall -Wextra -Werror -pthread

SRC_DIR     = src
OBJ_DIR     = obj

SRC         = main.c \
              engine/routine.c engine/queue.c engine/wait.c \
              engine/acquire.c engine/release.c \
              resources/mem.c resources/utils.c \
              parser/parser.c parser/init.c \
              monitor/monitor.c monitor/log.c

OBJ         = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/codexion.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all test clean fclean re