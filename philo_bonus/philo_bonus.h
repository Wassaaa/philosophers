/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:20:36 by aklein            #+#    #+#             */
/*   Updated: 2024/04/30 02:12:33 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# include <sys/types.h>
# include <sys/wait.h>

# define FORKS "/forks"
# define PRINT "/print"
# define DEATH "/death"
# define ZEN "/zen"
# define FED_LOCK "/lock"

# define ERR_SEM_OPEN "ERROR: sem_open failed"
# define ERR_SEM_CLOSE "ERROR: sem_close failed"
# define ERR_SEM_WAIT "ERROR: sem_wait failed"
# define ERR_SEM_POST "ERROR: sem_post failed"
# define ERR_TIME "ERROR: gettimeofday failed"
# define ERR_FORK "ERROR: Fork failed"
# define ERR_SIGNAL "ERROR: Sending signal failed"
# define ERR_THREAD_C "ERROR: Thread creation failed"
# define ERR_THREAD_D "ERROR: Thread detach failed"
# define ERR_MALLOC "ERROR: Malloc failed"


typedef struct s_philo
{
	int				id;
	int				num_philos;
	pid_t			*pids;
	int				to_die;
	int				to_eat;
	int				to_sleep;
	int				food;
	sem_t			*forks;
	sem_t			*print;
	sem_t			*death;
	sem_t			*zen;
	sem_t			*lock;
	struct timeval	start;
	struct timeval	fed;
}					t_philo;

typedef enum e_msg
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE
}	t_msg;

void	get_fed(t_philo *philo, int get);
int		ft_atoi(const char *str);
int		get_ms(t_philo *philo, struct timeval start);
int		verify_existence(t_philo *philo);
void	sentient_pause(int ms, t_philo *philo);
void	get_args(t_philo *philo, int argc, char **argv);
void	print_message(t_msg msg, t_philo *philo);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
void	check_args(int argc, char **argv);
void	print_usage(void);
void	error(t_philo *philo, int ret, char *msg);

#endif