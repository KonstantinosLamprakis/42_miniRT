/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_func_getters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flfische <flfische@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:15:54 by flfische          #+#    #+#             */
/*   Updated: 2024/06/17 15:38:53 by flfische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_hit_func	*ft_get_hit_func(void)
{
	static const t_hit_func	hit_functions[OBJECT_COUNT] = {
	[SPHERE] = ft_sphere_hit};

	return ((t_hit_func *)hit_functions);
}

t_normal_func	*ft_get_normal_func(void)
{
	static const t_normal_func	normal_functions[OBJECT_COUNT] = {
	[SPHERE] = ft_sphere_normal};

	return ((t_normal_func *)normal_functions);
}
