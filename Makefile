# ==========================================
#  CONFIGURAÇÕES GERAIS
# ==========================================
CC             := gcc
CFLAGS         := -Wall -Wextra -pedantic -Iinclude $(shell pkg-config --cflags glib-2.0) -fexec-charset=UTF-8
LIBS           := -lm $(shell pkg-config --libs glib-2.0)
DEBUG_CFLAGS   := -g
RELEASE_CFLAGS := -O2

# Nomes dos Executáveis
EXE_NAME       := programa-principal
TEST_NAME      := programa-testes
INTER_NAME     := programa-iterativo

# Diretorias
OBJDIR         := obj
DOCSDIR        := docs
RESDIR         := resultados

# ==========================================
#  FONTES E OBJETOS
# ==========================================
SOURCES := $(shell find src -name "*.c")
HEADERS := $(shell find include -name "*.h")
OBJECTS := $(patsubst src/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Modo Debug/Release
ifeq ($(DEBUG), 1)
    CFLAGS += $(DEBUG_CFLAGS)
else
    CFLAGS += $(RELEASE_CFLAGS)
endif

# ==========================================
#  REGRAS
# ==========================================

.PHONY: all setup clean run

# Compila os 3 executáveis
all: setup $(EXE_NAME) $(TEST_NAME) $(INTER_NAME)

setup:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(RESDIR)

# Compilação .c -> .o
$(OBJDIR)/%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

# 1. Programa Principal
$(EXE_NAME): $(OBJECTS)
	@echo "🔧 A gerar $(EXE_NAME)..."
	$(CC) -o $@ $^ $(LIBS)

# 2. Programa de Testes
$(TEST_NAME): $(OBJECTS)
	@echo "🔧 A gerar $(TEST_NAME)..."
	$(CC) -o $@ $^ $(LIBS)

# 3. Programa Iterativo
$(INTER_NAME): $(OBJECTS)
	@echo "🔧 A gerar $(INTER_NAME)..."
	$(CC) -o $@ $^ $(LIBS)

# Limpeza
clean:
	@rm -rf $(OBJDIR) $(EXE_NAME) $(TEST_NAME) $(INTER_NAME) $(DOCSDIR)
	@rm -f $(RESDIR)/*.csv $(RESDIR)/*.txt
	@echo "🧹 Limpeza concluída."