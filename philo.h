/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:55:55 by aklein            #+#    #+#             */
/*   Updated: 2024/04/23 00:40:21 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>

typedef struct s_philo
{
	int				id;
	int				num_philos;
	int				to_die;
	int				to_eat;
	int				to_sleep;
	pthread_mutex_t	*forks;
}					t_philo;

int	ft_atoi(const char *str);

#endif