#include "codexion.h"

static const char *args[] = {
    "NUM_CODERS", "BURNOUT",
    "COMPILE", "DEBUG",
    "REFACTOR", "NUM_COMPILES",
    "DONGLE_COOLDOWN", "SCHEDULER",
};

int ft_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int	ft_atoi(const char *s)
{
	int	number;
	int	sign;

	number = 0;
	sign = 1;
	while ((*s == 32) || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s++ == '-')
			sign *= -1;
	}
	while (*s >= '0' && *s <= '9')
		number = (number * 10) + (*s++ - '0');
	return (number * sign);
}

void ft_showinfo(int argc, char **argv) {
    int i;
    int j;

    i = 0;
    j = 1;
    printf("Data:\n");
    while (j < argc - 1) {   
        printf("%s: %i\n", args[i++], ft_atoi(argv[j++]));
    }
    printf("%s: %s\n", args[i], argv[j]);
}

const char *get_arg(int index) {
    return (args[index]);
}

