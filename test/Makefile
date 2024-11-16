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

LIBFTPP_DIR	:= ..

LIBFTPP		:= $(LIBFTPP_DIR)/libftpp.a
CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++17 -MMD -MP -I$(LIBFTPP_DIR)
RM			:= rm -f

ifeq ($(DEBUG), 1)
CXXFLAGS	+= -g3
endif

ifeq ($(DEBUG), 2)
CXXFLAGS	+= -g3 -fsanitize=address
endif

# **************************************************************************** #

SRCS_DIR	= src
OBJS_DIR	= obj
TARGET_DIR	= target

MAIN_SRCS	= $(wildcard $(SRCS_DIR)/main/*.cpp)
TEST_SRCS	= $(wildcard $(SRCS_DIR)/test/*.cpp)
SRCS		= $(TEST_SRCS) $(MAIN_SRCS)

OBJS	= $(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
DEPS	= $(OBJS:.o=.d)
TARGETS	= $(patsubst $(OBJS_DIR)/%.o, $(TARGET_DIR)/%.out, $(OBJS))

GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m

# **************************************************************************** #

all: $(TARGETS)
	@printf "\n$(GR)=== Compiled [$(CXX) $(CXXFLAGS)] ==="
	@printf "\n--- $(notdir $(SRCS))$(RC)\n"

-include $(DEPS)

$(TARGET_DIR)/main/%.out: $(OBJS_DIR)/main/%.o $(LIBFTPP) | $(TARGET_DIR)/main
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@printf "$(CY)+$(RC)"

$(TARGET_DIR)/test/%.out: $(OBJS_DIR)/test/%.o $(LIBFTPP) | $(TARGET_DIR)/test
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@printf "$(CY)+$(RC)"

$(OBJS_DIR)/main/%.o: src/main/%.cpp | $(OBJS_DIR)/main
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "$(GR)+$(RC)"

$(OBJS_DIR)/test/%.o: src/test/%.cpp | $(OBJS_DIR)/test
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "$(GR)+$(RC)"

$(TARGET_DIR)/main:
	@mkdir -p $@
$(TARGET_DIR)/test:
	@mkdir -p $@
$(OBJS_DIR)/main:
	@mkdir -p $@
$(OBJS_DIR)/test:
	@mkdir -p $@

clean:
	@printf "$(RE)--- Removing $(OBJS_DIR)$(RC)\n"
	@$(RM) -r $(OBJS_DIR)

fclean: clean
	@printf "$(RE)--- Removing $(TARGET_DIR)$(RC)\n"
	@$(RM) -r $(TARGET_DIR)

# **************************************************************************** #
