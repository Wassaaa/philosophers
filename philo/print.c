/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:55:35 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 07:19:42 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	print_it(t_msg msg, t_philo *philo)
{
	int	id;
	int	ms;

	if (msg == THINK)
		philo->thinking = 1;
	if (msg == DIE || msg == EAT || msg == SLEEP)
		philo->thinking = 0;
	id = philo->id + 1;
	ms = get_ms(philo->start);
	if (msg == DIE)
		printf("%d %d died\n", ms, id);
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
		lock_mutex(philo, philo->print_lock);
		if (halt_manager(philo, 0))
		{
			unlock_mutex(philo, philo->print_lock);
			return ;
		}
		if (msg == DIE || !verify_existence(philo))
		{
			print_it(DIE, philo);
			halt_manager(philo, 1);
			unlock_mutex(philo, philo->print_lock);
			return ;
		}
		print_it(msg, philo);
		unlock_mutex(philo, philo->print_lock);
	}
}
