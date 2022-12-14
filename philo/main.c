/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarapet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:43:48 by zkarapet          #+#    #+#             */
/*   Updated: 2022/11/22 21:28:46 by zkarapet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	filling_data(t_data *data, char **av)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	data->num_of_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->must_eat = ft_atoi(av[5]);
	data->start_time = start.tv_sec * 1000
		+ start.tv_usec / 1000;
	data->last_eating_time = 0;
	data->eating_count = 0;
}

pthread_mutex_t	*mutex_init(t_data *data, int n)
{
	int				i;
	pthread_mutex_t	*forks;

	i = -1;
	forks = malloc(sizeof(pthread_mutex_t) * n);
	if (pthread_mutex_init(&data[0].eating_count_mutex, NULL) != 0
		|| pthread_mutex_init(&data[0].is_dead_mutex, NULL))
	{
		error(5);
		return ((void *)0);
	}
	while (++i < n)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			error(5);
			return ((void *)0);
		}
	}
	return (forks);
}

int	main_thread_checks_dying(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data[0].num_of_philos)
	{
		if (is_dead(&data[i], data[0].time_to_die,
				get_time(data[0].start_time), 0))
		{
			pthread_mutex_lock(&data[0].is_dead_mutex);
			return (0);
		}
	}
	return (1);
}

int	conds(t_data *data, pthread_mutex_t *forks, int argc)
{	
	if (!main_thread_checks_dying(data))
	{
		destroying(data, forks);
		return (0);
	}
	if (argc == 6 && !eat_this_much(data))
	{
		destroying(data, forks);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	int				i;
	int				n;
	t_data			*data;
	pthread_mutex_t	*forks;

	i = -1;
	if (!parsing(argc, argv))
		return (0);
	n = ft_atoi(argv[1]);
	data = malloc(sizeof(t_data) * n);
	forks = mutex_init(data, n);
	while (++i < n)
		filling_data(&data[i], argv);
	creation(data, forks);
	while (1)
	{
		if (!conds(data, forks, argc))
			return (0);
	}
}
