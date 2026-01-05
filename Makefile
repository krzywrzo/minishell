# **************************************************************************** #
#                                   MAKEFILE                                   #
# **************************************************************************** #

NAME		=	minishell

CC			=	cc
CFLAGS		= -g 
# CFLAGS		=	-Wall -Wextra -Werror -g

# Detect OS (Darwin = macOS)
UNAME_S		:=	$(shell uname -s)

# Directories
SRC_DIR		=	src
OBJ_DIR		=	obj
INC_DIR		=	inc


SRC_FILES = lexer_utils.c lexer.c main.c parser.c parser_utils.c cleaners.c printers.c exec.c exec_utils.c env.c builtins.c
SRC			=	$(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Default (Linux)
LIBS		=	-lreadline
LIB_PATH = libft
LIB_PATH = libft
LIBFT = $(LIB_PATH)/libft.a

# macOS Configuration
ifeq ($(UNAME_S),Darwin)
	READLINE_DIR	=	/opt/homebrew/opt/readline
	LIBS			=	-L$(READLINE_DIR)/lib -I$(READLINE_DIR)/include -lreadline -lncurses
endif

# Colors (optional but 42-allowed)
GREEN		=	\033[0;32m
YELLOW		=	\033[0;33m
RESET		=	\033[0m

# **************************************************************************** #
#                                   RULES                                      #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -I$(INC_DIR) $(OBJ) $(LIBS) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)[OK]$(RESET) $(NAME) compiled successfully."

$(LIBFT):
		make -C $(LIB_PATH) all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
	@echo "$(YELLOW)Compiling:$(RESET) $<"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Objects cleaned.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)Executable removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
