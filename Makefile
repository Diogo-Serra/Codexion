NAME        = codexion

CC          = cc
INCLUDES    = -Isrc
CFLAGS      = -Wall -Wextra -Werror -pthread

SRC_DIR     = src
OBJ_DIR     = obj
TESTS_DIR   = src/test

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

test: $(NAME)
	@bash $(TESTS_DIR)/tests.sh
	@bash $(TESTS_DIR)/run_tests.sh

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all test clean fclean re