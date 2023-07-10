CC 	= gcc
CFLAGS 	= -Wall -std=gnu99 -w

GREEN 	= \033[0;32m
RESET 	= \033[0m
YELLOW	= \033[0;33m

all: F4Client F4Server F4Countdown F4autoPlayer

F4Client: F4Client.c err_exit.c shared_memory.c fifo.c function.c semaphore.c client_function.c
	@clear
	@echo "${YELLOW}> making executable: $@ <${RESET}"
	@$(CC) $(CFLAGS) -o F4Client F4Client.c err_exit.c shared_memory.c fifo.c function.c semaphore.c client_function.c
	@echo "${GREEN}> executable $@ created successfully <${RESET}"

F4Server: F4Server.c err_exit.c shared_memory.c fifo.c function.c semaphore.c server_function.c
	@echo "${YELLOW}> making executable: $@ <${RESET}"
	@$(CC) $(CFLAGS) -o F4Server F4Server.c err_exit.c shared_memory.c fifo.c function.c semaphore.c server_function.c
	@echo "${GREEN}> executable $@ created successfully <${RESET}"

F4Countdown: F4Countdown.c err_exit.c function.c
	@echo "${YELLOW}> making executable: $@ <${RESET}"
	@$(CC) $(CFLAGS) -o F4Countdown F4Countdown.c err_exit.c function.c
	@echo "${GREEN}> executable $@ created successfully <${RESET}"

F4autoPlayer: F4autoPlayer.c err_exit.c shared_memory.c fifo.c function.c semaphore.c autoPlayer_function.c
	@echo "${YELLOW}> making executable: $@ <${RESET}"
	@$(CC) $(CFLAGS) -o F4autoPlayer F4autoPlayer.c err_exit.c shared_memory.c fifo.c function.c semaphore.c autoPlayer_function.c
	@echo "${GREEN}> executable $@ created successfully <${RESET}"

clean:
	@clear
	@echo "${YELLOW}> removing F4Client... <${RESET}"
	@rm -f -r F4Client
	@echo "${GREEN}> F4Client removed successfully <${RESET}"
	@echo "${YELLOW}> removing F4Server... <${RESET}"
	@rm -f -r F4Server
	@echo "${GREEN}> F4Server removed successfully <${RESET}"
	@echo "${YELLOW}> removing F4Countdown... <${RESET}"
	@rm -f -r F4Countdown
	@echo "${GREEN}> F4Countdown removed successfully <${RESET}"
	@echo "${YELLOW}> removing F4autoPlayer... <${RESET}"
	@rm -f -r F4autoPlayer
	@echo "${GREEN}> F4autoPlayer removed successfully <${RESET}"
	@rm -f -r F4Client1Run F4Client2Run F4ServerRun pid_countDown_.txt pid_readToken_.txt
	@echo "${GREEN}> files and executables removed successfully <${RESET}"

.PHONY: run clean
