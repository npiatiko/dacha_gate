# Name of the program
NAME = dachaGATE

# Compiling flags
FLAGS = -std=c++14 -fno-exceptions #-Wall -Wextra -Werror
CC = arm-linux-gnueabihf-g++

# Folders
SRC_DIR = ./
OBJ_DIR = ./obj/
INC_DIR = ./includes/

# Source files and object files
SRC_FILES = ttyHandler.cpp \
			callHandler.cpp \
            main.cpp

INC_FILES = $(INC_DIR)ttyHandler.h \
			$(INC_DIR)callHandler.h

OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Paths
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

# all rule
all: obj $(NAME)

obj:
	@mkdir -p $(OBJ_DIR)
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(INC_FILES)
	$(CC) $(FLAGS) -I $(INC_DIR) -o $@ -c $<

# Compiling
$(NAME): $(OBJ)
	$(CC) $(OBJ) -lm -o $(NAME)

# clean rule
clean:
	rm -Rf $(OBJ_DIR)

# fclean rule
fclean: clean
	rm -f $(NAME)

# re rule
re: fclean all

# phony
.PHONY: all clean fclean re
