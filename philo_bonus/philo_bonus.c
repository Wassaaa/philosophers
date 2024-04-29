/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:25:09 by aklein            #+#    #+#             */
/*   Updated: 2024/04/29 11:09:58 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	close_sems(t_philo *philo)
{
	if (philo->forks)
		sem_close(philo->forks);
	if (philo->print)
		sem_close(philo->print);
	sem_unlink(FORKS);
	sem_unlink(PRINT);
}

void	error(t_philo *philo, int ret, char *msg)
{
	if (ret != 0)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	close_sems(philo);
	exit(ret);
}

void	pre_life(t_philo *philo)
{
	if (gettimeofday(&philo->start, NULL) == -1)
		error(philo, 1, ERR_TIME);
	philo->fed = philo->start;
}

void	existential_meal(t_philo *philo)
{
	if (sem_wait(philo->forks) == -1)
		error(philo, 1, ERR_SEM_WAIT);
	print_message(FORK, philo);
	if (philo->num_philos == 1)
		sentient_pause(philo->to_die * 2, philo);
	if (sem_wait(philo->forks) == -1)
		error(philo, 1, ERR_SEM_WAIT);
	print_message(FORK, philo);
	print_message(EAT, philo);
	if (gettimeofday(&philo->fed, NULL) == -1)
		error(philo, 1, ERR_TIME);
	sentient_pause(philo->to_eat, philo);
	print_message(SLEEP, philo);
	if (--philo->food == 0)
	{
		if (sem_wait(philo->print) == -1)
			error(philo, 1, ERR_SEM_WAIT);
		if (kill(getpid(), SIGSTOP) == -1)
			error(philo, 1, ERR_SIGNAL);
		if (sem_post(philo->print) == -1)
			error(philo, 1, ERR_SEM_POST);
	}
}

void	existential_cycle(t_philo *philo)
{
	pre_life(philo);
	while (verify_existence(philo))
	{
		existential_meal(philo);
		if (sem_post(philo->forks) == -1)
			error(philo, 1, ERR_SEM_POST);
		if (sem_post(philo->forks) == -1)
			error(philo, 1, ERR_SEM_POST);
		sentient_pause(philo->to_sleep, philo);
		print_message(THINK, philo);
	}
	error(philo, 0, NULL);
}

void	waitress(t_philo *philo)
{
	int	status;
	int	count;

	count = 0;
	while (42)
	{
		waitpid(0, &status, WUNTRACED);
		if (WIFSTOPPED(status))
		{
			kill(0, SIGCONT);
			if (++count >= philo->num_philos)
			{
				close_sems(philo);
				kill(0, SIGTERM);
			}
		}
	}
}

void	start_philos(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->num_philos)
	{
		philo->my_pid = fork();
		if (philo->my_pid == -1)
			error(philo, 1, ERR_FORK);
		if (philo->my_pid == 0)
		{
			philo->id = i;
			existential_cycle(philo);
		}
		i++;
	}
	waitress(philo);
}

void	start_sem(t_philo *philo)
{
	sem_unlink(FORKS);
	sem_unlink(PRINT);
	philo->forks = sem_open(FORKS, O_CREAT, 0600, philo->num_philos);
	if (philo->forks == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->print = sem_open(PRINT, O_CREAT, 0600, 1);
	if (philo->print == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
}

int	main(int argc, char **argv)
{
	t_philo	philo;
	
	if (argc >= 5)
	{
		check_args(argc, argv);
		memset(&philo, 0, sizeof(t_philo));
		get_args(&philo, argc, argv);
		start_sem(&philo);
		start_philos(&philo);
	}
	else
		print_usage();
	return (0);
}
