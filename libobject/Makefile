CC	=	cc
#CC	=	/opt/homebrew/Cellar/gcc/10.2.0_4/bin/gcc-10

AR	=	ar rcs

EXT	=	c

EXEC	=	libobject.a

OBJDIR	=	build

LIBSDIR	=	libs

CFLAGS	=	-Wall -Wextra -std=c11 -fms-extensions -Wno-microsoft-anon-tag -g3
CFLAGS	+=	-I./includes/

VPATH	=	src/

SRCS    =	types                   \
		new                     \
		utils                   \
		strings_design          \
		strings_methods         \
		containers_methods      \
		lists_design            \
		lists_methods           \
		arrays_design           \
		arrays_methods          \
		dicts_design            \
		dicts_methods           \
		iterators_design        \
		iterators_methods

SRCS	:=	$(addsuffix .$(EXT), $(SRCS))

OBJS	=	$(SRCS:.$(EXT)=.o)

OBJS	:=	$(addprefix $(OBJDIR)/, $(OBJS))

all: 		$(EXEC)

$(EXEC):	$(OBJDIR) $(OBJS)
		-@echo -n Building $@ ...
		@$(AR) $@ $(filter %.o, $^)
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
