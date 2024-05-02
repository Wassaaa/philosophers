/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:57:42 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 07:36:49 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	zen_monitor(t_philo *philo)
{
	while (!halt_manager(philo, 0))
	{
		lock_mutex(philo, philo->food_lock);
		if (*philo->food_finished == philo->num_philos)
		{
			halt_manager(philo, 1);
			unlock_mutex(philo, philo->food_lock);
			return ;
		}
		unlock_mutex(philo, philo->food_lock);
		usleep(1000);
	}
}

int	end(t_philo *philo)
{
	if (philo->err)
	{
		printf("%s\n", philo->err);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo		philo;
	int			i;

	i = 0;
	if (argc >= 5)
	{
		if (!check_args(argc, argv))
			return (1);
		memset(&philo, 0, sizeof(t_philo));
		get_args(&philo, argc, argv);
		philo.threads = malloc(philo.num_philos * sizeof(pthread_t));
		if (!philo.threads)
			return (end(&philo));
		if (init_struct(&philo))
			start_threads(&philo, &i);
		if (i == philo.num_philos)
			zen_monitor(&philo);
		while (i--)
			pthread_join(philo.threads[i], NULL);
		free_all(&philo);
	}
	else
		print_usage();
	return (end(&philo));
}
