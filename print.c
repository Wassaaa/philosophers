/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:55:35 by aklein            #+#    #+#             */
/*   Updated: 2024/04/29 00:11:18 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	write_it(char *str, int ms, int id)
{
	ft_putnbr_fd(ms, 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(id, 1);
	ft_putstr_fd(str, 1);
}

static void	print_it(t_msg msg, t_philo *philo)
{
	int	id;
	int	ms;

	id = philo->id + 1;
	ms = get_ms(philo->start);
	if (msg == DIE)
		write_it(" died\n", ms, id);
	if (msg == FORK)
		write_it(" has taken a fork\n", ms, id);
	if (msg == EAT)
		write_it(" is eating\n", ms, id);
	if (msg == SLEEP)
		write_it(" is sleeping\n", ms, id);
	if (msg == THINK)
		write_it(" is thinking\n", ms, id);
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
			print_it(DIE, philo);
			halt_manager(philo, 1);
			pthread_mutex_unlock(philo->print_lock);
			return ;
		}
		print_it(msg, philo);
		pthread_mutex_unlock(philo->print_lock);
	}
}
