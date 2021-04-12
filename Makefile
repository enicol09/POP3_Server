TARGET = bas

SRCS  = $(shell find ./src     -type f -name *.c)
HEADS = $(shell find ./include -type f -name *.h)
OBJS = $(SRCS:.c=.o)
DEPS = Makefile.depend

INCLUDES = -I./include
CFLAGS = -O3 -Wall $(INCLUDES)
LDFLAGS = -lm -pthread

all: $(TARGET)

$(TARGET): $(OBJS) $(HEADS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

run: all
	@./$(TARGET)

.PHONY: depend clean
depend:
	$(CC) $(INCLUDES) -MM $(SRCS) > $(DEPS)
	@sed -i -E "s/^(.+?).o: ([^ ]+?)\1/\2\1.o: \2\1/g" $(DEPS)

# To make all (program + manual) "make doxy"
doxy:
	$(DOXYGEN) doxygen.conf &> doxygen.log
# To clean .o files: "make clean"
clean:
	$(RM) $(OBJS) $(TARGET)

-include $(DEPS)
