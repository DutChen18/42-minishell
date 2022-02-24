NAME			:= minishell

FILE_NAMES		:= main.c memory.c assert.c lexer/expand.c lexer/init.c lexer/add.c \
				lexer/lex.c lexer/operator.c lexer/escape.c lexer/advance.c \
				lexer/main.c lexer/debug.c lexer/special.c lexer/source.c \
				input/input_file.c input/input_readline.c input/input_string.c \
				input/input.c parser/parser.c parser/convert.c parser/debug.c op.c

CC				:= cc
LINK_CMD		:= cc
CFLAGS			:= -Wall -Wextra -pedantic
LFLAGS			:= -Wall -Wextra

SRC_DIR			:= src
LIB_DIR			:= dependencies
INC_DIR			:= include $(LIB_DIR)/libft
OBJ_DIR			:= build
DEP_DIR			:= build

LIBFT_DIR			:= $(LIB_DIR)/libft
LIBFT_LIB			:= $(LIBFT_DIR)/libft.a

SOURCES			:= $(patsubst %.c,$(SRC_DIR)/%.c,$(FILE_NAMES))
OBJECTS			:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILE_NAMES))
DEPENDS			:= $(patsubst %.c,$(DEP_DIR)/%.d,$(FILE_NAMES))

ifndef config
	config = debug
endif

ifeq ($(config), debug)
	CFLAGS		+= -DSH_DEBUG=1 -fsanitize=address,undefined -g3 -Og
	LFLAGS		+= -DSH_DEBUG=1 -fsanitize=address,undefined
else ifeq ($(config), release)
	CFLAGS		+= -Werror -g3 -O2
	LFLAGS		+= -Werror
else ifeq ($(config), distr)
	CFLAGS		+= -Werror -g0 -Ofast
	LFLAGS		+= -Werror 
else
$(error "invalid config $(config"))
endif

all: $(NAME)

# TODO compile libft using its own makefile
$(NAME): $(OBJECTS) $(LIBFT_LIB)
	$(LINK_CMD) -o $@ $(OBJECTS) $(LIBFT_LIB) $(LFLAGS) -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $< -MMD $(patsubst %,-I%,$(INC_DIR))

$(LIBFT_LIB):
	${MAKE} -C $(LIBFT_DIR) bonus

crash: $(NAME)
	ln -s $(NAME) $@

clean:
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)
	${MAKE} -C $(LIBFT_DIR) fclean

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPENDS)
.PHONY: all clean fclean re