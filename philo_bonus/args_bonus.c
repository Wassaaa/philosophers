/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:24:42 by aklein            #+#    #+#             */
/*   Updated: 2024/04/29 09:02:56 by aklein           ###   ########.fr       */
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
	ft_putstr_fd("Usage: ./philo_bonus <philo_count> <time_to_die> \
<time_to_eat> <time_to_sleep> [times_each_must_eat]\n", 1);
	ft_putstr_fd("Where:\n", 1);
	ft_putstr_fd("\t- philo_count: Total number of philosophers.\n", 1);
	ft_putstr_fd("\t- time_to_die: Maximum time (ms) \
before a philosopher starves.\n", 1);
	ft_putstr_fd("\t- time_to_eat: Time (ms) a \
philosopher takes to eat.\n", 1);
	ft_putstr_fd("\t- time_to_sleep: Time (ms) a \
philosopher takes to sleep.\n", 1);
	ft_putstr_fd("\t- time_each_must_eat: Optional, the minimum \
number of meals each philosopher must eat.\n", 1);
	ft_putstr_fd("Example: \n\t./philo 5 800 200 200 5\n", 1);
}

void	check_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!check_arg_content(argv[i]) || ft_atoi(argv[i]) <= 0)
		{
			ft_putstr_fd("invalid argument nr <", STDOUT_FILENO);
			ft_putnbr_fd(i, STDOUT_FILENO);
			ft_putstr_fd(">\n", STDOUT_FILENO);
			print_usage();
			exit(1);
		}
		i++;
	}
}
