CC	=	gcc

CP	=	cp

MKDIR	=	mkdir -p

RM	=	rm -rf

NAME	=	pamela.so

SRCS	=	srcs/pam_handle.c	\
		srcs/container.c	\
		srcs/secure_volume.c \
		srcs/utils.c

OBJS	=	$(SRCS:.c=.o)

LDFLAGS	=	-lcryptsetup

CFLAGS	=	-fPIC -fno-stack-protector -I ./includes/

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -shared -rdynamic $(LDFLAGS) -o $(NAME)

install:
ifneq ("$(wildcard /lib/security)", "")
	@printf "\033[0;31mPAM module already installed\n\033[0m"
else
	apt-get install -y cryptsetup gcc libcryptsetup-dev libpam0g-dev
	make
	@$(MKDIR) /lib/security
	@$(CP) $(NAME) /lib/security/$(NAME)
	@echo "auth optional pamela.so" >> /etc/pam.d/common-auth
	@echo "session optional pamela.so" >> /etc/pam.d/common-session
	@echo "account optional pamela.so" >> /etc/pam.d/common-account
	@echo "password optional pamela.so" >> /etc/pam.d/common-password
	@printf "\033[0;32mPAM module installed successfully\n\033[0m"
endif

uninstall:
ifeq ("$(wildcard /lib/security)", "")
	@printf "\033[0;31mPAM module not installed\n\033[0m"
else
	make clean
	@$(RM) /lib/security
	@head -n -1 /etc/pam.d/common-auth > /tmp/common-auth && mv /tmp/common-auth /etc/pam.d/common-auth
	@head -n -1 /etc/pam.d/common-session > /tmp/common-session && mv /tmp/common-session /etc/pam.d/common-session
	@head -n -1 /etc/pam.d/common-account > /tmp/common-account && mv /tmp/common-account /etc/pam.d/common-account
	@head -n -1 /etc/pam.d/common-password > /tmp/common-password && mv /tmp/common-password /etc/pam.d/common-password
	@printf "\033[0;32mPAM module uninstalled successfully\n\033[0m"
endif

check:
ifeq ("$(wildcard /lib/security)", "")
	@printf "\033[0;31mPAM module not installed\n\033[0m"
else
	@printf "\033[0;32mPAM module installed\n\033[0m"
endif

test:

clean:
	$(RM) $(OBJS)
	$(RM) $(NAME)

re: clean all
