/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:57:42 by aklein            #+#    #+#             */
/*   Updated: 2024/04/29 00:09:05 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	zen_monitor(t_philo *philo)
{
	while (!halt_manager(philo, 0))
	{
		pthread_mutex_lock(philo->food_lock);
		if (*philo->food_finished == philo->num_philos)
		{
			halt_manager(philo, 1);
			pthread_mutex_unlock(philo->food_lock);
			return ;
		}
		pthread_mutex_unlock(philo->food_lock);
		usleep(1000);
	}
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
			return (1);
		if (init_struct(&philo))
			start_threads(&philo, &i);
		if (i == philo.num_philos)
			zen_monitor(&philo);
		while (i--)
			pthread_join(philo.threads[i], NULL);
		free_all(&philo);
	}
	return (0);
}
