/* In this header are macros tho facilitate debugging */

#pragma once
# define BOLD				"\033[1m"
# define BLACK				"\033[30m"
# define RED				"\033[31m"
# define GREEN				"\033[32m"
# define YELLOW				"\033[33m"
# define BLUE				"\033[34m"
# define MAGENTA			"\033[35m"
# define CYAN				"\033[36m"
# define WHITE				"\033[37m"
# define ORANGE				"\033[38;5;202m"

# define ANSI_RESET			"\033[0m"

# define DEBUG(msg,...) fprintf(stderr, BOLD GREEN"[LOG](%s:%d)" RED msg ANSI_RESET"\n" , __FILE__, __LINE__, ##__VA_ARGS__)

# define D_ERROR(msg){          \
    printf("Error : %s\n", msg);\
    exit(1);                    \
}
