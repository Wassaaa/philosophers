// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   print_bonus.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/04/26 16:55:35 by aklein            #+#    #+#             */
// /*   Updated: 2024/04/29 00:21:26 by aklein           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include <philo_bonus.h>

static void	write_it(char *str, int ms, int id)
{
	ft_putnbr_fd(ms, STDOUT_FILENO);
	ft_putstr_fd(" ", STDOUT_FILENO);
	ft_putnbr_fd(id, STDOUT_FILENO);
	ft_putstr_fd(str, STDOUT_FILENO);
}

static void	print_it(t_msg msg, t_philo *philo)
{
	int	id;
	int	ms;

	if (*philo->die == 1)
		return ;
	id = philo->id + 1;
	ms = get_ms(philo, philo->start);
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

// void	print_message(t_msg msg, t_philo *philo)
// {
// 	if (!halt_manager(philo, 0))
// 	{
// 		pthread_mutex_lock(philo->print_lock);
// 		if (halt_manager(philo, 0))
// 		{
// 			pthread_mutex_unlock(philo->print_lock);
// 			return ;
// 		}
// 		if (msg == DIE || !verify_existence(philo))
// 		{
// 			print_it(DIE, philo);
// 			halt_manager(philo, 1);
// 			pthread_mutex_unlock(philo->print_lock);
// 			return ;
// 		}
// 		print_it(msg, philo);
// 		pthread_mutex_unlock(philo->print_lock);
// 	}
// }

void	print_message(t_msg msg, t_philo *philo)
{
	lock_sem(philo, philo->lock);
	if (msg == DIE)
	{
		print_it(DIE, philo);
		*philo->die = 1;
		unlock_sem(philo, philo->lock);
		unlock_sem(philo, philo->death);
		return ;
	}
	print_it(msg, philo);
	unlock_sem(philo, philo->lock);
}
