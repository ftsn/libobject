CC	=	cc

EXT	=	c

EXEC	=	test

OBJDIR	=	build

LIBDIR	=	../libobject

CFLAGS	=	-Wall -Wextra -Werror -std=c99 -g3
CFLAGS	+=	-I$(LIBDIR)/includes

LDFLAGS	+=	-L$(LIBDIR) -lobject -lcmocka

VPATH	=	./

SRCS    =	main		\
		string_tests	\
		array_tests	\
		list_tests	\
		dict_tests
#		iterator_tests

SRCS	:=	$(addsuffix .$(EXT), $(SRCS))

OBJS	=	$(SRCS:.$(EXT)=.o)

OBJS	:=	$(addprefix $(OBJDIR)/, $(OBJS))

all: 		$(EXEC)

$(EXEC):	$(OBJDIR) $(OBJS)
		-@echo -n Building $@ ...
		@$(CC) -o $@ $(filter %.o, $^) $(LDFLAGS)
		-@echo " [OK]"

$(OBJDIR):
		-@echo -n Building objects directory ...
		@mkdir -p $(OBJDIR)
		-@echo " [OK]"

$(OBJDIR)/%.o:	%.$(EXT)
		-@echo -n Compilation of $< ...
		@$(CC) -o $@ -c $< $(CFLAGS);
		-@echo " [OK]"

clean:
		-@$(RM) -r $(OBJDIR)
		-@echo Cleaning objects files ... [OK]

fclean:		clean
		-@$(RM) $(EXEC)
		-@echo Cleaning the library file ... [OK]

re:		fclean all

.PHONY:		all clean fclean re
