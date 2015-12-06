# pmgt Makefile
#
# 2004 K.H

TARGET		= pmgt

SHELL		= /bin/csh
RM		= /bin/rm -f
LOCK		= /bin/chmod -w
UNLOCK		= /bin/chmod +w
INSTALL		= /bin/cp -p
INSTALL_DIR	= /etc/puppet/PMGT/sbin

COMPILE		= gcc -c -Wall -g -I . -Dlinux
LINK		= gcc -g

INCLUDES	= \
	libcommon.h \
	liboption.h \
	pmgt_common.h \
	pmgt_option.h \
        pmgt_test.h \
        pmgt_hosts.h \
        pmgt_consts.h \
	pmgt_gen.h \
	pmgt.h

SOURCES		= \
	liboption.c \
	pmgt_common.c \
	pmgt_option.c \
	pmgt_test.c \
	pmgt_hosts.c \
	pmgt_consts.c \
	pmgt_gen.c \
	$(TARGET).c

OBJECTS		= $(SOURCES:.c=.o)

.c.o		:
	$(COMPILE) $< -o $@


all		: $(TARGET)

$(TARGET)	: $(OBJECTS) $(LIB_A)
	$(LINK) $(OBJECTS) -o $(TARGET)
	@echo ""
	@echo "$(TARGET) を作成しました。"
	@echo ""

$(OBJECTS)	: $(INCLUDES)

# 現在はシンボリックリンクなので不要
install		:
	$(INSTALL) $(TARGET) $(INSTALL_DIR)

clean		: 
	@$(LOCK) $(SOURCES)
	$(RM) $(OBJECTS) $(TARGET)
	@$(UNLOCK) $(SOURCES)
