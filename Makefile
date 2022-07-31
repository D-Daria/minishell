NAME = minishell

#---mandatory part---#
OBJS_DIR =	objs

SRCS_MF =	main.c debug_tests.c parser.c reading_cmds.c\
			utils.c commands.c commands_utils.c execution.c\
			execution_errors.c redirection.c builtins.c\
			redirection_utils.c free_utils.c utils_envp.c\
			builtin_export.c

OBJS_M = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.o, $(SRCS_MF)))
D_FILES_M = $(addprefix $(OBJS_DIR)/, $(patsubst %.c,%.d, $(SRCS_MF)))

LIBFT = ./libft/libft.a
LIB_INC = -L./libft/ -lft

detected_OS := $(shell uname)

ifeq ($(detected_OS), Darwin)
	INCLUDES = -I./includes/ -I./libft/includes/ -I./Memd/
	RDLINE = -lreadline -lcurses
else
	INCLUDES = -I./includes/ -I./libft/includes/
	RDLINE = -lreadline
endif

OPFLAGS = -O2
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
SANIT_FLAGS = cc -fsanitize=address $(CFLAGS)

#colors
RED 	= 	\033[0;31m
GREEN 	= 	\033[0;32m
BLUE	=	\033[0;34m
BREAK 	= 	\033[0m
YELLOW	=	\033[0;33m

.PHONY : all lib bonus clean fclean sanit_m re re_minishell

all : lib $(NAME)

$(OBJS_DIR) :
	mkdir $@

lib :
	make -C libft/

$(NAME) : $(OBJS_M) $(LIBFT)
	$(CC) $(CFLAGS) $(OPFLAGS) $(OBJS_M) $(LIB_INC) $(MEM_LIB) $(RDLINE) -o $(NAME)
	@echo "$(BLUE)$(NAME)$(GREEN) --> DONE!$(BREAK)"

$(OBJS_DIR)/%.o : %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(OPFLAGS) $(INCLUDES) -c $< -o $@ -MD

clean :
	make -C libft/ $@
	rm -rf $(OBJS_DIR)
	@echo "$(BLUE)objs $(RED)--> DELETED$(BREAK)"

fclean : clean
	make -C libft/ $@
	@echo "$(BLUE)libft.a $(RED)--> DELETED$(BREAK)"
	rm -f $(NAME)
	@echo "$(BLUE)$(NAME) $(RED)--> DELETED$(BREAK)"

re : fclean all

re_minishell:
	rm -rf $(OBJS_DIR)
	@echo "$(BLUE)objs $(RED)--> DELETED$(BREAK)"
	make

#sanitizer
sanit_m : $(OBJS_M)
	make
	$(SANIT_FLAGS) $(OBJS_M) $(LIB_INC) $(MEM_LIB) $(RDLINE) -o $(NAME)

-include $(D_FILES_M)