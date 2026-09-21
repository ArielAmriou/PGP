##
## EPITECH PROJECT, 2026
## makefile
## File description:
## root makefile
##

CXX = clang++

MAIN = src/Main.cpp

SRC = 	$(addprefix src/, 						\
			ArgsParser.cpp						\
			MyPgp.cpp							\
			$(addprefix Cript/, 				\
				$(addprefix Xor/, 				\
					Xor.cpp						\
				)								\
			)									\
    	)

HEADERS = 	$(addprefix -iquote include/, 		\
			/									\
			Cript/Xor/							\
    	)

PATH_UNI = 	$(addprefix tests/, 				\
				tests_init.cpp 					\
				tests_xor.cpp 					\
			)

OBJ = $(SRC:.cpp=.o) $(MAIN:.cpp=.o)

TESTS = tests_run

CXXFLAGS = -Wall -Wextra --std=c++20

CPPFLAGS = $(HEADERS)

NAME = my_pgp

all:	$(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CPPFLAGS)

debug: CFLAGS += -g
debug: fclean $(OBJ) $(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

$(TESTS): LDFLAGS += --coverage -lcriterion
$(TESTS): uni_clean
	$(CXX) -o $(TESTS) $(SRC) $(PATH_UNI) \
		$(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)
	./$(TESTS)

uni_clean:
	$(RM) $(TESTS)*

clean: uni_clean
	$(RM) $(OBJ)

fclean:	clean
	$(RM) *.html coverage coverage.css
	$(RM) $(SERVER_NAME) $(NAME)

re: fclean all

.PHONY: all debug clean fclean re tests_run uni_clean
