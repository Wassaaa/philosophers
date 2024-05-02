/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:25:09 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 15:57:02 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

int	main(int argc, char **argv)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	check_args(argc, argv);
	memset(philo, 0, sizeof(t_philo));
	get_args(philo, argc, argv);
	start_sem(philo);
	start_philos(philo);
	return (0);
}
