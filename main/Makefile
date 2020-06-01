TARGET = BurningHells
BUILD_OUTPUT_DIR = ../output

SRC := $(wildcard ./*.c)
OBJS := $(SRC:./%.c=./$(BUILD_OUTPUT_DIR)/%.o)
DEPS := $(SRC:./%.c=./$(BUILD_OUTPUT_DIR)/%.d)

CFLAGS = -g -Wall -Werror 
INCLUDE_PATH = 
LD_FLAGS =
LIBS = -levent -levent_pthreads -lrabbitmq

VERSION = `bash version.sh`

all::

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),gencov)
ifneq ($(MAKECMDGOALS),package)
-include $(DEPS)
endif
endif
endif

${BUILD_OUTPUT_DIR}/%.d: %.c
	@mkdir -p $(BUILD_OUTPUT_DIR)
	$(CC) -MM -MT "$(BUILD_OUTPUT_DIR)/$(subst .c,.o,${notdir $<}) $(BUILD_OUTPUT_DIR)/$(subst .c,.d,${notdir $<})" -MF "$(subst .c,.d,${BUILD_OUTPUT_DIR}/${notdir $<})" $<

$(BUILD_OUTPUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@

cov : CFLAGS += -fprofile-arcs -ftest-coverage

cov : LD_FLAGS += -fprofile-arcs -ftest-coverage

cov : all

test : CFLAGS += -DENABLE_BURNING_HELLS_TEST

test : cov
	@valgrind --leak-check=full --show-reachable=yes -v ${BUILD_OUTPUT_DIR}/${TARGET} 2>&1 | tee valgrind.log
	@grep "0 errors from 0 contexts" valgrind.log

all:: ${BUILD_OUTPUT_DIR}/${TARGET}

gencov:
	@cp *.c $(BUILD_OUTPUT_DIR)
	@gcov -r -o $(BUILD_OUTPUT_DIR) $(BUILD_OUTPUT_DIR)/*.c

${BUILD_OUTPUT_DIR}/${TARGET}: ${OBJS}
	${CC} -g -Wall -Werror  ${LD_FLAGS} -o $@  $^ ${LIBS}

.PHONY: clean cov gencov package
