CC	=	cc
#CC	=	/opt/homebrew/Cellar/gcc/10.2.0_4/bin/gcc-10

EXT	=	c

EXEC	=	test

OBJDIR	=	build

LIBDIR	=	../libobject

CFLAGS	=	-Wall -Wextra -std=c11 -fms-extensions -Wno-microsoft-anon-tag -g3
CFLAGS	+=	-I$(LIBDIR)/includes

LDFLAGS	+=	-L$(LIBDIR) -lobject -lcmocka

VPATH	=	./

SRCS    =	main		\
		array_tests	\
		string_tests	\
		dict_tests	\
		spl_list_tests	\
		spl_clist_tests	\
		dbl_list_tests	\
		dbl_clist_tests	\
		iterator_tests

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
