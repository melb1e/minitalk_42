SERVER = server
CLIENT = client

CC = gcc
CFLAGS = -Wall -Wextra -Werror

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all:	$(SERVER) $(CLIENT)

server: $(if $(findstring bonus, $(MAKECMDGOALS)), server_bonus.c, server.c)
	cc $(CFLAGS) -o $@ $^

client: $(if $(findstring bonus, $(MAKECMDGOALS)), client_bonus.c, client.c)
	cc $(CFLAGS) -o $@ $^

bonus:	$(SERVER) $(CLIENT)

clean:
		rm -rf $(SERVER).o $(CLIENT).o

fclean:	clean
		rm -rf $(SERVER) $(CLIENT) $(SERVER).o $(CLIENT).o

re:		fclean all

.PHONY: all bonus clean fclean re