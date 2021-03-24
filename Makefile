NAME = webserv
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -std=c++98 -g3 -Iinc/
CC = clang++

TOTAL = $(shell find src -iname  "*.cpp" | wc -l | bc)
O_COUNT = 0
COUNT = 0

#Colors
RESET = \033[0m
GREEN_BOLD = \033[32;1m
LIGHT_BLUE = \033[94m


define update
	$(eval O_COUNT := $(shell find obj -iname "*.o" 2> /dev/null | wc -l | bc))
	printf "\r["
	printf "=%.0s" $(shell seq 0 ${O_COUNT})
	printf ">"
	$(eval COUNT := $(shell echo ${TOTAL} - ${O_COUNT} | bc))
	printf "%${COUNT}s" "]"
endef

ifeq ($(D), 1)
		CFLAGS += -fsanitize=address
endif
OS = $(shell uname -s)
ifeq ($(OS), Linux)
	TICK = \342\234\224
else
	TICK = \xE2\x9C\x94
endif

SRC = ${wildcard src/*.cpp}
SRC_CGI = ${wildcard src/cgi/*.cpp}
SRC_CONFIG = ${wildcard src/config/*.cpp}
SRC_GNL = ${wildcard src/get_next_line/*.cpp}

OBJ = ${patsubst src/%.cpp, obj/%.o, ${SRC}}
OBJ_CGI = ${patsubst src/cgi/%.cpp, obj/cgi/%.o, ${SRC_CGI}}
OBJ_CONFIG = ${patsubst src/config/%.cpp, obj/config/%.o, ${SRC_CONFIG}}
OBJ_GNL = ${patsubst src/get_next_line/%.cpp, obj/get_next_line/%.o, ${SRC_GNL}}

all: $(NAME)

$(NAME): ${OBJ} ${OBJ_CGI} ${OBJ_CONFIG} ${OBJ_GNL}
	@${CC} ${OBJ} ${OBJ_CGI} ${OBJ_CONFIG} ${OBJ_GNL} ${CFLAGS} -o ${NAME}
	@echo "${GREEN_BOLD}\r${NAME} by vgoldman, ablanar, ashishae ${TICK}${RESET}"

./obj/cgi/%.o: ./src/cgi/%.cpp
	@mkdir -p ./obj/cgi
	@${CC} ${CFLAGS} -c $< -o $@
	@$(call update)

./obj/config/%.o: ./src/config/%.cpp
	@mkdir -p ./obj/config
	@${CC} ${CFLAGS} -c $< -o $@
	@$(call update)

./obj/get_next_line/%.o: ./src/get_next_line/%.cpp
	@mkdir -p ./obj/get_next_line
	@${CC} ${CFLAGS} -c $< -o $@
	@$(call update)

./obj/%.o: ./src/%.cpp
	@mkdir -p ./obj
	@${CC} ${CFLAGS} -c $< -o $@
	@$(call update)

clean:
	@rm -rf obj
	@echo "${GREEN_BOLD}Cleaned ${NAME} ${TICK}${RESET}"

fclean:	clean
	@rm -rf ${NAME} ${NAME}.dySM
	@echo "${GREEN_BOLD}Forced cleaned ${NAME} ${TICK}${RESET}"

re:	fclean all

.PHONY: clean fclean all re
