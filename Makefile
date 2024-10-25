# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsudo <tsudo@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/29 00:00:00 by tsudo             #+#    #+#              #
#    Updated: 2024/08/29 00:00:00 by tsudo            ###   ##########         #
#                                                                              #
# **************************************************************************** #

NAME		:= libftpp.a
CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++17 -MMD -MP
AR			:= ar rc
RM			:= rm -f

ifeq ($(DEBUG), 1)
CXXFLAGS	+= -g3
endif

ifeq ($(DEBUG), 2)
CXXFLAGS	+= -g3 -fsanitize=address
endif

# **************************************************************************** #

SRCS += $(SRCS_DATA_STRUCRUES)
SRCS_DATA_STRUCRUES	:= \
	data_structures/data_buffer.cpp \

INCS	:= \
	. \

# **************************************************************************** #

OBJDIR		= objs/
OBJS		= $(patsubst %.cpp,$(OBJDIR)%.o, $(notdir $(SRCS)))
DEPS		= $(OBJS:.o=.d)
CXXFLAGS	+= $(addprefix -I,$(INCS))
vpath %.cpp $(sort $(dir $(SRCS)))

GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m

# **************************************************************************** #

$(NAME): $(OBJDIR) $(OBJS)
	@printf "\n$(GR)=== Compiled [$(CXX) $(CXXFLAGS)] ==="
	@printf "\n--- $(notdir $(SRCS))$(RC)\n"
	@$(AR) $(NAME) $(OBJS)
	@printf "$(CY)~~~ Archived [$(AR)] ~~~\n--- $(NAME)$(RC)\n"

all: $(NAME)

-include $(DEPS)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)%.o: %.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "$(GR)+$(RC)"

clean:
	@printf "$(RE)--- Removing $(OBJDIR)$(RC)\n"
	@$(RM) -r $(OBJDIR)

fclean: clean
	@printf "$(RE)--- Removing $(NAME)$(RC)\n"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

# **************************************************************************** #