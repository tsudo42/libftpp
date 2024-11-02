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

TARGET	:= test_pool
SRCS	:= \
	main_pool.cpp \
	test_pool.cpp \
	main_data_buffer.cpp \
	test_data_buffer.cpp \
	main_memento.cpp \
	main_observer.cpp \
	main_singleton.cpp \
	main_state_machine.cpp \
	main_thread_safe_iostream.cpp \
	main_thread_safe_queue.cpp \
	main_thread.cpp \
	main_worker_pool.cpp \
	main_persistent_worker.cpp \
	main_vector_2.cpp \
	main_vector_3.cpp \
	main_random_2D_coordinate_generator.cpp \
	main_perlin_2D.cpp \
	test_argparse.cpp \
	test_dual_key_map.cpp \

BUILD_DIR	:= build

OBJS	= $(patsubst %.cpp,$(BUILD_DIR)/%.o, $(notdir $(SRCS)))
DEPS	= $(OBJS:.o=.d)
EXECS	= $(OBJS:.o=.out)
vpath %.cpp $(sort $(dir $(SRCS)))

GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m

# **************************************************************************** #

run: tests
	@printf "$(CY)~~~ RUNNING TESTS ~~~$(RC)\n"
	@$(foreach test, $(EXECS), \
		echo "\033[1;34mRunning: ./$(test)\033[0m"; \
		./$(test) || exit 1;)
	@printf "$(CY)~~~ TESTS COMPLETED ~~~$(RC)\n"

all: tests

-include $(DEPS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

tests: $(EXECS)
	@printf "\n$(GR)=== Compiled [$(CXX) $(CXXFLAGS)] ==="
	@printf "\n--- $(notdir $(SRCS))$(RC)\n"

$(BUILD_DIR)/%.out: $(BUILD_DIR)/%.o $(LIBFTPP)
	@$(CXX) $(CXXFLAGS) -o $@ $^
	@printf "$(CY)+$(RC)"

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "$(GR)+$(RC)"

$(LIBFTPP):
	$(MAKE) -C $(LIBFTPP_DIR)

cleanlib:
	$(MAKE) -C $(LIBFTPP_DIR) clean

clean:
	@printf "$(RE)--- Removing $(BUILD_DIR)$(RC)\n"
	@$(RM) -r $(BUILD_DIR)

re: clean cleanlib run

.PHONY: run all tests clean re

# **************************************************************************** #
