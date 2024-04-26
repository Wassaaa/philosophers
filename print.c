/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:55:35 by aklein            #+#    #+#             */
/*   Updated: 2024/04/26 17:31:49 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	print_it(t_msg msg, t_philo *philo)
{
	int	id;
	int	ms;

	id = philo->id + 1;
	ms = get_ms(philo->start);
	if (msg == FORK)
		printf("%d %d has taken a fork\n", ms, id);
	if (msg == EAT)
		printf("%d %d is eating\n", ms, id);
	if (msg == SLEEP)
		printf("%d %d is sleeping\n", ms, id);
	if (msg == THINK)
		printf("%d %d is thinking\n", ms, id);
}

void	print_message(t_msg msg, t_philo *philo)
{
	if (!halt_manager(philo, 0))
	{
		pthread_mutex_lock(philo->print_lock);
		if (halt_manager(philo, 0))
		{
			pthread_mutex_unlock(philo->print_lock);
			return ;
		}
		if (msg == DIE || !verify_existence(philo))
		{
			printf("%d %d died\n", get_ms(philo->start), philo->id + 1);
			halt_manager(philo, 1);
			pthread_mutex_unlock(philo->print_lock);
			return ;
		}
		print_it(msg, philo);
		pthread_mutex_unlock(philo->print_lock);
	}
}
