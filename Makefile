EXEC	=	a.out

CC	=	cc
EXT	=	c
OBJDIR	=	build

LIB	=	libobject.a
LIBDIR	=	libobject

CFLAGS	=	-Wall -Wextra -Werror -g3 -D_GNU_SOURCE
CFLAGS	+=	-I./srcs/includes/	\
		-I./$(LIBDIR)/includes/
LDFLAGS	+=	-L$(LIBDIR) -lobject

VPATH	=	./

SRCS	=	main
SRCS	:=	$(addsuffix .$(EXT), $(SRCS))

OBJS	=	$(SRCS:.$(EXT)=.o)
OBJS	:=	$(addprefix $(OBJDIR)/, $(OBJS))

all: 		$(EXEC)

$(LIB):
		@$(MAKE) -C $(LIBDIR)

$(EXEC):	$(OBJDIR) $(LIB) $(OBJS)
		-@echo -n Building $@ ...
		@$(CC) -o $@ $(filter %.o, $^) $(LDFLAGS)
		-@echo " [OK]"

$(OBJDIR):
		-@echo -n Building objects directory ...
		@mkdir -p $(OBJDIR)
		-@echo " [OK]"

$(OBJDIR)/%.o:	%.$(EXT)
		-@echo -n Compilation of $< ...
		@$(CC) -o $@ -c $< $(CFLAGS)
		-@echo " [OK]"

clean:
		-@$(MAKE) clean -C $(LIBDIR)
		-@$(RM) -r $(OBJDIR)
		-@echo Cleaning objects files ... [OK]

fclean:		clean
		-@$(MAKE) fclean -C $(LIBDIR)
		-@$(RM) $(EXEC)
		-@echo Cleaning the executable ... [OK]

re:		fclean all

.PHONY:		all clean fclean re
