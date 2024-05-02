/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:24:42 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 06:13:20 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	get_args(t_philo *philo, int argc, char **argv)
{
	philo->num_philos = ft_atoi(argv[1]);
	philo->to_die = ft_atoi(argv[2]);
	philo->to_eat = ft_atoi(argv[3]);
	philo->to_sleep = ft_atoi(argv[4]);
	philo->food = 0;
	if (argc == 6)
		philo->food = ft_atoi(argv[5]);
}

static int	check_arg_content(char *arg)
{
	while (*arg)
	{
		if (*arg < '0' || *arg > '9')
			return (0);
		arg++;
	}
	return (1);
}

void	print_usage(void)
{
	printf("Usage: ./philo_bonus <philo_count> <time_to_die> "
		"<time_to_eat> <time_to_sleep> [times_each_must_eat]\n"
       "Where:\n"
       "\t- philo_count: Total number of philosophers.\n"
       "\t- time_to_die: Maximum time (ms) before a philosopher starves.\n"
       "\t- time_to_eat: Time (ms) a philosopher takes to eat.\n"
       "\t- time_to_sleep: Time (ms) a philosopher takes to sleep.\n"
       "\t- times_each_must_eat: Optional, the minimum number of "
	   "meals each philosopher must eat.\n"
       "Example: \n\t./philo_bonus 5 800 200 200 5\n");
}

void	check_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!check_arg_content(argv[i]) || ft_atoi(argv[i]) <= 0)
		{
			printf("invalid argument nr <%d>\n", i);
			print_usage();
			exit(1);
		}
		i++;
	}
}
